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

#include <libintl.h>
#include <locale.h>

#include "game.h"
#include "stateCredits.h"
#include "stateManager.h"
#include "levelManager.h"
#include "soundFXManager.h"


#define _(x) gettext(x)

StateCredits::StateCredits(StateManager* stateManager): State(stateManager) {
     _myGUI = MyGUI::Gui::getInstancePtr();
}

StateCredits::~StateCredits() {
    clear();
}
        
void StateCredits::load() {
    if (!_loaded) {
        // Cargamos sonidos
        _optionOver = SoundFXManager::getSingleton().load("optionover.wav");
        _optionSelected = SoundFXManager::getSingleton().load("optionselected.wav");
        
        // Cargamos el layout
        _widgetLayout = MyGUI::LayoutManager::getInstance().loadLayout("credits.layout");
        
        // Recuperamos widgets y asignamos event handlers
        _btnBack = _myGUI->findWidget<MyGUI::Button>("btnBack");
        _btnBack->setNeedMouseFocus(true);
        _btnBack->eventMouseButtonClick += MyGUI::newDelegate(this, &StateCredits::btnBackClicked);
        _btnBack->eventMouseSetFocus += MyGUI::newDelegate(this, &StateCredits::btnMouseFocus);
        
        _lblSubtitle = _myGUI->findWidget<MyGUI::TextBox>("lblSubtitle");
        _lblProgramming = _myGUI->findWidget<MyGUI::TextBox>("lblProgramming");
        _lbl3DArtCharacters = _myGUI->findWidget<MyGUI::TextBox>("lbl3DArtCharacters");
        _lbl3DArtScenarios = _myGUI->findWidget<MyGUI::TextBox>("lbl3DArtScenarios");
        _lblMusic = _myGUI->findWidget<MyGUI::TextBox>("lblMusic");
        _lblSFX = _myGUI->findWidget<MyGUI::TextBox>("lblSFX");
        _lbl2DSpells = _myGUI->findWidget<MyGUI::TextBox>("lbl2DSpells");
        
        _lblDavid = _myGUI->findWidget<MyGUI::TextBox>("lblDavid1");
        _lblDavid2 = _myGUI->findWidget<MyGUI::TextBox>("lblDavid2");
        _lblAntonio = _myGUI->findWidget<MyGUI::TextBox>("lblAntonio");
        _lblEvergreen = _myGUI->findWidget<MyGUI::TextBox>("lblEvergreen");
        _lblFrancisco = _myGUI->findWidget<MyGUI::TextBox>("lblFrancisco");
        _lblDaniel = _myGUI->findWidget<MyGUI::TextBox>("lblDaniel");
        
        
        // Cargamos el nivel
        _level = new Level("credits");
        _level->load();
        
        // Cargamos música
        _song = _level->getSong();
        //_song->play();
        
        loadCharacters();
        
        // Traducimos
        translate();
        
        // Ajustamos la altura de la fuente
        adjustFontHeight();
        
        _loaded = true;
    }
}

void StateCredits::clear() {
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
        
void StateCredits::update(Ogre::Real deltaT, bool active) {
    if (active) {
        std::vector<Ogre::AnimationState*>::iterator i = _charactersAnimations.begin();
        // Actualizamos enemigos
        for (; i != _charactersAnimations.end(); ++i)
            (*i)->addTime(deltaT);
    }
}
        
bool StateCredits::keyPressed(const OIS::KeyEvent &arg) {
   MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
   
   return true;
}

bool StateCredits::keyReleased(const OIS::KeyEvent &arg) {
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    
    return true;
}

bool StateCredits::mouseMoved(const OIS::MouseEvent &arg) {
     MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
     
     return true;
}

bool StateCredits::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
     MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
     return true;
}

bool StateCredits::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
     MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
     
     return true;
}
        
void StateCredits::btnBackClicked(MyGUI::WidgetPtr sender) {
    _optionSelected->play();
    _stateManager->changeState("stateMenu");
}

void StateCredits::btnMouseFocus(MyGUI::Widget* sender, MyGUI::Widget* old) {
    _optionOver->play();
}

void StateCredits::loadCharacters() {
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

void StateCredits::unloadCharacters() {
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

void StateCredits::translate() {
    _lblSubtitle->setCaption(_(_lblSubtitle->getCaption().asUTF8_c_str()));
    _lblProgramming->setCaption(_(_lblProgramming->getCaption().asUTF8_c_str()));
    _lbl3DArtCharacters->setCaption(_(_lbl3DArtCharacters->getCaption().asUTF8_c_str()));
    _lbl3DArtScenarios->setCaption(_(_lbl3DArtScenarios->getCaption().asUTF8_c_str()));
    _lblMusic->setCaption(_(_lblMusic->getCaption().asUTF8_c_str()));
    _lblSFX->setCaption(_(_lblSFX->getCaption().asUTF8_c_str()));
    _lbl2DSpells->setCaption(_(_lbl2DSpells->getCaption().asUTF8_c_str()));
    _btnBack->setCaption(_(_btnBack->getCaption().asUTF8_c_str()));
}

void StateCredits::adjustFontHeight() {
    int screenHeight = Game::getViewPort()->getActualHeight();
    
    _lblSubtitle->setFontHeight((int)(0.055555 * screenHeight));
    _lblProgramming->setFontHeight((int)(0.0277777 * screenHeight));
    _lbl3DArtCharacters->setFontHeight((int)(0.0277777 * screenHeight));
    _lbl3DArtScenarios->setFontHeight((int)(0.0277777 * screenHeight));
    _lblMusic->setFontHeight((int)(0.0277777 * screenHeight));
    _lblSFX->setFontHeight((int)(0.0277777 * screenHeight));
    _lblDavid->setFontHeight((int)(0.0277777 * screenHeight));
    _lblDavid2->setFontHeight((int)(0.0277777 * screenHeight));
    _lblFrancisco->setFontHeight((int)(0.0277777 * screenHeight));
    _lblAntonio->setFontHeight((int)(0.0277777 * screenHeight));
    _lblEvergreen->setFontHeight((int)(0.0277777 * screenHeight));
    _lblDaniel->setFontHeight((int)(0.0277777 * screenHeight));
    _lbl2DSpells->setFontHeight((int)(0.0277777 * screenHeight));
    _btnBack->setFontHeight((int)(0.0416666 * screenHeight));
    
}
