/*
 * This file es part of SionTower.
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
 *  @file collisionManager.cpp
 *  @author David Saltares Márquez
 *  @date 03-02-2011
 */


#include <iostream>

#include "collisionManager.h"

using std::cout;
using std::endl;

template<> CollisionManager* Ogre::Singleton<CollisionManager>::ms_Singleton = 0;

CollisionManager::CollisionManager() {
    cout << "CollisionManager::ColisionManager()" << endl;

    // Registramos los tests
    Shape::configureCollisionDispatching();

    // Creamos la esferas de partición
    _sphereA = new Sphere("spherePartitionA", Ogre::Vector3(0, 0, 0), 2.5f);
    _sphereB = new Sphere("spherePartitionB", Ogre::Vector3(0, 0, 0), 2.5f);
}

CollisionManager::~CollisionManager() {
    cout << "CollisionManager::~ColisionManager()" << endl;
    
    delete _sphereA;
    delete _sphereB;
}

CollisionManager& CollisionManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

CollisionManager* CollisionManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

void CollisionManager::addBody(Body* body) {
    _bodies.push_back(body);
}

bool CollisionManager::removeBody(Body* body) {
    // Lo eliminamos de la lista de bodies
    bool erased = false;
    std::list<Body*>::iterator i;

    for (i = _bodies.begin(); i != _bodies.end(); ++i){
        if (*i == body){
            _bodies.erase(i);
            erased = true;
            break;
        }
    }

    // Lo eliminamos de la tabla de cuerpos en colisión
    _collidingBodies.erase(body);
    
    CollidingBodies::iterator j;
    for (j = _collidingBodies.begin(); j != _collidingBodies.end(); ++j) 
        j->second.erase(body);
    

    return erased;
}

void CollisionManager::removeAllBodies() {
    _bodies.clear();
    _collidingBodies.clear();
}

void CollisionManager::addCollisionCallback(int typeA,
                                            int typeB,
                                            CollisionCallback callback,
                                            CallbackType callbackType) {
    switch(callbackType) {
        case BEGINCOLLISION:
            _beginCallbackTable[typeA][typeB] = callback;
            _beginCallbackTable[typeB][typeA] = callback;
            break;
        case COLLIDING:
            _inCallbackTable[typeA][typeB] = callback;
            _inCallbackTable[typeB][typeA] = callback;
            break;
        case ENDCOLLISION:
            _endCallbackTable[typeA][typeB] = callback;
            _endCallbackTable[typeB][typeA] = callback;
            break;
        case ALL:
            break;
    }
}

bool CollisionManager::removeCollisionCallback(int typeA, int typeB, CallbackType callbackType) {


    
    CollisionCallbackTable::iterator i;
    size_t numErased = 0;
    
    switch(callbackType) {
        case BEGINCOLLISION:
            // Si existe una entrada para A, eliminamos su relación con B
            i = _beginCallbackTable.find(typeA);

            if (i != _beginCallbackTable.end())
                numErased = i->second.erase(typeB);

            // Si existe una entrada para B, eliminamos su relacion con A
            i = _beginCallbackTable.find(typeB);

            if (i != _beginCallbackTable.end())
                i->second.erase(typeA);
            break;
        case COLLIDING:
            // Si existe una entrada para A, eliminamos su relación con B
            i = _inCallbackTable.find(typeA);

            if (i != _inCallbackTable.end())
                numErased = i->second.erase(typeB);

            // Si existe una entrada para B, eliminamos su relacion con A
            i = _inCallbackTable.find(typeB);

            if (i != _inCallbackTable.end())
                i->second.erase(typeA);
            break;
        case ENDCOLLISION:
            // Si existe una entrada para A, eliminamos su relación con B
            i = _endCallbackTable.find(typeA);

            if (i != _endCallbackTable.end())
                numErased = i->second.erase(typeB);

            // Si existe una entrada para B, eliminamos su relacion con A
            i = _endCallbackTable.find(typeB);

            if (i != _endCallbackTable.end())
                i->second.erase(typeA);
            break;
        case ALL:
            numErased += removeCollisionCallback(typeA, typeB, BEGINCOLLISION);
            numErased += removeCollisionCallback(typeA, typeB, COLLIDING);
            numErased += removeCollisionCallback(typeA, typeB, ENDCOLLISION);
            break;
    }
    
    return numErased > 0;
}

