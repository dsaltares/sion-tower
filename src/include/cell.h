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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_CELL_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_CELL_H_

#include <OGRE/Ogre.h>

#include "shape.h"
#include "line2D.h"

//! Celda triangular de una malla de navegación

/**
 * @author David Saltares Márquez y Francisco Javier Santacruz-López Cepero
 * @date 20-03-2011
 * 
 * Esta clase modela las celdas que forman el grafo de las mallas de navegación
 * (clase NavigationMesh). Se utilizan para la búsqueda de caminos para
 * entidades inteligentes como los enemigos del juego.
 */
class Cell {
    public:
        /** Enumerado con los lados de una celda */
        enum CellSide {
            SIDE_AB = 0,
            SIDE_BC,
            SIDE_CA
        };
        
        /** Enumerado con los vértices de una celda */
        enum CellVert {
            VERT_A = 0,
            VERT_B,
            VERT_C
        };
        
        /** Enumerado con el resultado de clasificar un camino con respecto a una celda */
        enum PathResult {
            NO_RELATIONSHIP,
            ENDING_CELL,	
            EXITING_CELL,
        };
        
        /**
         * Constructor
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
        Cell(int id,
             const Ogre::Vector3& pointA,
             const Ogre::Vector3& pointB,
             const Ogre::Vector3& pointC);
            
        /**
         * Destructor
         */
        ~Cell();
        
        /**
         * @return identificador de la celda (se utiliza para obtener el
         * índice dentro del grafo de búsqueda de caminos)
         */
        int getId() const;
        
        /**
         * @param index índice del vértice a recuperar
         * @return punto de la celda según el índice
         */
        const Ogre::Vector3& getVertex(int index);
        
        /**
         * @param side lado de la celda a consultar
         * @return celda vecina a la actual por ese lado si es que existe.
         * Si no hay celda vecina devuelve 0.
         */
        Cell* getLink(CellSide side) const;
        
        /**
         * @param side lado por la que las celdas serán vecinas
         * @param nueva celda vecina del lado side.
         */
        void setLink(CellSide side, Cell* cell);
        
        /**
         * @param pointA un vértice de una celda
         * @param pointB otro vértice de una celda
         * @param cell celda a la que pertenecen los vértices
         * 
         * @return true si los dos vértices pasados como parámetro corresponden
         * a un lado de la celda actual, false si las dos celdas no son vecinas.
         */
        bool requestLink(const Ogre::Vector3& pointA,
                         const Ogre::Vector3& pointB,
                         Cell* cell);
        
        /**
         * @return punto central de la celda
         */
        const Ogre::Vector3& getCenter() const;

        /**
         * @param point punto a consultar
         * @return true si el punto está dentro de la celda
         */
        bool containsPoint(const Ogre::Vector3& point) const;
        
        /**
         * @param path segmento con el camino en línea recta
         * @param nextCell siguiente celda por la que pasaría el camino
         * @param side lado por el que sale el camino de la celda (si sale)
         * @param intersection punto de intersección del camino con la celda
         * 
         * @return resultado de clasificar el camino en la celda
         */
        PathResult classifyPathToCell(const Line2D& path,
                                      Cell** nextCell,
                                      CellSide &side,
                                      Ogre::Vector2 &intersection);
        
        /**
         * @param point punto dentro de la celda, se establece point.y
         * para corregir la altura del punto y ajustarse al plano tridimensional
         * que forma la celda
         */
        void getHeight(Ogre::Vector3 &point);
    private:
        // ID
        int _id;
    
        // Geometría
        Ogre::Vector3 _vertex[3];
        Ogre::Vector3 _center;
        Cell* _links[3];
        Line2D _sides[3];
        Plane _plane;
};


inline int Cell::getId() const {return _id;}

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_CELL_H_
