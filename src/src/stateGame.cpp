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
 *  @file estadoJuego.cpp
 *  @author David Saltares Márquez
 *  @date 24-12-2010
 */

#include <iostream>
#include <cmath>
#include <libintl.h>
#include <locale.h>

#include <OGRE/Ogre.h>
#include <MYGUI/MyGUI.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "stateGame.h"
#include "stateVictory.h"
#include "stateManager.h"
#include "stateMenu.h"
#include "stateLevel.h"
#include "songManager.h"
#include "soundFXManager.h"
#include "collisionManager.h"
#include "levelManager.h"
#include "level.h"
#include "game.h"
#include "enemy.h"

#define _(x) gettext(x)

using std::cout;
using std::endl;

StateGame::StateGame(StateManager* stateManager): State(stateManager) {
    _log->logMessage("StateGame::StateGame()");
    
    // Objeto MyGUI
    _myGUI = MyGUI::Gui::getInstancePtr();

    // Recuperamos el teclado y ratón
    _keyboard = _stateManager->getKeyboard();
    _mouse = _stateManager->getMouse();
    
    // Recuperamos la cámara
    _camera = _sceneManager->getCamera("camera");
    
    // Nivel por defecto
    _levelCode = "level01";
    
    // Tiempo de juego
    _gameTime = 0.0f;
    
    // Tiempo de otro estado
    _otherStateTime = 0.0f;
    
    _state = START;
}

StateGame::~StateGame() {
    _log->logMessage("StateGame::~StateGame()");
    clear();
}

