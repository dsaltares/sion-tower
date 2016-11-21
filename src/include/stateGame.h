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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_STATEGAME_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_STATEGAME_H_

#include <vector>

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <MYGUI/MyGUI.h>

#include "state.h"
#include "song.h"
#include "soundFX.h"
#include "body.h"
#include "shape.h"
#include "player.h"
#include "spell.h"
#include "enemySpawn.h"
#include "gameStats.h"
#include "cameraController.h"

class StateManager;
class Level;
class Enemy;


//! Clase que modela la din&aacute;mica de juego

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 24-12-2010
 *
 *  StateManager es la clase que modela la jugabilidad de Sion Tower. Contiene
 *  el nivel, los enemigos y al personaje principal.
 */
class StateGame: public State{
    public:
        
        /** Estados de juego */
        enum GameState {
            START,
            PLAYING,
            PAUSE,
            LOSE,
            WIN
        };
    
        /**
         *  Constructor
         *
         *  @param stateManager gestor de estados asociado que controla el estado de
         *  juego.
         *
         *  Crea y carga todos los elementos del juego: protagonista, nivel,
         *  enemigo, GUI...
         */
        StateGame(StateManager* stateManager);

        /**
         *  Destructor
         *
         *  Destruye el estado de juego y todos sus elementos liberando la
         *  memoria.
         */
        ~StateGame();
        
        /**
         *  Carga los elementos del juego.
         */
        void load();

        /**
         *  Libera los elementos del juego.
         */
        void clear();

        /**
         *  @param deltaT tiempo en milisegundos desde la &uacute;ltima iteraci&oacute;n del
         *  bucle de juego.
         *  @param active true si el estado de juego es el estado activo en el
         *  gestor de estados (tope de la pila).
         *
         *  Actualiza todos los elementos del juego.
         */
        void update(Ogre::Real deltaT, bool active);
        
        /**
         * @param type tipo de hechizo a añadir
         * @param position posición en la que aparecerá el hechizo
         * @param direction dirección hacia la que se moverá el hechizo
         * 
         * Crea y añade un hechizo al estado de juego con los parámetros indicados.
         */
        void addSpell(Spell::Type type, const Ogre::Vector3& position, const Ogre::Vector3& direction);

        /**
         *  Manejador del evento pulsar tecla
         *
         *  @param arg evento de tecla
         *  @return true si todo ha ido bien
         */
        bool keyPressed(const OIS::KeyEvent &arg);
        
        /**
         *  Manejador del evento soltar tecla
         *
         *  @param arg evento de tecla
         *  @return true si todo ha ido bien
         */
        bool keyReleased(const OIS::KeyEvent &arg);
        
        /**
         *  Manejador del evento mover rat&oacute;n
         *
         *  @param arg evento de rat&oacute;n
         *  @param true si todo ha ido bien
         */
        bool mouseMoved(const OIS::MouseEvent &arg);
        
        /**
         *  Manejador del evento pulsar bot&oacute;n de rat&oacute;n
         *
         *  @param arg evento de rat&oacute;n
         *  @param true si todo ha ido bien
         */
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        
        /**
         *  Manejador del evento soltar bot&oacute;n de rat&oacute;n
         *
         *  @param arg evento de rat&oacute;n
         *  @param true si todo ha ido bien
         */
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        /**
         * Comienzo de colisión personaje - escenario
         */
        void beginCollisionCharacterWall(Body* bodyA, Body* bodyB);
        
        /**
         * Durante colisión personaje - escenario
         */
        void inCollisionCharacterWall(Body* bodyA, Body* bodyB);
        
        /**
         * Fin colisión personaje - escenario
         */
        void endCollisionCharacterWall(Body* bodyA, Body* bodyB);
        
        /**
         * Comienzo de colisión hechizo - escenario 
         */
        void collisionSpellScene(Body* bodyA, Body* bodyB);
        
        /**
         * Comienzo de colisión hechizo - enemigo
         */
        void collisionSpellEnemy(Body* bodyA, Body* bodyB);
        
        /**
         * Colisión entre jugador y enemigo
         */
        void collisionPlayerEnemy(Body* bodyA, Body* bodyB);
        
        /**
         * @return jugador
         */
        Player* getPlayer();
        
        /**
         * @return enemigos
         */
        std::vector<Enemy*>& getEnemies();
        
        /**
         * @return hechizos
         */
        std::vector<Spell*>& getSpells();
        
        /**
         * @return estadísticas de juego
         */
        GameStats& getGameStats();
        
        /**
         * Establece el código del nivel que se desea jugar
         * 
         * @param levelCode código del nivel a jugar
         */
        void setLevelCode(const Ogre::String& levelCode);
        
