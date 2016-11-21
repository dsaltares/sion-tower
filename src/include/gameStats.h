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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_GAMESTATS_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_GAMESTATS_H_

#include <OGRE/Ogre.h>
#include <boost/unordered_map.hpp>

#include "enemy.h"


//! Modela las estadísticas de juego

/**
 * @author David Saltares Márquez
 * @date 28-06-2011
 * 
 * Esta clase almacena la información sobre las estadísticas que miden
 * el rendimiento del jugador durante la partida y ayudan a calcular su
 * experiencia obtenida. Se miden los parámetros: vida restante, maná
 * utilizado, enemigos eliminados y tiempo empleado en completar el nivel.
 */
class GameStats {
    public:
        /**
         * Constructor
         * 
         * @param levelCode nivel al que estamos jugando
         */
        GameStats(const Ogre::String& levelCode = "level01");

        /**
         * @return código del nivel al que estamos jugando
         */
        const Ogre::String& getLevelCode() const;
        
        /**
         * @return número de enemigos eliminados
         */
        int getEnemiesKilled();
        
        /**
         * @return puntos de experiencia por enemigos eliminados
         */
        int getEnemiesPoints() const;
        
        /**
         * @param maná utilizado
         */
        int getManaUsed() const;
        
        /**
         * @return puntos de experiencia por maná utilizado
         */
        int getManaPoints() const;
        
        /**
         * @return vida restante
         */
        int getLife() const;
        
        /**
         * @return puntos de experiencia obtenidos por vida restante
         */
        int getLifePoints() const;
        
        /**
         * @return tiempo invertido en la partida
         */
        int getTime() const;
        
        /**
         * @return puntos de experiencia obtenidos por la partida
         */
        int getTimePoints();
        
        /**
         * @return puntos totales de experiencia
         */
        int getTotal() const;
        
        /**
         * Contabiliza la muerte de un enemigo
         * 
         * @param type tipo de enemigo, cada uno proporciona un número
         * distinto de puntos de experiencia.
         */
        void killEnemy(Enemy::Type type);
        
        /**
         * Contabiliza el uso de maná.
         * 
         * @param mana maná utilizado
         */
        void useMana(int mana);
        
        /**
         * Establece una nueva vida restante
         * 
         * @param life vida restante
         */
        void setLife(int life);
        
        /**
         * Establece un nuevo tiempo empleado
         * 
         * @param time tiempo empleado
         */
        void setTime(Ogre::Real time);
    
    private:
        Ogre::String _levelCode;
        int _enemiesKilled;
        boost::unordered_map<Enemy::Type, int> _pointsByEnemy;
        int _enemiesPoints;
        int _manaUsed;
        int _manaMax;
        int _manaPoints;
        int _life;
        int _lifeMax;
        int _lifePoints;
        Ogre::Real _time;
        Ogre::Real _timeMax;
        int _timePoints;
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_GAMESTATS_H_
