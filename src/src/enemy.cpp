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
#include <cmath>
#include <vector>

#include "enemy.h"
#include "stateGame.h"
#include "collisionManager.h"
#include "steering.h"
#include "steeringBehaviours.h"
#include "player.h"
#include "soundFXManager.h"


using std::cout;
using std::endl;

Enemy::Enemy(Ogre::SceneManager* sceneManager,
             StateGame* stateGame,
             Type type,
             const Ogre::Vector3& position): Actor(sceneManager, stateGame), _type(type) {
    
    // Creamos el timer de ataque
    _attackTimer = new Ogre::Timer();
    _attackTimer->reset();
    
    // Según tipo, cargar de una forma u otra
    if (type == GOBLIN)
        loadGoblinEnemy();
    else if (type == DEMON)
        loadDemonEnemy();
    else
        loadGolemEnemy();
        
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
    _updateMethods[DAMAGED] = boost::bind(&Enemy::stateDamaged, this, _1);
    _updateMethods[DIE] = boost::bind(&Enemy::stateDie, this, _1);
    _updateMethods[IDLE] = boost::bind(&Enemy::stateIdle, this, _1);
    _updateMethods[ATTACK] = boost::bind(&Enemy::stateAttack, this, _1);
    _updateMethods[RUN] = boost::bind(&Enemy::stateRun, this, _1);
    
    // Tipo de body
    _body->setType(GameObject::ENEMY);
        
    // Añadimos body al collisionManager
    CollisionManager::getSingleton().addBody(_body);
    
    // Establecemos la posicion
    //_node->setPosition(position);
    _kinematic.setPosition(position);
    
    // Sincronizamos todo con kinematic
    synchronizeFromKinematic();
    
    // Reproducimos sonido de entrada
    _enterSound->play();
}

Enemy::~Enemy() {
    // Destruimos el timer de ataque
    delete _attackTimer;
    
    // Destruimos el billboardset
    _lifeNode->detachObject(_bbSetLife);
    _sceneManager->destroyBillboardSet(_bbSetLife);
    _lifeNode->getParent()->removeChild(_lifeNode);
    _sceneManager->destroySceneNode(_lifeNode);
    
    // Eliminamos los updatemethods
    _updateMethods.clear();
}

GameObject::Type Enemy::getType() const {
    return GameObject::ENEMY;
}

Enemy::Type Enemy::getEnemyType() const {
    return _type;
}

void Enemy::update(Ogre::Real deltaT) {
    if (_currentAnimation)
        _currentAnimation->addTime(deltaT);
        
    // Buscamos la función de actualización de su estado
    UpdateStateMethods::iterator i = _updateMethods.find(_currentState);
    
    if (i != _updateMethods.end())
        i->second(deltaT);
        
    // Si hubo cambio de estado, tras esta actualización, el estado anterior
    // es el estado actual.
    _previousState = _currentState; // CUIDADO CON CAMBIAR DENTRO DE ESTADOS

    updateLifeBar();
    
    Ogre::Vector3 position = _kinematic.getPosition();
    position.y = -0.03;
    _kinematic.setPosition(position);
    _kinematic.setOrientationFromVelocity();
    
    //synchronizeBody();    
    synchronizeFromKinematic();
}

void Enemy::loadGoblinEnemy() {
    // Velocidad
    _kinematic.setMaxSpeed(5.0f);
    
    // Energías
    _maxEnergy = 5;
    _energy = _maxEnergy;
    _power = 10;
    
    // Retraso de ataque
    _attackDelay = 3000.0f;
    
    // Body
    _shapes.push_back(new OrientedBox("goblinShape", Ogre::Vector3(0, 0, 0),
                                      Ogre::Vector3(0.35 , 1, 0.3),
                                      Ogre::Matrix3::IDENTITY));
    _body = new Body(this, _shapes);
    
    // Entidad
    changeEntity("goblin.mesh");
    
    // Barra de vida
    _bbSetLife = _sceneManager->createBillboardSet();
    _bbSetLife->setMaterialName("lifeBar");
    _lifeBar = _bbSetLife->createBillboard(Ogre::Vector3(0, 1.6, 0));
    _lifeBar->setDimensions(1, 0.1);
    _lifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
    
    _lifeNode = _node->createChildSceneNode();

    _lifeNode->attachObject(_bbSetLife);
    
    // Sonido
    _enterSound = SoundFXManager::getSingleton().load("goblinEnter.wav");
    _attackSound = SoundFXManager::getSingleton().load("goblinAttack.wav");
    _damagedSound = SoundFXManager::getSingleton().load("goblinDamaged.wav");
}

