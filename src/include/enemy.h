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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_ENEMY_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_ENEMY_H_

#include "actor.h"
#include "gameObject.h"
#include "navigationMesh.h"
#include "cell.h"
#include "kinematic.h"
#include "soundFX.h"

class StateGame;


//! Clase que modela a los enemigos y contiene su comportamiento (IA)

/**
 * @author David Saltares Márquez
 * @date 20-05-2011
 * 
 * La clase Enemy se encarga de modelar a todos los enemigos del juego
 * (Goblin, Demonio, Araña y Gólem). Utiliza las clases SteeringBehaviour y
 * NavigationMesh para modelar su comportamiento.
 * 
 * Se ha modelado un comportamiento único para todos los enemigos: perseguir
 * al protagonista y atacar cuando esté dentro del rango de alcance. Los enemigos
 * evitan colisionar entre ellos mismos.
 */
class Enemy: public Actor {
    public:
        
        /** Tipos de enemigos */
        enum Type {
            GOBLIN,
            DEMON,
            GOLEM,
            SPIDER,
            PLAYER // Sólo para casos especiales
        };
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de la escena (para añadir nodos y entidades)
         * @param stateGame estado de juego que controla al enemigo
         * @param type tipo de enemigo a crear
         * @param position posición del mundo en el que debe aparecer el enemigo
         */
        Enemy(Ogre::SceneManager* sceneManager,
              StateGame* stateGame,
              Type type,
              const Ogre::Vector3& position);
        
        /**
         * Destructor
         * 
         * Libera los recursos ocupados por el enemigo
         */
        ~Enemy();
        
        /**
         * @return tipo de objeto de juego, en este caso devuelve GameObject::ENEMY
         */
        virtual GameObject::Type getType() const;
        
        /**
         * @return tipo de enemigo
         */
        Type getEnemyType() const;
        
        /**
         * @param deltaT tiempo en ms desde el último frame
         * 
         * Actualiza al enemigo según la IA
         */
        virtual void update(Ogre::Real deltaT);
        
        /**
         * @param navigationMesh malla de navegación para realizar las consultas
         * de la búsqueda de caminos.
         */
        void setNavigationMesh(NavigationMesh* navigationMesh);
        
    private:
        Type _type;
        
        // Timer de ataque
        Ogre::Timer* _attackTimer;
        Ogre::Real _attackDelay;
        
        // Búsqueda de caminos
        NavigationMesh* _navigationMesh;
        NavigationMesh::PointPath _path;
        NavigationMesh::PointPath::iterator _pathIt;
        Cell* _currentCell;
        bool _pathActive;
        
        // Barra de vida
        Ogre::BillboardSet* _bbSetLife;
        Ogre::Billboard* _lifeBar;
        Ogre::SceneNode* _lifeNode;
        
        // Sonidos
        SoundFXPtr _enterSound;
        SoundFXPtr _attackSound;
        SoundFXPtr _damagedSound;
        
        void loadGoblinEnemy();
        void loadDemonEnemy();
        void loadGolemEnemy();
        
        void stateDamaged(Ogre::Real deltaT);
        void stateDie(Ogre::Real deltaT);
        void stateAttack(Ogre::Real deltaT);
        void stateIdle(Ogre::Real deltaT);
        void stateRun(Ogre::Real deltaT);
        
        void goToLocation(const Ogre::Vector3& goal, Cell* goalCell);
        
        void updateLifeBar();
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_ENEMY_H_
