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
 *  @file soundFX.cpp
 *  @author David Saltares Márquez
 *  @date 29-12-2010
 */

#include <fstream>

#include "soundFX.h"


SoundFX::SoundFX(Ogre::ResourceManager* creator,
                 const Ogre::String& name,
                 Ogre::ResourceHandle handle,
                 const Ogre::String& group,
                 bool isManual,
                 Ogre::ManualResourceLoader* loader):
                 Ogre::Resource(creator, name, handle, group, isManual, loader){

    // Creamos el tipo de recurso si no existía ya
    createParamDictionary("SoundFX");

    // Damos valor a los atributos
    _sound = 0;
    _path = "";
    _size = 0;
}

SoundFX::~SoundFX() {
    unload();
}

int SoundFX::play(int loop) {
    int channel;

    if ((channel = Mix_PlayChannel(-1, _sound, loop)) == -1) {
        Ogre::LogManager::getSingleton().logMessage("SoundFX::play(): error al reproducir el efecto");
        throw (Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                               "No se puede reproducir el efecto",
                               "SoundFX::play()"));
    }

    return channel;
}


void SoundFX::loadImpl() {
    // Buscar la ruta del fichero
    Ogre::FileInfoListPtr info;
    info = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, mName);
    for (Ogre::FileInfoList::iterator i = info->begin(); i != info->end(); ++i) {
        _path = i->archive->getName() + "/" + i->filename;
    }

    if (_path == "") {
            Ogre::LogManager::getSingleton().logMessage("SoundFX::loadImpl(): no se encuentra el recurso");
            throw (Ogre::Exception(Ogre::Exception::ERR_FILE_NOT_FOUND,
                                   "No se encuentra el fichero del efecto",
                                   "SoundFX::loadImpl()"));
    }

    // Cargar el efecto
    if ((_sound = Mix_LoadWAV(_path.c_str())) == NULL) {
            Ogre::LogManager::getSingleton().logMessage("SoundFX::loadImpl(): no se ha podido cargar el efecto");
            throw (Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                                   "No se ha podido cargar el efecto",
                                   "SoundFX::loadImpl()"));
    }

    // Calculamos el tamaño
    std::ifstream stream;
    char byteBuffer;

    stream.open(_path.c_str(), std::ios_base::binary);
    
    while (stream >> byteBuffer)
        ++_size;

    stream.close();
}

void SoundFX::unloadImpl() {
    if (_sound)
        Mix_FreeChunk(_sound);
}

size_t SoundFX::calculateSize() const {
    return _size;
}


SoundFXPtr::SoundFXPtr(const Ogre::ResourcePtr& r) {
    // Si r no es un recurso válido, no hacemos nada
    if (r.isNull())
        return;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)

    pRep = static_cast<SoundFX*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();

    if (pUseCount)
        ++(*pUseCount);
}

SoundFXPtr& SoundFXPtr::operator= (const Ogre::ResourcePtr& r) {
    if (pRep == static_cast<SoundFX*>(r.getPointer()))
        return *this;

    release();

    if (r.isNull())
        return *this;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
    
    pRep = static_cast<SoundFX*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();
    
    if (pUseCount)
        ++(*pUseCount);

    return *this;
}