void Enemy::loadDemonEnemy() {
    // Velocidad
    _kinematic.setMaxSpeed(4.5f);
    
    // Energías
    _maxEnergy = 7;
    _energy = _maxEnergy;
    _power = 15;
    
    // Retraso de ataque
    _attackDelay = 3000.0f;
    
    // Body
    _shapes.push_back(new OrientedBox("demonioShape", Ogre::Vector3(0, 0, 0),
                                      Ogre::Vector3(0.45 , 1.5, 0.4),
                                      Ogre::Matrix3::IDENTITY));
    _body = new Body(this, _shapes);
    
    // Entidad
    changeEntity("demonio.mesh");
    
    // Barra de vida
    _bbSetLife = _sceneManager->createBillboardSet();
    _bbSetLife->setMaterialName("lifeBar");
    _lifeBar = _bbSetLife->createBillboard(Ogre::Vector3(0, 2.2, 0));
    _lifeBar->setDimensions(1, 0.1);
    _lifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
    
    _lifeNode = _node->createChildSceneNode();

    _lifeNode->attachObject(_bbSetLife);
    
    // Sonido
    _enterSound = SoundFXManager::getSingleton().load("demonEnter.wav");
    _attackSound = SoundFXManager::getSingleton().load("demonAttack.wav");
    _damagedSound = SoundFXManager::getSingleton().load("demonDamaged.wav");
}

void Enemy::loadGolemEnemy() {
    // Velocidad
    _kinematic.setMaxSpeed(2.0f);
    
    // Energías
    _maxEnergy = 10;
    _energy = _maxEnergy;
    _power = 20;
    
    // Retraso de ataque
    _attackDelay = 5000.0f;
    
    // Body
    _shapes.push_back(new OrientedBox("golemShape", Ogre::Vector3(0, 0, 0),
                                      Ogre::Vector3(0.9 , 2, 0.8),
                                      Ogre::Matrix3::IDENTITY));
    _body = new Body(this, _shapes);
    
    // Entidad
    changeEntity("golem.mesh");
    
    // Barra de vida
    _bbSetLife = _sceneManager->createBillboardSet();
    _bbSetLife->setMaterialName("lifeBar");
    _lifeBar = _bbSetLife->createBillboard(Ogre::Vector3(0, 3.2, 0));
    _lifeBar->setDimensions(1, 0.1);
    _lifeBar->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
    
    _lifeNode = _node->createChildSceneNode();

    _lifeNode->attachObject(_bbSetLife);
    
    // Sonido
    _enterSound = SoundFXManager::getSingleton().load("golemEnter.wav");
    _attackSound = SoundFXManager::getSingleton().load("golemAttack.wav");
    _damagedSound = SoundFXManager::getSingleton().load("golemDamaged.wav");
}

void Enemy::stateDamaged(Ogre::Real deltaT) {
    if (_previousState != _currentState)
        _damagedSound->play();
    
    if (_energy <= 0)
        setState(DIE, false);
    
    if (_currentAnimation->hasEnded())
        setState(IDLE);
}

void Enemy::stateDie(Ogre::Real deltaT) {
    if (_currentAnimation->hasEnded()) {
        _currentState = ERASE;
    }
}

