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

#include <OGRE/Ogre.h>
#include "pugixml.hpp"

#include "levelManager.h"
#include "level.h"
#include "body.h"
#include "shape.h"

using std::endl;
using std::cout;
using std::cerr;

template<> LevelManager* Ogre::Singleton<LevelManager>::ms_Singleton = 0;

LevelManager::LevelManager(): _numLevels(0) {
    cout << "LevelManager::LevelManager()" << endl;

    // Buscamos los niveles
    lookForLevels();

    // Inicializamos el catálogo de objetos
    initialiseBodyCatalog();
}

void LevelManager::lookForLevels() {
    // Lee el orden de los niveles desde media/levels.xml
    pugi::xml_document doc;

    if (!doc.load_file("media/levels/levels.xml")) {
        cerr << "LevelManager::lookForLevels(): error al cargar el fichero media/levels.xml" << endl;
        exit(1);
    }
    
    pugi::xml_node levelsNode = doc.child("levels");
    pugi::xml_node levelNode;
    
    for (levelNode = levelsNode.child("level"); levelNode; levelNode = levelNode.next_sibling()) {
        Ogre::String id = levelNode.attribute("id").value();
        Level* level = new Level(id);
        _levels.push_back(level);
        ++_numLevels;
    }
}

void LevelManager::initialiseBodyCatalog() {
    // Procesamos el fichero basepath + objectCatalog.xml
    pugi::xml_document doc;
    if (!doc.load_file("media/levels/bodyCatalog.xml")) {
        cerr << "LevelManager::initialiseBodyCatalog(): error al leer el fichero  media/levels/bodyCatalog.xml" << endl;
        exit(1);
    }

    // Recorremos los nodos object
    pugi::xml_node bodiesNode = doc.child("bodies");
    pugi::xml_node bodyNode;

    for (bodyNode = bodiesNode.child("body"); bodyNode; bodyNode = bodyNode.next_sibling("body")) {
        Body* body = new Body();

        // Tomamos el nombre y el tipo
        Ogre::String name = bodyNode.attribute("name").value();
        int type = bodyNode.attribute("type").as_int();
        body->setType(type);

       //Recorremos las shapes
        pugi::xml_node shapeNode;
        for (shapeNode = bodyNode.child("shape"); shapeNode; shapeNode = shapeNode.next_sibling("shape")) {
            Shape* shape;

            // Tomamos la forma concreta
            Ogre::String shapeType = shapeNode.attribute("type").value();

            // Si es una esfera
            if (shapeType == "sphere") {
                // Tomamos el centro
                Ogre::Vector3 center;
                pugi::xml_node centerNode = shapeNode.child("center");
                center.x = centerNode.attribute("x").as_float();
                center.y = centerNode.attribute("y").as_float();
                center.z = centerNode.attribute("z").as_float();

                // Tomamos el radio
                Ogre::Real radius = shapeNode.child("radius").attribute("value").as_float();

                // Creamos la esfera
                shape = new Sphere("", center, radius);
            }

            // Si es un plano
            else if (shapeType == "plane") {
                // Tomamos la posición
                Ogre::Vector3 position;
                pugi::xml_node positionNode = shapeNode.child("position");
                position.x = positionNode.attribute("x").as_float();
                position.y = positionNode.attribute("y").as_float();
                position.z = positionNode.attribute("z").as_float();

                // Tomamos la normal
                Ogre::Vector3 normal;
                pugi::xml_node normalNode = shapeNode.child("normal");
                normal.x = normalNode.attribute("x").as_float();
                normal.y = normalNode.attribute("y").as_float();
                normal.z = normalNode.attribute("z").as_float();

                // Creamos el plano
                shape = new Plane("", position, normal);
            }

            // Si es un AABB
            else if (shapeType == "aabb") {
                // Tomamos la minPos
                Ogre::Vector3 minPos;
                pugi::xml_node minNode = shapeNode.child("minPos");
                minPos.x = minNode.attribute("x").as_float();
                minPos.y = minNode.attribute("y").as_float();
                minPos.z = minNode.attribute("z").as_float();

                // Tomamos la maxPos
                Ogre::Vector3 maxPos;
                pugi::xml_node maxNode = shapeNode.child("maxPos");
                maxPos.x = maxNode.attribute("x").as_float();
                maxPos.y = maxNode.attribute("y").as_float();
                maxPos.z = maxNode.attribute("z").as_float();

                // Creamos el AABB
                shape = new AxisAlignedBox("", minPos, maxPos);
            }

            // Si es un OBB
            else if (shapeType == "obb") {
                // Tomamos el centro
                Ogre::Vector3 center;
                pugi::xml_node centerNode = shapeNode.child("center");
                center.x = centerNode.attribute("x").as_float();
                center.y = centerNode.attribute("y").as_float();
                center.z = centerNode.attribute("z").as_float();

                // Tomamos la extension
                Ogre::Vector3 extent;
                pugi::xml_node extentNode = shapeNode.child("extent");
                extent.x = extentNode.attribute("x").as_float();
                extent.y = extentNode.attribute("y").as_float();
                extent.z = extentNode.attribute("z").as_float();

                // Tomamos los ejes
                Ogre::Matrix3 axes;
                pugi::xml_node axesNode = shapeNode.child("axes");
                axes[0][0] = axesNode.attribute("a00").as_float();
                axes[0][1] = axesNode.attribute("a01").as_float();
                axes[0][2] = axesNode.attribute("a02").as_float();
                axes[1][0] = axesNode.attribute("a10").as_float();
                axes[1][1] = axesNode.attribute("a11").as_float();
                axes[1][2] = axesNode.attribute("a12").as_float();
                axes[2][0] = axesNode.attribute("a20").as_float();
                axes[2][1] = axesNode.attribute("a21").as_float();
                axes[2][2] = axesNode.attribute("a22").as_float();

                // Creamos el AABB
                shape = new OrientedBox("", center, extent, axes);
            }

            // Si no, damos un error
            else {
                cerr << "LevelManager::initializeBodyCatalog(): forma desconocida " << shapeType << endl;
                exit(1);
            }

            // Añadimos la forma
            body->addShape(shape);
            _shapes.push_back(shape);
        }

        // Intorducimos el body en el BodyCatalog
        _bodyCatalog[name] = body;
    }
}

