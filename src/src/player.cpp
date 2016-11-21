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
#include <algorithm>
#include <cstdlib>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "game.h"
#include "player.h"
#include "actor.h"
#include "shape.h"
#include "body.h"
#include "collisionManager.h"
#include "stateGame.h"
#include "spell.h"
#include "soundFXManager.h"

/**
 *  @file player.cpp
 *  @author David Saltares Márquez
 *  @date 4-02-2011
 */

using std::cout;
using std::endl;

Player::Player(Ogre::SceneManager* sceneManager,
               StateGame* stateGame,
               Ogre::Camera* camera,
               OIS::Keyboard* keyboard,
               OIS::Mouse* mouse): Actor(sceneManager, stateGame, "personaje.mesh"),
                                   _camera(camera),
                                   _keyboard(keyboard),
                                   _mouse(mouse),
                                   _mana(100),
                                   _timeMana(0.0),
                                   _selectedSpell(Spell::FIRE) {
    cout << "Player::Player()" << endl;


    // Creamos el body
    _shapes.push_back(new OrientedBox("playerShape", Ogre::Vector3(0, 0, 0), Ogre::Vector3(0.35 , 0.7, 0.3), Ogre::Matrix3::IDENTITY));
    _body = new Body(this, _shapes);
    _body->setType(PLAYER);
    
    //_node->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3(0, 1, 0)));

    // Añadimos el body al CollisionManager
    CollisionManager::getSingleton().addBody(_body);

    // Cinemática
    _kinematic.setMaxSpeed(5.0f);
    _kinematic.setOrientation(0.0f);
    _oldPos = _node->getPosition();
    
    // Energía
    _maxEnergy = 100.0f;
    _energy = _maxEnergy;
    _power = 5.0;
    
    // Animaciones
    _animations[IDLE] = _entity->getAnimationState("idle");
    _animations[RUN] = _entity->getAnimationState("run");
    _animations[ATTACK] = _entity->getAnimationState("attack");
    _animations[DAMAGED] = _entity->getAnimationState("damaged");
    _animations[WIN] = _entity->getAnimationState("win");
    _animations[DIE] = _entity->getAnimationState("die");
    
    _currentAnimation = _animations[IDLE];
    _currentAnimation->setEnabled(true);
    _currentAnimation->setLoop(true);
    _currentState = IDLE;
    
    // Gestores de estados
    _updateMethods[IDLE] = boost::bind(&Player::stateIdle, this, _1);
    _updateMethods[RUN] = boost::bind(&Player::stateRun, this, _1);
    _updateMethods[ATTACK] = boost::bind(&Player::stateAttack, this, _1);
    _updateMethods[DAMAGED] = boost::bind(&Player::stateDamaged, this, _1);
    _updateMethods[WIN] = boost::bind(&Player::stateWin, this, _1);
    _updateMethods[DIE] = boost::bind(&Player::stateDie, this, _1);

    // Sonido
    _damagedSound = SoundFXManager::getSingleton().load("playerDamaged.wav");

    // Sincronizamos la parte visual con la física
    synchronizeBody();
}

Player::~Player() {
    cout << "Player::~Player()" << endl;
}

GameObject::Type Player::getType() const {
    return PLAYER;
}

void Player::update(Ogre::Real deltaT) {
    if (_currentAnimation && _currentState != ERASE)
        _currentAnimation->addTime(deltaT * 1.75);
        
    // Buscamos la función de actualización de su estado
    UpdateStateMethods::iterator i = _updateMethods.find(_currentState);
    
    if (i != _updateMethods.end())
        i->second(deltaT);
        
    // Recuperación de maná
    _timeMana += deltaT;
    
    if (_mana < 100 && (int)_timeMana / 5 >= 1) {
        _mana += 10;
        _timeMana = 0;
        
        if (_mana > 100)
            _mana = 100;
    }
    
    // Si hubo cambio de estado, tras esta actualización, el estado anterior
    // es el estado actual.
    _previousState = _currentState; // CUIDADO CON CAMBIAR DENTRO DE ESTADOS
    
    synchronizeBody();
}
    
void Player::stateIdle(Ogre::Real deltaT) {
    if (_keyboard->isKeyDown(OIS::KC_W) ||
            _keyboard->isKeyDown(OIS::KC_A) ||
            _keyboard->isKeyDown(OIS::KC_S) ||
            _keyboard->isKeyDown(OIS::KC_D)) {
        stateRun(deltaT);
        setState(RUN);
    }
}

