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
#include <libintl.h>
#include <locale.h>

#include <OGRE/Ogre.h>

#include "stateMenu.h"
#include "stateManager.h"
#include "songManager.h"
#include "soundFXManager.h"
#include "game.h"
#include "levelManager.h"
#include "enemy.h"
#include "enemySpawn.h"

#define _(x) gettext(x)

using std::cout;
using std::endl;    

StateMenu::StateMenu(StateManager* stateManager): State(stateManager) {
    _myGUI = MyGUI::Gui::getInstancePtr();
}

StateMenu::~StateMenu() {
    clear();
}
        
void StateMenu::load() {
    if (!_loaded) {
        // Cargamos sonidos
        _optionOver = SoundFXManager::getSingleton().load("optionover.wav");
        _optionSelected = SoundFXManager::getSingleton().load("optionselected.wav");
        
        // Cargamos el layout
        _widgetLayout = MyGUI::LayoutManager::getInstance().loadLayout("mainmenu.layout");
        
        // Recuperamos widgets y asignamos event handlers
        _btnPlay = _myGUI->findWidget<MyGUI::Button>("btnPlay");
        _btnPlay->setNeedMouseFocus(true);
        _btnPlay->eventMouseButtonClick += MyGUI::newDelegate(this, &StateMenu::btnPlayClicked);
        _btnPlay->eventMouseSetFocus += MyGUI::newDelegate(this, &StateMenu::btnMouseFocus);
        
        _btnCredits = _myGUI->findWidget<MyGUI::Button>("btnCredits");
        _btnCredits->setNeedMouseFocus(true);
        _btnCredits->eventMouseButtonClick += MyGUI::newDelegate(this, &StateMenu::btnCreditsClicked);
        _btnCredits->eventMouseSetFocus += MyGUI::newDelegate(this, &StateMenu::btnMouseFocus);
        
        _btnExit = _myGUI->findWidget<MyGUI::Button>("btnExit");
        _btnExit->setNeedMouseFocus(true);
        _btnExit->eventMouseButtonClick += MyGUI::newDelegate(this, &StateMenu::btnExitClicked);
        _btnExit->eventMouseSetFocus += MyGUI::newDelegate(this, &StateMenu::btnMouseFocus);
        
        _lblSubtitle = _myGUI->findWidget<MyGUI::TextBox>("lblSubtitle");
        
        // Cargamos el nivel
        _level = new Level("mainmenu");
        _level->load();
        
        // Cargamos música
        _song = _level->getSong();
        _song->play();
        
        // SkyDome
        _sceneManager->setSkyDome(true, "sky", 30, 1.5, 5000, false);
        
        loadCharacters();
        
        // Traducimos
        translate();
        
        // Ajustamos la altura de la fuente
        adjustFontHeight();
        
        _loaded = true;
    }
}

void StateMenu::clear() {
    if (_loaded) {
        // Eliminamos el layout
        MyGUI::LayoutManager::getInstance().unloadLayout(_widgetLayout);
        _widgetLayout.clear();
        
        _level->unload();
        delete _level;
        
        // SkyDome
        _sceneManager->setSkyDome(false, "sky");
        
        // Limpiamos la escena
        //_sceneManager->clearScene();
    
        unloadCharacters();
        
        _loaded = false;
    }
}
        
void StateMenu::update(Ogre::Real deltaT, bool active) {
    if (active) {
        std::vector<Ogre::AnimationState*>::iterator i = _charactersAnimations.begin();
        // Actualizamos enemigos
        for (; i != _charactersAnimations.end(); ++i)
            (*i)->addTime(deltaT);
    }
}

bool StateMenu::keyPressed(const OIS::KeyEvent &arg) {
   MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
   
   return true;
}

bool StateMenu::keyReleased(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    
    return true;
}

bool StateMenu::mouseMoved(const OIS::MouseEvent &arg) {
     MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
     
     return true;
}

bool StateMenu::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
     MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
     return true;
}

bool StateMenu::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
     MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
     return true;
}

void StateMenu::btnPlayClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateProfile");
}

void StateMenu::btnCreditsClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateCredits");
}

void StateMenu::btnExitClicked(MyGUI::WidgetPtr sender)  {
    _optionSelected->play();
    _stateManager->exitGame();
}

void StateMenu::btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old) {
    _optionOver->play();
}

void StateMenu::loadCharacters() {
    Ogre::Entity* entity;
    Ogre::SceneNode* sceneNode;
    Ogre::AnimationState* animState;
    
    std::vector<EnemySpawn>::iterator i;
    for (i = _level->getEnemySpawns().begin(); i != _level->getEnemySpawns().end(); ++i) {
        if (i->getType() == Enemy::GOBLIN)
            entity = _sceneManager->createEntity("goblin.mesh");
        else if (i->getType() == Enemy::DEMON)
            entity = _sceneManager->createEntity("demonio.mesh");
        else if (i->getType() == Enemy::GOLEM)
            entity = _sceneManager->createEntity("golem.mesh");
        else
            entity = _sceneManager->createEntity("personaje.mesh");
        
        _charactersEntities.push_back(entity);
            
        sceneNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
        sceneNode->attachObject(entity);
        sceneNode->setPosition(i->getPosition());
        sceneNode->setOrientation(i->getOrientation());
        
        _charactersNodes.push_back(sceneNode);
        
        animState = entity->getAnimationState("idle");
        animState->setEnabled(true);
        animState->setLoop(true);
        
        _charactersAnimations.push_back(animState);
    }
}

void StateMenu::unloadCharacters() {
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


void StateMenu::translate() {
    _btnPlay->setCaption(_(_btnPlay->getCaption().asUTF8_c_str()));
    _btnCredits->setCaption(_(_btnCredits->getCaption().asUTF8_c_str()));
    _btnExit->setCaption(_(_btnExit->getCaption().asUTF8_c_str()));
    _lblSubtitle->setCaption(_(_lblSubtitle->getCaption().asUTF8_c_str()));
}

void StateMenu::adjustFontHeight() {
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    _lblSubtitle->setFontHeight((int)(0.0416667 * screenHeight));
    _btnPlay->setFontHeight((int)(0.0416667 * screenHeight));
    _btnCredits->setFontHeight((int)(0.0416667 * screenHeight));
    _btnExit->setFontHeight((int)(0.0416667 * screenHeight));
}
