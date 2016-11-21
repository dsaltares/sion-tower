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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_STATE_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_STATE_H_


#include <OIS/OIS.h>
#include <OGRE/Ogre.h>

class StateManager;

//! Clase abstracta que modela un estado de juego

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 26-11-2010
 *
 *  Clase abstracta que modela un estado de juego. Cada estado real (Men&uacute;,
 *  Juego, etc) deber&aacute; implementar su interfaz.
 */
class State {
    public:
        /**
         *  Constructor
         *
         *  @param stateManager puntero al gestor de estados que controla el estado
         *
         *  Crea el estado y carga sus elementos.
         */
        State(StateManager* stateManager);

        /**
         *  Destructor
         *
         *  Libera la memoria ocupada por el estado y sus elementos.
         */
        virtual ~State() {};
       
        /**
         *  @param stateManager gestor de estados que controlar&aacute; este estado
         *
         *  Asigna el gestor de estados al estado
         */
        void setStateManager(StateManager* stateManager) {_stateManager = stateManager;};

        /**
         *  Carga el estado y sus elementos
         */
        virtual void load() {};

        /**
         *  Libera la memoria de los elementos del estado.
         */
        virtual void clear() {};

        /**
         *  @param deltaT tiempo desde el &uacute;ltimo frame en milisegundos.
         *  @param active true si el estado es el tope de la pila (est&aacute;
         *  activo).
         *
         *  Actualiza el estado y sus elementos.
         */
        virtual void update(Ogre::Real deltaT, bool active) = 0;
       
        /**
         *  Manejador del evento pulsar tecla
         *
         *  @param arg evento de tecla
         *  @return true si todo ha ido bien
         */
        virtual bool keyPressed(const OIS::KeyEvent &arg) {return true;};
        
        /**
         *  Manejador del evento soltar tecla
         *
         *  @param arg evento de tecla
         *  @return true si todo ha ido bien
         */
        virtual bool keyReleased(const OIS::KeyEvent &arg) {return true;};
        
        /**
         *  Manejador del evento mover rat&oacute;n
         *
         *  @param arg evento de rat&oacute;n
         *  @param true si todo ha ido bien
         */
        virtual bool mouseMoved(const OIS::MouseEvent &arg) {return true;};
        
        /**
         *  Manejador del evento pulsar bot&oacute;n de rat&oacute;n
         *
         *  @param arg evento de rat&oacute;n
         *  @param true si todo ha ido bien
         */
        virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;};
        
        /**
         *  Manejador del evento soltar bot&oacute;n de rat&oacute;n
         *
         *  @param arg evento de rat&oacute;n
         *  @param true si todo ha ido bien
         */
        virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;};

    protected:
        // Log
        Ogre::Log* _log;

        // Gestor escena
        Ogre::SceneManager* _sceneManager;
        
        StateManager* _stateManager;
        bool _loaded;
        
        // Obliga a traducir todos los estados
        virtual void translate() = 0;
        
        // Obliga a ajustar el tamaño del texto
        virtual void adjustFontHeight() = 0;
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_STATE_H_
