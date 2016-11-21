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



/**
 *  @author David Saltares M&aacute;rquez
 *  @date 12-02-2011
 */

#include <iostream>
#include <algorithm>

#include "pugixml.hpp"

#include "level.h"
#include "levelManager.h"
#include "game.h"
#include "gameMesh.h"
#include "player.h"
#include "songManager.h"
#include "stateManager.h"
#include "collisionManager.h"

using std::cout;
using std::endl;
using std::cerr;

Level::Level(const Ogre::String& id): _id(id), _name(""), _description(""), _loaded(false), _navigationMesh(0) {
    cout << "Level::Level()" << endl;
    loadBasicInfo();
}

Level::~Level() {
    cout << "Level::~Level()" << endl;
    unload();
}

void Level::loadBasicInfo() {
    // Reconstruimos el nombre del fichero
    Ogre::String fileName = "media/levels/" +  _id + "_info.xml";

    // Cargamos el fichero xml
    pugi::xml_document doc;

    if (!doc.load_file(fileName.c_str())) {
        cerr << "Level::loadBasicInfo(): error al cargar el fichero xml " << fileName << endl;
        exit(1);
    }

    // Tomamos el nodo raíz
    pugi::xml_node basicInfo = doc.child("basicInfo");

    if (!basicInfo) {
        cerr << "Level::loadBasicInfo(): error al procesar el nodo basicInfo" << endl;
        exit(1);
    }

    // Nombre
    pugi::xml_node node = basicInfo.child("name");
    _name = node.child_value();

    // Descripcion
    node = basicInfo.child("description");
    _description = node.child_value();

    // Song
    node = basicInfo.child("song");
    _musicName = node.attribute("name").value();
    _musicGroup = node.attribute("group").value();
}

void Level::load() {
    if (!_loaded) {
        _loaded = true;
        
        // Reconstruimos el nombre del fichero
        Ogre::String fileName = "media/levels/" +  _id + "_scene.xml";

        // Cargamos el fichero xml
        pugi::xml_document doc;

        if (!doc.load_file(fileName.c_str())) {
            cerr << "Level::load(): error al cargar el fichero xml " << fileName << endl;
            exit(1);
        }

        // Cargamos información del ambiente
        loadAmbientInfo(doc);

        // Cargamos información de nodos
        loadNodesInfo(doc);
        
        // Ordenamos el vector de enemy spawns
        std::sort(_enemySpawns.begin(), _enemySpawns.end());

        // Cargamos la canción
        _song = SongManager::getSingleton().load(_musicName);
    }
}
        
void Level::loadAmbientInfo(const pugi::xml_document& doc) {
    // Cargamos el documento
    pugi::xml_node rootNode;
    
    if (!(rootNode = doc.child("scene"))) {
        cerr << "Level::loadNodesInfo(): error al leer el fichero xml en elemento scene" << endl;
        exit(1);
    }
    
    // Buscamos el nodo ambient
    pugi::xml_node enviromentNode;
    
    if (!(enviromentNode = rootNode.child("environment"))) {
        cerr << "Level::loadNodesInfo(): error al leer el fichero xml en elemento enviroment" << endl;
        exit(1);
    }
    
    // Tomamos el SceneManager
    Ogre::SceneManager* sceneManager = Game::getSceneManager();
    
    // Leemos ambientColour
    Ogre::ColourValue ambientColour;
    pugi::xml_node colourNode = enviromentNode.child("colourAmbient");
    ambientColour.r = colourNode.attribute("r").as_float();
    ambientColour.g = colourNode.attribute("g").as_float();
    ambientColour.b = colourNode.attribute("b").as_float();
    
    // Establecemos el ambient colour
    sceneManager->setAmbientLight(ambientColour);

    // Leemos backgroundColour
    Ogre::ColourValue backgroundColour;
    pugi::xml_node backgroundNode = enviromentNode.child("colourBackground");
    backgroundColour.r = backgroundNode.attribute("r").as_float();
    backgroundColour.g = backgroundNode.attribute("g").as_float();
    backgroundColour.b = backgroundNode.attribute("b").as_float();

    // Establecemos el background colour
    Game::getViewPort()->setBackgroundColour(backgroundColour);
}

