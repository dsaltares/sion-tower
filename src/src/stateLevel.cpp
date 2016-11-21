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

#include "game.h"
#include "stateManager.h"
#include "stateLevel.h"
#include "stateGame.h"
#include "level.h"
#include "levelManager.h"
#include "profileManager.h"
#include "profile.h"
#include "soundFXManager.h"

#define _(x) gettext(x)

using std::cout;
using std::endl;

StateLevel::StateLevel(StateManager* stateManager): State(stateManager) {
    _myGUI = MyGUI::Gui::getInstancePtr();
    _levelManager = LevelManager::getSingletonPtr();
    _unlockedLevel = 1;
}

StateLevel::~StateLevel() {
    clear();
}

void StateLevel::load() {
    if (!_loaded) {
        // Cargamos sonidos
        _optionOver = SoundFXManager::getSingleton().load("optionover.wav");
        _optionSelected = SoundFXManager::getSingleton().load("optionselected.wav");
        _optionError = SoundFXManager::getSingleton().load("optionerror.wav");
        
        // Nivel desbloqueado
        Profile* profile = ProfileManager::getSingleton().getSelectedProfile();
        
        if (profile)
            _unlockedLevel = profile->getUnlockedLevel();
        
        // Cargamos el layout
        _widgetLayout = MyGUI::LayoutManager::getInstance().loadLayout("level.layout");
        
        // Recuperamos widgets
        _btnBack = _myGUI->findWidget<MyGUI::Button>("btnBack");
        _lblSubtitle = _myGUI->findWidget<MyGUI::TextBox>("lblSubtitle");
        
        // Asignamos event handlers
        _btnBack->setNeedMouseFocus(true);
        _btnBack->eventMouseButtonClick += MyGUI::newDelegate(this, &StateLevel::btnBackClicked);
        _btnBack->eventMouseSetFocus += MyGUI::newDelegate(this, &StateLevel::btnMouseFocus);

		loadLevelsMenu();
        
        // Cargamos el nivel
        _level = new Level("levelselection");
        _level->load();
        
        // Cargamos música
        _song = _level->getSong();
        _song->play();
        
        loadCharacters();
        
        // SkyDome
        _sceneManager->setSkyDome(true, "sky", 30, 1.5, 5000, false);
        
        // Traducimos
        translate();
        
        // Ajustamos la altura de la fuente
        adjustFontHeight();
        
        _loaded = true;
    }
}

void StateLevel::clear() {
    if (_loaded) {
        // Eliminamos el layout
        MyGUI::LayoutManager::getInstance().unloadLayout(_widgetLayout);
        _widgetLayout.clear();
        
        _myGUI->destroyAllChildWidget();
        
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

void StateLevel::update(Ogre::Real deltaT, bool active) {
    if (active) {
        std::vector<Ogre::AnimationState*>::iterator i = _charactersAnimations.begin();
        // Actualizamos enemigos
        for (; i != _charactersAnimations.end(); ++i)
            (*i)->addTime(deltaT);
    }
}

bool StateLevel::keyPressed(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
   
    return true;
}

bool StateLevel::keyReleased(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    
    return true;
}

bool StateLevel::mouseMoved(const OIS::MouseEvent &arg) {
    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
     
    return true;
}

bool StateLevel::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
    return true;
}

bool StateLevel::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
    return true;
}

void StateLevel::btnBackClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateProfile");
}