void StateGame::load() {
    if (!_loaded) {
        _log->logMessage("StateGame::load()");
        _loaded = true;
        
        // Cargamos sonidos
        _optionOver = SoundFXManager::getSingleton().load("optionover.wav");
        _optionSelected = SoundFXManager::getSingleton().load("optionselected.wav");
        
        // INTERFAZ MyGUI
        
        // Cargamos el layout
        _widgetLayout = MyGUI::LayoutManager::getInstance().loadLayout("game.layout");
        
        // Recuperamos widgets
        _panelPause = _myGUI->findWidget<MyGUI::Widget>("panelPause");
        _panelTools = _myGUI->findWidget<MyGUI::Widget>("panelTools");
        _btnResume = _myGUI->findWidget<MyGUI::Button>("btnResume");
        _btnLevelSelection = _myGUI->findWidget<MyGUI::Button>("btnLevelSelection");
        _btnMenu = _myGUI->findWidget<MyGUI::Button>("btnMenu");
        _btnFire = _myGUI->findWidget<MyGUI::Button>("btnFire");
        _btnGea = _myGUI->findWidget<MyGUI::Button>("btnGea");
        _btnBlizzard = _myGUI->findWidget<MyGUI::Button>("btnBlizzard");
        _btnPause = _myGUI->findWidget<MyGUI::Button>("btnPause");
        _lblPause = _myGUI->findWidget<MyGUI::TextBox>("lblPause");
        _lblTime = _myGUI->findWidget<MyGUI::TextBox>("lblTime");
        _lblTime2 = _myGUI->findWidget<MyGUI::TextBox>("lblTime2");
        _lblLife = _myGUI->findWidget<MyGUI::TextBox>("lblLife");
        _lblMana = _myGUI->findWidget<MyGUI::TextBox>("lblMana");
        _lblExperience = _myGUI->findWidget<MyGUI::TextBox>("lblExperience");
        _lblExperience2 = _myGUI->findWidget<MyGUI::TextBox>("lblExperience2");
        _lblSpell = _myGUI->findWidget<MyGUI::TextBox>("lblSpell");
        _lblDescription = _myGUI->findWidget<MyGUI::TextBox>("lblDescription");
        _lblManaCost = _myGUI->findWidget<MyGUI::TextBox>("lblManaCost");
        _lblPower = _myGUI->findWidget<MyGUI::TextBox>("lblPower");
        _lblFPS = _myGUI->findWidget<MyGUI::TextBox>("lblFPS");
        _lblState = _myGUI->findWidget<MyGUI::TextBox>("lblState");
        _imgSelectedFire = _myGUI->findWidget<MyGUI::ImageBox>("imgSelectedFire");
        _imgSelectedGea = _myGUI->findWidget<MyGUI::ImageBox>("imgSelectedGea");
        _imgSelectedBlizzard = _myGUI->findWidget<MyGUI::ImageBox>("imgSelectedBlizzard");
        _imgLifeHUD = _myGUI->findWidget<MyGUI::ImageBox>("imgLife");
        _imgManaHUD = _myGUI->findWidget<MyGUI::ImageBox>("imgMana");
        
        // Asignamos event handlers
        _btnResume->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnResumeClicked);
        _btnLevelSelection->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnLevelSelectionClicked);
        _btnMenu->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnMenuClicked);
        _btnPause->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnPauseClicked);
        _btnFire->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnFireClicked);
        _btnGea->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnGeaClicked);
        _btnBlizzard->eventMouseButtonClick += MyGUI::newDelegate(this, &StateGame::btnBlizzardClicked);
        
        _btnResume->setNeedMouseFocus(true);
        _btnPause->setNeedMouseFocus(true);
        _btnLevelSelection->setNeedMouseFocus(true);
        _btnMenu->setNeedMouseFocus(true);
        _btnFire->setNeedMouseFocus(true);
        _btnGea->setNeedMouseFocus(true);
        _btnBlizzard->setNeedMouseFocus(true);
        
        _btnResume->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnPause->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnLevelSelection->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnMenu->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnFire->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnFire->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnFireFocus);
        _btnFire->eventMouseLostFocus += MyGUI::newDelegate(this, &StateGame::btnSpellLostFocus);
        _btnGea->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnGea->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnGeaFocus);
        _btnGea->eventMouseLostFocus += MyGUI::newDelegate(this, &StateGame::btnSpellLostFocus);
        _btnBlizzard->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnMouseFocus);
        _btnBlizzard->eventMouseSetFocus += MyGUI::newDelegate(this, &StateGame::btnBlizzardFocus);
        _btnBlizzard->eventMouseLostFocus += MyGUI::newDelegate(this, &StateGame::btnSpellLostFocus);
        
        
        // Luz ambiente
        _sceneManager->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
        
        // Cargamos el personaje
        _player = new Player(_sceneManager, this, _camera, _keyboard, _mouse);

        // Cargamos escenario
        _level = LevelManager::getSingleton().getLevel(_levelCode);
        _level->load();
        _song = _level->getSong();
        _song->play();
        _player->setPosition(_level->getPlayerPosition());
        _player->setPosition(_player->getPosition() - Ogre::Vector3(0, 0.7, 0));
        
        // Canción derrota
        _loseSong = SongManager::getSingleton().load("Sion tower - 07 Derrota.ogg");
        
        // Inicializamos el iterador de spawn
        _nextEnemy = _level->getEnemySpawns().begin();
        
        // Estadísticas
        _gameStats = new GameStats(_levelCode);
        
        // Estado inicial
        _state = START;
        _panelPause->setVisible(false);
        
        // Camera controller
        _cameraController = new CameraController(_camera, _mouse, _player);

        // CALLBACKS de colisiones
        CollisionManager::CollisionCallback callback;
        CollisionManager* collisionManager = CollisionManager::getSingletonPtr();
        
        // Player - Scene
        callback = boost::bind(&StateGame::beginCollisionCharacterWall, this, _1, _2);
        collisionManager->addCollisionCallback(GameObject::PLAYER, GameObject::SCENE, callback, CollisionManager::BEGINCOLLISION);
        callback = boost::bind(&StateGame::inCollisionCharacterWall, this, _1, _2);
        collisionManager->addCollisionCallback(GameObject::PLAYER, GameObject::SCENE, callback, CollisionManager::COLLIDING);
        callback = boost::bind(&StateGame::endCollisionCharacterWall, this, _1, _2);
        collisionManager->addCollisionCallback(GameObject::PLAYER, GameObject::SCENE, callback, CollisionManager::ENDCOLLISION);
        
        
        // Spells - Scene
        callback = boost::bind(&StateGame::collisionSpellScene, this, _1, _2);
        collisionManager->addCollisionCallback(GameObject::SPELL, GameObject::SCENE, callback, CollisionManager::BEGINCOLLISION);
        
        // Spells - Enemy
        callback = boost::bind(&StateGame::collisionSpellEnemy, this, _1, _2);
        collisionManager->addCollisionCallback(GameObject::SPELL, GameObject::ENEMY, callback, CollisionManager::BEGINCOLLISION);
        
        // Player - Enemy
        callback = boost::bind(&StateGame::collisionPlayerEnemy, this, _1, _2);
        collisionManager->addCollisionCallback(GameObject::PLAYER, GameObject::ENEMY, callback, CollisionManager::COLLIDING);
        
        // Traducimos
        translate();
        
        // Ajustamos la altura de la fuente
        adjustFontHeight();
    }
}

