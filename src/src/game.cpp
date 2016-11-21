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
 *  @file game.cpp
 *  @author David Saltares Márquez
 *  @date 23-12-2010
 */

#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>

#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

#include "game.h"
#include "stateManager.h"
#include "songManager.h"
#include "soundFXManager.h"
#include "collisionManager.h"
#include "levelManager.h"
#include "profileManager.h"
#include "spell.h"

Ogre::SceneManager* Game::_sceneManager = 0;
Ogre::RenderWindow* Game::_window = 0;
Ogre::Viewport* Game::_viewport = 0;
Ogre::Log* Game::_log = 0;
Ogre::Camera* Game::_camera = 0;

Game::Game() {
    // Semilla aleatoria
    srand(time(NULL));
    
    // Iniciamos Ogre
    if(!initialiseOgre()) {
        std::cerr << "Game::Game(): Error al iniciar Ogre" << std::endl;
        exit(1);
    }

    configureSceneManager();
    createCamera();

    // Iniciamos OIS
    if(!initialiseOIS()) {
        std::cerr << "Game::Game(): Error al iniciar OIS" << std::endl;
        exit(1);
    }


    // Iniciamos SDL
    if (!initialiseSDL()) {
        std::cerr << "Game::Game(): Error al iniciar SDL" << std::endl;
        exit(1);
    }
    
    prepareResources();
    
    if (!initialiseMYGUI()) {
        std::cerr << "Game::Game(): Error al iniciar MYGUI" << std::endl;
        exit(1);
    }

    // Creamos el gestor de colisiones
    _collisionManager = new CollisionManager();

    // Creamos el gestor de niveles
    _levelManager = new LevelManager();

    // Creamos el gestor de estados
    _stateManager = new StateManager(this, _inputManager);
    
    // Creamos el gestor de perfiles
    _profileManager = new ProfileManager();
    
    // Inicializamos los hechizos
    Spell::initialiseSpellData();
}

Game::~Game() {
    _log->logMessage("Game::~Game()");

    // Destruimos el gestor de perfiles
    delete _profileManager;

    // Destruimos el gestor de estados
    delete _stateManager;

    // Destruimos el gestor de niveles
    delete _levelManager;

    // Destruimos el CollisionManager
    delete _collisionManager;

    // Destruimos el SongManager
    delete _songManager;

    // Destruimos el SoundFXManager
    delete _soundFXManager;
    
    // Destruimos MyGUI
    _myGUI->shutdown();
    delete _myGUI;
    
    // Destruimos OgrePlatform
    _ogrePlatform->shutdown();
    delete _ogrePlatform;

    // Destruimos la raiz de Ogre
    delete _root;
}

void Game::start() {
    _log->logMessage("Game::start()");

    _stateManager->start();
}

bool Game::initialiseOgre() {
    // Elegimos el fichero plugins (según Debug o Release)
#ifdef _DEBUG
    _pluginsCfg = "pluginsDebug.cfg";
    _windowName = "Sion Tower 1.0";
#else
    _pluginsCfg = "plugins.cfg";
    _windowName = "Sion Tower 0.1";
#endif

    // Fichero de configuración y de logging
    _ogreCfg = "ogre.cfg";
    _ogreLog = "ogre.log";

    // Establecemos el nivel de Log
#ifdef _DEBUG
    bool logOutput = true;
#else
    bool logOutput = false;
#endif

    // Creamos el sistema de log de Ogre (fichero)
    _logManager = new Ogre::LogManager();
    _logManager->createLog(_ogreLog, true, false, false);

    // Log de Sion Tower
    _log = _logManager->createLog("sionTowerLog", false, logOutput, true);

    _log->logMessage("Game::Game()");
    _log->logMessage("Game::initialiseOgre()");

    // Creamos el objeto principal de Ogre, la raíz
    _root = new Ogre::Root(_pluginsCfg, _ogreCfg, _ogreLog);

    // Creamos el MusicaManager
    _songManager = new SongManager();

    // Creamos el SonidoManager
    _soundFXManager = new SoundFXManager();

    // Diálogo para configurar Ogre
    if (!_root->restoreConfig() && !_root->showConfigDialog()) {
        _log->logMessage("Game::initialiseOgre() -> El usuario ha cancelado el diálogo de configuración");
        return false;
    }

    // Inicializamos Ogre y creamos una ventana
    if (!_root->initialise(true, _windowName)) {
        _log->logMessage("Game::initialiseOgre() -> No se pudo inicializar Ogre");
        return false;
    }

    // Tomamos la ventana creada
    _window = _root->getAutoCreatedWindow();

    return true;
}

