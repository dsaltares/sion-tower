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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_STEERINGBEHAVIOURS_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_STEERINGBEHAVIOURS_H_

#include <vector>

#include <OGRE/Ogre.h>

#include "steering.h"
#include "kinematic.h"
#include "navigationMesh.h"


class Enemy;

// Clase base

//! Clase base abstracta que modela los Steering Behaviours de forma general

/**
 * @author David Saltares Márquez
 * @date 06-06-2011
 * 
 * Los Steering Behaviours utilizan la información del entorno para que
 * afectar al movimiento de un Actor. Cada tipo de Steering Behaviour modela
 * un comportamiento: perseguir, huir, vagar, alinearse, seguir un camino...
 * 
 * El uso de un steering seria el siguiente:
 * 
 * \code
 * Steering steering;
 * 
 * Seek seek(&characterKinematic, &targetKinematic);
 * 
 * seek.getSteering(steering);
 * 
 * characterKinematic.update(steering, deltaT);
 * \endcode
 */
class SteeringBehaviour {
    public:
        /** Información cinemática del personaje */
        Kinematic* character;
        
        /**
         * Constructor
         * 
         * @param character información cinemática del personaje sobre el que actuará el comportamiento
         */
        SteeringBehaviour(Kinematic* character = 0);
        
        /**
         * Método virtual puro
         * 
         * @param steering steering sobre el que escribirá el comportamiento
         * 
         * Toma información del entorno y produce un steering que deberá afectar
         * a la cinemática del personaje para conseguir el comportamiento buscado.
         */
        virtual void getSteering(Steering& steering) = 0;
    
    protected:
        inline const Ogre::Vector3 angleToVector(Ogre::Real angle) {
            return Ogre::Vector3(sin(angle), 0.0f, cos(angle));
        }
};



// Steering Behaviours básicos

//! Comportamiento para buscar un objetivo a máxima velocidad

/**
 * @author David Saltares Márquez
 * @date 06-06-2011
 * 
 * Seek cuenta con un personaje y un objetivo, hace que el personaje se
 * dirija hacia el objetivo a máxima velocidad. No está preparado para que
 * llegue y se pare. Por su aceleración se pasará y tratará de volver.
 * Si se quiere llegar al objetivo, es más recomendable utilizar Arrive.
 */
class Seek: public SteeringBehaviour {
    public:
        Kinematic* target;
        Ogre::Real maxAcceleration;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Seek(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Seek
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
};

//! Comportamiento para huir de un objetivo a máxima velocidad

/**
 * @author David Saltares Márquez
 * @date 06-06-2011
 */
class Flee: public SteeringBehaviour {
    public:
        Kinematic* target;
        Ogre::Real maxAcceleration;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Flee(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Flee
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
};

//! Comportamiento para alcanzar un objetivo decelerando al final

/**
 * @author David Saltares Márquez
 * @date 06-06-2011
 */
class Arrive: public SteeringBehaviour {
    public:
        Kinematic* target;
        Ogre::Real maxAcceleration;
        Ogre::Real targetRadius;
        Ogre::Real slowRadius;
        Ogre::Real timeToTarget;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Arrive(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Arrive
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
};

//! Comportamieno para tomar la misma orientación que el objetivo

/**
 * @author David Saltares Márquez
 * @date 06-06-2011
 */
class Align: public SteeringBehaviour {
    public:
        Kinematic* target;
        Ogre::Real maxAngularAcceleration;
        Ogre::Real maxRotation;
        Ogre::Real targetRadius;
        Ogre::Real slowRadius;
        Ogre::Real timeToTarget;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Align(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Align
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
        
    protected:
        Ogre::Real mapToRange(Ogre::Real angle);
};

//! Comportamiento para alcanzar la misma velocidad que el objetivo

/**
 * @author David Saltares Márquez
 * @date 12-06-2011
 */
class VelocityMatch: public SteeringBehaviour {
    public:
        Kinematic* target;
        Ogre::Real maxAcceleration;
        Ogre::Real timeToTarget;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        VelocityMatch(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de VelocityMatch
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
    
    protected:
        Ogre::Real mapToRange(Ogre::Real angle);
};



// Steering Behaviours compuestos

//! Comportamiento para perseguir un objetivo prediciendo su posición futura

/**
 * @author David Saltares Márquez
 * @date 12-06-2011
 */
class Pursue: public Seek {
    public:
        Kinematic* target; // Tenemos dos: pursue.target y seek.target
        Ogre::Real maxPrediction;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Pursue(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Pursue
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para huir de un objetivo prediciendo su posición futura

/**
 * @author David Saltares Márquez
 * @date 12-06-2011
 */
class Evade: public Flee {
    public:
        Kinematic* target; // Tenemos dos: evade.target y flee.target
        Ogre::Real maxPrediction;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Evade(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Evade
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para mirar hacia un objetivo

/**
 * @author David Saltares Márquez
 * @date 12-06-2011
 */
class Face: public Align {
    public:
        Kinematic* target; // Tenemos dos: face.target y align.target
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Face(Kinematic* character = 0, Kinematic* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Face
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para vagar por el espacio de forma aleatoria

/**
 * @author David Saltares Márquez
 * @date 12-06-2011
 */
class Wander: public Face {
    public:
        Ogre::Real wanderOffset;
        Ogre::Real wanderRadius;
        Ogre::Real wanderRate;
        Ogre::Real wanderOrientation;
        Ogre::Real maxAcceleration;
        
        /**
         * Constructor
         * 
         * @param character personaje
         */
        Wander(Kinematic* character = 0);
        
        /**
         * Modifica el steering según el comportamiento de Wander
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering);
        
    protected:
        inline Ogre::Real randomBinomial() {
            return (rand() - rand()) / (RAND_MAX * 1.0f);
        }
};

//! Comportamiento para evitar colisiones entre enemigos

/**
 * @author David Saltares Márquez
 * @date 12-06-2011
 */
class CollisionAvoidance: public Flee {
    public:
        Ogre::Real maxAcceleration;
        std::vector<Enemy*> targets;
        Enemy* myself;
        Ogre::Real radius;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param targets lista de enemigos a evitar
         * @param enemy el propio enemigo, no se tiene en cuenta para evitar (o se evitaría a sí mismo)
         */
        CollisionAvoidance(Kinematic* character,
                           const std::vector<Enemy*>& targets,
                           Enemy* myself);
        
        /**
         * Modifica el steering según el comportamiento de CollisionAvoidance
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para seguir un camino PointPath

/**
 * @author David Saltares Márquez
 * @date 13-06-2011
 */
class FollowPath: public Arrive {
    public:
        NavigationMesh::PointPath* path;
        Ogre::Real pathOffset;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param path camino que debe seguir el personaje
         */
        FollowPath(Kinematic* character, NavigationMesh::PointPath* path);
        
        /**
         * Modifica el steering según el comportamiento de FollowPath
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering);
        
    protected:
        Ogre::Vector3 findClosestPathPoint(const Ogre::Vector3& point);
        Ogre::Vector3 findTargetInPath();
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_STEERINGBEHAVIOURS_H_
