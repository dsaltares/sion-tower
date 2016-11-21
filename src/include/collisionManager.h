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
 
#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_COLLISIONMANAGER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_COLLISIONMANAGER_H_

#include <OGRE/Ogre.h>
#include <list>
#include <set>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

#include "body.h"
#include "shape.h"

//! Gestor que registra los bodies, detecta colisions y proporciona un sistema de callbacks

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 02-02-2011
 *
 *  CollisionManager lleva el registro de los elementos colisionables (Body)
 *  que componen la escena. Realiza tests de colisi&oacute;n entre dichos bodies y
 *  llama al callback oportuno si es que lo hubiese. Sigue el patr&oacute;n de dise&ntilde;o
 *  Singleton, es decir, una &uacute;nica instancia accesible desde todo el sistema.
 *  No es un sistema de f&iacute;sicas, s&oacute;lo detecta colisiones e informa de ellas.
 *
 *  Para que un cuerpo sea considerado como colisionable debe de ser registrado
 *  en el CollisionManager. Los cuerpos tienen un tipo determinado (entero)
 *  &uacute;til para clasificarlos dentro de la gesti&oacute;n de colisiones. Debemos
 *  indicarle expl&iacute;citamente al sistema que detecte e informe de las colisiones
 *  entre dos tipos de cuerpos. Esto lo conseguimos a trav&eacute;s del sistema de
 *  callbacks. Un callback es una funci&oacute;n (independiente o miembro de alguna
 *  clase) que tiene la siguiente forma:
 *
 *  <code>
 *  bool collisionCallbacK(Body* bodyA, Body* bodyB);
 *  </code>
 *
 *  Para registrar un callback utilizamos boost::bind y boost:funcion y le
 *  indicamos al CollisionManager qu&eacute; tipos de cuerpos estar&aacute;n asociados. En
 *  cada iteraci&oacute;n del bucle de juego debemos llamar al m&eacute;todo
 *  CollisionManager::checkCollisions para detectar las colisiones y llamar a
 *  los callbacks. A continuaci&oacute;n de muestra de un ejemplo de uso.
 *
 *  \code
 *  // Al comienzo de la aplicaci&oacute;n creamos el CollisionManager
 *  CollisionManager* collisionManager = new CollisionManager();
 *  ...
 *  
 *  // Tenemos bodyA, bodyB y bodyC, los registramos en el CollisionManager
 *  CollisionManager::getSingleton().addBody(bodyA);
 *  CollisionManager::getSingleton().addBody(bodyB);
 *  CollisionManager::getSingleton().addBody(bodyC);
 *
 *  // Registramos un callback para la colisi&oacute;n entre bodies de tipo 1 y 2
 *  boost::function<bool(Body*, Body*)> callback = boost::bind(&miFuncionCallback, _1, _2);
 *  CollisionManager::getSingleton().addCollisionCallback(callback, 1, 2);
 *
 *  ...
 *  
 *  // En el bucle de juego
 *  while(!exit) {
 *      // Actualizar elementos
 *
 *      // Comprobar colisiones
 *      CollisionManager::getSingleton().checkCollisions();
 *
 *      // Renderizar
 *  }
 *  
 *  ...
 *
 *  // Al cerrar la aplicaci&oacute;n, destruimos el CollisionManager
 *  delete collisionManager;
 *  \endcode
 *
 */
class CollisionManager: public Ogre::Singleton<CollisionManager> {
    public:
        /**
         *  Objecto funci&oacute;n que modela los callback de colisiones: recibe dos
         *  cuerpos y devuelve un boolean
         */
        typedef boost::function<void(Body*, Body*)> CollisionCallback;

        /**
         *  Tipo de callback. Según el tipo de callback se llamará en un
         *  momento u otro:
         *
         *  BEGINCOLLISION: iteración del game loop en el que dos cuerpos
         *  entran en contacto.
         *  COLLIDING: iteraciones posteriores en las que los cuerpos siguen en contacto.
         *  ENDCOLLISION: iteración en el que cuerpos que estaban colisionando
         *  dejan de hacerlo.
         */
        enum CallbackType {BEGINCOLLISION, COLLIDING, ENDCOLLISION, ALL};
       
