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

#include <iostream>
#include <cstdio>
#include <cstring>
#include <libintl.h>
#include <locale.h>

#include "game.h"
#include "stateManager.h"
#include "stateVictory.h"
#include "stateGame.h"
#include "level.h"
#include "songManager.h"
#include "soundFXManager.h"
#include "profile.h"
#include "profileManager.h"
#include "levelManager.h"

#define _(x) gettext(x)

using std::cout;
using std::endl;

StateVictory::StateVictory(StateManager* stateManager): State(stateManager) {
    _myGUI = MyGUI::Gui::getInstancePtr();
    _levelManager = LevelManager::getSingletonPtr();
}

StateVictory::~StateVictory() {
    clear();
}
        
void StateVictory::load() {
    if (!_loaded) {
        // Cargamos sonidos
        _optionOver = SoundFXManager::getSingleton().load("optionover.wav");
        _optionSelected = SoundFXManager::getSingleton().load("optionselected.wav");
        
        // Cargamos el layout
        _widgetLayout = MyGUI::LayoutManager::getInstance().loadLayout("victory.layout");
        
        // Recuperamos widgets
        _lblNewLevel = _myGUI->findWidget<MyGUI::TextBox>("lblNewLevel");
        _lblSubtitle = _myGUI->findWidget<MyGUI::TextBox>("lblSubtitle");
        _btnMenu = _myGUI->findWidget<MyGUI::Button>("btnMenu");
        _btnRetry = _myGUI->findWidget<MyGUI::Button>("btnRetry");
        _btnLevelSelection = _myGUI->findWidget<MyGUI::Button>("btnLevelSelection");
        _lblExperience = _myGUI->findWidget<MyGUI::TextBox>("lblExperience");
        _lblEnemies = _myGUI->findWidget<MyGUI::TextBox>("lblEnemies");
        _lblLife = _myGUI->findWidget<MyGUI::TextBox>("lblLife");
        _lblMana = _myGUI->findWidget<MyGUI::TextBox>("lblMana");
        _lblTime = _myGUI->findWidget<MyGUI::TextBox>("lblTime");
        _lblTotal = _myGUI->findWidget<MyGUI::TextBox>("lblTotal");
        _lblEnemies2 = _myGUI->findWidget<MyGUI::TextBox>("lblEnemies2");
        _lblLife2 = _myGUI->findWidget<MyGUI::TextBox>("lblLife2");
        _lblMana2 = _myGUI->findWidget<MyGUI::TextBox>("lblMana2");
        _lblTime2 = _myGUI->findWidget<MyGUI::TextBox>("lblTime2");
        _lblTotal2 = _myGUI->findWidget<MyGUI::TextBox>("lblTotal2");
        _lblEnemies3 = _myGUI->findWidget<MyGUI::TextBox>("lblEnemies3");
        _lblLife3 = _myGUI->findWidget<MyGUI::TextBox>("lblLife3");
        _lblMana3 = _myGUI->findWidget<MyGUI::TextBox>("lblMana3");
        _lblTime3 = _myGUI->findWidget<MyGUI::TextBox>("lblTime3");
        
        // Enlazamos event handlers
        _btnMenu->setNeedMouseFocus(true);
        _btnMenu->eventMouseButtonClick += MyGUI::newDelegate(this, &StateVictory::btnMenuClicked);
        _btnMenu->eventMouseSetFocus += MyGUI::newDelegate(this, &StateVictory::btnMouseFocus);
        
        _btnRetry->setNeedMouseFocus(true);
        _btnRetry->eventMouseButtonClick += MyGUI::newDelegate(this, &StateVictory::btnRetryClicked);
        _btnRetry->eventMouseSetFocus += MyGUI::newDelegate(this, &StateVictory::btnMouseFocus);
        
        _btnLevelSelection->setNeedMouseFocus(true);
        _btnLevelSelection->eventMouseButtonClick += MyGUI::newDelegate(this, &StateVictory::btnLevelSelectionClicked);
        _btnLevelSelection->eventMouseSetFocus += MyGUI::newDelegate(this, &StateVictory::btnMouseFocus);
        
        // Cargamos el nivel
        _level = new Level("victory");
        _level->load();
        
        // Cargamos música
        _song = _level->getSong();
        _song->play();
        
        loadCharacters();
        
        // Estadísticas
        updateStats();
        
        // Traducimos
        translate();
        
        // Actualizamos los perfiles con la nueva experiencia
        Profile* profile = ProfileManager::getSingleton().getSelectedProfile();
        profile->addExperience(_gameStats.getTotal());
        
        // Intentamos desbloquear el nivel
        int result = _levelManager->tryUnlockLevel(_gameStats.getLevelCode(), profile->getUnlockedLevel());
        
        // El nivel ya estaba desbloqueado
        if (result == LevelManager::ALREADYUNLOCKED)
            _lblNewLevel->setCaption(_("You have completed this level again!"));
            
        // Era el último nivel
        else if (result == LevelManager::LASTLEVEL)
            _lblNewLevel->setCaption(_("You have saved the Sacred Tower!"));
        
        // Desbloqueamos el nivel
        else if (result == LevelManager::UNLOCKED)
            profile->setUnlockedLevel(profile->getUnlockedLevel() + 1);
        
        // Guardamos los perfiles
        ProfileManager::getSingleton().saveProfiles();
        
        // Ajustamos la altura de la fuente
        adjustFontHeight();
        
        _loaded = true;
    }
}

