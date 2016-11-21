/*
 * This file is part of SionTower.
 *
 * 
 * David Saltares M&aacute;rquez (C) 2011
 * <david.saltares@gmail.com>
 *
 * 
 * SionTower examples are free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License ad
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) ant later version.
 *
 * SionTower examples are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SionTower examples.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

#include "pugixml.hpp"

#include "navigationMesh.h"


using std::cout;
using std::cerr;
using std::endl;

NavigationMesh::NavigationMesh(const Ogre::String& fileName): _cellNumber(0) {
    // Si hemos suministrado un nombre para el fichero
    if (fileName != "") {
        // Cargamos las celdas del fichero XML
        loadCellsFromXML(fileName);
        
        // Enlazamos las celdas
        linkCells();
	
	// Construimos grafo
	initGraph();
	
	// Distancias mínimas
	floyd();
	
	// Precomputamos caminos simplificados
	precomputePaths();
    }
}

NavigationMesh::~NavigationMesh() {
    // Limpiamos el vector de celdas
    clear();
    
    // Liberamos la memoria del grafo
    delete [] _graph;
    delete [] _paths;
}
        
void NavigationMesh::loadCellsFromXML(const Ogre::String& fileName) {
    pugi::xml_document doc;
    std::vector<int> a, b, c;
    std::vector<Ogre::Vector3> vertex;
    
    // Abrimos el fichero XML
    if (!doc.load_file(fileName.c_str())) {
        cerr << "NavigationMesh::loadCellsFromXML(): error al leer el fichero " << fileName << endl;
        exit(1);
    }
    
    // Buscamos los nodos face
    pugi::xml_node meshNode = doc.child("mesh");
    pugi::xml_node subMeshesNode = meshNode.child("submeshes");
    pugi::xml_node subMeshNode = subMeshesNode.child("submesh");
    pugi::xml_node facesNode = subMeshNode.child("faces");
    
    
    // Tomamos las caras
    for (pugi::xml_node faceNode = facesNode.child("face"); faceNode; faceNode = faceNode.next_sibling()) {
        int v1 = faceNode.attribute("v1").as_int();
        int v2 = faceNode.attribute("v2").as_int();
        int v3 = faceNode.attribute("v3").as_int();
        
        a.push_back(v1);
        b.push_back(v2);
        c.push_back(v3);
    }
    
    pugi::xml_node geometryNode = subMeshNode.child("geometry");
    pugi::xml_node vertexBufferNode = geometryNode.child("vertexbuffer");
    
    // Tomamos los vértices
    for (pugi::xml_node vertexNode = vertexBufferNode.child("vertex"); vertexNode; vertexNode = vertexNode.next_sibling()) {
        pugi::xml_node positionNode = vertexNode.child("position");
        Ogre::Vector3 v = Ogre::Vector3(positionNode.attribute("x").as_float(),
                                       positionNode.attribute("y").as_float(),
                                       positionNode.attribute("z").as_float());
        
        vertex.push_back(v);
    }
    
    int faceCount = a.size();
    
    // Recorremos las caras añadiendo celdas
    for (int i = 0; i < faceCount; ++i)
        addCell(i, vertex[a[i]], vertex[b[i]], vertex[c[i]]);
}

void NavigationMesh::clear() {
    // Recorremos y destruimos las celdas
    for (Cells::iterator i = _cells.begin(); i != _cells.end(); ++i)
        delete (*i);
    
    // Vaciamos el vector
    _cells.clear();
    
    _cellNumber = 0;
}
        
void NavigationMesh::addCell(int id,
			     const Ogre::Vector3& pointA,
                             const Ogre::Vector3& pointB,
                             const Ogre::Vector3& pointC) {
    // Creamos la celda nueva
    Cell* cell = new Cell(id, pointA, pointB, pointC);
    
    // Añadimos la celda al conjunto de celdas
    _cells.push_back(cell);
    
    // Aumentamos el número de celdas
    ++_cellNumber;
}
                     
void NavigationMesh::linkCells() {
    Cell* cellA;
    Cell* cellB;
    
    // Cruzamos las celdas y unimos aristas comunes
    for (Cells::iterator i = _cells.begin(); i != _cells.end(); ++i) {
        for (Cells::iterator j = _cells.begin(); j != _cells.end(); ++j) {
            // Evitamos tomar la misma celda
            if (i != j) {
                cellA = *i;
                cellB = *j;
                
                if (!cellA->getLink(Cell::SIDE_AB) && cellB->requestLink(cellA->getVertex(0), cellA->getVertex(1), cellA))
                    cellA->setLink(Cell::SIDE_AB, cellB);
                else if (!cellA->getLink(Cell::SIDE_BC) && cellB->requestLink(cellA->getVertex(1), cellA->getVertex(2), cellA))
                    cellA->setLink(Cell::SIDE_BC, cellB);
                else if (!cellA->getLink(Cell::SIDE_CA) && cellB->requestLink(cellA->getVertex(2), cellA->getVertex(0), cellA))
                    cellA->setLink(Cell::SIDE_CA, cellB);
            }
        }
    }
}
        
Cell* NavigationMesh::getCell(int index) {
    if (index < 0 || index >= _cellNumber) {
        cerr << "NavigationMesh::getCell(): la celda con índice: " << index << " no existe" << endl;
        exit(1);
    }
    
    return _cells[index];
}
        
int NavigationMesh::getCellNumber() {
    return _cellNumber;
}

Ogre::Vector3 CatmullRollSpline(const Ogre::Vector3& p0,
				const Ogre::Vector3& p1,
				const Ogre::Vector3& p2,
				const Ogre::Vector3& p3,
				Ogre::Real t ) {
	/* Un Spline de Catmull-Roll es un Spline Cúbico por interpolación de Hermite en el cual se calculan las tangentes mediante:
	 *
	 * mk = (p1 - p0) / (t0 - t1)
	 *
	 * La forma rápida con matriz precalculada es la siguiente:
	 * q(t) = 1/2 (1, t, t^2, t^3) * (catmull-roll matrix) * (points)
	 *
	 * float crmatrix[4][4] = {
	 *	    {  0,  2,  0,  0 },
	 *	    { -1,  0,  1,  0 },
	 *	    {  2, -5,  4, -1 },
	 *	    { -1,  3, -3,  1 }
	 *   }
	 *
	 */
	Ogre::Real t2 = t * t;
	Ogre::Real t3 = t2 * t;

	Ogre::Vector3 v = 0.5 * ( (2 * p1)
		    + (-1 * p0 + p2) * t
	            + (2 * p0 - 5 * p1 + 4 * p2 - 1 * p3) * t2 
		    + (-1 * p0 + 3 * p1 - 3 * p2 + 1 * p3) * t3);

	//cout << "Generando: " << v << " a partir de: " << p1 << " con t: " << t << endl;

	return v;
}

