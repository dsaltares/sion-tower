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
#include <cstring>
#include <cstdio>
#include <libintl.h>
#include <locale.h>

#include "game.h"
#include "stateProfile.h"
#include "stateLevel.h"
#include "stateManager.h"
#include "level.h"
#include "profileManager.h"
#include "levelManager.h"
#include "soundFXManager.h"

#define _(x) gettext(x)

using std::endl;
using std::cout;
using std::cerr;

StateProfile::StateProfile(StateManager* stateManager): State(stateManager) {
    _myGUI = MyGUI::Gui::getInstancePtr();
    _profileManager = ProfileManager::getSingletonPtr();
}

StateProfile::~StateProfile() {
    clear();
}
        
void StateProfile::load() {
    if (!_loaded) {
        // Cargamos sonidos
        _optionOver = SoundFXManager::getSingleton().load("optionover.wav");
        _optionSelected = SoundFXManager::getSingleton().load("optionselected.wav");
        _optionError = SoundFXManager::getSingleton().load("optionerror.wav");
        
        // Cargamos el layout
        _widgetLayout = MyGUI::LayoutManager::getInstance().loadLayout("profile.layout");
        
        // Recuperamos widgets y asignamos event handlers
        _btnBack = _myGUI->findWidget<MyGUI::Button>("btnBack");
        _btnBack->setNeedMouseFocus(true);
        _btnBack->eventMouseButtonClick += MyGUI::newDelegate(this, &StateProfile::btnBackClicked);
        _btnBack->eventMouseSetFocus += MyGUI::newDelegate(this, &StateProfile::btnMouseFocus);
        
        _btnAccept = _myGUI->findWidget<MyGUI::Button>("btnAccept");
        _btnAccept->setNeedMouseFocus(true);
        _btnAccept->eventMouseButtonClick += MyGUI::newDelegate(this, &StateProfile::btnAcceptClicked);
        _btnAccept->eventMouseSetFocus += MyGUI::newDelegate(this, &StateProfile::btnMouseFocus);
        
        _btnCreate = _myGUI->findWidget<MyGUI::Button>("btnCreate");
        _btnCreate->setNeedMouseFocus(true);
        _btnCreate->eventMouseButtonClick += MyGUI::newDelegate(this, &StateProfile::btnCreateClicked);
        _btnCreate->eventMouseSetFocus += MyGUI::newDelegate(this, &StateProfile::btnMouseFocus);
        
        _btnDelete = _myGUI->findWidget<MyGUI::Button>("btnDelete");
        _btnDelete->setNeedMouseFocus(true);
        _btnDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &StateProfile::btnDeleteClicked);
        _btnDelete->eventMouseSetFocus += MyGUI::newDelegate(this, &StateProfile::btnMouseFocus);
        
        _listProfiles = _myGUI->findWidget<MyGUI::ListBox>("listProfiles");
        _listProfiles->eventListChangePosition += MyGUI::newDelegate(this, &StateProfile::listItemAccepted);
        
        _editName = _myGUI->findWidget<MyGUI::Edit>("editName");
        _lblErrorCreate = _myGUI->findWidget<MyGUI::TextBox>("lblErrorCreate");
        _lblExperience = _myGUI->findWidget<MyGUI::TextBox>("lblExperience");
        _lblExperience2 = _myGUI->findWidget<MyGUI::TextBox>("lblExperience2");
        _lblLevels = _myGUI->findWidget<MyGUI::TextBox>("lblLevels");
        _lblLevels2 = _myGUI->findWidget<MyGUI::TextBox>("lblLevels2");
        _lblProfileSel = _myGUI->findWidget<MyGUI::TextBox>("lblProfileSel");
        _lblProfileCreation = _myGUI->findWidget<MyGUI::TextBox>("lblProfileCreation");
        _lblName = _myGUI->findWidget<MyGUI::TextBox>("lblName");
        _lblSubtitle = _myGUI->findWidget<MyGUI::TextBox>("lblSubtitle");
    
        
        _lblLevels2->setCaption("");
        _lblExperience2->setCaption(""); 
        
        // Cargamos el nivel
        _level = new Level("profileselection");
        _level->load();
        
        // Cargamos música
        _song = _level->getSong();
        _song->play();
        
        loadCharacters();
        
        // Rellenamos la lista de perfiles
        const std::vector<Profile*>& profiles = _profileManager->getProfiles();
        for (std::vector<Profile*>::const_iterator i = profiles.begin(); i != profiles.end(); ++i)
            _listProfiles->addItem((*i)->getName());
        
        // Traducimos
        translate();
        
        // Ajustamos la altura de la fuente
        adjustFontHeight();
        
        _loaded = true;
    }
}

void StateProfile::clear() {
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
        
void StateProfile::update(Ogre::Real deltaT, bool active) {
    if (active) {
        std::vector<Ogre::AnimationState*>::iterator i = _charactersAnimations.begin();
        // Actualizamos enemigos
        for (; i != _charactersAnimations.end(); ++i)
            (*i)->addTime(deltaT);
    }
}
        
bool StateProfile::keyPressed(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
   
    return true;
}

bool StateProfile::keyReleased(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    
    return true;
}

bool StateProfile::mouseMoved(const OIS::MouseEvent &arg) {
    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
     
    return true;
}

bool StateProfile::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
    return true;
}

