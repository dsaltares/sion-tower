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
 *  @file state.cpp
 *  @author David Saltares Márquez
 *  @date 26-11-2010
 */

#include "state.h"

State::State(StateManager* stateManager): _stateManager(stateManager), _loaded(false) {
    // Tomamos el log
    _log = Ogre::LogManager::getSingleton().getLog("sionTowerLog");

    // Tomamos el SceneManager
    _sceneManager = Ogre::Root::getSingleton().getSceneManager("sceneManager");
};
