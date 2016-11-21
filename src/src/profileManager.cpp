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

#include "pugixml.hpp"

#include "profileManager.h"

using std::cout;
using std::endl;
using std::cerr;

template<> ProfileManager* Ogre::Singleton<ProfileManager>::ms_Singleton = 0;

ProfileManager::ProfileManager() {
    cout << "ProfileManager::ProfileManager()" << endl;
    
    _selectedProfile = -1;
    
    readProfiles();
}

ProfileManager::~ProfileManager() {
    cout << "ProfileManager::~ProfileManager()" << endl;
    
    // Limpiamos los perfiles
    for (std::vector<Profile*>::iterator i = _profiles.begin(); i != _profiles.end(); ++i)
        delete *i;
}
        
ProfileManager& ProfileManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

ProfileManager* ProfileManager::getSingletonPtr() {
    assert(ms_Singleton);
    return (ms_Singleton);
}
        
const std::vector<Profile*>& ProfileManager::getProfiles() const {
    return _profiles;
}

Profile* ProfileManager::getProfile(const Ogre::String& name) {
    // Recorremos los perfiles buscando el deseado
    for (std::vector<Profile*>::iterator i = _profiles.begin(); i != _profiles.end(); ++i)
        if ((*i)->getName() == name)
            return *i;
    
    // No hemos encontrado el perfil buscado        
    return 0;
}

Profile* ProfileManager::getSelectedProfile() {
    if (_selectedProfile >= 0 && _selectedProfile < _profiles.size())
        return _profiles[_selectedProfile];
    else
        return 0;
}

void ProfileManager::addProfile(const Ogre::String& name) {
    // Creamos un nuevo perfil
    Profile* newProfile = new Profile(name);
    _profiles.push_back(newProfile);
    
    // Actualizamos el fichero de perfiles
    saveProfiles();
}

void ProfileManager::deleteProfile(const Ogre::String& name) {
    // Buscamos el perfil con el nombre dado
    for (std::vector<Profile*>::iterator i = _profiles.begin(); i != _profiles.end(); ++i) {
        if ((*i)->getName() == name) {
            // Eliminamos el perfil
            delete *i;
            _profiles.erase(i);
            
            // Actualizamos el fichero de perfiles
            saveProfiles();
            
            break;
        }
    }
}

void ProfileManager::selectProfile(const Ogre::String& name) {
    int counter = 0;
    
    // Buscamos el perfil con el nombre dado
    for (std::vector<Profile*>::iterator i = _profiles.begin(); i != _profiles.end(); ++i) {
        if ((*i)->getName() == name) {
            _selectedProfile = counter;
            break;
        }
        
        ++counter;
    }
}

void ProfileManager::readProfiles() {
    // Abrimos el fichero media/profiles.xml
    pugi::xml_document doc;
    if (!doc.load_file("media/profiles.xml")) {
        cerr << "ProfileManager::readProfiles(): error al leer el fichero media/profiles.xml" << endl;
        exit(1);
    }
    
    // Recorremos los perfiles
    pugi::xml_node profilesNode = doc.child("profiles");
    pugi::xml_node profileNode;
    
    for (profileNode = profilesNode.child("profile"); profileNode; profileNode = profileNode.next_sibling("profile")) {
        // Tomamos sus atributos
        Ogre::String name = profileNode.attribute("name").value();
        int experience = profileNode.attribute("experience").as_int();
        int unlockedLevel = profileNode.attribute("unlockedLevel").as_int();
        
        // Creamos el perfil
        Profile* profile = new Profile(name, experience, unlockedLevel);
        _profiles.push_back(profile);
    }
    
    // Perfil provisional
    if (_profiles.size() != 0)
        _selectedProfile = 0;
}

void ProfileManager::saveProfiles() {
    // Documento
    pugi::xml_document doc;
    
    // Añadimos un nodo raíz
    pugi::xml_node profilesNode = doc.append_child("profiles");
    
    // Recorremos el vector de perfiles añadiéndolos al XML
    for (std::vector<Profile*>::iterator i = _profiles.begin(); i != _profiles.end(); ++i) {
        // Creamos el nodo profile
        pugi::xml_node profileNode = profilesNode.append_child("profile");
        
        // Añadimos los atributos
        profileNode.append_attribute("name");
        profileNode.append_attribute("experience");
        profileNode.append_attribute("unlockedLevel");
        
        // Damos valor a los atributos
        profileNode.attribute("name").set_value((*i)->getName().c_str());
        profileNode.attribute("experience").set_value((*i)->getExperience());
        profileNode.attribute("unlockedLevel").set_value((*i)->getUnlockedLevel());
    }
    
    // Guardamos el xml en media/profiles.xml
    std::cout << "Saving result: " << doc.save_file("media/profiles.xml") << std::endl;
}
