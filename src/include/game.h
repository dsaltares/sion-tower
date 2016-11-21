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



#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_GAME_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_GAME_H_

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <MYGUI/MyGUI.h>
#include <MYGUI/MyGUI_OgrePlatform.h>

class StateManager;
class SongManager;
class SoundFXManager;
class CollisionManager;
class LevelManager;
class ProfileManager;

//! Clase principal que inicia el sistema y comienza el bucle de juego.

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 23-12-2010
 *
 *  Game es la clase principal de Sion Tower. Es la encargada de iniciar el
 *  sistema (bibliotecas Ogre, SDL, OIS y MYGUI junto a otros subsistemas). Al
 *  llamar a comenzar se inicia el bucle de juego.
 *
 *  Delega las funciones de control de estados y de eventos en StateManager
 */
class Game {
    public:
        /**
         *  Constructor
         *
         *  Inicia Ogre, OIS, SDL y MYGUI. Prepara el juego y crea el gestor de
         *  estados.
         */
        Game();

        /**
         *  Destructor
         *
         *  Libera la memoria ocupada por el juego y todos sus elementos.
         */
        ~Game();
        
        /**
         *  Comienza el bucle de juego hasta que el usuario decida salir.
         */
        void start();
        
        /**
         *  @return SceneManager principal del juego
         */
        static Ogre::SceneManager* getSceneManager();
        
        /**
         *  @return ventana de la aplicación
         */
        static Ogre::RenderWindow* getRenderWindow();
        
        /**
         *  @return viewport asociado a la ventana de la aplicación
         */
        static Ogre::Viewport* getViewPort();
        
        /**
         *  @return log interno del juego
         */
        static Ogre::Log* getGameLog();
        
        /**
         *  @return cámara del juego
         */
        static Ogre::Camera* getCamera();
        
    private:
        // Objetos Ogre
        Ogre::Root* _root;
        Ogre::LogManager* _logManager;

        // Sencilla recuperación (método estático)
        static Ogre::RenderWindow* _window;
        static Ogre::Camera* _camera;
        static Ogre::Viewport* _viewport;
        static Ogre::SceneManager* _sceneManager;
        static Ogre::Log* _log;
        
        
        // Gestor entrada OIS
        OIS::InputManager* _inputManager;
        
        // MYGUI
        MyGUI::Gui* _myGUI;
        MyGUI::OgrePlatform* _ogrePlatform;
        
        // Gestor Estados
        StateManager* _stateManager;
        
        // Gestor de perfiles
        ProfileManager* _profileManager;
        
        // Nombre aplicaci&oacute;n
        Ogre::String _windowName;
        
        // Ficheros de plugins, logging y configuraci&oacute;n
        Ogre::String _pluginsCfg;
        Ogre::String _ogreLog;
        Ogre::String _ogreCfg;
    
        // Subsistema de audio
        SongManager* _songManager;
        SoundFXManager* _soundFXManager;

        // Colisiones
        CollisionManager* _collisionManager;

        // Niveles
        LevelManager* _levelManager;
        
        // M&eacute;todos auxiliares
        bool initialiseOgre();
        bool initialiseOIS();
        bool initialiseSDL();
        bool initialiseMYGUI();
        
        // Configuración
        void configureSceneManager();
        void createCamera();
        void prepareResources();
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_GAME_H_
