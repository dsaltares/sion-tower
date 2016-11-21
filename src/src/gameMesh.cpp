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

#include "gameMesh.h"

using std::cout;
using std::endl;

GameMesh::GameMesh(Ogre::SceneManager* sceneManager): GameObject(sceneManager), _entity(0) {
}

GameMesh::GameMesh(Ogre::SceneManager* sceneManager,
                   const Ogre::String& nodeName,
                   const Ogre::String& entityName,
                   const Ogre::String& entityFile): GameObject(sceneManager) {
    
    // Creamos el SceneNode
    _node = _sceneManager->getRootSceneNode()->createChildSceneNode(nodeName);

    // Creamos la entidad
    _entity = _sceneManager->createEntity(entityName, entityFile);

    // Adjuntamos la entidad al nodo
    _node->attachObject(_entity);
}

GameMesh::GameMesh(Ogre::SceneManager* sceneManager,
                   const Ogre::String& entityFile): GameObject(sceneManager) {

    // Creamos el SceneNode
    _node = _sceneManager->getRootSceneNode()->createChildSceneNode();

    // Creamos la entidad
    _entity = _sceneManager->createEntity(entityFile);

    // Adjuntamos la entidad al nodo
    _node->attachObject(_entity);
}
                 
GameMesh::GameMesh(Ogre::SceneManager* sceneManager,
                 Ogre::SceneNode* sceneNode,
                 Ogre::Entity* entity): GameObject(sceneManager, sceneNode), _entity(entity) {
}
                 
GameMesh::~GameMesh() {
    // Desvinculamos la entidad del nodo
    if (_entity && _node) {
        _node->detachObject(_entity);
        _sceneManager->destroyEntity(_entity);
        _entity = 0;
    }
}

GameObject::Type GameMesh::getType() const {
    return SIMPLEMESH;
}

Ogre::Entity* GameMesh::getEntity() {
    return _entity;
}
        
void GameMesh::changeEntity(const Ogre::String& entityFile) {
    // Si el nodo tenía una entidad
    if (_entity && _node) {
        // Desvinculamos la entidad
        _node->detachObject(_entity);
        
        // Destruimos la entidad
        _sceneManager->destroyEntity(_entity);
        
        // Cargamos la nueva entidad
        _entity = _sceneManager->createEntity(entityFile);
        
        // La adjuntamos al nodo
        _node->attachObject(_entity);
    }
    // Si sólo existía la entidad
    else if (_entity && !_node) {
        // Destruimos la entidad
        _sceneManager->destroyEntity(_entity);
        
        // Cargamos la nueva entidad
        _entity = _sceneManager->createEntity(entityFile);
    }
    // Si sólo existía el nodo
    else if (_node && !_entity) {
        // Cargamos la nueva entidad
        _entity = _sceneManager->createEntity(entityFile);
        
        // La adjuntamos al nodo
        _node->attachObject(_entity);
    }
    // Si no existía ni el nodo ni la entidad
    else {
        // Cargamos la nueva entidad
        _entity = _sceneManager->createEntity(entityFile);
    }
}
