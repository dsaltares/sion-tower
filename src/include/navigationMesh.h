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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_NAVIGATIONMESH_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_NAVIGATIONMESH_H_

#include <vector>
#include <list>

#include <OGRE/Ogre.h>

#include "cell.h"

//! Grafo de celdas transitable del escenario, utilizado para búsqueda de caminos

/**
 * @author David Saltares Márquez y Francisco Javier Santacruz-López Cepero
 * @date 20-03-2011
 * 
 * La malla de navegación es un grafo de celdas triangulares que contiene
 * información sobre qué zonas del nivel son transitables. Este conocimiento
 * se utiliza para construir rutas de un punto a otro esquivando obstáculos
 * estáticos usando el algoritmo de Floyd precomputando caminos. Está diseñada
 * como apoyo a la IA de los enemigos.
 * 
 * Una malla de navegación se crea a partir de un fichero .mesh.xml exportado
 * desde cualquier programa de diseño 3D compatible como Blender.
 */
class NavigationMesh {
    public:
        /** Conjunto de celdas que forman el grafo de la malla */
        typedef std::vector<Cell*> Cells;
        
        /** Camino formado por una lista de celdas */
        typedef std::list<Cell*> CellPath;
        
        /** Camino formado por una lista de puntos */
        typedef std::list<Ogre::Vector3> PointPath;
        
        /**
         * Constructor
         * 
         * @param fileName ruta al fichero .mesh.xml
         * 
         * Por ahora sólo soporta ficheros mesh en formato XML, más
         * adelante se añadirá soporte para ficheros binarios .mesh
         * e integración con el sistema de gestión de recursos de Ogre.
         * 
         * Crea la malla de navegación y precomputa todos los caminos
         * posibles utilizando Floyd y la simplificación de caminos.
         */
        NavigationMesh(const Ogre::String& fileName = "");
        
        /**
         * Destructor
         */
        ~NavigationMesh();
        
        /**
         * Añade una celda a la malla de navegación
         * 
         * @param id identificador de la celda
         * @param pointA primer punto del triángulo
         * @param pointB segundo punto del triángulo
         * @param pointC tercero punto del triángulo
         * 
         * El orden es relevante. Se supone que desde A hasta B, el triángulo
         * queda a la derecha. Lo mismo debe ocurrir desde B a C y desde
         * C a A. Esto es útil para optimizar intersecciones con los caminos
         * dentro de las celdas de la malla.
         */
        void addCell(int id,
                     const Ogre::Vector3& pointA,
                     const Ogre::Vector3& pointB,
                     const Ogre::Vector3& pointC);
        
        /**
         * Crea los enlaces entre las celdas de la malla, detecta automáticamente
         * qué celdas son vecinas. Debería llamarse una sóla vez tras haber
         * añadido todas las celdas.
         */
        void linkCells();
        
        /**
         * Elimina todas las celdas de la malla.
         */
        void clear();
        
        /**
         * @param index índice de la celda a consultar
         * @return celda correspondiente al índice
         */
        Cell* getCell(int index);
        
        /**
         * @return número de celdas que contiene la malla 
         */
        int getCellNumber();

        /**
         * Reconstruye el camino a partir de las rutas producidas por el
         * algoritmo de Floyd y aplica un spline
         * creando puntos intermedios ficticios para suavizar la ruta.
         * 
         * @param path camino de puntos (salida)
         * @param startPos posición de comienzo (debe estar en la malla)
         * @param endPos posición de destino (debe estar en la malla)
         * @param startCell celda de inicio para mayor eficiencia. Si no
         * se facilita se buscará automáticamente (más lento).
         * @param endCell celda final para mayor eficiencia. Si no
         * se facilita se buscará automáticamente (más lento).
         * 
         * @return true si existe camino, false en caso contrario
         */
        bool buildPath(PointPath& path,
                       const Ogre::Vector3& startPos,
                       const Ogre::Vector3& endPos,
                       Cell* startCell = 0,
                       Cell* endCell = 0);
        
        /**
         * @param pos posición a clasificar en la malla
         * 
         * @return celda que contiene al punto dado. Se devuelve la más cercana
         * si el punto no está en la malla.
         */
        Cell* findCell(const Ogre::Vector3& pos);
        
        /**
         * @param start punto de comienzo
         * @param end punto final
         * @param startCell celda de comienzo
         * @param endCell celda final
         * 
         * @return true si hay línea de visión entre los dos puntos, false
         * en caso contrario.
         */
        bool lineOfSightTest(const Ogre::Vector3& start,
                             const Ogre::Vector3& end,
                             Cell* startCell,
                             Cell* endCell);
        
        /**
         * @param path camino
         * @param startIt iterador dentro del camino dado
         * 
         * @return iterador del camino apuntando a la celda más lejana
         * con la que haya una línea de visión directa. No habría riesgo
         * de obstáculos.
         */
        CellPath::iterator getFurthestVisibleCell(CellPath& path, CellPath::iterator startIt);
        
    private:
        void loadCellsFromXML(const Ogre::String& fileName);
        bool makeSpline(PointPath& path);
        int simplifyPath(CellPath& cellPath);
    
        Cells _cells;
        int _cellNumber;
        
        // Grafo y Floyd
        Ogre::Real* _graph;
        int* _paths;
        
        void initGraph();
        void floyd();
        void precomputePaths();
        void recoverPath(int i, int j, CellPath& cellPath);
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_NAVIGATIONMESH_H_