// Devuelve el n de celdas borradas.
int NavigationMesh::simplifyPath(CellPath& cellPath) {
	// Cribar todas las celdas a las que haya visión directa, evitando pasar por centros innecesarios.
	// Lo ideal seria encontrar una linea recta en terreno seguro entre la posicion inicial y final.
	// De manera que intentaremos unir la celda más cercana con la celda más lejana posible.
	// En caso de poder unir alguna de estas celdas mediante una línea recta, borraremos las intermedias.

	// Terminará cuando alcancemos la última celda.
	CellPath::iterator beginIt = cellPath.begin(),
		endIt;
	size_t sz = cellPath.size();
	
	//cout << "Eliminando celdas" << endl;

	while( (endIt = getFurthestVisibleCell(cellPath, beginIt)) != cellPath.end() ) {
		
		//cout << "Comprueba siguiente celda" << endl;
		
		// Eliminar las celdas de enmedio, que no sirven.
		++beginIt;

		sz = cellPath.size(); // DEBUG

		if( beginIt != cellPath.end() )
			beginIt = cellPath.erase(beginIt, endIt);

		// DEBUG info
		//if( cellPath.size() < sz ) 
		//	cout << "Borradas " << sz - cellPath.size() << " celdas." << endl;
	}

	return sz - cellPath.size();
}

