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
 *  @file soundManager.cpp
 *  @author David Saltares Márquez
 *  @date 29-12-2010
 */

#include "songManager.h"


template<> SongManager* Ogre::Singleton<SongManager>::ms_Singleton = 0;

SongManager::SongManager() {
    mResourceType = "Song";
    mLoadOrder = 30.f;
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

SongManager::~SongManager() {
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

SongPtr SongManager::load(const Ogre::String& name, const Ogre::String& group) {
    SongPtr songPtr = getByName(name);

    if (songPtr.isNull()) 
        songPtr = create(name, group);

    songPtr->load();

    return songPtr;
}

SongManager& SongManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

SongManager* SongManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

Ogre::Resource* SongManager::createImpl(const Ogre::String& name,
                                          Ogre::ResourceHandle handle,
                                          const Ogre::String& group,
                                          bool isManual,
                                          Ogre::ManualResourceLoader* loader,
                                          const Ogre::NameValuePairList* createParams) {
    return new Song(this, name, handle, group, isManual, loader);
}
