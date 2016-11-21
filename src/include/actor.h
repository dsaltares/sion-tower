/*
 * This file is part of SionTower.
 *
 * 
 * David Saltares Márquez (C) 2011
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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_ACTOR_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_ACTOR_H_

#include <OGRE/Ogre.h>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

#include "gameMesh.h"
#include "kinematic.h"

class StateGame;

//! Clase abstracta descendiente de GameMesh que modela a los elementos dinámicos del juego (protagonista, enemigos...)

/**
 * @author David Saltares Márquez
 * @date 05-04-2011
 * 
 * Actor incorpora atributos para modelar personajes dinámicos del juego
 * como el propio protagonista o los enemigos. Modela elementos con energía,
 * poder de ataque y propiedades cinemáticas (Kinematic). Incorpora
 * un método virtual para actualizarlo de forma lógica en cada cuadro.
 */

class Actor: public GameMesh {
    public:
        /** Estados en los que se puede encontrar un actor */
        enum State {
            IDLE,
            RUN,
            DAMAGED,
            ATTACK,
            WIN,
            DIE,
            ERASE
        };
        
        /** Diccionario de animaciones para cada estado del actor */
        typedef boost::unordered_map<State, Ogre::AnimationState*> Animations;
        
        /** Funciones de actualización lógica según cada estado del actor */
        typedef boost::unordered_map<State, boost::function<void(Ogre::Real)> > UpdateStateMethods;
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena en el que se introducirán
         * los elementos visuales del actor.
         * @param stateGame estado de juego que controla al personaje
         */
        Actor(Ogre::SceneManager* sceneManager, StateGame* stateGame);
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena en el que se introducirán
         * los elementos visuales del actor.
         * @param stateGame estado de juego que controla al personaje
         * @param entityName nombre de la entidad del actor
         * @param entityFile nombre del mesh del actor
         * @param nodeName nombre del nodo del actor
         */
        Actor(Ogre::SceneManager* sceneManager,
              StateGame* stateGame,
              const Ogre::String& entityName,
              const Ogre::String& entityFile,
              const Ogre::String& nodeName);
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena en el que se introducirán
         * los elementos visuales del actor.
         * @param stateGame estado de juego que controla al personaje
         * @param entityFile nombre del mesh del actor
         */
        Actor(Ogre::SceneManager* sceneManager,
              StateGame* stateGame,
              const Ogre::String& entityFile);
        
        /**
         * Destructor
         * 
         * Hay que recordar que GameObject ya elimina por nosotros: Entity,
         * SceneNode y Body. En el destructor se libera el resto de memoria
         * reservada.
         */
        virtual ~Actor();
        
        /**
         * @param deltaT tiempo en milisegundos desde el último frame.
         * 
         * Actualiza lógicamente el actor, es virtual y está diseñada para
         * sobrecargarse. Por defecto actualiza la animación, busca la
         * función relacionada con el estado actual y la llama si existe.
         */
        virtual void update(Ogre::Real deltaT);
        
        /**
         * Inicializa los parámetros para crear las barras de vida. Debe
         * llamarse una vez al inicio del programa.
         */
        static void initialiseBillboardSet();
        
        /**
         *  @return vida del actor
         */
        int getEnergy() const;
        
        /**
         * @return máxima energía del actor
         */
        int getMaxEnergy() const;
        
        /**
         * @param energy nueva vída del actor
         */
        void setEnergy(int energy);
        
        /**
         * @param damage daño inflingido
         * 
         * Inflinge daño al actor, su vida se ve disminuida según indica
         * damage.
         */
        void getHurt(int damage);
        
        /**
         * @return daño base que inflinge el actor
         */
        int getPower() const;
        
        /**
         * @param power nuevo daño base que inflinge el actor
         */
        void setPower(int power);
        
        /**
         * @return estado actual en el que se encuentra el actor
         */
        State getState();
        
        /**
         * @param state nuevo estado del actor
         * @param loop indica si la animación asociada al estado debe repetirse
         * en un bucle infinito.
         * 
         * Establece un nuevo estado para el actor y reproduce la animación
         * asociada.
         */
        void setState(State state, bool loop = true);
        
        /**
         * @param orientation nueva orientación del personaje.
         * 
         * Actualiza la orientación del personaje dentro del nodo, del body
         * y de la estructura kinematic.
         */
        virtual void setOrientation(const Ogre::Quaternion& orientation);
        
        /**
         * @param position nueva posición del personaje.
         * 
         * Actualiza la posición del personaje dentro del nodo, del body
         * y de la estructura kinematic.
         */
        virtual void setPosition(const Ogre::Vector3& position);
        
        /**
         *  @return información cinemática del personaje
         */
        Kinematic& getKinematic();
        
        /**
         * @param kinematic nueva información cinemática del personaje
         */
        void setKinematic(const Kinematic& kinematic);
    protected:
        // Estado de juego
        StateGame* _stateGame;
    
        // Energía y daño
        int _maxEnergy;
        int _energy;
        int _power;
        
        // Cinemática para Steering
        Kinematic _kinematic;
        
        // Animaciones y estados
        Animations _animations;
        Ogre::AnimationState* _currentAnimation;
        State _currentState;
        State _previousState;
        UpdateStateMethods _updateMethods;
        
        void synchronizeFromKinematic();
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_ACTOR_H_
