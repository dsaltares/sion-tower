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
 
#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_ENEMYSPAWN_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_ENEMYSPAWN_H_

#include <OGRE/Ogre.h>

#include "enemy.h"

//! Modela la aparición de enemigos en el nivel

/**
 * @author David Saltares Márquez
 * @date 25-05-2011 
 * 
 * La clase EnemySpawn se encarga almacenar la información de la aparición
 * de un enemigo dentro del nivel. Guarda información sobre el tipo de enemigo,
 * su posición, orientación y el momento en el que debe aparecer. La clase
 * Level se encarga de parsear el fichero dotScene del nivel creado con Blender
 * y construir un vector de EnemySpawn con todas las apariciones de los enemigos.
 * StateGame es la encargada de crear los enemigos según lo que dicte el vector
 * de EnemySpawn.
 */
class EnemySpawn {
    public:
        
        /**
         * Constructor
         * 
         * @param type tipo de enemigo
         * @param position posición en la que aparecerá el enemigo
         * @param orientation orientación que tendrá inicialmente el enemigo
         * @param time tiempo en segundos desde que se inicia el nivel
         * hasta que debería aparecer el enemigo.
         */
        EnemySpawn(Enemy::Type type,
                   const Ogre::Vector3& position,
                   const Ogre::Quaternion& orientation,
                   Ogre::Real time);
        
        /**
         * Destructor
         * 
         * Libera la memoria utilizada
         */
        ~EnemySpawn();
        
        /**
         * @return tipo de enemigo que debe aparecer
         */
        Enemy::Type getType() const;
        
        /**
         * @return posición en la que debe aparecer el enemigo
         */
        const Ogre::Vector3& getPosition() const;
        
        /**
         * @return otientación inicial que debe tomar el enemigo
         */
        const Ogre::Quaternion& getOrientation() const;
        
        /**
         * @return tiempo desde el inicio del nivel hasta el momento en el que
         * debe aparecer el enemigo en segundos
         */
        Ogre::Real getTime() const;
    
        /**
         * @param spawn otra aparición de enemigo
         * @return true si la aparición actual tiene un tiempo menor que spawn
         */
        bool operator < (const EnemySpawn& spawn) const;
        
        /**
         * @param spawn otra aparición de enemigo
         * @return true si la aparición actual el mismo tiempo que spawn
         */
        bool operator == (const EnemySpawn& spawn) const;
    
    private:
        Enemy::Type _type;
        Ogre::Vector3 _position;
        Ogre::Quaternion _orientation;
        Ogre::Real _time;
};


inline Enemy::Type EnemySpawn::getType() const {return _type;}
inline const Ogre::Vector3& EnemySpawn::getPosition() const {return _position;}
inline const Ogre::Quaternion& EnemySpawn::getOrientation() const {return _orientation;}
inline Ogre::Real EnemySpawn::getTime() const {return _time;}

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_ENEMYSPAWN_H_