void Level::loadNodesInfo(const pugi::xml_document& doc) {
    // Cargamos el documento
    pugi::xml_node rootNode;
    
    if (!(rootNode = doc.child("scene"))) {
        cerr << "Level::loadNodesInfo(): error al leer el fichero xml en elemento scene" << endl;
        exit(1);
    }

    pugi::xml_node nodes;
    
    if (!(nodes = rootNode.child("nodes"))) {
        cerr << "Level::loadNodesInfo(): error al leer el fichero xml en elemento nodes" << endl;
        exit(1);
    }

    // Recorremos los nodos
    for (pugi::xml_node node = nodes.first_child(); node; node = node.next_sibling()) {
        // Leemos nombre
        Ogre::String name = node.attribute("name").value();

        // Leemos posicion
        Ogre::Vector3 position;
        pugi::xml_node positionNode = node.child("position");
        position.x = positionNode.attribute("x").as_float();
        position.y = positionNode.attribute("y").as_float();
        position.z = positionNode.attribute("z").as_float();

        // Leemos escala
        Ogre::Vector3 scale;
        pugi::xml_node scaleNode = node.child("scale");
        scale.x = scaleNode.attribute("x").as_float();
        scale.y = scaleNode.attribute("y").as_float();
        scale.z = scaleNode.attribute("z").as_float();

        // Leemos rotación
        Ogre::Quaternion orientation;
        pugi::xml_node orientationNode = node.child("quaternion");
        orientation.x = orientationNode.attribute("x").as_float();
        orientation.y = orientationNode.attribute("y").as_float();
        orientation.z = orientationNode.attribute("z").as_float();
        orientation.w = orientationNode.attribute("w").as_float();

        // Leemos el tipo de nodo
        pugi::xml_node nodeType;
        
        // Podemos cargar una entidad o una fuente de luz
        if (!(nodeType = node.child("entity")).empty())
            loadEntity(nodeType, name, position, scale, orientation);
        else if (!(nodeType = node.child("light")).empty())
            loadLight(nodeType, name, position, scale, orientation);
        else if (!(nodeType = node.child("camera")).empty())
            loadCamera(nodeType, position, orientation);
    }
}

