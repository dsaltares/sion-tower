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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_PROFILEMANAGER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_PROFILEMANAGER_H_

#include <vector>

#include <OGRE/Ogre.h>

#include "profile.h"


//! Gestiona los perfiles (Profile) y los guarda en memoria secundaria


/**
 * @author David Saltares Márquez
 * @date 23-06-2011
 * 
 * Clase que sigue el patrón de diseño Singleton y se encarga de gestionar
 * los perfiles de usuario en Sion Tower (Profile). Al inicio los lee
 * del disco duro, los actualiza en caso de ser necesario y es capaz
 * de volverlos a guardar.
 * 
 * Al inicio lee el fichero media/profiles.xml
 * 
 * Ejemplo de uso:
 * 
 * \code
 * // Al comienzo
 * ProfileManager* profileManager = new ProfileManager();
 * 
 * ...
 * 
 * // Crear un perfil
 * ProfileManager::getSingletonPtr()->addProfile("david");
 * 
 * // Seleccionar perfil
 * ProfileManager::getSingleton().selectProfile("david");
 * 
 * // Eliminar perfil
 * profileManager->deleteProfile("david");
 * 
 * ...
 * 
 * // Al final
 * delete profileManager;
 * \endcode
 */
class ProfileManager: public Ogre::Singleton<ProfileManager> {
    public:
        /**
         * Constructor
         */
        ProfileManager();
        
        /**
         * Destructor
         */
        ~ProfileManager();
        
        /**
         * @return referencia a la única instancia del gestor de perfiles
         */
        static ProfileManager& getSingleton();
        
        /**
         * @return puntero a la única instancia del gestor de perfiles
         */
        static ProfileManager* getSingletonPtr();
        
        /**
         * @return perfiles del sistema
         */
        const std::vector<Profile*>& getProfiles() const;
        
        /**
         * @param name nombre del perfil deseado
         * @return devuelve el perfil con el nombre dado, null si no existe.
         */
        Profile* getProfile(const Ogre::String& name);
        
        /**
         * @return perfil seleccionado
         */
        Profile* getSelectedProfile();
        
        /**
         * Crea y añade un perfil
         * 
         * @param name nombre del nuevo perfil
         */
        void addProfile(const Ogre::String& name);
        
        /**
         * Elimina un perfil
         * 
         * @param name nombre del perfil a eliminar
         */
        void deleteProfile(const Ogre::String& name);
        
        /**
         * Selecciona un perfil
         * 
         * @param name nombre del perfil a seleccionar
         */
        void selectProfile(const Ogre::String& name);
        
        /**
         * Guarda los perfiles del sistema en memoria secundaria
         */
        void saveProfiles();
    private:
        std::vector<Profile*> _profiles;
        unsigned int _selectedProfile;
        
        void readProfiles();
};

#endif   // SIONTOWER_TRUNK_SRC_INCLUDE_PROFILEMANAGER_H_
