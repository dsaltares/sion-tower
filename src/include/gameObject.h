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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_GAMEOBJECT_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_GAMEOBJECT_H_

#include <vector>

#include <OGRE/Ogre.h>

#include "shape.h"

class Body;

//! Clase que abstrae la parte visual (Entity/SceneNode) de la colisionable (Body) de un elemento de juego

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 3-02-2011
 *
 *  GameObject se encarga de encapsular la parte visual
 *  (Ogre::SceneNode y Ogre::Entity) con el componente colisionable (Body) de
 *  los elementos del juego. Proporciona un m&eacute;todo virtual GameObject::update
 *  para actualizar los elementos en cada iteraci&oacute;n del bucle de juego pero es
 *  importante asegurarse de mantener sincronizadas la parte colisionable con
 *  la visual.
 *
 *  A la hora de modificar posici&oacute;n, escala u orientaci&oacute;n del elemento de juego
 *  podemos hacerlo utilizando los m&eacute;todos dispuestos para ello. No obstante,
 *  es posible recuperar el puntero a Body o SceneNode para trabajar con ellos.
 *  En tal caso, es imprescindible llamar a GameObject::synchronizeSceneNode o
 *  GameObject::synchronizeBody respectivamente. As&iacute; se sincronizar&aacute; la parte
 *  visual con la colisionable o viceversa (seg&uacute;n lo que se haya modificado).
 */
class GameObject {
    public:
        
        /**
         * Tipos de objetos de juego, utilizado para hacer conversiones
         * seguras junto al método GameObject::getType.
         */
        enum Type {
            SIMPLEMESH = 0,
            FLOOR = 1,
            SCENE = 2,
            PLAYER = 3,
            ENEMY = 4,
            SPELL = 5
        };
        
        /**
         *  Constructor
         *
         *  @param sceneManager gestor de escena que controla la parte visual
         *  del elemento.
         */
        GameObject(Ogre::SceneManager* sceneManager);
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de escena que controla la parte visual del elemento
         * @param sceneNode nodo de escena
         * @param entity entidad cargada, parte visual  
         */
        GameObject(Ogre::SceneManager* sceneManager,
                   Ogre::SceneNode* sceneNode);

        /**
         *  Destructor
         *
         *  Si est&aacute;n inicializados desvincula la entidad del nodo, retira el
         *  nodo de la escena y destruye a ambos. Si el body es v&aacute;lido (no
         *  apunta a NULL), lo destruye y elimina sus shapes.
         */
        virtual ~GameObject();
        
        /**
         * @return tipo del objeto de juego
         * 
         * Es virtual puro y debe ser sobrecargado por las clases hijas
         * que no deseen ser abstractas. Permite hacer conversiones seguras
         * con static_cast.
         */
        virtual Type getType() const = 0;
        
        /**
         *  @return cuerpo colisionable que compone el objeto
         */
        Body* getBody();

        /**
         *  @param body nuevo cuerpo colisionable que compondr&aacute; al objeto.
         *
         *  Si el cuerpo anterior era v&aacute;lido es destruido junto a la shapes que
         *  lo compon&iacute;an. Es importantes ser conscientes de que cuando se
         *  destruya el GameObject, el nuevo cuerpo y sus shapes ser&aacute;n
         *  destruidas.
         */
        void setBody(Body* body);

        /**
         *  @return nodo perteneciente al grafo de escena que compone el
         *  objeto.
         */
        Ogre::SceneNode* getSceneNode();

        /**
         *  @return posici&oacute;n del elemento de juego
         */
        const Ogre::Vector3& getPosition();

        /**
         *  @param position nueva posici&oacute;n del elemento de juego
         */
        virtual void setPosition(const Ogre::Vector3& position);
        
        /**
         *  @return escala del elemento de juego
         */
        const Ogre::Vector3& getScale();

        /**
         *  @param scale nueva escala del elemento de juego
         */
        void setScale(const Ogre::Vector3& scale);

        /**
         *  @return orientaci&oacute;n del elemento de juego
         */
        const Ogre::Quaternion& getOrientation();

        /**
         *  @param orientation nueva orientaci&oacute;n del elemento de juego
         */
        virtual void setOrientation(const Ogre::Quaternion& orientation);

        /**
         *  @param deltaT incremento de tiempo en milisegundos desde la &uacute;ltima
         *  actualizaci&oacute;n.
         *
         *  Por degecto no hace nada, est&aacute; pensado para ser sobrecargado por
         *  clases especializadas.
         */
        virtual void update(Ogre::Real deltaT);

        /**
         *  Sincroniza el estado colisionable (Body) con el visual (SceneNode).
         *  Es imprescindible llamarlo tras haber actualizado el elemento de
         *  juego modificando sus par&aacute;metros visuales.
         */
        void synchronizeBody();

        /**
         *  Sincroniza el estado visual (SceneNode) con el colisionable (Body).
         *  Es imprescindible llamarlo tras haber actualizado el elemento de
         *  juego modificando par&aacute;metros del body.
         */
        void synchronizeSceneNode();
    protected:
        Ogre::SceneManager* _sceneManager;
        Ogre::SceneNode* _node;
        Body* _body;
        std::vector<Shape*> _shapes;
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_GAMEOBJECT_H_