void Level::loadEntity(const pugi::xml_node& node,
                       const Ogre::String& name,
                       const Ogre::Vector3& position,
                       const Ogre::Vector3& scale,
                       const Ogre::Quaternion& orientation) {
                           
    // Leemos entidad
    Ogre::String entityName = node.attribute("name").value();
    Ogre::String entityMesh = node.attribute("meshFile").value();
    
    Ogre::SceneManager* sceneManager = Game::getSceneManager();

    // Dividimos el nombre para saber qué tipo de entidad es
    Ogre::StringVector nameParts = Ogre::StringUtil::split(entityName, "-.");
    
    // Objeto de la forma scene.tipo.número
    if (nameParts.size() >= 2 && nameParts[0] == "scene") {
        // Creamos nodo
        Ogre::SceneNode* sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(name,
                                                                                            position,
                                                                                            orientation);
                                                                                        
        sceneNode->setScale(scale);
        
        // Creamos entidad
        Ogre::Entity* entity = sceneManager->createEntity(entityName, entityMesh);
        sceneNode->attachObject(entity);
        
        // Creamos GameObject
        GameMesh* gameMesh = new GameMesh(sceneManager, sceneNode, entity);

        // Obtenemos el body
        // Buscamos en el catálogo su tipo y shapes
        Body *body = LevelManager::getSingleton().createBodyFromCatalog(nameParts[1]);
       
        // Le asignamos el gameObject al body
        body->setGameObject(gameMesh);

        // Le asignamos el body al GameObject
        gameMesh->setBody(body);

        // Lo registramos en CollisionManager
        CollisionManager::getSingleton().addBody(body);

        // En principio, lo metemos en escenario
        _sceneObjects.push_back(gameMesh);
    }
    // Objeto con la forma "enemy.enemyType.time"
    else if ((nameParts.size() == 3 || nameParts.size() == 4) && nameParts[0] == "enemy") {
        
        // Tomamos tipo de enemigo
        Enemy::Type type = Enemy::GOBLIN;
        
        if (nameParts[1] == "goblin")
            type = Enemy::GOBLIN;
        else if (nameParts[1] == "demon")
            type = Enemy::DEMON;
        else if (nameParts[1] == "golem")
            type = Enemy::GOLEM;
        else if (nameParts[1] == "spider")
            type = Enemy::SPIDER;
        else if (nameParts[1] == "player")
            type = Enemy::PLAYER;
        
        // Tomamos el tiempo
        Ogre::Real time;
        sscanf(nameParts[2].c_str(), "%f", &time);
        
        // Creamos un EnemySpawn y lo añadimos
        EnemySpawn enemySpawn(type, position, orientation, time);
        
        _enemySpawns.push_back(enemySpawn);
    }
    // Objeto de nombre player
    else if (nameParts.size() == 1 && nameParts[0] == "player") {
        // Tomamos la posición del jugador
        _playerPos = position;
    }
    // Objeto de nombre "navMesh"
    else if (nameParts.size() == 1 && nameParts[0] == "navMesh") {
        // Creamos el navigation mesh
        _navigationMesh = new NavigationMesh("media/" + entityMesh + ".xml");
    }
    // Objeto con forma particle.nombreParticulas.id
    else if (nameParts.size() == 3 && nameParts[0] == "particle") {
        Ogre::ParticleSystem* particle = sceneManager->createParticleSystem(entityName, nameParts[1]);
        Ogre::SceneNode* sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
        sceneNode->attachObject(particle);
        sceneNode->setPosition(position);
        sceneNode->setOrientation(orientation);
        sceneNode->setScale(scale);
        
        _particles.push_back(std::make_pair(sceneNode, particle));
    }
    // Cargamos geometría genérica
    else {
        Ogre::Entity* entity = sceneManager->createEntity(entityName, entityMesh);
        Ogre::SceneNode* sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(name);
        sceneNode->attachObject(entity);
        sceneNode->setPosition(position);
        sceneNode->setOrientation(orientation);
        sceneNode->setScale(scale);
        
        // Insertamos en el vector
        _geometry.push_back(std::make_pair(sceneNode, entity));
    }
}

void Level::loadLight(const pugi::xml_node& node,
                      const Ogre::String& name,
                      const Ogre::Vector3& position,
                      const Ogre::Vector3& scale,
                      const Ogre::Quaternion& orientation) {
                          
    // Creamos nodo
    Ogre::SceneManager* sceneManager = Game::getSceneManager();
    Ogre::SceneNode* sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(name,
                                                                                        position,
                                                                                        orientation);
                                                                                        
    sceneNode->setScale(scale);
                          
    // Leemos nombre y tipo
    Ogre::String lightName = node.attribute("name").value();
    Ogre::String lightType = node.attribute("type").value();

    // Leemos color
    Ogre::ColourValue diffuse;
    pugi::xml_node diffuseNode = node.child("colourDiffuse");
    diffuse.r = diffuseNode.attribute("r").as_float();
    diffuse.g = diffuseNode.attribute("g").as_float();
    diffuse.b = diffuseNode.attribute("b").as_float();
    
    Ogre::ColourValue specular;
    pugi::xml_node specularNode = node.child("colourSpecular");
    specular.r = specularNode.attribute("r").as_float();
    specular.g = specularNode.attribute("g").as_float();
    specular.b = specularNode.attribute("b").as_float();

    // Atenuación si la hay
    Ogre::Real atRange = 0.0;
    Ogre::Real atConstant = 0.0;
    Ogre::Real atLinear = 0.0;
    Ogre::Real atQuadratic = 0.0;
    pugi::xml_node attenuation = node.child("lightAttenuation");
    if (attenuation) {
        atRange = attenuation.attribute("range").as_float();
        atConstant = attenuation.attribute("constant").as_float();
        atLinear = attenuation.attribute("linear").as_float();
        atQuadratic = attenuation.attribute("quadratic").as_float();
    }

    // Range si lo hay
    pugi::xml_node range = node.child("lightRange");
    Ogre::Real ranInner = 0.0;
    Ogre::Real ranOuter = 0.0;
    Ogre::Real ranFalloff= 0.0;
    if (range) {
        ranInner = range.attribute("inner").as_float();
        ranOuter = range.attribute("outer").as_float();
        ranFalloff = range.attribute("falloff").as_float();
    }

    // Creamos la luz y la metemos en el nodo
    Ogre::Light* light = sceneManager->createLight(lightName);
    light->setDiffuseColour(diffuse);
    light->setSpecularColour(specular);
    sceneNode->attachObject(light);

    // Según el tipo
    if (lightType == "point") {
        light->setType(Ogre::Light::LT_POINT);
        light->setAttenuation(atRange, atConstant, atLinear, atQuadratic);
    } else if (lightType == "directional") {
        light->setType(Ogre::Light::LT_DIRECTIONAL);
    } else if (lightType == "spotLight") {
        light->setType(Ogre::Light::LT_SPOTLIGHT);
        light->setAttenuation(atRange, atConstant, atLinear, atQuadratic);
        light->setSpotlightRange(Ogre::Radian(ranInner), Ogre::Radian(ranOuter), ranFalloff);
    } else {
        cerr << "Level::loadLight(): tipo desconocido de luz" << endl;
    }

    // Introducimos la luz en el vector
    _lights.push_back(std::make_pair(sceneNode, light));
}

