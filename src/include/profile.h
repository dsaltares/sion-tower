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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_PROFILE_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_PROFILE_H_

#include <OGRE/Ogre.h>


//! Modela un perfil de usuario con su experiencia y niveles desbloqueados

/**
 * @author David Saltares Márquez
 * @date 23-06-2011
 * 
 * La clase Level se utiliza para representar un perfil de usuario. Los
 * perfiles de usuario cuentan con un nombre que debe ser único, una cantidad
 * de experiencia obtenida y un nivel desbloqueado.
 */
class Profile {
    public:
        /**
         * Constructor
         * 
         * @param name nombre del usuario
         * @param experience experiencia inicial
         * @param unlockedLevel nivel más alto desbloqueado
         */
        Profile(const Ogre::String& name, int experience = 0, int unlockedLevel = 1);
        
        /**
         * @return nombre del perfil
         */
        const Ogre::String& getName() const;
        
        /**
         * @return experiencia acumulada
         */
        int getExperience() const;
        
        /**
         * @return nivel desbloqueado más avanzado
         */
        int getUnlockedLevel() const;
        
        /**
         * Añade experiencia al perfil
         * 
         * @param points puntos a añadir
         */
        void addExperience(unsigned int points);
        
        /**
         * Establece un nuevo nivel desbloqueado más alto
         * 
         * @param level nuevo nivel desbloqueado
         */
        void setUnlockedLevel(unsigned int level);
    private:
        Ogre::String _name;
        int _experience;
        int _unlockedLevel;
};

inline const Ogre::String& Profile::getName() const {return _name;}
inline int Profile::getExperience() const {return _experience;}
inline int Profile::getUnlockedLevel() const {return _unlockedLevel;}
        
inline void Profile::addExperience(unsigned int points) {_experience += points;}
inline void Profile::setUnlockedLevel(unsigned int level) {_unlockedLevel = level;}


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_PROFILE_H_
