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


#include <iostream>

#include "actor.h"
#include "stateGame.h"


using std::cout;
using std::endl;

Actor::Actor(Ogre::SceneManager* sceneManager,
             StateGame* stateGame): GameMesh(sceneManager), _stateGame(stateGame) {
    // Inicializamos energías
    _maxEnergy = 0;
    _energy = 0;
    _power = 0;
    
    // Inicializamos estados y animación
    _currentAnimation = 0;
    _currentState = IDLE;
    _previousState = IDLE;
}

Actor::Actor(Ogre::SceneManager* sceneManager,
             StateGame* stateGame,
             const Ogre::String& nodeName,
             const Ogre::String& entityName,
             const Ogre::String& entityFile): GameMesh(sceneManager, nodeName, entityName, entityFile), _stateGame(stateGame) {
    
    // Inicializamos energías
    _maxEnergy = 0;
    _energy = 0;
    _power = 0;

    
    // Inicializamos estados y animación
    _currentAnimation = 0;
    _currentState = IDLE;
    _previousState = IDLE;
}

Actor::Actor(Ogre::SceneManager* sceneManager,
             StateGame* stateGame,
             const Ogre::String& entityFile): GameMesh(sceneManager, entityFile), _stateGame(stateGame) {
    // Inicializamos energías
    _maxEnergy = 0;
    _energy = 0;
    _power = 0;
    
    // Inicializamos estados y animación
    _currentAnimation = 0;
    _currentState = IDLE;
    _previousState = IDLE;
}

Actor::~Actor() {}
        
void Actor::update(Ogre::Real deltaT) {
    // Actualizamos la animación
    if (_currentAnimation)
        _currentAnimation->addTime(deltaT);
        
    // Buscamos la función de actualización de su estado
    UpdateStateMethods::iterator i = _updateMethods.find(_currentState);
    
    if (i != _updateMethods.end())
        i->second(deltaT);
}
        
int Actor::getEnergy() const {
    return _energy;
}

int Actor::getMaxEnergy() const {
    return _maxEnergy;
}

void Actor::setEnergy(int energy) {
    _energy = energy;
}

void Actor::getHurt(int damage) {
    _energy -= damage;
    
    if (_energy < 0)
        _energy = 0;
}
        
int Actor::getPower() const {
    return _power;
}

void Actor::setPower(int power) {
    _power = power;
}

Actor::State Actor::getState() {
    return _currentState;
}

void Actor::setState(State state, bool loop) {
    // Cambiamos estado
    _previousState = _currentState;
    _currentState = state;
    
    // Paramos animación
    _currentAnimation->setEnabled(false);
    
    // Activamos nueva animación
    _currentAnimation = _animations[_currentState];
    _currentAnimation->setEnabled(true);
    _currentAnimation->setLoop(loop);
    _currentAnimation->setTimePosition(0.0f);
}

Kinematic& Actor::getKinematic() {
    return _kinematic;
}

void Actor::setKinematic(const Kinematic& kinematic) {
    _kinematic = kinematic;
}

void Actor::synchronizeFromKinematic() {
    // Posición
    _node->setPosition(_kinematic.getPosition());
    
    // Orientación
    Ogre::Quaternion q;
    q.FromAngleAxis(Ogre::Radian(_kinematic.getOrientation()), Ogre::Vector3::UNIT_Y);
    _node->setOrientation(q);
    
    // Sincronizamos body
    synchronizeBody();
}

void Actor::setOrientation(const Ogre::Quaternion& orientation) {
    if (_node)
        _node->setOrientation(orientation);

    if (_body)
        _body->setOrientation(orientation);
        
    _kinematic.setOrientation(orientation.getYaw().valueRadians());
}

void Actor::setPosition(const Ogre::Vector3& position) {
    if (_node)
        _node->setPosition(position);
    
    if (_body)
        _body->setPosition(position);
        
    _kinematic.setPosition(position);
}