void Enemy::stateAttack(Ogre::Real deltaT) {
    if (_currentAnimation->hasEnded()) {
        setState(IDLE);
        return;
    }
    
    Ogre::Real time = _currentAnimation->getTimePosition();
    Ogre::Real length = _currentAnimation->getLength();
    
    // Si estamos al 30% de la animación y el timer ha pasado
    if (time / length > 0.3f && _attackTimer->getMilliseconds() > _attackDelay) {
        
        // Reiniciamos timer, ya no puede atacar hasta dentro de un tiempo
        _attackTimer->reset();
        
        Player* player = _stateGame->getPlayer();
        Ogre::Vector3 distance = _kinematic.getPosition() - player->getPosition();
        
        // Si el personaje está a tiro y no está sufriendo daño, hacemos daño
        if (distance.squaredLength() <= 4/*attackRange * _attackRange*/ &&
            player->getState() != DAMAGED && 
            player->getState() != DIE && 
            player->getState() != ERASE) {
            
            player->setState(Actor::DAMAGED, false);
            player->getHurt(_power);
        }
    }
}

void Enemy::stateIdle(Ogre::Real deltaT) {
    Player* player = _stateGame->getPlayer();
    
    // Hacemos algo
    // Si el personaje está cerca, atacamos
    Ogre::Vector3 distance = _node->getPosition() - player->getKinematic().getPosition();
    
    
   if (distance.length() <= 2) {
        if (_attackTimer->getMilliseconds() >= _attackDelay) {
            // Atacamos
            _kinematic.lookAt(_stateGame->getPlayer()->getPosition());
            setState(ATTACK, false);
            // Reproducimos sonido
            _attackSound->play();
        }
    }
    else {
        // Comenzamos un camino hasta el personaje
        setState(RUN);
        goToLocation(_stateGame->getPlayer()->getPosition(),
                     _navigationMesh->findCell(_stateGame->getPlayer()->getPosition()));
    }
}

void Enemy::stateRun(Ogre::Real deltaT) {     
    Player* player = _stateGame->getPlayer();
    
    Steering steering;
    
    CollisionAvoidance avoidance(&_kinematic, _stateGame->getEnemies(), this);
    avoidance.getSteering(steering);
    
    if (steering.getLinear() == Ogre::Vector3::ZERO) {
    
        Ogre::Vector3 distance = player->getPosition() - _path.back();
        
        if (distance.length() > 2) {
            goToLocation(_stateGame->getPlayer()->getPosition(),
                         _navigationMesh->findCell(_stateGame->getPlayer()->getPosition()));
                         
            return;
        }
                    
        distance = player->getPosition() - _kinematic.getPosition();

        if (distance.length() < 2.0f && _attackTimer->getMilliseconds() >= _attackDelay) {
            _kinematic.setVelocity(Ogre::Vector3::ZERO);
            _kinematic.lookAt(_stateGame->getPlayer()->getPosition());
            setState(ATTACK, false);
            // Reproducimos sonido
            _attackSound->play();
            return;
        }

        FollowPath followPath(&_kinematic, &_path);
        followPath.getSteering(steering);
    }
    
    _kinematic.update(steering, deltaT);
}

void Enemy::setNavigationMesh(NavigationMesh* navigationMesh) {
    _navigationMesh = navigationMesh;
    Ogre::Vector3 position = _node->getPosition();
    position.y = 0;
    _currentCell = _navigationMesh->findCell(position);
}

void Enemy::goToLocation(const Ogre::Vector3& goal, Cell* goalCell) {
    _currentCell = _navigationMesh->findCell(_node->getPosition());
    
    _pathActive = _navigationMesh->buildPath(_path, _node->getPosition(), goal, _currentCell, goalCell);
  	
    if(_pathActive)
		_pathIt = _path.begin();
    else
        setState(IDLE);
}

void Enemy::updateLifeBar() {
    Ogre::Real ratio = (Ogre::Real)_energy / (Ogre::Real)_maxEnergy;
    
    if (ratio < 0.0f)
        ratio = 0.0f;
        
    _lifeBar->setTexcoordRect((1.0f - ratio) / 2.0f, 0.0f, 0.5f + (1.0f - ratio) / 2.0f, 1.0f);
}
