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

#include <libintl.h>
#include <locale.h>

#include "spell.h"
#include "body.h"
#include "shape.h"
#include "collisionManager.h"
#include "soundFXManager.h"

#define _(x) gettext(x)

/**
 *  @file spell.cpp
 *  @author David Saltares Márquez
 *  @date 23-05-2011
 */

using std::cout;
using std::endl;

// Contador de partículas
int Spell::_particleCounter = 0;

// Datos de hechizos
boost::unordered_map<Spell::Type, Spell::SpellData> Spell::_spells;

Spell::Spell(Ogre::SceneManager* sceneManager,
             Type type,
             const Ogre::Vector3& position,
             const Ogre::Vector3& direction): GameObject(sceneManager),
                                              _type(type),
                                              _state(MOVE),
                                              _direction(direction),
                                              _soundExplode(0),
                                              _soundCast(0) {
    cout << "Spell::Spell()" << endl;
    
    _node->setPosition(position);
    
    // Creamos spell según tipo
    _spellData = _spells[type];
    
    // Sistema de partículas MOVE
    char number[6];
    sprintf(number, "%5d", _particleCounter);
    ++_particleCounter;
    _particleMove = _sceneManager->createParticleSystem(Ogre::String("particle") + Ogre::String(number), _spellData.particleMove);
    
    // Sistema de partículas EXPLODE
    sprintf(number, "%5d", _particleCounter);
    ++_particleCounter;
    _particleExplode = _sceneManager->createParticleSystem(Ogre::String("particle") + Ogre::String(number), _spellData.particleExplode);
    
    // Adjuntamos el sistema de partículas MOVE al nodo
    _node->attachObject(_particleMove);
    
    // Efectos de sonido
    _soundExplode = SoundFXManager::getSingleton().load(_spellData.soundExplode);
    _soundCast = SoundFXManager::getSingleton().load(_spellData.soundCast);
    
    // Shapes y body
    std::vector<Shape*> shapes;
    shapes.push_back(new Sphere("esfera", Ogre::Vector3::ZERO, 0.2f));
    _body = new Body(this, shapes);
    _body->setType(SPELL);
    CollisionManager::getSingleton().addBody(_body);
        
    // Creamos el timer para la explosión
    _explosionTimer = new Ogre::Timer();
    
    // Sincronizamos
    synchronizeBody();
    
    // Reproducimos el sonido de disparo
    _soundCast->play();
}

Spell::~Spell() {
    cout << "Spell::~Spell()" << endl;
    
    // El body y el node se eliminan en ~GameObject
    
    // Eliminar timer
    delete _explosionTimer;
    
    // Eliminar sistemas de partículas
    _sceneManager->destroyParticleSystem(_particleExplode);
    _sceneManager->destroyParticleSystem(_particleMove);
}

void Spell::initialiseSpellData() {
    SpellData data;
    
    // Hechizo FIRE
    data.name = _("Fire ball");
    data.description = _("Casts a huge fire ball to burn\nyour enemies");
    data.power = 2;
    data.mana = 3;
    data.speed = 8;
    data.explosionTime = 1500;
    data.particleExplode = "fireExplosion";
    data.particleMove = "fire";
    data.soundCast = "fireCast.wav";
    data.soundExplode = "fireExplosion.wav";
    _spells[FIRE] = data;
    
    // Hechizo GEA
    data.name = _("Gea's fury");
    data.description = _("Casts the fury of nature\nitself");
    data.power = 3;
    data.mana = 6;
    data.speed = 8;
    data.explosionTime = 1500;
    data.particleExplode = "geaExplosion";
    data.particleMove = "gea";
    data.soundCast = "geaCast.wav";
    data.soundExplode = "geaExplosion.wav";
    _spells[GEA] = data;
                    
    // Hechizo BLIZZARD
    data.name = _("Blizzard");
    data.description = _("Casts ice projectiles, sharp\nas razors");
    data.power = 4;
    data.mana = 8;
    data.speed = 8;
    data.explosionTime = 1500;
    data.particleExplode = "blizzardExplosion";
    data.particleMove = "blizzard";
    data.soundCast = "blizzardCast.wav";
    data.soundExplode = "blizzardExplosion.wav";
    _spells[BLIZZARD] = data;
}

GameObject::Type Spell::getType() const {
    return SPELL;
}

Spell::Type Spell::getSpellType() const {
    return _type;
}

int Spell::getPower() const {
    return _spellData.power;
}

Ogre::Real Spell::getSpeed() const {
    return _spellData.speed;
}

const Ogre::Vector3& Spell::getDirection() const {
    return _direction;
}

Spell::State Spell::getState() const {
    return _state;
}

const Ogre::String& Spell::getName() const {
    return _spellData.name;
}

const Ogre::String& Spell::getDescription() const {
    return _spellData.description;
}

void Spell::explode() {
    if (_state != EXPLODE) {
        // Reproducimos el sonido de explosión
        _soundExplode->play();
        
        // Desvinculamos el sistema de partículas del estado MOVE del nodo
        _node->detachObject(_particleMove);
        
        // Adjuntamos el sistema de partículas del estado EXPLODE al nodo
        _node->attachObject(_particleExplode);
        
        // Cambiamos el estado
        _state = EXPLODE;
        
        // Lanzamos el timer
        _explosionTimer->reset();   
    }
}

void Spell::update(Ogre::Real deltaT) {
    if (_state == MOVE) {
        _node->setPosition(_node->getPosition() + _spellData.speed * _direction * deltaT);
        synchronizeBody();
    }
    else if (_state == EXPLODE) {
        if (_explosionTimer->getMilliseconds() >= _spellData.explosionTime) {
            // Desvincula el sistema de partículas del nodo
            _node->detachObject(_particleExplode);
            
            // Cambia el estado
            _state = ERASE;
        }
    }
}

int Spell::getMana() const {
    return _spellData.mana;
}

int Spell::getMana(Type type) {
    return _spells[type].mana;
}

Spell::SpellData Spell::getSpellData(Type type) {
    return _spells[type];
}