LevelManager::~LevelManager() {
    cout << "LevelManager::~LevelManager()" << endl;

    // Destruir levels
    std::vector<Level*>::iterator i;
    for (i = _levels.begin(); i != _levels.end(); ++i)
        delete (*i);

    // Destruir body catalog
    BodyCatalog::iterator j;
    for (j = _bodyCatalog.begin(); j != _bodyCatalog.end(); ++j)
        delete j->second;

    // Destruir shapes del body catalog
    std::vector<Shape*>::iterator k;
    for (k = _shapes.begin(); k != _shapes.end(); ++k)
        delete (*k);
}

LevelManager& LevelManager::getSingleton() {
    assert(ms_Singleton);
    return (*ms_Singleton);
}

LevelManager* LevelManager::getSingletonPtr() {
    assert(ms_Singleton);
    return ms_Singleton;
}

std::vector<Level*>& LevelManager::getLevels() {
    return _levels;
}

Level* LevelManager::getLevel(const Ogre::String& id) {
    // Recorremos los niveles buscando el que posee el nombre dado
    std::vector<Level*>::iterator it;
    for (it = _levels.begin(); it != _levels.end(); ++it)
        if ((*it)->getId() == id)
            return (*it);

    return 0;
}

int LevelManager::getNumLevels() const {
    return _numLevels;
}


Body* LevelManager::createBodyFromCatalog(const Ogre::String& name) {
    // Buscamos el body en el catálogo
    BodyCatalog::iterator it = _bodyCatalog.find(name);

    // Si no lo encontramos devolvemos 0
    if (it == _bodyCatalog.end())
        return 0;

    // Si lo encontramos, obtenemos una copia
    Body* body = new Body();
    body->setType(it->second->getType());

    // Copiamos las shapes
    std::vector<Shape*> bodyShapes = it->second->getShapes();
    std::vector<Shape*>::iterator j;

    for (j = bodyShapes.begin(); j != bodyShapes.end(); ++j)
        body->addShape((*j)->getTransformedCopy());

    return body;
}

LevelManager::UnlockCode LevelManager::tryUnlockLevel(const Ogre::String& code,
                                                      int lastUnlockedLevel) {
    int order = 1;
    
    for (std::vector<Level*>::iterator i = _levels.begin(); i != _levels.end(); ++i) {
        // Si es el nivel jugado y es el último nivel
        if ((*i)->getId() == code && order == _numLevels)
            return LASTLEVEL;
        
        // Si es el nivel jugado y era el último nivel desbloqueado
        else if ((*i)->getId() == code && lastUnlockedLevel == order) 
            return UNLOCKED;
        
        // Si es el nivel jugado pero no era el último desbloqueado
        else if ((*i)->getId() == code && lastUnlockedLevel > order)
            return ALREADYUNLOCKED;
        
        ++order;
    }
    
    return UNLOCKED;
}
