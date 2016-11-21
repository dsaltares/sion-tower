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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_GAMEMESH_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_GAMEMESH_H_

#include <OGRE/Ogre.h>

#include "gameObject.h"

//! Clase hija de GameObject que modela elementos del juego con una malla poligonal

/**
 * @author David Saltares Márquez
 * @date 25-05-2011
 * 
 * La clase GameMesh hereda de GameObject y añade una referencia a Ogre::Entity
 * para modelar elementos de juego con una malla poligonal asociada. Puede
 * utilizarse directamente para partes del escenario o utilizarla como
 * base para modelar actores (clase Actor).
 */
class GameMesh: public GameObject {
    public:
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena
         */
        GameMesh(Ogre::SceneManager *sceneManager);
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena
         * @param nodeName nombre del nodo
         * @param entityName nombre de la entidad
         * @param entityFile nombre del fichero .mesh que contiene la información de la malla
         */
        GameMesh(Ogre::SceneManager* sceneManager,
                 const Ogre::String& nodeName,
                 const Ogre::String& entityName,
                 const Ogre::String& entityFile);
                 
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena
         * @param entityFile nombre del fichero .mesh que contiene la información de la malla
         */
        GameMesh(Ogre::SceneManager* sceneManager,
                 const Ogre::String& entityFile);
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena
         * @param sceneNode nodo que contendrá la entiddad
         * @param entity entidad con la malla poligonal
         */
        GameMesh(Ogre::SceneManager* sceneManager,
                 Ogre::SceneNode* sceneNode,
                 Ogre::Entity* entity);
        
        /**
         * Destructor
         * 
         * Libera la memoria de GameMesh, destruye la entidad y el nodo
         */
        virtual ~GameMesh();
        
        /**
         * @return tipo de objeto de juego. Devuelve SIMPLEMESH pero puede
         * sobrecargarse por una clase hija si es necesario.
         */
        virtual Type getType() const;
        
        /**
         * @return entidad actual que contiene el nodo
         */
        Ogre::Entity* getEntity();
        
        /**
         * @param entityFile nuevo fichero .mesh con la nueva entidad que contendrá el nodo
         */
        void changeEntity(const Ogre::String& entityFile);
    protected:
        Ogre::Entity* _entity;
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_GAMEMESH_H_
