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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_KINEMATIC_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_KINEMATIC_H_

#include <OGRE/Ogre.h>

#include "steering.h"

//! Modela la información cinemática de un Actor

/**
 * @author David Saltares Márquez
 * @date 06-06-2011
 * 
 * Esta clase se encarga de almacenar la información cinemática de un Actor
 * como: posición, orientación, velocidad lineal, velocidad angular y máxima
 * velocidad. Se utiliza en conjunción con Steering y SteeringBehaviour
 * para afectar al movimiento de los personajes. Steering afecta la cinemática
 * con aceleraciones (lineales o angulares) mientras que los comportamientos
 * generan steerings según el entorno del personaje.
 */
class Kinematic {
    public:
    
        /**
         * Constructor
         * 
         * @param position posición inicial
         * @param orientation orientación en radianes con respecto al eje Y
         * @param velocity dirección y rapidez iniciales
         * @param rotation velocidad angular en radianes/segundo sobre el eje Y
         * @param maxSpeed máxima rapidez
         */
        Kinematic(const Ogre::Vector3& position = Ogre::Vector3::ZERO,
                  Ogre::Real orientation = 0.0f,
                  const Ogre::Vector3& velocity = Ogre::Vector3::UNIT_SCALE,
                  Ogre::Real rotation = 0.0f,
                  Ogre::Real maxSpeed = 0.0f);
        
        /**
         * @return posición actual
         */
        const Ogre::Vector3& getPosition() const;
        
        /**
         * @return orientación actual en radianes sobre el eje Y
         */
        Ogre::Real getOrientation() const;
        
        /**
         * @return velocidad lineal actual
         */
        const Ogre::Vector3& getVelocity() const;
        
        /**
         * @return velocidad angular actual en radianes / segundo sobre el eje Y
         */
        Ogre::Real getRotation() const;
        
        /**
         * @return velocidad máxima en metros / segundo
         */
        Ogre::Real getMaxSpeed() const;
        
        /**
         * @param position nueva posición
         */
        void setPosition(const Ogre::Vector3& position);
        
        /**
         * @param orientation nueva orientación en radianes sobre el eje Y
         */
        void setOrientation(Ogre::Real orientation);
        
        /**
         * @param velocity nueva velocidad
         */
        void setVelocity(const Ogre::Vector3& velocity);
        
        /**
         * @param rotation nueva velocidad angular
         */
        void setRotation(Ogre::Real rotation);
        
        /**
         * @param maxSpeed nueva velocidad máxima
         */
        void setMaxSpeed(Ogre::Real maxSpeed);
        
        /**
         * @param steering steering sobre el que actualizar
         * @param deltaT tiempo en ms desde el último frame
         * 
         * Actualiza la posición y orientación utilizando integración de Euler
         * en función de steering y deltaT. También actualiza las velocidades
         * angular y lineal (con el límite marcado por maxSpeed).
         */
        void update(const Steering& steering, Ogre::Real deltaT);
        
        /**
         * Establece la orientación según la velocidad, mira hacia donde se dirige
         */
        void setOrientationFromVelocity();
        
        /**
         * Establece la orientación de forma que mire a un punto.
         */
        void lookAt(const Ogre::Vector3& point);
    private:
        Ogre::Vector3 _position;
        Ogre::Real _orientation;
        Ogre::Vector3 _velocity;
        Ogre::Real _rotation;
        Ogre::Real _maxSpeed;
};


// Métodos inline
inline const Ogre::Vector3& Kinematic::getPosition() const {return _position;}
inline Ogre::Real Kinematic::getOrientation() const {return _orientation;}
inline const Ogre::Vector3& Kinematic::getVelocity() const {return _velocity;}
inline Ogre::Real Kinematic::getRotation() const {return _rotation;}
inline Ogre::Real Kinematic::getMaxSpeed() const {return _maxSpeed;}
        
inline void Kinematic::setPosition(const Ogre::Vector3& position) {_position = position;}
inline void Kinematic::setOrientation(Ogre::Real orientation) {_orientation = orientation;}
inline void Kinematic::setVelocity(const Ogre::Vector3& velocity) {_velocity = velocity;}
inline void Kinematic::setRotation(Ogre::Real rotation) {_rotation = rotation;}
inline void Kinematic::setMaxSpeed(Ogre::Real maxSpeed) {_maxSpeed = maxSpeed;}

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_KINEMATIC_H_
