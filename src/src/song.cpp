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
 *  @file song.cpp
 *  @author David Saltares Márquez
 *  @date 28-12-2010
 */

#include <fstream>
#include <iostream>

#include "song.h"

using std::endl;
using std::cout;

Song::Song(Ogre::ResourceManager* creator,
               const Ogre::String& name,
               Ogre::ResourceHandle handle,
               const Ogre::String& group,
               bool isManual,
               Ogre::ManualResourceLoader* loader):
               Ogre::Resource(creator, name, handle, group, isManual, loader) {
    // Creamos el tipo de recurso si no existía ya
    createParamDictionary("Song");

    // Puntero a Song no apunta a nada
    _song = 0;

    // Tamaño inicial
    _size = 0;
}

Song::~Song() {
    unload();
}

void Song::play(int loop) {
    if(Mix_PausedMusic())
            Mix_ResumeMusic();
    else{
        if(Mix_PlayMusic(_song, loop) == -1){
            Ogre::LogManager::getSingleton().logMessage("Song::play(): error al reproducir");
            throw (Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                                   "No se puede reproducir la música",
                                   "Song::play()"));
        }
    }
}

void Song::pause() {
    Mix_PauseMusic();
}

void Song::stop() {
    Mix_HaltMusic();
}

void Song::fadeIn(int ms, int loop) {
    if (Mix_FadeInMusic(_song, ms, loop) == -1) {
            Ogre::LogManager::getSingleton().logMessage("Song::fadeIn(): error al entrar suavizado");
            throw (Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                                   "No se puede reproducir la música con efecto suavizado",
                                   "Song::fadeIn()"));
    }
}

void Song::fadeOut(int ms) {
    if (Mix_FadeOutMusic(ms) == -1) {
            Ogre::LogManager::getSingleton().logMessage("Song::fadeOut(): error al salir suavizado");
            throw (Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                                   "No se puede parar la música con efecto suavizado",
                                   "Song::fadeOut()"));
    }
}

bool Song::isPlaying() {
    return Mix_PlayingMusic()? true : false;
}

void Song::loadImpl() {
    _path = "";

    // Buscar la ruta del fichero
    Ogre::FileInfoListPtr info;
    info = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, mName);
    for (Ogre::FileInfoList::iterator i = info->begin(); i != info->end(); ++i) {
        _path = i->archive->getName() + "/" + i->filename;
    }

    if (_path == "") {
            Ogre::LogManager::getSingleton().logMessage("Song::loadImpl(): no se encuentra el recurso");
            throw (Ogre::Exception(Ogre::Exception::ERR_FILE_NOT_FOUND,
                                   "No se encuentra el fichero de la música",
                                   "Song::loadImpl()"));
    }

    // Cargar la música
    if ((_song = Mix_LoadMUS(_path.c_str())) == NULL) {
            Ogre::LogManager::getSingleton().logMessage("Song::loadImpl(): no se puede cargar la música");
            throw (Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
                                   "No se puede cargar la música",
                                   "Song::loadImpl()"));
    }

    // Calculamos el tamaño
    std::ifstream stream;
    char byteBuffer;

    stream.open(_path.c_str(), std::ios_base::binary);
    
    while (stream >> byteBuffer)
        ++_size;

    stream.close();
}

void Song::unloadImpl() {
    if (_song)
        Mix_FreeMusic(_song);
}

size_t Song::calculateSize() const {
   
    //if(_song)
     //   return sizeof(*_song);
    return _size;
}


SongPtr::SongPtr(const Ogre::ResourcePtr &r): Ogre::SharedPtr<Song>() {
    // Si r no es un recurso válido, no hacemos nada
    if (r.isNull())
        return;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)

    pRep = static_cast<Song*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();

    if (pUseCount)
        ++(*pUseCount);

}

SongPtr& SongPtr::operator= (const Ogre::ResourcePtr& r) {
    if (pRep == static_cast<Song*>(r.getPointer()))
        return *this;

    release();

    if (r.isNull())
        return *this;

    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
    
    pRep = static_cast<Song*>(r.getPointer());
    pRep = static_cast<Song*>(r.getPointer());
    pUseCount = r.useCountPointer();
    useFreeMethod = r.freeMethod();
    
    if (pUseCount)
        ++(*pUseCount);

    return *this;
}
