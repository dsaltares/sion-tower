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
 *  @file soundFXManager.cpp
 *  @author David Saltares Márquez
 *  @date 29-12-2010
 */

#include <SDL/SDL_mixer.h>

#include "soundFXManager.h"



template<> SoundFXManager* Ogre::Singleton<SoundFXManager>::ms_Singleton = 0;

int SoundFXManager::numChannels = 32;
        
SoundFXManager::SoundFXManager() {
    // Tipo de recursos que gestiona
    mResourceType = "SoundFX";

    // Prioridad de carga
    mLoadOrder = 30.f;

    // Registramos el nuevo ResourceManager
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

    // Reservamos 32 canales de audio
    Mix_AllocateChannels(numChannels);
}

SoundFXManager::~SoundFXManager() {
    // Borramos el gestor de la lista de ResourceManager
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

SoundFXPtr SoundFXManager::load(const Ogre::String& name, const Ogre::String& group) {
    // Obtenemos el SoundFXPtr
    SoundFXPtr soundFXPtr = getByName(name);

    // Si no está creado, lo creamos
    if (soundFXPtr.isNull())
        soundFXPtr = create(name, group);
    
    // Cargamos el recurso
    soundFXPtr->load();

    return soundFXPtr;
}

SoundFXManager& SoundFXManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

SoundFXManager* SoundFXManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

int SoundFXManager::getAvailableChannels() {
    return numChannels;
}

Ogre::Resource* SoundFXManager::createImpl(const Ogre::String& name,
                                           Ogre::ResourceHandle handle,
                                           const Ogre::String& group,
                                           bool isManual,
                                           Ogre::ManualResourceLoader* loader,
                                           const Ogre::NameValuePairList* createParams) {
    return new SoundFX(this, name, handle, group, isManual, loader);
}