bool Game::initialiseOIS() {
    _log->logMessage("Game::initialiseOIS()");

    // Configuramos el listener de teclado OIS
    OIS::ParamList paramList;
    size_t windowHandle = 0;
    std::ostringstream windowHandleStr;

    _window->getCustomAttribute("WINDOW", &windowHandle);
    windowHandleStr << windowHandle;
    paramList.insert(std::make_pair(Ogre::String("WINDOW"),
                                    windowHandleStr.str()));

#if defined OIS_WIN32_PLATFORM
    paramList.insert(std::make_pair(std::string("w32_mouse"),
                                    std::string("DISCL_NONEXCLUSIVE") ));
    paramList.insert(std::make_pair(std::string("w32_mouse"),
                                    std::string("DISCL_FOREGROUND" )));
    paramList.insert(std::make_pair(Ogre::String("w32_keyboard"),
                                    Ogre::String("DISCL_FOREGROUND")));
    paramList.insert(std::make_pair(Ogre::String("w32_keyboard"),
                                    Ogre::String("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    paramList.insert(std::make_pair(Ogre::String("x11_mouse_grab"),
                                    Ogre::String("false")));
    paramList.insert(std::make_pair(Ogre::String("x11_mouse_hide"),
                                    Ogre::String("true")));
    paramList.insert(std::make_pair(Ogre::String("x11_keyboard_grab"),
                                    Ogre::String("false")));
    paramList.insert(std::make_pair(Ogre::String("XAutoRepeatOn"),
                                    Ogre::String("true")));
#endif

    // Creamos el gestor de entrada
    _inputManager = OIS::InputManager::createInputSystem(paramList);

    return true;
}

bool Game::initialiseSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

    atexit(SDL_Quit);

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0){
        return false;
    }

    Mix_AllocateChannels(32);

    atexit(Mix_CloseAudio);

    return true;
}

bool Game::initialiseMYGUI() {
    // Creamos e inicializamos OgrePlatform
    _ogrePlatform = new MyGUI::OgrePlatform();
    _ogrePlatform->initialise(_window, _sceneManager);
    
    // Creamos e inicializamos el gestor de MyGUI
    _myGUI = new MyGUI::Gui();
    _myGUI->initialise();
    
    // Cargamos botones con imágenes
    MyGUI::ResourceManager::getInstance().load("button-fire.resource");
    MyGUI::ResourceManager::getInstance().load("button-gea.resource");
    MyGUI::ResourceManager::getInstance().load("button-blizzard.resource");
    
    return true;
}

void Game::configureSceneManager() {
    _log->logMessage("Game::configureSceneManager()");

    // Creamos un SceneManager genérico
    _sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "sceneManager");

    // Elegimos el tipo de sombra
    _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}

void Game::createCamera() {
    _log->logMessage("Game::createCamera()");

    // Creamos y configuramos la cámara
    _camera = _sceneManager->createCamera("camera");
    _camera->setNearClipDistance(1);
    _camera->setFarClipDistance(1000);
    _camera->setPosition(0, 15, 15);
    _camera->lookAt(0, 0, 0);

    // Añadimos un Viewport
    _viewport = _window->addViewport(_camera);
    _viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    // Adaptamos la proporción de la cámara para que encaje con el viewport
    _camera->setAspectRatio(Ogre::Real(_viewport->getActualWidth()) /
                            Ogre::Real(_viewport->getActualHeight()));                           
}


Ogre::SceneManager* Game::getSceneManager() {
    return _sceneManager;
}

Ogre::RenderWindow* Game::getRenderWindow() {
    return _window;
}

Ogre::Viewport* Game::getViewPort() {
    return _viewport;
}

Ogre::Log* Game::getGameLog() {
    return _log;
}

Ogre::Camera* Game::getCamera() {
    return _camera;
}

void Game::prepareResources() {
    _log->logMessage("StateManager::prepareResources()");

    Ogre::String groupName, typeName, archiveName;
    Ogre::ConfigFile configFile;

    // Abrimos el fichero de recursos
    configFile.load("resources.cfg");

    // Iterador para recorrer grupos
    Ogre::ConfigFile::SectionIterator sectionIterator = configFile.getSectionIterator();

    // Mientras haya grupos
    while (sectionIterator.hasMoreElements()) {
        // Tomamos el nombre del grupo
        groupName = sectionIterator.peekNextKey();

        // Vamos a recorrer los archivos del grupo
        Ogre::ConfigFile::SettingsMultiMap *elements = sectionIterator.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = elements->begin(); i != elements->end(); ++i) {
            typeName = i->first;
            archiveName = i->second;

            // Añadimos la localización del archivo con su nombre, su tipo y grupo
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archiveName,
                                                                           typeName,
                                                                           groupName,
                                                                           false);
        }
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("General");
}
