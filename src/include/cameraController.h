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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_CAMERACONTROLLER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_CAMERACONTROLLER_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

class Player;

//! Modela el comportamiento de la cámara

/**
 * @author David Saltares Márquez
 * @date 10-06-2011
 * 
 * Esta clase modela el movimiento de la cámara que depende del ratón y
 * de la posición del personaje. Es una cámara que se situa a una distancia
 * sobre el personaje con una inclinación determinada. Cuando el personaje
 * se desplaza, la cámara realiza un seguimiento suavizado. El usuario puede
 * rotar la cámara con el ratón alrededor del personaje y hacer zoom.
 * 
 */
class CameraController {
    public:
        /**
         * Constructor
         * 
         * @param camera cámara que hemos de controlar
         * @param mouse ratón del sistema
         * @param player jugador al que hacemos el seguimiento
         */
        CameraController(Ogre::Camera* camera, OIS::Mouse* mouse, Player* player);
        
        /**
         * Actualiza la cámara en función del ratón y del personaje. Es
         * necesario llamar a este método una vez por cuadro de juego.
         * 
         * @param deltaT tiempo en segundos desde el anterior frame.
         */
        void update(Ogre::Real deltaT);
    private:
        // Referencias
        Ogre::Camera* _camera;
        OIS::Mouse* _mouse;
        Player* _player;
        
        // Parámetros constantes
        Ogre::Real _height;
        Ogre::Real _travellingRadius;
        Ogre::Vector3 _speed;
        bool _travelling;
        
        // Coordenadas esféricas
        Ogre::Vector3 _center;
        Ogre::Real _radius;
        Ogre::Real _pitch;
        Ogre::Real _yaw;
        
        Ogre::Vector3 _targetCenter;
        
        void updateCameraFromParameters();
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_CAMERACONTROLLER_H_