void StateGame::clear() {
    if (_loaded) {
        _log->logMessage("StateGame::limpiar()");
        _loaded = false;
        
        // Eliminamos el layout
        MyGUI::LayoutManager::getInstance().unloadLayout(_widgetLayout);
        _widgetLayout.clear();
        
        // Eliminamos al jugador
        delete _player;
        
        // Eliminamos los hechizos
        for (std::vector<Spell*>::iterator i = _spells.begin(); i != _spells.end(); ++i)
            delete (*i);
            
        _spells.clear();
        
        // Eliminamos los enemigos
        for (std::vector<Enemy*>::iterator i = _enemies.begin(); i != _enemies.end(); ++i)
            delete (*i);
            
        _enemies.clear();
        
        // Destruimos las estadísticas
        delete _gameStats;
        
        // Eliminamos el nivel (se destruye al destruir el LevelManager)
        _level->unload();

        // Limpiamos el CollisionManager
        CollisionManager::getSingleton().removeAllBodies();
        CollisionManager::getSingleton().removeAllCollisionCallbacks();
        
        // Camera controller
        delete _cameraController;
    }
}


void StateGame::update(Ogre::Real deltaT, bool active) {
    if (active && _state == PLAYING) {  
        // Actualizar personaje
        _player->update(deltaT);
        
        // Actualizar hechizos
        for (std::vector<Spell*>::iterator i = _spells.begin(); i != _spells.end(); ++i)
            (*i)->update(deltaT);

        // Actualizar enemigos
        for (std::vector<Enemy*>::iterator i = _enemies.begin(); i != _enemies.end(); ++i)
            (*i)->update(deltaT);
        
        // Actualizar cámara
        _cameraController->update(deltaT);
        
        // Comprobar colisiones
        CollisionManager::getSingleton().checkCollisions();
        
        // Borramos elementos innecesarios
        eraseEndedSpells();
        eraseDeadEnemies();
        
        // Comprobamos las apariciones de los enemigos
        checkEnemySpawning();
        
        // Actualizamos el tiempo de partida
        _gameStats->setTime(_gameTime);
        
        // Actualizamos la vida
        _gameStats->setLife(_player->getEnergy());
        
        // Actualizamos HUD
        updateHUD();
        
        // Si hemos terminado
        if (_enemies.size() == 0 && _nextEnemy == _level->getEnemySpawns().end()) {
            StateVictory* state = static_cast<StateVictory*>(_stateManager->changeState("stateVictory"));
            state->setGameStats(*_gameStats);
        }
        
        // Actualizamos el tiempo
        _gameTime += deltaT;
        
        char buffer[30];
        sprintf(buffer, "FPS: %f", Game::getRenderWindow()->getLastFPS());
        _lblFPS->setCaption(buffer);
        
        // Si el jugador ha muerto
        if (_player->getState() == Actor::ERASE) {
            _state = LOSE;
            _lblState->setCaption(_("#ffffffYou have failed!\nPress space to try again!"));
            _lblState->setVisible(true);
            _loseSong->play();
        }
    }
    
    if (active && _state == LOSE) {
        // Actualizar hechizos
        for (std::vector<Spell*>::iterator i = _spells.begin(); i != _spells.end(); ++i)
            (*i)->update(deltaT);

        // Actualizar enemigos
        for (std::vector<Enemy*>::iterator i = _enemies.begin(); i != _enemies.end(); ++i)
            (*i)->update(deltaT);
        
        // Actualizar cámara
        _cameraController->update(deltaT);
        
        // Comprobar colisiones
        CollisionManager::getSingleton().checkCollisions();
        
        // Borramos elementos innecesarios
        eraseEndedSpells();
        eraseDeadEnemies();
    }
    
    if (active && _state == START) {
        
        // Actualizar personaje
        _player->update(deltaT);
        
        // Actualizar cámara
        _cameraController->update(deltaT);
        
        // Comprobar colisiones
        CollisionManager::getSingleton().checkCollisions();
        
        // Borramos elementos innecesarios
        eraseEndedSpells();
        eraseDeadEnemies();
        
        // Actualizamos tiempo
        _otherStateTime += deltaT;
        
        if (_otherStateTime >= 4) {
            _lblState->setVisible(false);
            _state = PLAYING;
            _otherStateTime = 0.0f;
        }
            
    }
}