// Toma una ruta de puntos y genera puntos extra entre ellos creando un spline.
//
// Devuelve verdadero si se pudo hacer el spline.
bool NavigationMesh::makeSpline(PointPath& path) {

	/* Crear spline añadiendo puntos nuevos entre cada nodo.
	 * Emplearemos un Spline de Hermite cúbico
	 * Generamos n-2 puntos entre cada nodo.
	 * Necesitaremos al menos 4 puntos (2 reales).
	 * Generaremos puntos entre p1 y p2 empleando p0 y p3 como control.
	 * El primer y último segmento no tendrán puntos intermedios.
	 */ 
	if( path.size() < 2 )
		return false;

	// Añadimos al path dos puntos de control falsos. Uno al principio y otro al final.
	// Esto lo hacemos para poder interpolar el primer y último segmento, aunque sea recto.
	// Estos puntos de control estarán en la misma línea que los segmentos inicial y final.
	PointPath::iterator p0, p1, p2, p3;

	// Punto extra inicial
	// Lo calculamos retrocediendo 0.5 en la dirección del segmento.
	p0 = p1 = path.begin();
	++p1;
	Ogre::Vector3 pextra = *p0 - ((*p1 - *p0).normalise() * 0.5);
	path.push_front(pextra);

	// Punto extra final
	p0 = p1 = path.end();
	--p1;
	--p0;
	--p0;
	pextra = *p1 + ((*p1 - *p0).normalise() * 0.5);
	path.push_back(pextra);

	// Puntos de la ruta por 'metro'
	int dotsPerUnit = 2;

	// Establecer p0, p1, p2 y p3
	p0 = path.begin();

	p1 = p0; 
	++p1;

	p2 = p1; 
	++p2;

	p3 = p2; 
	++p3;


	while( p3 != path.end() ){

		// El n de puntos a calcular debe ser proporcional a la distancia que se recorre.
		// Calculamos la distancia entre p1 y p2 para saber cuantos puntos debemos calcular.
		// El mantener el n de puntos constante entre distancias diferentes da problemas de velocidad.
		int n = (*p1 - *p2).length() * dotsPerUnit;
		Ogre::Real step = 1.0/n;
		Ogre::Real s = step;

		// Añadimos a la ruta n-2 puntos.
		for(int i = 1; i < n; ++i) {
			path.insert(p2, CatmullRollSpline(*p0, *p1, *p2, *p3, s));
			s += step;
		}

		p0 = p1;
		p1 = p2;
		++p2;
		++p3;

		//cout << endl;
	}

	// Eliminar los puntos extra en los extremos, que se han usado para el
	// cálculo pero que no forman parte de la ruta.
	path.pop_front();
	path.pop_back();

	return true;
}

bool NavigationMesh::buildPath(PointPath& path,
                               const Ogre::Vector3& startPos,
                               const Ogre::Vector3& endPos,
                               Cell* startCell,
                               Cell* endCell) {
    
    // Si no hemos proporcionado celdas las buscamos
    if (!startCell)
	startCell = findCell(startPos);

    if (!endCell)
	endCell = findCell(endPos);

    // Si alguna pos no pertenece a la malla
    if (!startCell || !endCell) {
	cout << "Celdas no pertenecen a la rejilla" << endl;
	return false;
    }

    // Comprobamos que existe camino
    int startId = startCell->getId();
    int endId = endCell->getId();
    
    if (_graph[startId * _cellNumber + endId] == Ogre::Math::POS_INFINITY) {
	cout << "No se ha encontrado camino" << endl;
	return false;
    }

    
    // Camino de celdas
    CellPath cellPath;
    
    // Reconstruir camino de celdas
    recoverPath(startId, endId, cellPath);
    
    // Simplificar la ruta eliminando celdas innecesarias.
    //simplifyPath(cellPath);
    
    // Limpiamos el camino anterior
    path.clear();

    // Obtener los puntos del camino, que es finalmente lo que vamos a usar.
    /*std::for_each(cellPath.begin(), 
	cellPath.end(), 
	boost::bind(&PointPath::push_back, 
	     path, 
	     boost::bind(&Cell::getCenter, _1)
	    )
	);*/
	
    for(CellPath::iterator i = cellPath.begin(); i != cellPath.end(); ++i)
	path.push_back((*i)->getCenter());

    // El primer punto es el punto actual del personaje.
    path.push_front(startPos);

    // El último punto es el punto exacto a donde hay que ir.
    path.push_back(endPos);

    makeSpline(path);

    // Hemos encontrado el camino
    return true;
}
        
