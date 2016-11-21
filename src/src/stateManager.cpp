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

/**
 *  @file stateManager.cpp
 *  @author David Saltares Márquez
 *  @date 23-12-2010
 */

#include "stateManager.h"
#include "state.h"
#include "stateGame.h"
#include "stateMenu.h"
#include "stateCredits.h"
#include "stateProfile.h"
#include "stateLevel.h"
#include "stateVictory.h"
#include "game.h"


StateManager::StateManager(Game* game, OIS::InputManager* inputManager): _game(game),
                                                                         _inputManager(inputManager),
                                                                         _numStates(0),
                                                                         _exit(false) {
    // Tomamos el log
    _log = Ogre::LogManager::getSingleton().getLog("sionTowerLog");

    _log->logMessage("StateManager::StateManager()");

    // Tomamos la ventana
    _window = Game::getRenderWindow();

    // Configuramos recursos y OIS
    //prepareResources();
    configureOIS();

    // Escuchamos eventos de OIS
    Ogre::Root::getSingleton().addFrameListener(this);

    // Escuchamos eventos de ventana
    Ogre::WindowEventUtilities::addWindowEventListener(_window, this);

    // Comenzamos en EstadoMenu
    changeState("stateMenu");
}

StateManager::~StateManager() {
    _log->logMessage("StateManager::~StateManager()");

    // Destruimos todos los estados
    popAllStates();
    performOperations();

    // Destruimos OIS
    if (_inputManager) {
        _inputManager->destroyInputObject(_keyboard);
        _inputManager->destroyInputObject(_mouse);
        OIS::InputManager::destroyInputSystem(_inputManager);
        _keyboard = 0;
        _mouse = 0;
        _inputManager = 0;
    }
}



void StateManager::configureOIS() {
    _log->logMessage("StateManager::configureOIS()");

    _keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject(OIS::OISKeyboard,
                                                                           true));
    _mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse,
                                                                      true));

    // Pedimos que nos envíen eventos de ratón teclado
    _mouse->setEventCallback(this);
    _keyboard->setEventCallback(this);

    // Método de traducción de texto
    _keyboard->setTextTranslation(OIS::Keyboard::Unicode);

    // Ajustar ratón al tamaño de la ventana
    unsigned int width, height, depth;
    int top, left;
    _window->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &mouseState = _mouse->getMouseState();
    mouseState.width = width;
    mouseState.height = height;
}

void StateManager::start() {
    _log->logMessage("StateManager::start()");

    // Crear estado inicial

    // Comenzamos a renderizar
    Ogre::Root::getSingleton().startRendering();
}

void StateManager::exitGame() {
    _log->logMessage("StateManager::exitGame()");
    _exit = true;
}

void StateManager::changeState(State* state) {
    //_log->logMessage("StateManager::changeState() -> Se pidió cambiar de estado");

    // Primero hacemos pop de un estado si existe
    if(!_states.empty()) {
        popState();
    }

    // Después metemos el nuevo estado
    pushState(state);
}

State* StateManager::changeState(const Ogre::String& stateName) {
    State* newState = 0;
    
    if(stateName == "stateGame") {
        _log->logMessage("StateManager::changeState() -> Se pidió cambiar a stateGame");
        newState = new StateGame(this);
        changeState(newState);
    }
    else if (stateName == "stateMenu") {
        _log->logMessage("StateManager::changeState() -> Se pidió cambiar a stateMenu");
        newState = new StateMenu(this);
        changeState(newState);
    }
    else if (stateName == "stateCredits") {
        _log->logMessage("StateManager::changeState() -> Se pidió cambiar a stateCredits");
        newState = new StateCredits(this);
        changeState(newState);
    }
    else if (stateName == "stateProfile") {
        _log->logMessage("StateManager::changeState() -> Se pidió cambiar a stateProfile");
        newState = new StateProfile(this);
        changeState(newState);
    }
    else if (stateName == "stateLevel") {
        _log->logMessage("StateManager::changeState() -> Se pidió cambiar a stateLevel");
        newState = new StateLevel(this);
        changeState(newState);
    }
    else if (stateName == "stateVictory") {
        _log->logMessage("StateManager::changeState() -> Se pidió cambiar a stateVictory");
        newState = new StateVictory(this);
        changeState(newState);
    }
    else 
        _log->logMessage("StateManager::changeState() -> Estado no reconocido");
        
    return newState;
}

void StateManager::popState() {
    _log->logMessage("StateManager::popState() -> Se pidió hacer pop de un estado");

    // Añadimos la operación pendiente
    _pendingOperations.push(StateOperation(opPop));
    --_numStates;
}

void StateManager::popAllStates() {
    _log->logMessage("StateManager::popAllStates() -> Se pidió eliminar todos los estados");

    // Recorremos los estados y vamos eliminándolos
    while(_numStates)
        popState();
}

void StateManager::pushState(State* state) {
    _log->logMessage("StateManager::pushState() -> Se pidió hacer push de un estado");

    // Añadimos la operación pendiente
    _pendingOperations.push(StateOperation(opPush, state));
    ++_numStates;
}

void StateManager::performOperations() {
    // Procesamos todas las operaciones
    while(!_pendingOperations.empty()) {
        StateOperation operation = _pendingOperations.front();

        // Según el tipo de operación (push o pop)
        if(operation._type == opPush) {
            _log->logMessage("StateManager::performOperations -> Realizando push pendiente");
            operation._state->setStateManager(this);
            operation._state->load();
            _states.push_back(operation._state);
        } else {
            _log->logMessage("StateManager::performOperations -> Realizando pop pendiente");
            State* state = _states.back();
            state->clear();
            delete state;
            _states.pop_back();
        }

        // Sacamos la operación de la cola
        _pendingOperations.pop();
    }
}

bool StateManager::keyPressed(const OIS::KeyEvent &arg) {
    // Le enviamos el evento al estado actual
    if(!_states.empty())
        _states[_numStates - 1]->keyPressed(arg);

    return true;
}

bool StateManager::keyReleased(const OIS::KeyEvent &arg) {
    // Le enviamos el evento al estado actual
    if(!_states.empty())
        _states[_numStates - 1]->keyReleased(arg);

    return true;
}

bool StateManager::mouseMoved(const OIS::MouseEvent &arg) {
    // Le enviamos el evento al estado actual
    if(!_states.empty())
        _states[_numStates - 1]->mouseMoved(arg);

    return true;
}

bool StateManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // Le enviamos el evento al estado actual
    if(!_states.empty())
        _states[_numStates - 1]->mousePressed(arg, id);

    return true;
}

bool StateManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // Le enviamos el evento al estado actual
    if(!_states.empty())
        _states[_numStates - 1]->mouseReleased(arg, id);

    return true;
}

bool StateManager::frameStarted(const Ogre::FrameEvent& event) {
    // Realizamos las operaciones pendientes
    performOperations();

    // Actualizamos la entrada
    _keyboard->capture();
    _mouse->capture();

    // Recorremos el vector de estados al revés
    bool active = true;
    for (rev_it = _states.rbegin(); rev_it != _states.rend(); ++rev_it) {
        (*rev_it)->update(event.timeSinceLastFrame, active);
        active = false;
    }

    return !_exit;

}

bool StateManager::frameEnded(const Ogre::FrameEvent& event) {
    return true;
}

bool StateManager::frameRenderingQueued(const Ogre::FrameEvent& event) {return true;}

bool StateManager::windowClosing(Ogre::RenderWindow* window) {
    _exit = true;
    return false;
}
