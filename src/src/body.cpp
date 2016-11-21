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
 *  @file body.cpp
 *  @author David Saltares Márquez
 *  @date 1-02-2011
 */

#include <iostream>

#include "shape.h"
#include "body.h"
#include "gameObject.h"

using std::cout;
using std::endl;

Body::Body(GameObject* gameObject,
           const std::vector<Shape*>& shapes,
           const Ogre::Vector3& position,
           const Ogre::Vector3& scale,
           const Ogre::Quaternion& orientation,
           int type): _gameObject(gameObject), _shapes(shapes), _position(position), _scale(scale), _orientation(orientation), _type(type) {
    
    // Aplicamos la transformación a las worldShapes
    createWorldShapes(); 
}

Body::Body(GameObject* gameObject,
           const Ogre::Vector3& position,
           const Ogre::Vector3& scale,
           const Ogre::Quaternion& orientation,
           int type): _gameObject(gameObject), _position(position), _scale(scale), _orientation(orientation), _type(type) {
}

Body::~Body() {
    std::vector<Shape*>::iterator i;

    // Destruimos las world shapes
    for (i = _worldShapes.begin(); i != _worldShapes.end(); ++i)
        delete (*i);
}

GameObject* Body::getGameObject() {
    return _gameObject;
}

void Body::setGameObject(GameObject* gameObject) {
    _gameObject = gameObject;
}

const std::vector<Shape*> Body::getShapes() const {
    return _shapes;
}

const std::vector<Shape*> Body::getWorldSpaceShapes() const {
    return _worldShapes;
}

void Body::addShape(Shape* shape) {
    // Añadimos la forma en local space
    _shapes.push_back(shape);

    // Creamos una nueva forma en world space
    _worldShapes.push_back(shape->getTransformedCopy(_position, _scale, _orientation));
}

bool Body::removeShape(Shape* shape) {
    std::vector<Shape*>::iterator i;
    std::vector<Shape*>::iterator j = _worldShapes.begin();

    for (i = _shapes.begin(); i != _shapes.end(); ++i) {
        if (*i == shape) {
            // Eliminamos i del vector shapes
            _shapes.erase(i);

            // Destruimos j y la eliminamos del vector worldShapes
            delete (*j);
            _worldShapes.erase(j);

            return true;
        }

        ++j;
    }

    return false;
}

bool Body::removeShape(const Ogre::String& name) {
    std::vector<Shape*>::iterator i;
    std::vector<Shape*>::iterator j = _worldShapes.begin();

    for (i = _shapes.begin(); i != _shapes.end(); ++i) {
        if ((*i)->getName() == name) {
            // Eliminamos i del vector shapes
            _shapes.erase(i);

            // Destruimos j y la eliminamos del vector worldshapes
            delete (*j);
            _worldShapes.erase(j);

            return true;
        }

        ++j;
    }

    return false;
}


Ogre::Matrix4 Body::getTransform() const {
    Ogre::Matrix4 transform;

    transform.makeTransform(_position, _scale, _orientation);

    return transform;
}

void Body::setTransform(const Ogre::Matrix4& transform) {
    transform.decomposition(_position, _scale, _orientation);
    updateWorldShapes();
}

const Ogre::Vector3& Body::getPosition() const {
    return _position;
}

void Body::setPosition(const Ogre::Vector3& position) {
    _position = position;
    updateWorldShapes();
}

const Ogre::Vector3& Body::getScale() const {
    return _scale;
}

void Body::setScale(const Ogre::Vector3& scale) {
    _scale = scale;
    updateWorldShapes();
}

const Ogre::Quaternion& Body::getOrientation() const {
    return _orientation;
}

void Body::setOrientation(const Ogre::Quaternion& orientation) {
    _orientation = orientation;
    updateWorldShapes();
}

bool Body::getCollision(Body* bodyA, Body* bodyB) {
    std::vector<Shape*>::const_iterator i;
    std::vector<Shape*>::const_iterator j;

    // Cruzamos las formas de cada cuerpo comprobando colisiones
    for (i = bodyA->_worldShapes.begin(); i != bodyA->_worldShapes.end(); ++i)
        for (j = bodyB->_worldShapes.begin(); j != bodyB->_worldShapes.end(); ++j) 
            if (Shape::getCollision(*i, *j))
                return true;

    return false;
}

void Body::createWorldShapes() {
    std::vector<Shape*>::iterator i;

    for (i = _shapes.begin(); i != _shapes.end(); ++i) {
        _worldShapes.push_back((*i)->getTransformedCopy(_position, _scale, _orientation));
    }
}

int Body::getType() const {
    return _type;
}

void Body::setType(int type) {
    _type = type;
}

void Body::updateWorldShapes() {
    std::vector<Shape*>::iterator i = _shapes.begin();
    std::vector<Shape*>::iterator j = _worldShapes.begin();

    // Recorremos las shapes básicas y actualizamos las worldShapes
    for (; i != _shapes.end(); ++i) {
        // Aplicamos la transformación con respecto a la original
        (*j)->applyTransform(*i, _position, _scale, _orientation);
        ++j;
    }
}