bool StateGame::keyPressed(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
    
    // Pausar el juego
    if (arg.key == OIS::KC_ESCAPE && _state == PLAYING) {
        _state = PAUSE;
        _panelPause->setVisible(true);
        _btnPause->setEnabled(false);
        _btnFire->setEnabled(false);
        _btnGea->setEnabled(false);
        _btnBlizzard->setEnabled(false);
    }
    
    // Quitar pausa
    else if (arg.key == OIS::KC_ESCAPE && _state == PAUSE) {
        _state = PLAYING;
        _panelPause->setVisible(false);
        _btnPause->setEnabled(true);
        _btnFire->setEnabled(true);
        _btnGea->setEnabled(true);
        _btnBlizzard->setEnabled(true);
    }
    
    // Teclas de acceso rápido a hechizos
    if (arg.key == OIS::KC_1) {
        _optionSelected->play();
        _player->setSpell(Spell::FIRE);
    
        _imgSelectedBlizzard->setVisible(false);
        _imgSelectedFire->setVisible(true);
        _imgSelectedGea->setVisible(false);
    } else if (arg.key == OIS::KC_2) {
        _optionSelected->play();
        _player->setSpell(Spell::GEA);
    
        _imgSelectedBlizzard->setVisible(false);
        _imgSelectedFire->setVisible(false);
        _imgSelectedGea->setVisible(true);
    } else if (arg.key == OIS::KC_3) {
        _optionSelected->play();
        _player->setSpell(Spell::BLIZZARD);
    
        _imgSelectedBlizzard->setVisible(true);
        _imgSelectedFire->setVisible(false);
        _imgSelectedGea->setVisible(false);
    }
    
    if (arg.key == OIS::KC_SPACE && _state == LOSE) {
        _stateManager->changeState("stateLevel");
    }
    
    return true;
}

bool StateGame::keyReleased(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    
    return true;
}

bool StateGame::mouseMoved(const OIS::MouseEvent &arg) {
    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
    
    return true;
}

bool StateGame::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    
    // Dimensiones del panel
    Ogre::Real x1, y1, w1, h1;
    Ogre::Real x2, y2;
    
    int screenWidth = Game::getViewPort()->getActualWidth();
    int screenHeight = Game::getViewPort()->getActualHeight();

    x1 = 0.0578125 * screenWidth;
    y1 = 0.859722 * screenHeight;
    w1 = 0.9 * screenWidth;
    h1 = 0.133333 * screenHeight;
    x2 = arg.state.X.abs;
    y2 = arg.state.Y.abs;
    
    bool mouseInPanel = (x2 >= x1 && x2 <= x1 + w1 && y2 >= y1 && y2 <= y1 + h1);
    
    // Si no pulsamos en la interfaz, disparamos
    if (!mouseInPanel &&
        id == OIS::MB_Left &&
        _player->getState() != Actor::ATTACK &&
        _state == PLAYING && 
        _player->getMana() >= Spell::getMana(_player->getSpell()))
        _player->setState(Actor::ATTACK, false);
        
    return true;
}

bool StateGame::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    
    return true;
}


void StateGame::beginCollisionCharacterWall(Body* bodyA, Body* bodyB) {
    _player->restoreOldPosition();
}

void StateGame::inCollisionCharacterWall(Body* bodyA, Body* bodyB) {
    _player->restoreOldPosition();
}

void StateGame::endCollisionCharacterWall(Body* bodyA, Body* bodyB) {
}

void StateGame::collisionSpellScene(Body* bodyA, Body* bodyB) {
    // Recuperamos el hechizo
    GameObject* objectA = bodyA->getGameObject();
    GameObject* objectB = bodyB->getGameObject();
    
    Spell* spell;
    
    if (objectA->getType() == GameObject::SPELL)
        spell = static_cast<Spell*>(objectA);
    else
        spell = static_cast<Spell*>(objectB);
        
    // Hacemos explotar el hechizo
    spell->explode();
}