Cell* NavigationMesh::findCell(const Ogre::Vector3& pos) {
    Ogre::Vector3 v;
    Ogre::Vector3 minDistance = Ogre::Vector3(500.0, 500.0, 500.0);
    Cell* closestCell = 0;
    
    // Recorremos las celdas buscando una que contenga al punto deseado
    // En caso de no encontrar, devolvemos la más cercana
    for (Cells::iterator i = _cells.begin(); i != _cells.end(); ++i) {
        if ((*i)->containsPoint(pos)) {
            return *i;
        }
	
	v = (*i)->getCenter() - pos;
	if (v.squaredLength() < minDistance.squaredLength()) {
	    minDistance = v;
	    closestCell = (*i);
	}
    }
    
    // Ninguna celda contiene el punto
    // devolvemos la más cercana
    return closestCell;
}

bool NavigationMesh::lineOfSightTest(const Ogre::Vector3& start,
                                     const Ogre::Vector3& end,
                                     Cell* startCell,
                                     Cell* endCell) {
    
    Cell* nextCell = startCell;
    Cell::PathResult result = Cell::EXITING_CELL;
    Line2D line2D(Ogre::Vector2(start.x, start.z),
                  Ogre::Vector2(end.x, end.z));
    Cell::CellSide side;
    Ogre::Vector2 intersection;
    
    while (result == Cell::EXITING_CELL) {
        
        result = nextCell->classifyPathToCell(line2D, &nextCell, side, intersection);
        
        if (!nextCell) {
            return false;
        }
    }
    
    return (result == Cell::ENDING_CELL);
}

NavigationMesh::CellPath::iterator NavigationMesh::getFurthestVisibleCell(CellPath& path, CellPath::iterator startIt) {
    
    // Si el punto de comienzo es inválido devolvemos end
    if (startIt == path.end())
        return startIt;
    
    // con cellIt iteramos por las celdas
    CellPath::iterator cellIt = startIt;
    ++cellIt;
    
    // lastIt es la última celda válida con línea de visión
    CellPath::iterator lastIt = cellIt;
    
    if (lastIt == path.end())
	return lastIt;
	
    for (; cellIt != path.end(); ++cellIt) {
	if (lineOfSightTest((*startIt)->getCenter(), (*cellIt)->getCenter(), *startIt, *cellIt))
	    lastIt = cellIt;
	else
	    return lastIt;
    }
    
    return lastIt;
    
    /*CellPath::iterator cellIt = path.end();
    --cellIt;
    
    // Si startIt era la última celda devolvemos end
    if (cellIt == startIt) {
        return path.end();
    }
    
    // Recorremos las celdas de la más alejada a la origen.
    // La primera que encontremos con la que tengamos linea directa, será la
    // que abarque más y, por tanto, la mejor.
    for(; cellIt != startIt; --cellIt) {
        if (lineOfSightTest((*startIt)->getCenter(), (*cellIt)->getCenter(), *startIt, *cellIt))
		return cellIt;
    }

    //cout << "No se ha encontrado linea entre dos celdas" << endl;

    // No hemos encontrado una linea entre start y otra.
    return path.end();*/
}