void Player::stateRun(Ogre::Real deltaT) {
    bool walking = false;
    
    Ogre::Vector3 targetDirection;
    
    // Tomamos la dirección de la cámara
    Ogre::Vector3 forward = _camera->getDirection();
    forward.y = 0;

    // Vector relativo a la cámara, ortogonal hacia su right
    Ogre::Vector3 right(-forward.z, 0, forward.x);

    // Calculamos la dirección objetivo en función de la pulsación de las
    // teclas.
    if (_keyboard->isKeyDown(OIS::KC_W) && _keyboard->isKeyDown(OIS::KC_D)) {
        targetDirection = forward + right;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_W) && _keyboard->isKeyDown(OIS::KC_A)) {
        targetDirection = forward - right;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_S) && _keyboard->isKeyDown(OIS::KC_D)) {
        targetDirection = -forward + right;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_S) && _keyboard->isKeyDown(OIS::KC_A)) {
        targetDirection = -forward - right;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_W)) {
        targetDirection = forward;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_S)) {
        targetDirection = -forward;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_D)) {
        targetDirection = right;
        walking = true;
    }
    else if (_keyboard->isKeyDown(OIS::KC_A)){
        targetDirection = -right;
        walking = true;
    }
    else
        walking = false;

    // Si debemos desplazarnos, aplicamos la traslación y calculamos la
    // rotación a aplicar
    if (walking) {
        // Normalizamos el vector direccion
        targetDirection.normalise();
        
        // Vieja posición
        _oldPos = _node->getPosition();
        
        // Movemos al personaje
        _node->translate(_kinematic.getMaxSpeed() * targetDirection * deltaT, Ogre::Node::TS_WORLD);

        // Rotación
        Ogre::Vector3 initialDirection = Ogre::Vector3(0, 0, 1);
        Ogre::Quaternion q = initialDirection.getRotationTo(targetDirection, Ogre::Vector3(0, 1, 0));
        _node->setOrientation(q);
        _kinematic.setOrientation(q.getYaw().valueRadians());
        _kinematic.setPosition(_node->getPosition());
        _kinematic.setVelocity(_kinematic.getMaxSpeed() * targetDirection);
    } else {
        setState(IDLE);
        _kinematic.setVelocity(Ogre::Vector3::ZERO);
    }
}

void Player::stateAttack(Ogre::Real deltaT) {
    // Si es el primer frame de este estado lanzamos el hechizo
    if (_previousState != _currentState) { 
        /*Ogre::Real orientation = _kinematic.getOrientation();
        Ogre::Quaternion q;
        q.FromAngleAxis(Ogre::Radian(orientation), Ogre::Vector3::UNIT_Y);
        Ogre::Vector3 direction(0.0f, 0.0f, 1.0f);
        direction = q * direction;*/
        
        // Tomamos la dirección a disparar
        Ogre::Real width = Game::getViewPort()->getActualWidth();
        Ogre::Real height = Game::getViewPort()->getActualHeight();
        
        Ogre::Ray ray = _camera->getCameraToViewportRay(_mouse->getMouseState().X.abs / width, _mouse->getMouseState().Y.abs / height);
        std::pair<bool, Ogre::Real> intersection = ray.intersects(Ogre::Plane(Ogre::Vector3(Ogre::Vector3::UNIT_Y), 0.0f));
        if (intersection.first) {
            Ogre::Real d = intersection.second;
            Ogre::Vector3 point = ray.getPoint(d);
            point.y = _kinematic.getPosition().y;
            Ogre::Vector3 direction = point - _kinematic.getPosition();
            direction.normalise();
            
            
            // Miramos a dicha dirección
            _kinematic.lookAt(point);
            synchronizeFromKinematic();
            
            // Recuperación de maná
            _timeMana = 0;
            
            // Disparamos
            _stateGame->addSpell(_selectedSpell, _node->getPosition() + Ogre::Vector3(0, 0.9, 0), direction);
        }
    }
    
    if (_currentAnimation->hasEnded())
        setState(IDLE);
}

void Player::stateDamaged(Ogre::Real deltaT) {
    if (_previousState != _currentState)
        _damagedSound->play();
    
    if (_currentAnimation->hasEnded())
        setState(IDLE);
        
    if (_energy <= 0)
        setState(DIE, false);
}

void Player::stateWin(Ogre::Real deltaT) {
    if (_currentAnimation->hasEnded())
        setState(IDLE);
}

void Player::stateDie(Ogre::Real deltaT) {
    if (_currentAnimation->getTimePosition() >= 0.95 * _currentAnimation->getLength())
        _currentState = ERASE;
}

const Ogre::Vector3 Player::getOldPosition() const {
    return _oldPos;
}
        
void Player::restoreOldPosition() {
    setPosition(_oldPos);
}

int Player::getMana() const {
    return _mana;
}

void Player::useMana(int mana) {
    _mana -= mana;
    _timeMana = 0;
    
    if (mana < 0);
        mana = 0;}

Spell::Type Player::getSpell() const {
    return _selectedSpell;
}

void Player::setSpell(Spell::Type type) {
    _selectedSpell = type;
}
