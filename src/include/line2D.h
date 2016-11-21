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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_LINE2D_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_LINE2D_H_

#include <cmath>

#include <OGRE/Ogre.h>

//! Segmento en dos dimensiones para representar desplazamientos en el plano

/**
 * @author David Saltares Márquez
 * @date 28-02-2011
 * 
 * Modela un segmento en el plano y se utiliza para trazar rutas de desplazamiento
 * sobre la malla de navegación. Incorporá métodos para calcular intersecciones
 * entre otros segmentos 2D y puntos 2D.
 */
class Line2D {
    public:
        
        /** Enumerado para clasificar un punto con respecto a una línea 2D */
        enum PointClassification
        {
            /** El punto está en la línea */
            ON_LINE,
            
            /** Mirando desde A hacia B, el punto está a la izquierda */
            LEFT_SIDE,
            
            /** Mirando desde A hacia B, el punto está a la derecha  */
            RIGHT_SIDE
        };

        /** Enumerado para clasificar dos líneas entre sí */
        enum LineClassification
        {
            /** Las líneas son paralelas y se solapan  */
            COLLINEAR,
            
            /** Las líneas intersectan pero los segmentos no */
            LINES_INTERSECT,
            
            /** Las líneas se biseccionan */
            SEGMENTS_INTERSECT,
            
            /** El segmento B está cruzado por A */
            A_BISECTS_B,
            
            /** El segmento A está cruzado por B */
            B_BISECTS_A,
            
            /** Los segmentos son paralelos */
            PARALELL
        };

        /**
         * Constructor
         * 
         * Crea un segmento que va desde (0, 0) hasta (0, 0)
         */
        Line2D();
        
        /**
         * Constructor de copia
         */
        Line2D(const Line2D& line2D);
        
        /**
         * Constructor
         * 
         * @param pointA punto de comienzo del segmento
         * @param pointB punto de final del segmento
         */
        Line2D(const Ogre::Vector2& pointA, const Ogre::Vector2& pointB);
        
        /**
         * Destructor
         */
        ~Line2D();

        /**
         * Operador de asignación
         */
        Line2D& operator=(const Line2D& line2D);

        /**
         * @param point establece el primer punto del segmento
         */
        void setBeginPoint(const Ogre::Vector2& point);
        
        /**
         * @param point establece el punto final del segmento 
         */
        void setEndPoint(const Ogre::Vector2& point);
        
        /**
         * @param pointA nuevo punto de comienzo del segmento
         * @param pointB nuevo punto final del segmento
         */
        void setPoints(const Ogre::Vector2& pointA, const Ogre::Vector2& pointB);
        
        /**
         * @param pointAx coordenada en el eje X del nuevo punto de comienzo
         * @param pointAy coordenada en el eje Y del nuevo punto de comienzo
         * @param pointBx coordenada en el eje X del nuevo punto de final
         * @param pointBy coordenada en el eje Y del nuevo punto de final
         */
        void setPoints(Ogre::Real pointAx, Ogre::Real pointAy, Ogre::Real pointBx, Ogre::Real pointBy);
        
        /**
         * @param point punto para comparar
         * @return distancia entre el segmento y el punto
         */
        Ogre::Real signedDistance(const Ogre::Vector2 point) const;
        
        /**
         * @param point punto a clasificar
         * @param epsilon rango de error para considerar que el punto está en el segmento
         * 
         * @return valor del enumerado clasificando el punto
         */
        PointClassification classifyPoint(const Ogre::Vector2 point, float epsilon = 0.0f) const;
        
        /**
         * @param line segmento con el que calcular la intersección
         * @param intersectionPoint punto 2D en el que se guardará el punto de intersección si lo hubiese
         * 
         * @param valor del enumerado clasificando los dos segmentos entre sí
         */
        LineClassification getIntersection(const Line2D& line, Ogre::Vector2* intersectionPoint = 0) const;
        
        /**
         * @param direction valor de salida para obtener la dirección de la línea
         */
        void getDirection(Ogre::Vector2& direction) const;
        
        /**
         * @return punto de comienzo del segmento
         */
        const Ogre::Vector2& getBeginPoint() const;
        
        /**
         * @return punto final del segmento
         */
        const Ogre::Vector2& getEndPoint() const;
        
        /**
         * @return vector normal al segmento
         */
        const Ogre::Vector2& getNormal() const;
        
        /**
         * @return longitud del segmento
         */
        Ogre::Real getLength() const;


    private:
        
        Ogre::Vector2 _beginPoint;
        Ogre::Vector2 _endPoint;
        
        mutable Ogre::Vector2 _normal;
        
        mutable bool _normalCalculated;
        
        void computeNormal() const;
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_LINE2D_H_