void StateGame::collisionSpellEnemy(Body* bodyA, Body* bodyB) {
    // Recuperamos el hechizo
    GameObject* objectA = bodyA->getGameObject();
    GameObject* objectB = bodyB->getGameObject();
    
    Spell* spell;
    Enemy* enemy;
    
    if (objectA->getType() == GameObject::SPELL) {
        spell = static_cast<Spell*>(objectA);
        enemy = static_cast<Enemy*>(objectB);
    }
    else {
        spell = static_cast<Spell*>(objectB);
        enemy = static_cast<Enemy*>(objectA);
    }
    
    // Hacemos daño al enemigo
    if (enemy->getState() != Actor::DAMAGED &&
        enemy->getState() != Actor::DIE &&
        enemy->getState() != Actor::ERASE &&
        spell->getState() == Spell::MOVE) {
    
        enemy->getHurt(spell->getPower());
        enemy->setState(Actor::DAMAGED, false);
    }
    
    // Hacemos explotar el hechizo    
    if (spell->getState() == Spell::MOVE)
        spell->explode();
}

void StateGame::collisionPlayerEnemy(Body* bodyA, Body* bodyB) {
    // Recuperamos el enemigo y el personaje
    GameObject* objectA = bodyA->getGameObject();
    GameObject* objectB = bodyB->getGameObject();
    
    Player* player;
    Enemy* enemy;
    
    if (objectA->getType() == GameObject::PLAYER) {
        player = static_cast<Player*>(objectA);
        enemy = static_cast<Enemy*>(objectB);
    }
    else {
        player = static_cast<Player*>(objectB);
        enemy = static_cast<Enemy*>(objectA);
    }
    
    // Separamos al personaje un poco
    Ogre::Vector3 direction =  player->getOldPosition() - player->getPosition();
    direction.normalise();
    player->setPosition(player->getPosition() + direction * 0.35);
    enemy->setPosition(enemy->getPosition() + -direction * 0.15);
}

void StateGame::updateHUD() {
    char buffer[30];

    int screenWidth = Game::getViewPort()->getActualWidth();
    
    // Update life
    //sprintf(buffer, "%d", _player->getEnergy());
    //_lblLife2->setCaption(buffer);
    int newWidth = (int)(0.064 * screenWidth) * _player->getEnergy() / 100;
    _imgLifeHUD->setSize(newWidth, _imgLifeHUD->getSize().height);
    
    // Update mana
    //sprintf(buffer, "%d", _player->getMana());
    //_lblMana2->setCaption(buffer);
    newWidth = (int)(0.064 * screenWidth) * _player->getMana() / 100;
    _imgManaHUD->setSize(newWidth, _imgManaHUD->getSize().height);
    
    // Update time
    Ogre::Real time = _gameTime;
    int minutes = time / 60;
    int seconds = 0;
    seconds = (minutes == 0)? (int)time : ((int)time) % 60;
    sprintf(buffer, "%02d:%02d", minutes, seconds);
    _lblTime2->setCaption(buffer);
    
    // Update experience
    sprintf(buffer, "%d", _gameStats->getTotal());
    _lblExperience2->setCaption(buffer);
    
    // Comprobamos maná y botones de hechizos
    if (_player->getMana() >= Spell::getMana(Spell::FIRE))
        _btnFire->setEnabled(true);
        
    if (_player->getMana() >= Spell::getMana(Spell::GEA))
        _btnGea->setEnabled(true);
        
    if (_player->getMana() >= Spell::getMana(Spell::BLIZZARD))
        _btnBlizzard->setEnabled(true);
}

void StateGame::addSpell(Spell::Type type, const Ogre::Vector3& position, const Ogre::Vector3& direction) {
    Spell* spell = new Spell(_sceneManager, type, position, direction);
    _gameStats->useMana(spell->getMana());
    _player->useMana(spell->getMana());
    _spells.push_back(spell);
    
    if (_player->getMana() < Spell::getMana(Spell::FIRE))
        _btnFire->setEnabled(false);
        
    if (_player->getMana() < Spell::getMana(Spell::GEA))
        _btnGea->setEnabled(false);
        
    if (_player->getMana() < Spell::getMana(Spell::BLIZZARD))
        _btnBlizzard->setEnabled(false);
}

