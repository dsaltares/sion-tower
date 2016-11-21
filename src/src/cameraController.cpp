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

#include <iostream>

#include "cameraController.h"
#include "player.h"

using std::cout;
using std::endl;

CameraController::CameraController(Ogre::Camera* camera,
                                   OIS::Mouse* mouse,
                                   Player* player): _camera(camera),
                                                    _mouse(mouse),
                                                    _player(player) {
    
    // Inicializamos parámetros
    _height = 2.0f;
    _travellingRadius = 2.5f;
    _speed = 5.0f;
    _travelling = false;
    _center = _player->getPosition() + Ogre::Vector3::UNIT_Y * _height;
    _targetCenter = _center;
    _radius = 10.0f;
    _pitch = -0.5f;
    _yaw = 0.0f;

    
    // Establecemos posición inicial de la cámara
    updateCameraFromParameters();
} 

void CameraController::update(Ogre::Real deltaT) {
    
    // Controlamos zoom
    _radius += _mouse->getMouseState().Z.rel * deltaT * -0.2;
    
    if (_radius < 6.0f)
        _radius = 6.0f;
    else if (_radius > 20.f)
        _radius = 20.0f;
    
    // Controlamos pitch y yaw
    if (_mouse->getMouseState().buttonDown(OIS::MB_Right)) {
        _pitch += _mouse->getMouseState().Y.rel * deltaT * 0.1;
        
        if (_pitch < -0.9f)
            _pitch = -0.9f;
        else if (_pitch > -0.03f)
            _pitch = -0.03f;
        
        _yaw += _mouse->getMouseState().X.rel * deltaT * 0.1;
    }
    
    // Controlamos travelling
    Ogre::Vector3 direction = _player->getPosition() + Ogre::Vector3::UNIT_Y * _height - _targetCenter;
    if (direction.squaredLength() > _travellingRadius * _travellingRadius) {
        _travelling = true;
        _targetCenter = _player->getPosition() + Ogre::Vector3::UNIT_Y * _height;
    }
    
    if (_travelling == true) {
        
        Ogre::Vector3 toNewCenter =  _targetCenter - _center;
        
        if (toNewCenter.squaredLength() > 1) {
            toNewCenter.normalise();
            _center += toNewCenter * _speed * deltaT;
        } else 
            _center += toNewCenter * deltaT;
        
        if (toNewCenter.squaredLength() < 0.1)
            _travelling = false;
    }
    
    
    updateCameraFromParameters();
}

void CameraController::updateCameraFromParameters() {
    // Conseguimos la rotación hacia la dirección deseada
    Ogre::Vector3 v = Ogre::Vector3::UNIT_Z;
    Ogre::Quaternion pitchRot(Ogre::Radian(_pitch), Ogre::Vector3::UNIT_X);
    v = pitchRot * v;
    Ogre::Quaternion yawRot(Ogre::Radian(_yaw), Ogre::Vector3::UNIT_Y);
    v = yawRot * v;
    
    // Aplicamos v a center con longitud _radius
    Ogre::Vector3 cameraPos = _center + v * _radius;    
    
    _camera->setPosition(cameraPos);
    _camera->lookAt(_center);
}