bool StateProfile::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    _lblErrorCreate->setCaption("");
    
    MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
    return true;
}
        
void StateProfile::btnBackClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateMenu");
}

void StateProfile::btnCreateClicked(MyGUI::WidgetPtr sender) {
    // Recuperamos el nombre seleccionado
    MyGUI::UString profileName = _editName->getCaption();
    
    // Comprobamos que no está vacío
    if (profileName == "") {
        _optionError->play();
        _lblErrorCreate->setCaption(_("#ff0000Error: you must enter a name"));
        return;
    }
    
    // Comprobamos que no existe
    if (_listProfiles->findItemIndexWith(profileName) != MyGUI::ITEM_NONE) {
        _optionError->play();
        _lblErrorCreate->setCaption(_("#ff0000Error: the profile already exists"));
        return;
    }
    
    _optionSelected->play();
    
    // Añadimos el perfil
    _profileManager->addProfile(profileName);
    _listProfiles->addItem(profileName);
    
    // Limpiamos el edit
    _editName->setCaption("");
    
    // Lo seleccionamos en la lista
    _listProfiles->setIndexSelected(_listProfiles->findItemIndexWith(profileName));
    retrieveData(_listProfiles->getIndexSelected());
}

void StateProfile::btnAcceptClicked(MyGUI::WidgetPtr sender) {
    size_t indexSelected = _listProfiles->getIndexSelected();
    
    // Seleccionamos el perfil y pasamos al estado selección de nivel
    if (indexSelected != MyGUI::ITEM_NONE) {
        _optionSelected->play();
        Ogre::String name = _listProfiles->getItemNameAt(indexSelected);
        _profileManager->selectProfile(name);
        _stateManager->changeState("stateLevel");
    }
    else {
        _optionError->play();
    }
}

void StateProfile::btnDeleteClicked(MyGUI::WidgetPtr sender) {
    size_t indexSelected = _listProfiles->getIndexSelected();
    
    if (indexSelected != MyGUI::ITEM_NONE) {
        _profileManager->deleteProfile(_listProfiles->getItemNameAt(indexSelected));
        _listProfiles->removeItemAt(indexSelected);
        _lblLevels2->setCaption("");
        _lblExperience2->setCaption(""); 
    }
    else
        _optionError->play();
}

void StateProfile::listItemAccepted(MyGUI::WidgetPtr sender, size_t index) {
    retrieveData(index);
}

void StateProfile::btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old) {
    _optionOver->play();
}

void StateProfile::retrieveData(size_t index) {
    if (index != MyGUI::ITEM_NONE) {
        MyGUI::UString name = _listProfiles->getItemNameAt(index);
        Profile* profile = _profileManager->getProfile(name);
        
        if (profile) {
            char buffer[20];
            
            sprintf(buffer, "%d", profile->getUnlockedLevel());
            _lblLevels2->setCaption(buffer);
            
            sprintf(buffer, "%d", profile->getExperience());
            _lblExperience2->setCaption(buffer); 
        }
    }
}

void StateProfile::loadCharacters() {
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

void StateProfile::unloadCharacters() {
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


void StateProfile::translate() {
    _lblSubtitle->setCaption(_(_lblSubtitle->getCaption().asUTF8_c_str()));
    _btnBack->setCaption(_(_btnBack->getCaption().asUTF8_c_str()));
    _btnAccept->setCaption(_(_btnAccept->getCaption().asUTF8_c_str()));
    _btnCreate->setCaption(_(_btnCreate->getCaption().asUTF8_c_str()));
    _btnDelete->setCaption(_(_btnDelete->getCaption().asUTF8_c_str()));
    _lblProfileSel->setCaption(_(_lblProfileSel->getCaption().asUTF8_c_str()));
    _lblLevels->setCaption(_(_lblLevels->getCaption().asUTF8_c_str()));
    _lblExperience->setCaption(_(_lblExperience->getCaption().asUTF8_c_str()));
    _lblProfileCreation->setCaption(_(_lblProfileCreation->getCaption().asUTF8_c_str()));
    _lblName->setCaption(_(_lblName->getCaption().asUTF8_c_str()));
}

void StateProfile::adjustFontHeight() {
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    _lblSubtitle->setFontHeight((int)(0.055555 * screenHeight));
    _lblProfileSel->setFontHeight((int)(0.027777 * screenHeight));
    _lblProfileCreation->setFontHeight((int)(0.027777 * screenHeight));
    _lblLevels->setFontHeight((int)(0.027777 * screenHeight));
    _lblLevels2->setFontHeight((int)(0.027777 * screenHeight));
    _lblExperience->setFontHeight((int)(0.027777 * screenHeight));
    _lblExperience2->setFontHeight((int)(0.027777 * screenHeight));
    _lblErrorCreate->setFontHeight((int)(0.027777 * screenHeight));
    _lblName->setFontHeight((int)(0.027777 * screenHeight));
    _btnAccept->setFontHeight((int)(0.027777 * screenHeight));
    _btnCreate->setFontHeight((int)(0.027777 * screenHeight));
    _btnDelete->setFontHeight((int)(0.027777 * screenHeight));
    _btnBack->setFontHeight((int)(0.0416666 * screenHeight));
    _editName->setFontHeight((int)(0.027777 * screenHeight));
}