void StateGame::eraseEndedSpells() {
    // Destruimos todos los hechizos que hayan finalizado
    for (std::vector<Spell*>::iterator i = _spells.begin(); i != _spells.end(); ) {
        if ((*i)->getState() == Spell::ERASE) {
            delete (*i);
            i = _spells.erase(i);
        }
        else {
            ++i;
        }
    }
}

void StateGame::checkEnemySpawning() {
    // Recorremos el vector de enemy spawning
    // Añadimos todos los que su time sea menor o igual al timer (s)
    // Actualizamos el iterador
    
    bool keepSpawning = true;
    
    for (; keepSpawning && _nextEnemy != _level->getEnemySpawns().end(); ++_nextEnemy) {
        // Si ha pasado suficiente tiempo añadimos enemigo
        if (_nextEnemy->getTime() <= _gameTime && _enemies.size() < 5) {
            Enemy* enemy = new Enemy(Game::getSceneManager(), this, _nextEnemy->getType(), _nextEnemy->getPosition());
            enemy->setOrientation(_nextEnemy->getOrientation());
            enemy->setNavigationMesh(_level->getNavigationMesh());
            _enemies.push_back(enemy);
        }
        // Si no, paramos y corregimos la posición del iterador
        else {
            keepSpawning = false;
            --_nextEnemy;
        }
    }
}

void StateGame::eraseDeadEnemies() {
    // Destruimos todos los enemigos muertos
    for (std::vector<Enemy*>::iterator i = _enemies.begin(); i != _enemies.end(); ) {
        if ((*i)->getState() == Actor::ERASE) {
            // Añadimos los puntos
            _gameStats->killEnemy((*i)->getEnemyType());
            delete (*i);
            i = _enemies.erase(i);
        }
        else {
            ++i;
        }
    }    
}

Player* StateGame::getPlayer() {
    return _player;
}

GameStats& StateGame::getGameStats() {
    return *_gameStats;
}

void StateGame::setLevelCode(const Ogre::String& levelCode) {
    _levelCode = levelCode;
}


void StateGame::btnResumeClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    
    // Desactivamos la pausa
    _state = PLAYING;
    
    // Ocultamos el panel pausa
    _panelPause->setVisible(false);
    
    // Activamos botones
    _btnPause->setEnabled(true);
    _btnFire->setEnabled(true);
    _btnGea->setEnabled(true);
    _btnBlizzard->setEnabled(true);
}

void StateGame::btnLevelSelectionClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    
    // Cambiamos a estado de selección de nivel
    _stateManager->changeState("stateLevel");
}

void StateGame::btnMenuClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    
    _stateManager->changeState("stateMenu");
}


void StateGame::btnPauseClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    
    _state = PAUSE;
    _panelPause->setVisible(true);
    _btnPause->setEnabled(false);
    _btnFire->setEnabled(false);
    _btnGea->setEnabled(false);
    _btnBlizzard->setEnabled(false);
}

void StateGame::btnFireClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    //_lblSpell->setCaption(_("Fire ball"));
    _player->setSpell(Spell::FIRE);
    
    _imgSelectedFire->setVisible(true);
    _imgSelectedBlizzard->setVisible(false);
    _imgSelectedGea->setVisible(false);
}

void StateGame::btnGeaClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    //_lblSpell->setCaption(_("Gea's fury"));
    _player->setSpell(Spell::GEA);
    
    _imgSelectedGea->setVisible(true);
    _imgSelectedFire->setVisible(false);
    _imgSelectedBlizzard->setVisible(false);
}
 
void StateGame::btnBlizzardClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    //_lblSpell->setCaption(_("Blizzard"));
    _player->setSpell(Spell::BLIZZARD);
    
    _imgSelectedBlizzard->setVisible(true);
    _imgSelectedFire->setVisible(false);
    _imgSelectedGea->setVisible(false);
}

void StateGame::btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old) {
    _optionOver->play();
}

void StateGame::btnFireFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old) {
    Spell::SpellData data = Spell::getSpellData(Spell::FIRE);
    
    _lblSpell->setCaption(data.name);
    _lblDescription->setCaption(data.description);
    
    char buffer[20];
    sprintf(buffer, _("Mana: %d"), data.mana);
    _lblManaCost->setCaption(buffer);
    
    sprintf(buffer, _("Power: %d"), data.power);
    _lblPower->setCaption(buffer);
    
    _lblSpell->setVisible(true);
    _lblDescription->setVisible(true);    
    _lblManaCost->setVisible(true); 
    _lblPower->setVisible(true);
}

