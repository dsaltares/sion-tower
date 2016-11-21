/*
 * This file is part of SionTower.
 *
 * 
 * David Saltares Marquez (C) 2011
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

#include "gameStats.h"

GameStats::GameStats(const Ogre::String& levelCode): _levelCode(levelCode) {
    // Enemigos
    _enemiesKilled = 0;
    _pointsByEnemy[Enemy::GOBLIN] = 10;
    _pointsByEnemy[Enemy::DEMON] = 15;
    _pointsByEnemy[Enemy::GOLEM] = 20;
    _pointsByEnemy[Enemy::SPIDER] = 20;
    _enemiesPoints = 0;
    
    // Mana
    _manaUsed = 0;
    _manaMax = 500;
    _manaPoints = 100;
    
    // Vida
    _life = 100;
    _lifeMax = 100;
    _lifePoints = 100;
    
    // Tiempo
    _time = 0;
    _timeMax = 300;
    _timePoints = 100;
}

const Ogre::String& GameStats::getLevelCode() const {
    return _levelCode;
}

int GameStats::getEnemiesKilled() {
    return _enemiesKilled;
}

int GameStats::getEnemiesPoints() const  {
    return _enemiesPoints;
}

int GameStats::getManaUsed() const {
    return _manaUsed;
}

int GameStats::getManaPoints() const {
    return _manaPoints;
}

int GameStats::getLife() const {
    return _life;
}

int GameStats::getLifePoints() const {
    return _lifePoints;
}

int GameStats::getTime() const {
    return _time;
}

int GameStats::getTimePoints() {
    return _timePoints;
}

int GameStats::getTotal() const {
    return _enemiesPoints + _manaPoints + _lifePoints + _timePoints;
}

void GameStats::killEnemy(Enemy::Type type) {
    ++_enemiesKilled;
    _enemiesPoints += _pointsByEnemy[type];
}

void GameStats::useMana(int mana) {
    _manaUsed += mana;
    _manaPoints = 100 - ((Ogre::Real)_manaUsed / (Ogre::Real)_manaMax) * 100;
}

void GameStats::setLife(int life) {
    _life = life;
    _lifePoints = ((Ogre::Real)_life / (Ogre::Real)_lifeMax) * 100;
}

void GameStats::setTime(Ogre::Real time) {
    _time = time;
    _timePoints = 100 - (_time / _timeMax) * 100;
}