        /**
         * Manejador del evento que se dispara
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnResumeClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnLevelSelectionClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara al pulsar sobre el botón "menu"
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnMenuClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara al pulsar sobre el botón "pausa"
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnPauseClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara al seleccionar el hechizo de fuego
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnFireClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara al seleccionar el hechizo de GEA
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnGeaClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara al seleccionar el hechizo de ventisca
         * 
         * @param sender elemento de MyGUI sobre el que se ha pulsado
         */
        void btnBlizzardClicked(MyGUI::WidgetPtr sender);
        
        /**
         * Manejador del evento que se dispara al pasar el ratón por el hechizo de fuego
         * 
         * @param sender elemento de MyGUI sobre el que se ha centrado el foco
         * @param old elemento de MyGUI que ha perdido el foco
         */
        void btnFireFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old);
        
        /**
         * Manejador del evento que se dispara al pasar el ratón por el hechizo de Gea
         * 
         * @param sender elemento de MyGUI sobre el que se ha centrado el foco
         * @param old elemento de MyGUI que ha perdido el foco
         */
        void btnGeaFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old);
        
        /**
         * Manejador del evento que se dispara al pasar el ratón por el hechizo de ventisca
         * 
         * @param sender elemento de MyGUI sobre el que se ha centrado el foco
         * @param old elemento de MyGUI que ha perdido el foco
         */
        void btnBlizzardFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old);
        
        /**
         * Manejador del evento que se dispara cuando un hechizo pierde el foco
         * 
         * @param sender elemento de MyGUI sobre el que se ha centrado el foco
         * @param old elemento de MyGUI que ha perdido el foco
         */
        void btnSpellLostFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old);
        
        /**
         * Manejador del evento que se dispara cuando un botón consigue el foco
         * 
         * @param sender elemento de MyGUI sobre el que se ha centrado el foco
         * @param old elemento de MyGUI que ha perdido el foco
         */
        void btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old);
     
    private:
        // Estado de juego
        GameState _state;
        
        // Interfaz
        MyGUI::Gui* _myGUI;
        MyGUI::VectorWidgetPtr _widgetLayout;
        MyGUI::Widget* _panelPause;
        MyGUI::Widget* _panelTools;
        MyGUI::Button* _btnResume;
        MyGUI::Button* _btnLevelSelection;
        MyGUI::Button* _btnMenu;
        MyGUI::Button* _btnPause;
        MyGUI::Button* _btnFire;
        MyGUI::Button* _btnGea;
        MyGUI::Button* _btnBlizzard;
        MyGUI::TextBox* _lblPause;
        MyGUI::TextBox* _lblLife;
        MyGUI::TextBox* _lblMana;
        MyGUI::TextBox* _lblExperience;
        MyGUI::TextBox* _lblExperience2;
        MyGUI::TextBox* _lblTime;
        MyGUI::TextBox* _lblTime2;
        MyGUI::TextBox* _lblSpell;
        MyGUI::TextBox* _lblDescription;
        MyGUI::TextBox* _lblManaCost;
        MyGUI::TextBox* _lblPower;
        MyGUI::TextBox* _lblFPS;
        MyGUI::TextBox* _lblState;
        MyGUI::ImageBox* _imgSelectedFire;
        MyGUI::ImageBox* _imgSelectedGea;
        MyGUI::ImageBox* _imgSelectedBlizzard;
        MyGUI::ImageBox* _imgLifeHUD;
        MyGUI::ImageBox* _imgManaHUD;
        
        
        // Sonido
        SoundFXPtr _optionOver;
        SoundFXPtr _optionSelected;
        
        // Cámara y teclado
        Ogre::Camera* _camera;
        CameraController* _cameraController;
        OIS::Keyboard* _keyboard;
        OIS::Mouse* _mouse;

        // Datos del nivel
        Ogre::String _levelCode;
        SongPtr _song;
        SongPtr _loseSong;
        Player* _player;
        Level* _level;
        std::vector<Spell*> _spells;
        std::vector<Enemy*> _enemies;
        std::vector<EnemySpawn>::iterator _nextEnemy;
        
        // Estadísticas de juego
        GameStats* _gameStats;
        Ogre::Real _gameTime;
        Ogre::Real _otherStateTime;
        
        void updateHUD();
        void eraseEndedSpells();
        void checkEnemySpawning();
        void eraseDeadEnemies();
        
        // Traducción
        virtual void translate();
        
        // Altura del texto
        virtual void adjustFontHeight();
};


inline std::vector<Enemy*>& StateGame::getEnemies() {return _enemies;}
inline std::vector<Spell*>& StateGame::getSpells() {return _spells;}


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_STATEGAME_H_