void Level::loadCamera(const pugi::xml_node& node, const Ogre::Vector3& position, const Ogre::Quaternion& orientation) {
    Ogre::Camera* camera = Game::getCamera();
    
    camera->setPosition(position);
    camera->setOrientation(orientation);
}

void Level::unload() {
    if (_loaded) {
        _loaded = false;

        Ogre::SceneManager* sceneManager = Game::getSceneManager();

        // Eliminamos los objetos del escenario
        std::vector<GameMesh*>::iterator i;
        for (i = _sceneObjects.begin(); i != _sceneObjects.end(); ++i) {
            delete (*i);
        }
        
        _sceneObjects.clear();

        // Eliminamos las luces
        std ::vector<std::pair<Ogre::SceneNode*, Ogre::Light*> >::iterator j;
        for (j = _lights.begin(); j != _lights.end(); ++j) {
            j->first->detachObject(j->second);
            sceneManager->destroyLight(j->second);
            j->first->getParentSceneNode()->removeChild(j->first);
            sceneManager->destroySceneNode(j->first);
        }

        _lights.clear();
        
        // Eliminamos la geometría
        std ::vector<std::pair<Ogre::SceneNode*, Ogre::Entity*> >::iterator k;
        for (k = _geometry.begin(); k != _geometry.end(); ++k) {
            k->first->detachObject(k->second);
            sceneManager->destroyEntity(k->second);
            k->first->getParentSceneNode()->removeChild(k->first);
            sceneManager->destroySceneNode(k->first);
        }
        
        _geometry.clear();
        
        // Eliminamos los efectos de partículas
        std::vector<std::pair<Ogre::SceneNode*, Ogre::ParticleSystem*> >::iterator l;
        for (l = _particles.begin(); l != _particles.end(); ++l) {
            l->first->detachObject(l->second);
            sceneManager->destroyParticleSystem(l->second);
            l->first->getParentSceneNode()->removeChild(l->first);
            sceneManager->destroySceneNode(l->first);
        }
        
        _particles.clear();
        
        // Limpiamos el vector de enemy spawns
        _enemySpawns.clear();
        
        // Maya de navegación
        if (_navigationMesh)
            delete _navigationMesh;
    }
}

bool Level::isLoaded() const {
    return _loaded;
}

const Ogre::String& Level::getId() const {
    return _id;
}

const Ogre::String& Level::getName() const {
    return _name;
}

const Ogre::String& Level::getDescription() const {
    return _description;
}
        
const Ogre::String& Level::getSongName() const {
    return _musicName;
}

SongPtr Level::getSong() {
    return _song;
}

std::vector<GameMesh*>& Level::getSceneObjects() {
    return _sceneObjects;
}

const Ogre::Vector3& Level::getPlayerPosition() const {
    return _playerPos;
}

NavigationMesh* Level::getNavigationMesh() {
    return _navigationMesh;
}


std::vector<EnemySpawn>& Level::getEnemySpawns() {
    return _enemySpawns;
}