        /**
         *  Constructor
         *
         *  Configura el reparto de colisiones de la clase Shape (ya no es
         *  necesario llamar a Shape::configureCollisionDispatching de forma
         *  manual).
         */
        CollisionManager();

        /**
         *  Destructor
         *
         *  No libera la memoria ocupada por los cuerpos registrados, debes
         *  destruirlos de forma manual. Si el cuerpo est&aacute; bajo el control
         *  de la clase GameObject o derivados no ser&aacute; necesario, ella lo
         *  har&aacute; de forma autom&aacute;tica.
         */
        ~CollisionManager();

        /**
         *  @return &uacute;nica instancia de la clase CollisionManager
         */
        static CollisionManager& getSingleton();

        /**
         *  @return puntero a la &uacute;nica instancia de la clase CollisionManager
         */
        static CollisionManager* getSingletonPtr();

        /**
         *  @param body cuerpo a a&ntilde;adir
         *
         *  Registra un cuerpo en el gestor de colisiones. A partir de ahora
         *  ser&aacute; tenido en cuenta en los tests de colisi&oacute;n siempre y cuando
         *  haya un callback definido.
         */
        void addBody(Body* body);

        /**
         *  @param body cuerpo a borrar del gestor de colisiones.
         *  @return true si se ha borrado, false si no estaba registrado.
         *
         *  Elimina el registro del body en el gestor de colisiones. No elimina
         *  el cuerpo, si no estaba bajo la gesti&oacute;n de un GameObject debes
         *  hacerlo de forma manual.
         */
        bool removeBody(Body* body);

        /**
         *  Elimina todos los cuerpos bajo el conrol del gestor de colisiones.
         *  No los destruye, si no estaban bajo el control de objetos
         *  GameObject debes hacerlo de forma manual.
         */
        void removeAllBodies();

        /**
         *  @param typeA tipo del primer objeto
         *  @param typeB tipo del segundo objeto
         *  @param callback objeto funci&oacute;n que recibe dos punteros a cuerpos y
         *  devuelve un booleano.
         *  @param callbackType tipo de callback (begin, in o end)
         *  
         *  Registra un callback que será llamado cuando dos cuerpos de los
         *  tipos dados colisionen. Dependiendo del tipo de callback la llamada
         *  se producirá en un momento u otro: al comienzo, durante o al final
         *  de la colisión.
         */
        void addCollisionCallback(int typeA,
                                  int typeB,
                                  CollisionCallback callback,
                                  CallbackType callbackType = BEGINCOLLISION);

        /**
         *  @param typeA tipo del primer objeto
         *  @param typeB tipo del segundo objeto
         *  @param callbackType tipo de callback que se va a eliminar
         *  @return true si se ha eliminado el callback, false si no exist&iacute;a
         *
         *  Elimina el tipo de callback entre objetos de tipos typeA-typeB (y
         *  viceversa). Ya no se detectar&aacute;n colisiones entre dichos tipos.
         */
        bool removeCollisionCallback(int typeA, int typeB, CallbackType callbackType = ALL);

        /**
         *  @param type tipo de callback a eliminar.
         *
         *  Elimina todos los callbacks registrados en el gestor de colisiones
         *  del tipo indicado. Si se pasa ALL, se eliminan todos los callbacks.
         */
        void removeAllCollisionCallbacks(CallbackType type = ALL);

        /**
         *  Efect&uacute;a todos los tests de colisi&oacute;n oportunos entre los cuerpos
         *  registrados y llama a los callbacks pertinentes.
         */
        void checkCollisions();
    private:
        typedef boost::unordered_map<int, boost::unordered_map<int, CollisionCallback> > CollisionCallbackTable;
        typedef boost::unordered_map<Body*, std::set<Body*> > CollidingBodies;

        CollisionCallbackTable _beginCallbackTable;
        CollisionCallbackTable _inCallbackTable;
        CollisionCallbackTable _endCallbackTable;

        CollidingBodies _collidingBodies;
        std::list<Body*> _bodies;

        Sphere* _sphereA;
        Sphere* _sphereB;

        bool existsCallback(int typeA, int typeB, CallbackType calbackType, CollisionCallback* collisionCallback);
        bool wereColliding(Body* bodyA, Body* bodyB);
};

#endif   // SIONTOWER_TRUNK_SRC_INCLUDE_COLLISIONMANAGER_H_