void NavigationMesh::initGraph() {
    // Reservamos memoria para el grafo
    _graph = new Ogre::Real[_cellNumber * _cellNumber];
    _paths = new int[_cellNumber * _cellNumber];
    
    // Inicializamos las matrices
    for (int i = 0; i < _cellNumber * _cellNumber; ++i) {
	// Matriz de costes: coste infinito
	_graph[i] = Ogre::Math::POS_INFINITY;
	
	// Matriz de caminos: no existe camino
	_paths[i] = -1;
    }
    
    // Recorremos las celdas
    for (Cells::iterator it = _cells.begin(); it != _cells.end(); ++it) {
	// Tomamos la celda y su id
	Cell* cellA = *it;
	int idA = cellA->getId();
	
	// El coste de ir de la celda a sí misma es 0
	_graph[idA * _cellNumber + idA] = 0.0f;
	
	Cell* cellB;
	int idB;
	
	// Probamos el lado SIDE_AB
	cellB = cellA->getLink(Cell::SIDE_AB);
	if (cellB != 0) {
	    idB = cellB->getId();
	    _graph[idA * _cellNumber + idB] = 1.0f;
	    _graph[idB * _cellNumber + idA] = 1.0f;
	}
	
	// Probamos el lado SIDE_BC
	cellB = cellA->getLink(Cell::SIDE_BC);
	if (cellB != 0) {
	    idB = cellB->getId();
	    _graph[idA * _cellNumber + idB] = 1.0f;
	    _graph[idB * _cellNumber + idA] = 1.0f;
	}
	
	// Probamos el lado SIDE_CA
	cellB = cellA->getLink(Cell::SIDE_CA);
	if (cellB != 0) {
	    idB = cellB->getId();
	    _graph[idA * _cellNumber + idB] = 1.0f;
	    _graph[idB * _cellNumber + idA] = 1.0f;
	}
    }
}

void NavigationMesh::floyd() {
    for (int k = 0; k < _cellNumber; ++k) {
	for (int i = 0; i < _cellNumber; ++i) {
	    for (int j = 0; j < _cellNumber; ++j) {
		Ogre::Real ikj = _graph[i * _cellNumber + k] + _graph[k * _cellNumber + j];
		if (ikj < _graph[i * _cellNumber + j]) {
		    _graph[i * _cellNumber + j] = ikj;
		    _paths[i * _cellNumber + j] = k;
		}
	    }
	}
    }
}

void NavigationMesh::precomputePaths() {

    //int* newPaths = new int[_cellNumber * _cellNumber];
    
    // Recorremos todas las combinaciones inicio - destino
    for (int i = 0; i < _cellNumber; ++i) {
	for (int j = 0; j < _cellNumber; ++j) {
	    
	    //newPaths[i * _cellNumber + j] = _paths[i * _cellNumber + j];
	    
	    // Recuperamos el camino de i a j
	    CellPath cellPath;
	    recoverPath(i, j, cellPath);
	    
	    // Simplificamos el camino de i a j
	    simplifyPath(cellPath);
	    
	    // Actualizamos la matriz de caminos con el camino simplificado de i a j 
	    for (CellPath::iterator it = cellPath.begin(); it != cellPath.end(); ++it) {
		CellPath::iterator nextIt = it;
		++nextIt;

		if (nextIt != cellPath.end()) {
		    int idA = (*it)->getId();
		    int idB = (*nextIt)->getId();
		    
		    // El camino entre nodos consecutivos es directo
		    _paths[idA * _cellNumber + idB] = -1;
		}
	    }
	}
    }
    
    //delete [] _paths;
    //_paths = newPaths;
}

void NavigationMesh::recoverPath(int i, int j, CellPath& cellPath) {
    // Tomamos el nodo intermedio por el que pasa el camino de i a j
    int k = _paths[i * _cellNumber + j];
    
    // Si hay atajo
    if (k != -1) {
	// Recuperamos el camino de i a k
	recoverPath(i, k, cellPath);
	
	// Insertamos la celda de id k en el camino
	cellPath.push_back(_cells[k]);
	
	// Recuperamos el camino de k a j
	recoverPath(k, j, cellPath);
    }
}