void StateLevel::loadCharacters() {
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

void StateLevel::unloadCharacters() {
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

void StateLevel::loadLevelsMenu() {
    int screenWidth = Game::getViewPort()->getActualWidth();
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    MyGUI::IntCoord winSize((int)(0.55078 * screenWidth),
                            (int)(0.2638 * screenHeight),
                            (int)(0.429687 * screenWidth),
                            (int)(0.513888 * screenHeight));
    
    // Creamos la ventana
    MyGUI::Window* window = _myGUI->createWidget<MyGUI::Window>("WindowCS", winSize, MyGUI::Align::Default, "Main");
    window->setMinSize(winSize.width, winSize.height);
    window->setMaxSize(winSize.width, winSize.height);
    window->setMovable(false);
	window->setCaption(_("Select a level"));
    MyGUI::TextBox* lblWindow = window->getCaptionWidget();
    lblWindow->setFontHeight((int)(0.0277777 * screenHeight));
    lblWindow->setInheritsAlpha(false);
    
    // Creamos el ScrollView
	MyGUI::ScrollView* scrollView = window->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(2, 2, window->getClientCoord().width - 2, window->getClientCoord().height - 2), MyGUI::Align::Stretch);
	scrollView->setCanvasSize((int)(0.41406 * screenWidth), (int)(0.513888 * screenHeight));
    
    int top = 0;
    int levelNumber = 1;
    
    // Recorremos los niveles
    std::vector<Level*>::iterator i;
    for (i = _levelManager->getLevels().begin(); i != _levelManager->getLevels().end(); ++i) {
        // Creamos el panel
        MyGUI::IntCoord panelSize((int)(0.003125 * screenWidth), top, (int)(0.398437 * screenWidth), (int)(0.1722222 * screenHeight));
        MyGUI::Widget* panelLevel = scrollView->createWidget<MyGUI::Widget>("PanelSkin", panelSize, MyGUI::Align::Default);
        
        // Creamos la imagen
        MyGUI::IntCoord imgSize((int)(0.00546 * screenWidth), (int)(0.0097222 * screenHeight), (int)(0.0859 * screenWidth), (int)(0.152777 * screenHeight));
        MyGUI::ImageBox* imgIcon = panelLevel->createWidget<MyGUI::ImageBox>("ImageBox", imgSize, MyGUI::Align::Default);
        imgIcon->setImageTexture((*i)->getId() + Ogre::String(".png"));
        
        // Nombre
        MyGUI::IntCoord nameSize((int)(0.105468 * screenWidth), (int)(0.027777 * screenHeight), (int)(0.292968 * screenWidth), (int)(0.027777 * screenHeight));
        MyGUI::TextBox* lblName = panelLevel->createWidget<MyGUI::TextBox>("TextBox", nameSize, MyGUI::Align::Default);
        lblName->setFontHeight((int)(0.0277777 * screenHeight));
        lblName->setCaption(_((*i)->getName().c_str()));
        
        // Descripción
        MyGUI::IntCoord descSize((int)(0.105468 * screenWidth), (int)(0.06944 * screenHeight), (int)(0.292968 * screenWidth), (int)(0.11111 * screenHeight));
        MyGUI::TextBox* lblDescription = panelLevel->createWidget<MyGUI::TextBox>("TextBox", descSize, MyGUI::Align::Default);
        lblDescription->setFontHeight((int)(0.0277777 * screenHeight));
        lblDescription->setCaptionWithReplacing(_((*i)->getDescription().c_str()));
        
        // Asociamos el widget con el código de nivel
        _levels[imgIcon] = (*i)->getId();
        
        // Actualizamos top
        top += (int)(0.1805555 * screenHeight);
        
        // Cambiamos el tamaño del canvas
        scrollView->setCanvasSize((int)(0.41406 * screenWidth), top + (int)(0.02777 * screenHeight));
        
        if (levelNumber <= _unlockedLevel) {
            imgIcon->setNeedMouseFocus(true);
            imgIcon->eventMouseButtonClick += MyGUI::newDelegate(this, &StateLevel::btnLevelClicked);
            imgIcon->eventMouseSetFocus += MyGUI::newDelegate(this, &StateLevel::btnMouseFocus);
        }
        else {
            imgIcon->eventMouseButtonClick += MyGUI::newDelegate(this, &StateLevel::btnLevelLockedClicked);
            imgIcon->setAlpha(0.5);
            MyGUI::TextBox* lblLocked;
            MyGUI::IntCoord lockedSize((int)(0.273437 * screenWidth), (int)(0.027777 * screenHeight), (int)(0.292968 * screenWidth), (int)(0.027777 * screenHeight));
            lblLocked = panelLevel->createWidget<MyGUI::TextBox>("TextBox", lockedSize, MyGUI::Align::Default);
            lblLocked->setFontHeight((int)(0.0277777 * screenHeight));
            lblLocked->setCaption(_("#ff0000Locked level"));
        }
        
        ++levelNumber;
    }
}

void StateLevel::btnLevelClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    
    Ogre::String levelId = _levels[sender];
    
    StateGame* state = static_cast<StateGame*>(_stateManager->changeState("stateGame"));
    state->setLevelCode(levelId);
}


void StateLevel::btnLevelLockedClicked(MyGUI::WidgetPtr sender) {
    _optionError->play();
}

void StateLevel::btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old) {
    _optionOver->play();
}


void StateLevel::translate() {
    _lblSubtitle->setCaption(_(_lblSubtitle->getCaption().asUTF8_c_str()));
    _btnBack->setCaption(_(_btnBack->getCaption().asUTF8_c_str()));
}


void StateLevel::adjustFontHeight() {
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    _btnBack->setFontHeight((int)(0.0416666 * screenHeight));
    _lblSubtitle->setFontHeight((int)(0.055555 * screenHeight));
}


