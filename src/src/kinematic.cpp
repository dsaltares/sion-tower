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
 
#include <cmath>
#include <iostream>

#include "kinematic.h"

using std::cout;
using std::endl;

Kinematic::Kinematic(const Ogre::Vector3& position,
                     Ogre::Real orientation,
                     const Ogre::Vector3& velocity,
                     Ogre::Real rotation,
                     Ogre::Real maxSpeed): _position(position),
                                           _orientation(orientation),
                                           _velocity(velocity),
                                           _rotation(rotation),
                                           _maxSpeed(maxSpeed) {}
        
void Kinematic::update(const Steering& steering, Ogre::Real deltaT) {
    // Actualizamos posición y orientación
    _position += _velocity * deltaT;
    _orientation += _rotation * deltaT;
    
    // Actualizamos velocidad y rotación
    if (steering.getLinear() == Ogre::Vector3::ZERO)
        _velocity = Ogre::Vector3::ZERO;
    else
        _velocity += steering.getLinear() * deltaT;
        
    _rotation += steering.getAngular() * deltaT;
        
    // Comprobamos si sobrepasamos la velocidad máxima
    if (_velocity.squaredLength() > _maxSpeed * _maxSpeed) {
        _velocity.normalise();
        _velocity *= _maxSpeed;
    }
}


void Kinematic::setOrientationFromVelocity() {
    if (_velocity.squaredLength() > 0)
        _orientation = atan2(_velocity.x, _velocity.z);
}

void Kinematic::lookAt(const Ogre::Vector3& point) {
    Ogre::Vector3 direction = point - _position;
    
    if (direction.squaredLength() > 0)
        _orientation = atan2(direction.x, direction.z);
}
