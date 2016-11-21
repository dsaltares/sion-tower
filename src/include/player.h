/*
 * This file is part of SionTower.
 *
 * 
 * David Saltares Marquez (C) 2011
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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_PLAYER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_PLAYER_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include "gameObject.h"
#include "navigationMesh.h"
#include "cell.h"
#include "actor.h"
#include "kinematic.h"
#include "spell.h"
#include "soundFX.h"

class Shape;

//! Clase que modela al personaje principal de Sion Tower y su control

/**
 * @author David Saltares Márquez
 * @date 4-02-2011
 * 
 * Player representa al protagonista del juego Sion Tower y modela tanto
 * sus características (ataque, vida) como su control (utilizando OIS).
 */
class Player: public Actor {
    public:
    
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena (para crear nodos y entidades)
         * @param stateGame estado de juego en el que vive el personaje
         * @param camera cámara que proporciona la vista de la escena (influye en las direcciones del personaje)
         * @param keyboard teclado para controlar al personaje
         * @param mouse ratón para controlar al personaje
         */
        Player(Ogre::SceneManager* sceneManager,
               StateGame* stateGame,
               Ogre::Camera* camera,
               OIS::Keyboard* keyboard,
               OIS::Mouse* mouse);
               
        /**
         * Destructor
         * 
         * Libera la memoria ocupada por el personaje
         */
        ~Player();
        
        /**
         * @return tipo de objeto de juego, devuelve GameObject::PLAYER
         */
        Type getType() const;
        
        /**
         * @param deltaT tiempo desde el último frame en ms
         * 
         * Actualiza de forma lógica al personaje según el teclado, la cámara
         * y el tiempo transcurrido desde el último frame.
         */
        void update(Ogre::Real deltaT);
        
        /**
         *  @return anterior posición del actor (por si colisiona poder restaurarla)
         */
        const Ogre::Vector3 getOldPosition() const;
        
        /**
         * En caso de que haya colisionado podemos restaurar la antigua posición
         * del actor
         */
        void restoreOldPosition();
        
        /**
         * @return maná disponible
         */
        int getMana() const;
        
        /**
         * Emplea maná para lanzar un hechizo
         * 
         * @param mana maná empleado.
         */
        void useMana(int mana);
        
        /**
         * @return hechizo seleccionado
         */
        Spell::Type getSpell() const;
        
        /**
         * Selecciona un hechizo
         * 
         * @param type hechizo a seleccionar
         */
        void setSpell(Spell::Type type);
        
    private:
        Ogre::Camera* _camera;
        OIS::Keyboard* _keyboard;
        OIS::Mouse* _mouse;
        
        // Mana
        int _mana;
        Ogre::Real _timeMana;
        
        // Cinemática
        Ogre::Vector3 _oldPos;
        
        // Hechizo seleccionado
        Spell::Type _selectedSpell;
        
        // Sonidos
        SoundFXPtr _damagedSound;
        
        // Métodos de actualización
        void stateIdle(Ogre::Real deltaT);
        void stateRun(Ogre::Real deltaT);
        void stateAttack(Ogre::Real deltaT);
        void stateDamaged(Ogre::Real deltaT);
        void stateWin(Ogre::Real deltaT);
        void stateDie(Ogre::Real deltaT);
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_PLAYER_H_
