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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_STATEMANAGER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_STATEMANAGER_H_

#include <vector>
#include <queue>

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

class State;
class Game;

//! Clase que controla los estados de Sion Tower y sus transiciones

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 23-12-2010
 *
 *
 *  StateManager es la clase que se encarga de controlar los estados de Sion
 *  Tower y las transiciones entre los mismos.
 *
 *  Mantiene una pila de estados de los que s&oacute;lo se actualiza el frente.
 *  Podemos cambiar el estado actual por otro (StateManager::changeState), a&ntilde;adir un nuevo
 *  estado (StateManager::pushState) o eliminar el tope de la pila (stateManager::popState).
 *
 *  Captura los eventos de teclado, rat&oacute;n y ventana. Se encarga de transmitir
 *  estos eventos al estado que se encuentre actualmente activo.
 *  
 */

class StateManager: public Ogre::FrameListener,
                     public Ogre::WindowEventListener,
                     public OIS::KeyListener,
                     public OIS::MouseListener {
    public:
        /**
         *  Constructor
         *
         *  @param game juego al que pertenece el StateManager
         *  @param inputManager gestor de la entrada para configurar los dispositivos
         *  (rat&oacute;n y teclado)
         *
         *  Configura el SceneManager.
         *  Crea la c&aacute;mara.
         *  Prepara los recursos (sin cargarlos).
         *  Inicia el men&uacute; de juego.
         */
        StateManager(Game* game, OIS::InputManager* inputManager);

        /**
         *  Destructor
         *
         *  Destruye todas las escenas de la pila.
         */
        ~StateManager();
        
        /**
         *  Inicia el bucle de juego.
         */
        void start();
        
        /**
         *  Finaliza el bucle de juego al finalizar la iteraci&oacute;n.
         */
        void exitGame();
        
        /**
         *  @param state nuevo estado.
         *
         *  Cambia el estado actual por el estado deseado.
         *  El cambio se produce al finalizar la iteraci&oacute;n actual del bucle de
         *  juego.
         */
        void changeState(State* state);

        /**
         *  @param stateName nombre del nuevo estado.
         *
         *  Cambia el estado actual por el estado deseado.
         *  El cambio se produce al finalizar la iteraci&oacute;n actual del bucle de
         *  juego.
         *
         *  Los posibles nombres son:
         *
         *  <ul>
         *      <li>'stateGame': StateGame</li>
         *      <li>'stateMenu': StateMenu</li>
         *  </ul>
         */
        State* changeState(const Ogre::String& stateName);

        /**
         *  Saca el estado actual de la pila de estados.
         *  El cambio no se produce hasta finalizar la iteraci&oacute;n actual del
         *  bucle de juego.
         */
        void popState();

        /**
         *  Introduce un nuevo estado en la pila.
         *  El cambio no se produce hasta finalizar la iteraci&oacute;n actual del
         *  bucle de juego.
         */
        void pushState(State* state);

        /**
         *  Saca todos los estados de la pila.
         */
        void popAllStates();

        /**
         *  @return rat&oacute;n utilizado por el juego.
         *
         *  Este m&eacute;todo se utiliza para actualizar elementos del juego en
         *  funci&oacute;n de alg&uacute;n dispositivo de entrada. En este caso el rat&oacute;n.
         */
        OIS::Mouse* getMouse();

        /**
         *  @return teclado utilizado por el juego.
         *
         *  Este m&eacute;todo se utiliza para actualizar elementos del juego en
         *  funci&oacute;n de alg&uacute;n dispositivo de entrada. En este caso, el teclado.
         */
        OIS::Keyboard* getKeyboard();
        
        /**
         *  @param arg evento de teclado
         *  @return true si todo ha ido bien
         *
         *  Evento que se dispara al presionar una tecla.
         */
        bool keyPressed(const OIS::KeyEvent &arg);
        
        /**
         *  @param arg evento de teclado
         *  @return true si todo ha ido bien
         *
         *  Evento que se dispara al liberar una tecla.
         */
        bool keyReleased(const OIS::KeyEvent &arg);

        /**
         *  @param arg evento de rat&oacute;n
         *  @return true si todo ha ido bien
         *
         *  Evento que se dispara al mover el rat&oacute;n.
         */
        bool mouseMoved(const OIS::MouseEvent &arg);

        /**
         *  @param arg evento de rat&oacute;n
         *  @param id identificador del bot&oacute;n del rat&oacute;n
         *  @return true si todo ha ido bien
         *
         *  Evento que se dispara al presionar un bot&oacute;n del rat&oacute;n.
         */
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        /**
         *  @param arg evento de rat&oacute;n
         *  @param id identificador del bot&oacute;n del rat&oacute;n
         *  @return true si todo ha ido bien
         *
         *  Evento que se dispara al liberar un bot&oacute;n del rat&oacute;n.
         */
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
       
        /**
         *  @param event informaci&oacute;n del evento
         *  @return true si se contin&uacute;a con el bucle de renderizado. Si se
         *  devuelve false, el bucle se detiene 
         *
         *  Evento que se dispara al iniciar un cuadro de renderizado.
         */
        bool frameStarted(const Ogre::FrameEvent& event);

        /**
         *  @param event informaci&oacute;n del evento
         *  @return true si se contin&uacute;a con el bucle de renderizado. Si se
         *  devuelve false, el bucle se detiene 
         *
         *  Evento que se dispara al acabar el renderizado de la escena en cada
         *  iteraci&oacute;n del bucle de juego. Despu&eacute;s de intercambiar los buffers.
         */
        bool frameEnded(const Ogre::FrameEvent& event);

        /**
         *  @param event informaci&oacute;n del evento
         *  @return true si se contin&uacute;a con el bucle de renderizado. Si se
         *  devuelve false, el bucle se detiene 
         *
         *  Evento que se dispara al acabar el renderizado de la escena en cada
         *  iteraci&oacute;n del bucle de juego. Antes de intercambiar los buffers. 
         */
        bool frameRenderingQueued(const Ogre::FrameEvent& event);
       
        /**
         *  @param window ventana que se cierra
         *
         *  Evento que se dispara cuando se cierra la ventana de la aplicaci&oacute;n.
         */
        void windowClosed(Ogre::RenderWindow* window) {};

        /**
         *  @param window ventana redimensionada
         *
         *  Evento que se dispara al redimensionar la ventana.
         */
        void windowResized(Ogre::RenderWindow* window) {};

        /**
         *  @param window ventana que se est&aacute; cerrando
         *  @return true si confirmados el cierre, false en caso contrario.
         *
         *  Evento que se dispara al cerrar la ventana de la aplicaci&oacute;n.
         *  Permite confirmaci&oacute;n.
         */
        bool windowClosing(Ogre::RenderWindow* window);
        
    private:
        // Juego
        Game* _game;

        // Objetos Ogre
        Ogre::Log* _log;
        Ogre::RenderWindow* _window;

        // Managers de entrada OIS
        OIS::InputManager* _inputManager;
        OIS::Keyboard* _keyboard;
        OIS::Mouse* _mouse;
        
        // Fichero de recursos
        Ogre::String _resourcesCfg;

        // M&eacute;todos auxiliares
        void configureOIS();
    
        // Vector de estados
        std::vector<State*> _states;
        
        // Iterador inverso
        std::vector<State*>::const_reverse_iterator rev_it;
        
        // N&uacute;mero de estados (temporal)
        int _numStates;
        
        // Tipo de operaci&oacute;n (push o pop)
        enum tOperation {opPush, opPop};
        
        // Operaci&oacute;n de pop o push sobre un estado que se ejecutar&aacute;
        // al comienzo de update
        struct StateOperation {
            tOperation _type;
            State* _state;
            
            StateOperation(tOperation type, State* state = 0): _type(type), _state(state) {};
        };
        
        // Cola de operaciones por realizar
        std::queue<StateOperation> _pendingOperations;
        
        // Controlamos la salida
        bool _exit;
        
        // Realiza las operaciones pendientes
        void performOperations();
};

inline OIS::Mouse* StateManager::getMouse() {return _mouse;}
inline OIS::Keyboard* StateManager::getKeyboard() {return _keyboard;}

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_STATEMANAGER_H_