void StateVictory::clear() {
    if (_loaded) {
        // Eliminamos el layout
        MyGUI::LayoutManager::getInstance().unloadLayout(_widgetLayout);
        _widgetLayout.clear();
    
        _level->unload();
        delete _level;
        
        // Limpiamos la escena
        //_sceneManager->clearScene();
    
        unloadCharacters();
        
        _loaded = false;
    }
}
        
void StateVictory::update(Ogre::Real deltaT, bool active) {
    if (active) {
        // Actualizamos personajes
        for (size_t i = 0; i < _charactersEntities.size(); ++i) {
            if (_charactersEntities[i]->getMesh()->getName() == "personaje.mesh")
                _charactersAnimations[i]->addTime(deltaT * 1.25);
        }
    }
}
    

bool StateVictory::keyPressed(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
   
    return true;
}

bool StateVictory::keyReleased(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    
    return true;
}

bool StateVictory::mouseMoved(const OIS::MouseEvent &arg) {
    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
     
    return true;
}

bool StateVictory::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
    return true;
}

bool StateVictory::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
    return true;
}

void StateVictory::loadCharacters() {
    Ogre::Entity* entity;
    Ogre::SceneNode* sceneNode;
    Ogre::AnimationState* animState;
    
    std::vector<EnemySpawn>::iterator i;
    for (i = _level->getEnemySpawns().begin(); i != _level->getEnemySpawns().end(); ++i) {
        if (i->getType() == Enemy::PLAYER) {
            entity = _sceneManager->createEntity("personaje.mesh"); 
            _charactersEntities.push_back(entity);
                       
            sceneNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
            sceneNode->attachObject(entity);
            sceneNode->setPosition(i->getPosition());
            sceneNode->setOrientation(i->getOrientation());
            _charactersNodes.push_back(sceneNode);
            
            animState = entity->getAnimationState("win");
            animState->setEnabled(true);
            animState->setLoop(true);
            
            _charactersAnimations.push_back(animState);
        }
        else if (i->getType() == Enemy::GOBLIN) {
            entity = _sceneManager->createEntity("goblin.mesh");
            _charactersEntities.push_back(entity);
            
            sceneNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
            sceneNode->attachObject(entity);
            sceneNode->setPosition(i->getPosition());
            sceneNode->setOrientation(i->getOrientation());
            _charactersNodes.push_back(sceneNode);
            
            animState = entity->getAnimationState("die");
            animState->setEnabled(true);
            animState->setTimePosition(1.10);
            _charactersAnimations.push_back(animState);
        }
    }
}

void StateVictory::unloadCharacters() {
    for (size_t i = 0; i < _charactersEntities.size(); ++i) {
        _charactersNodes[i]->detachObject(_charactersEntities[i]);
        _sceneManager->destroyEntity(_charactersEntities[i]);
        _charactersNodes[i]->getParentSceneNode()->removeChild(_charactersNodes[i]);
        _sceneManager->destroySceneNode(_charactersNodes[i]);
    }
    
    _charactersAnimations.clear();
    _charactersEntities.clear();
    _charactersNodes.clear();
}


void StateVictory::btnMenuClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateMenu");
}

void StateVictory::btnRetryClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    StateGame* stateGame = static_cast<StateGame*>(_stateManager->changeState("stateGame"));
    stateGame->setLevelCode(_gameStats.getLevelCode());
}

void StateVictory::btnLevelSelectionClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateLevel");
}

void StateVictory::btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old) {
    _optionOver->play();
}

void StateVictory::setGameStats(const GameStats& gameStats) {
    _gameStats = gameStats;
}

