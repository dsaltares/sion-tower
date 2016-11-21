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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_LEVEL_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_LEVEL_H_

#include <vector>

#include <OGRE/Ogre.h>
#include "pugixml.hpp"

#include "song.h"
#include "navigationMesh.h"
#include "enemy.h"
#include "enemySpawn.h"

class GameMesh;
class Player;



//! Clase que carga y mantiene todos los elementos de un nivel


/**
 *  @author David Saltares M&aacute;rquez
 *  @date 12-02-2011
 *
 *  La clase Level es la encargada de cargar desde disco y mantener todos los
 *  elementos de un nivel de Sion Tower. Contiene información básica: nombre,
 *  descripción, música que debe sonar... También contiene los elementos del
 *  escenario, enemigos y personaje.
 *
 *  Tiene dos tipos de estados: información básica y completa. Cuando creamos
 *  un objeto de la clase Level sólo contiene la información básica, no ha
 *  cargado el nivel. Tenemos que indicarle explícitamente que cargue el nivel
 *  completo.
 */
class Level {
    public:
        /**
         *  Constructor
         *
         *  @param id identificador del nivel.
         *
         *  El identificador es la base para encontrar los ficheros del nivel.
         *  Cada nivel se compone de dos ficheros: id_info.xml e id_scene.xml.
         *  El primero contiene la información básica mientras que el segundo
         *  representa la escena completa en formato DotScene.
         */
        Level(const Ogre::String& id);

        /**
         *  Destructor
         */
        ~Level();

        /**
         *  Carga el escenario de forma completa. Si ya estaba cargado, llama a
         *  unload y después carga.
         */
        void load();

        /**
         *  Elimina el escenario, los enemigos, etc. Sólo se mantiene la
         *  información básica.
         */
        void unload();

        /**
         *  @return true si el escenario está completamente cargado, false en
         *  caso contrario.
         */
        bool isLoaded() const;

        /**
         *  @return identificador del nivel
         */
        const Ogre::String& getId() const;

        /**
         *  @return nombre del nivel
         */
        const Ogre::String& getName() const;

        /**
         *  @return descripción del nivel
         */
        const Ogre::String& getDescription() const;

        /**
         *  @return nombre de la canción que debe sonar mientras se juega al
         *  nivel.
         */
        const Ogre::String& getSongName() const;

        /**
         *  @return canción que debe sonar mientras se juega al nivel.
         */
        SongPtr getSong();

        /**
         *  @return objetos que componen el escenario.
         */
        std::vector<GameMesh*>& getSceneObjects();
        
        /**
         *  @return posición inicial del jugador.
         */
        const Ogre::Vector3& getPlayerPosition() const;
        
        /**
         * @return vector con la información para las apariciones de los enemigos
         */
        std::vector<EnemySpawn>& getEnemySpawns();
        
        /**
         *  @return maya que guía los movimientos de los personajes
         *  controlados por la IA.
         **/
        NavigationMesh* getNavigationMesh();
        
    private:
        // Información básica
        Ogre::String _id;
        Ogre::String _name;
        Ogre::String _description;
        Ogre::String _musicName;
        Ogre::String _musicGroup;
        bool _loaded;
        
        // Canción
        SongPtr _song;
        
        // Elemento del juego y escenario
        Player* _player;
        std::vector<GameMesh*> _sceneObjects;
        std::vector<std::pair <Ogre::SceneNode*, Ogre::Light*> > _lights;
        std::vector<std::pair <Ogre::SceneNode*, Ogre::Entity*> > _geometry;
        std::vector<std::pair <Ogre::SceneNode*, Ogre::ParticleSystem*> > _particles;
        NavigationMesh* _navigationMesh;
        Ogre::Vector3 _playerPos;
        std::vector<EnemySpawn> _enemySpawns;
        
        void loadBasicInfo();

        void loadAmbientInfo(const pugi::xml_document& doc);
        void loadNodesInfo(const pugi::xml_document& doc);
        void loadEntity(const pugi::xml_node& node, const Ogre::String& name, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& orientation);
        void loadLight(const pugi::xml_node& node, const Ogre::String& name, const Ogre::Vector3& position, const Ogre::Vector3& scale, const Ogre::Quaternion& orientation);
        void loadCamera(const pugi::xml_node& node, const Ogre::Vector3& position, const Ogre::Quaternion& orientation);
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_LEVEL_H_
