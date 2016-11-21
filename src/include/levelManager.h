/*
 * This file is part of SionTower.
 *
 * 
 * David Saltares M&aacute;rquez (C) 2011
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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_LEVELMANAGER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_LEVELMANAGER_H_

#include <vector>

#include <boost/unordered_map.hpp>
#include <OGRE/Ogre.h>


class Level;
class Body;
class Shape;

//! Clase Singleton que gestiona los niveles de Sion Tower

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 12-02-2011
 *
 *  LevelManager es la encargada de crear, gestionar y mantener todos los
 *  niveles de Sion Tower. Sigue el patrón de diseño Singleton (una instancia
 *  accesible desde todo el sistema).
 *
 *  Cuando es creado busca y carga la información básica de todos los Level que
 *  encuentra en la ruta correspondiente (media/levels/).
 */
class LevelManager: public Ogre::Singleton<LevelManager> {
    public:
        
        /** Códigos a devolver cuando tratamos de desbloquear un nivel */
        enum UnlockCode {
            ALREADYUNLOCKED,
            LASTLEVEL,
            UNLOCKED
        };
    
        /**
         *  Constructor
         *
         *  Carga la información básica de todos los niveles
         */
        LevelManager();

        /**
         *  Destructor
         */
        ~LevelManager();

        /**
         *  @return única instancia de LevelManager del sistema
         */
        static LevelManager& getSingleton();

        /**
         *  @return puntero a la única instancia de LevelManager en todo el
         *  sistema.
         */
        static LevelManager* getSingletonPtr();

        /**
         *  @return vector con los niveles del juego en orden
         */
        std::vector<Level*>& getLevels();

        /**
         *  @param id identificador del nivel a buscar
         *  @return nivel con el id dado, si no se encuentra se devuelve 0.
         */
        Level* getLevel(const Ogre::String& id);

        /**
         *  @return número de niveles del juego bajo el control de
         *  LevelManager.
         */
        int getNumLevels() const;
        
        /**
         * @param name nombre del tipo de objeto
         * @return crea y devuelve un cuerpo a partir del catálogo de objetos.
         * Permite conseguir cuerpos de puertas, columnas y otros objetos del
         * escenario definidos en el fichero bodyCatalog.xml
         */
        Body* createBodyFromCatalog(const Ogre::String& name);
        
        /**
         * Trata de desbloquear un nivel para el jugador
         * 
         * @param code código del nivel a desbloquear
         * @param lastUnlockedLevel último nivel desbloqueado por el jugador
         * 
         * @return un valor de UnlockCode indicando si se ha podido desbloquear
         * el nivel.
         */
        UnlockCode tryUnlockLevel(const Ogre::String& code, int lastUnlockedLevel);
    private:
        std::vector<Level*> _levels;
        int _numLevels;

        typedef boost::unordered_map<Ogre::String, Body* > BodyCatalog;
        BodyCatalog _bodyCatalog;
        std::vector<Shape*> _shapes;

        void lookForLevels();
        void initialiseBodyCatalog();
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_LEVELMANAGER_H_
