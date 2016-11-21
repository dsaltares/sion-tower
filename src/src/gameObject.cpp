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



/**
 *  @file gameObject.cpp
 *  @author David Saltares Márquez
 *  @date 3-02-2011
 */


#include <iostream>

#include "gameObject.h"
#include "body.h"
#include "shape.h"
#include "collisionManager.h"

using std::cout;
using std::endl;


GameObject::GameObject(Ogre::SceneManager* sceneManager): _sceneManager(sceneManager), _body(0) {
    // Creamos el SceneNode
    _node = _sceneManager->getRootSceneNode()->createChildSceneNode();
}

GameObject::GameObject(Ogre::SceneManager* sceneManager,
                       Ogre::SceneNode* sceneNode): _sceneManager(sceneManager), _node(sceneNode), _body(0){
}

GameObject::~GameObject() {

    // Destruimos el nodo
    if (_node) {
        _node->getParentSceneNode()->removeChild(_node);
        _sceneManager->destroySceneNode(_node);
        _node = 0;
    }

    // Desvinculamos al body del CollisionManager
    // Destruimos el body
    if (_body) {
        CollisionManager::getSingleton().removeBody(_body);
        delete _body;
        _body = 0;
    }

    // Destruimos todas las shapes
    std::vector<Shape*>::iterator i;
    for (i = _shapes.begin(); i != _shapes.end(); ++i)
        if (*i)
           delete *i;
}

Body* GameObject::getBody() {
    return _body;
}

void GameObject::setBody(Body* body) {
    // Eliminamos el body anterior
    // Lo desvinculamos del collisionManager
    if (_body) {
        CollisionManager::getSingleton().removeBody(_body);
        delete _body;
    }

    _body = body;

    // Sustituimos las Shapes
    std::vector<Shape*>::iterator i;
    for (i = _shapes.begin(); i != _shapes.end(); ++i)
        if (*i)
           delete *i;
    
    _shapes.clear();

    if (_body)
        _shapes = _body->getShapes();

    // Sincronizamos
    synchronizeBody();
}

Ogre::SceneNode* GameObject::getSceneNode() {
    return _node;
}

const Ogre::Vector3& GameObject::getPosition() {
    return (_body)? _body->getPosition() : Ogre::Vector3::ZERO;
}

void GameObject::setPosition(const Ogre::Vector3& position) {
    if (_node)
        _node->setPosition(position);
    
    if (_body)
        _body->setPosition(position);
}

const Ogre::Vector3& GameObject::getScale() {
    return (_body)? _body->getScale() : Ogre::Vector3::UNIT_SCALE;
}

void GameObject::setScale(const Ogre::Vector3& scale) {
    if (_node)
        _node->setScale(scale);

    if (_body)
        _body->setScale(scale);
}

const Ogre::Quaternion& GameObject::getOrientation() {
    return (_body)? _body->getOrientation() : Ogre::Quaternion::IDENTITY;
}

void GameObject::setOrientation(const Ogre::Quaternion& orientation) {
    if (_node)
        _node->setOrientation(orientation);

    if (_body)
        _body->setOrientation(orientation);
}


void GameObject::update(Ogre::Real deltaT) {}


void GameObject::synchronizeBody() {
    // Sincronizamos el body con el node
    if (_body && _node) {
        _body->setPosition(_node->getPosition());
        _body->setScale(_node->getScale());
        _body->setOrientation(_node->getOrientation());
    }
}

void GameObject::synchronizeSceneNode() {
    // Sincronizamos el node con el body
    if (_body && _node) {
        _node->setPosition(_body->getPosition());
        _node->setOrientation(_body->getOrientation());
        _node->setScale(_body->getScale());
    }
}