void StateVictory::updateStats() {
    char buffer[20];
        
    // Enemigos
    sprintf(buffer, "%d", _gameStats.getEnemiesKilled());
    _lblEnemies2->setCaption(buffer);
    
    sprintf(buffer, "#00ff00%d", _gameStats.getEnemiesPoints()); 
    _lblEnemies3->setCaption(buffer);
    
    // Vida
    sprintf(buffer, "%d", _gameStats.getLife());
    _lblLife2->setCaption(buffer);
    
    sprintf(buffer, "#00ff00%d", _gameStats.getLifePoints());
    _lblLife3->setCaption(buffer);
    
    // Mana
    sprintf(buffer, "%d", _gameStats.getManaUsed());
    _lblMana2->setCaption(buffer);
    
    if (_gameStats.getManaPoints() >= 0)
        sprintf(buffer, "#00ff00%d", _gameStats.getManaPoints());
    else
        sprintf(buffer, "#ff0000%d", _gameStats.getManaPoints());
        
    _lblMana3->setCaption(buffer);
    
    // Tiempo
    sprintf(buffer, "%d", _gameStats.getTime());
    _lblTime2->setCaption(buffer);
    
    if (_gameStats.getTimePoints() >= 0)
        sprintf(buffer, "#00ff00%d", _gameStats.getTimePoints());
    else
        sprintf(buffer, "#ff0000%d", _gameStats.getTimePoints());
        
    _lblTime3->setCaption(buffer);
    
    // Total
    if (_gameStats.getTotal() >= 0)
        sprintf(buffer, "#00ff00%d", _gameStats.getTotal());
    else
        sprintf(buffer, "#ff0000%d", _gameStats.getTotal());
        
    _lblTotal2->setCaption(buffer);
}

void StateVictory::translate() {
    _btnLevelSelection->setCaption(_(_btnLevelSelection->getCaption().asUTF8_c_str()));
    _btnMenu->setCaption(_(_btnMenu->getCaption().asUTF8_c_str()));
    _btnRetry->setCaption(_(_btnRetry->getCaption().asUTF8_c_str()));
    _lblEnemies->setCaption(_(_lblEnemies->getCaption().asUTF8_c_str()));
    _lblExperience->setCaption(_(_lblExperience->getCaption().asUTF8_c_str()));
    _lblLife->setCaption(_(_lblLife->getCaption().asUTF8_c_str()));
    _lblMana->setCaption(_(_lblMana->getCaption().asUTF8_c_str()));
    _lblNewLevel->setCaption(_(_lblNewLevel->getCaption().asUTF8_c_str()));
    _lblSubtitle->setCaption(_(_lblSubtitle->getCaption().asUTF8_c_str()));
    _lblTime->setCaption(_(_lblTime->getCaption().asUTF8_c_str()));
    _lblTotal->setCaption(_(_lblTotal->getCaption().asUTF8_c_str()));
}


void StateVictory::adjustFontHeight() {
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    _btnLevelSelection->setFontHeight((int)(0.0416666 * screenHeight));
    _btnMenu->setFontHeight((int)(0.0416666 * screenHeight));
    _btnRetry->setFontHeight((int)(0.0416666 * screenHeight));
    _lblEnemies->setFontHeight((int)(0.027777 * screenHeight));
    _lblEnemies2->setFontHeight((int)(0.027777 * screenHeight));
    _lblEnemies3->setFontHeight((int)(0.027777 * screenHeight));
    _lblLife->setFontHeight((int)(0.027777 * screenHeight));
    _lblLife2->setFontHeight((int)(0.027777 * screenHeight));
    _lblLife3->setFontHeight((int)(0.027777 * screenHeight));
    _lblLife2->setFontHeight((int)(0.027777 * screenHeight));
    _lblMana->setFontHeight((int)(0.027777 * screenHeight));
    _lblMana2->setFontHeight((int)(0.027777 * screenHeight));
    _lblMana3->setFontHeight((int)(0.027777 * screenHeight));
    _lblTime->setFontHeight((int)(0.027777 * screenHeight));
    _lblTime2->setFontHeight((int)(0.027777 * screenHeight));
    _lblTime3->setFontHeight((int)(0.027777 * screenHeight));
    _lblTotal->setFontHeight((int)(0.027777 * screenHeight));
    _lblTotal2->setFontHeight((int)(0.027777 * screenHeight));
    _lblNewLevel->setFontHeight((int)(0.0416666 * screenHeight));
    _lblSubtitle->setFontHeight((int)(0.055555 * screenHeight));
    _lblExperience->setFontHeight((int)(0.027777 * screenHeight));
}