void CollisionManager::removeAllCollisionCallbacks(CallbackType type) {
    switch(type) {
        case BEGINCOLLISION:
            _beginCallbackTable.clear();
            break;
        case COLLIDING:
            _inCallbackTable.clear();
            break;
        case ENDCOLLISION:
            _endCallbackTable.clear();
        case ALL:
            _beginCallbackTable.clear();
            _inCallbackTable.clear();
            _endCallbackTable.clear();
            break;
    }
}

void CollisionManager::checkCollisions() {
    // Cruzamos los Bodies y comprobamos:
    // - Existe un collisionCallback para su tipo
    // - Están a una distancia razonable (sphere test)
    // - Test de colisión profundo
    // - Llamada al callback

    std::list<Body*>::iterator i;
    std::list<Body*>::iterator j;
    CollisionCallback collisionCallback;

    // Cruzamos los bodies
    for (i = _bodies.begin(); i != _bodies.end(); ++i) {
        j = i;
        ++j;
        for (; j != _bodies.end(); ++j) {
            // Posicionamos las esferas
            _sphereA->setCenter((*i)->getPosition());
            _sphereB->setCenter((*j)->getPosition());

            // Si estaban colisionando
            if (wereColliding(*i, *j)) {

                // Si hay colision
                if (Shape::getCollision(_sphereA, _sphereB) && Body::getCollision(*i, *j)) {
                    // Si hay inCallback
                    if (existsCallback((*i)->getType(), (*j)->getType(), COLLIDING, &collisionCallback))
                        // Llamar inCallback
                        collisionCallback(*i, *j);
                }
                // Si no hay colisión
                else {
                    // eliminar de colliding
                    CollidingBodies::iterator k = _collidingBodies.find(*i);
                    k->second.erase(*j);
                    if (k->second.size() == 0)
                        _collidingBodies.erase(k);

                    k = _collidingBodies.find(*j);
                    k->second.erase(*i);
                    if (k->second.size() == 0)
                        _collidingBodies.erase(k);
                    
                    
                
                    // Si hay endCallback
                    if (existsCallback((*i)->getType(), (*j)->getType(), ENDCOLLISION, &collisionCallback))
                        // llamar endCallback 
                        collisionCallback(*i, *j);
                }
            }
            // Si no estaban colisionando
            else {
                // Si hay colisión
                if (Shape::getCollision(_sphereA, _sphereB) && Body::getCollision(*i, *j)) {
                    // insertar en colliding
                    _collidingBodies[*i].insert(*j);
                    _collidingBodies[*j].insert(*i);
                    
                    // Si hay beginCallback
                    if (existsCallback((*i)->getType(), (*j)->getType(), BEGINCOLLISION, &collisionCallback))
                        // llamar endCallback 
                        collisionCallback(*i, *j);
                }
            }
        }
    }
}



bool CollisionManager::existsCallback(int typeA,
                                      int typeB,
                                      CallbackType callbackType,
                                      CollisionCallback* collisionCallback) {
    CollisionCallbackTable::iterator i;
    boost::unordered_map<int, CollisionCallback>::iterator j;

    switch(callbackType) {
        case BEGINCOLLISION:
            i = _beginCallbackTable.find(typeA);
            
            if (i == _beginCallbackTable.end())
                return false;

            // Bucamos una entrada en typeA para typeB
            j = i->second.find(typeB);

            break;
        case COLLIDING:
            i = _inCallbackTable.find(typeA);
            
            if (i == _inCallbackTable.end())
                return false;

            // Bucamos una entrada en typeA para typeB
            j = i->second.find(typeB);

            break;
        case ENDCOLLISION:
            i = _endCallbackTable.find(typeA);
            
            if (i == _endCallbackTable.end())
                return false;

            // Bucamos una entrada en typeA para typeB
            j = i->second.find(typeB);
            break;
        case ALL:
            break;
    }
    
    // Comprobamos si existe
    if (j == i->second.end())
        return false;

    // Hemos encontrado el callback
    *collisionCallback = j->second;
    return true;
}


bool CollisionManager::wereColliding(Body* bodyA, Body* bodyB) {
    CollidingBodies::iterator i = _collidingBodies.find(bodyA);

    if (i == _collidingBodies.end())
        return false;
    
    if (i->second.find(bodyB) == i->second.end())
        return false;

    return true;
}
