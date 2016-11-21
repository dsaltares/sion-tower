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

#include "steering.h"

Steering::Steering(const Ogre::Vector3& linear,
                   Ogre::Real angular): _linear(linear),
                                        _angular(angular) {}



// Operadores
bool Steering::operator == (const Steering& other) {
    return _angular == other._angular && _linear == other._linear;
}

Steering& Steering::operator + (const Steering& other) {
    return Steering(*this) += other;
}

Steering& Steering::operator += (const Steering& other) {
    _linear += other._linear;
    _angular += other._angular;
    
    return *this;
}

Steering& Steering::operator * (Ogre::Real f) {
    return Steering(*this) *= f;
}

Steering& Steering::operator *= (Ogre::Real f) {
    _linear *= f;
    _angular *= f;
    
    return *this;
}

Steering& Steering::operator / (Ogre::Real f) {
    return Steering(*this) /= f;
}

Steering& Steering::operator /= (Ogre::Real f) {
    _linear /= f;
    _angular /= f;
    
    return *this;
}
