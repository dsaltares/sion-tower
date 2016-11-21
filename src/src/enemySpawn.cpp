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
 
 
#include "enemySpawn.h"

EnemySpawn::EnemySpawn(Enemy::Type type,
                       const Ogre::Vector3& position,
                       const Ogre::Quaternion& orientation,
                       Ogre::Real time): _type(type), _position(position), _orientation(orientation), _time(time) {}
                       
EnemySpawn::~EnemySpawn() {}

bool EnemySpawn::operator < (const EnemySpawn& spawn) const {
    return _time < spawn._time;
}

bool EnemySpawn::operator == (const EnemySpawn& spawn) const {
    return _time == spawn._time;
}