void StateGame::btnGeaFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old) {
    Spell::SpellData data = Spell::getSpellData(Spell::GEA);
    
    _lblSpell->setCaption(data.name);
    _lblDescription->setCaption(data.description);
    
    char buffer[20];
    sprintf(buffer, _("Mana: %d"), data.mana);
    _lblManaCost->setCaption(buffer);
    
    sprintf(buffer, _("Power: %d"), data.power);
    _lblPower->setCaption(buffer);
    
    _lblSpell->setVisible(true);
    _lblDescription->setVisible(true);    
    _lblManaCost->setVisible(true); 
    _lblPower->setVisible(true);
}

void StateGame::btnBlizzardFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old) {
    Spell::SpellData data = Spell::getSpellData(Spell::BLIZZARD);
    
    _lblSpell->setCaption(data.name);
    _lblDescription->setCaption(data.description);
    
    char buffer[20];
    sprintf(buffer, _("Mana: %d"), data.mana);
    _lblManaCost->setCaption(buffer);
    
    sprintf(buffer, _("Power: %d"), data.power);
    _lblPower->setCaption(buffer);
    
    _lblSpell->setVisible(true);
    _lblDescription->setVisible(true);    
    _lblManaCost->setVisible(true); 
    _lblPower->setVisible(true);
}

void StateGame::btnSpellLostFocus(MyGUI::WidgetPtr sender, MyGUI::Widget* old) {
    _lblSpell->setVisible(false);
    _lblDescription->setVisible(false);    
    _lblManaCost->setVisible(false); 
    _lblPower->setVisible(false); 
}

void StateGame::translate() {
    _btnLevelSelection->setCaption(_(_btnLevelSelection->getCaption().asUTF8_c_str()));
    _btnMenu->setCaption(_(_btnMenu->getCaption().asUTF8_c_str()));
    _btnResume->setCaption(_(_btnResume->getCaption().asUTF8_c_str()));
    _btnPause->setCaption(_(_btnPause->getCaption().asUTF8_c_str()));
    _lblPause->setCaption(_(_lblPause->getCaption().asUTF8_c_str()));
    _lblLife->setCaption(_(_lblLife->getCaption().asUTF8_c_str()));
    _lblTime->setCaption(_(_lblTime->getCaption().asUTF8_c_str()));
    _lblMana->setCaption(_(_lblMana->getCaption().asUTF8_c_str()));
    _lblExperience->setCaption(_(_lblExperience->getCaption().asUTF8_c_str()));
    _lblSpell->setCaption(_(_lblSpell->getCaption().asUTF8_c_str()));
    _lblState->setCaption(_(_lblState->getCaption().asUTF8_c_str()));
}

void StateGame::adjustFontHeight() {
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    _btnLevelSelection->setFontHeight((int)(0.0347222 * screenHeight));
    _btnMenu->setFontHeight((int)(0.0347222 * screenHeight));
    _btnResume->setFontHeight((int)(0.0347222 * screenHeight));
    _btnPause->setFontHeight((int)(0.0277777 * screenHeight));
    _lblLife->setFontHeight((int)(0.0277777 * screenHeight));
    _lblMana->setFontHeight((int)(0.0277777 * screenHeight));
    _lblManaCost->setFontHeight((int)(0.0347222 * screenHeight));
    _lblPower->setFontHeight((int)(0.0347222 * screenHeight));
    _lblSpell->setFontHeight((int)(0.0277777 * screenHeight));
    _lblDescription->setFontHeight((int)(0.0277777 * screenHeight));
    _lblTime->setFontHeight((int)(0.0277777 * screenHeight));
    _lblTime2->setFontHeight((int)(0.0277777 * screenHeight));
    _lblExperience->setFontHeight((int)(0.0277777 * screenHeight));
    _lblExperience2->setFontHeight((int)(0.0277777 * screenHeight));
    _lblFPS->setFontHeight((int)(0.041666 * screenHeight));
    _lblPause->setFontHeight((int)(0.0347222 * screenHeight));
}
