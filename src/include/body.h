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
 
#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_BODY_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_BODY_H_

#include <vector>

#include <OGRE/Ogre.h>

class Shape;
class GameObject;

//! Clase que modela un cuerpo colisionable, compuesto de varias Shape


/**
 *  @author David Saltares M&aacute;rquez
 *  @date 1-02-2011
 *
 *  Representa un cuerpo colisionable del juego. Est&aacute; compuesto de un vector de
 *  punteros a Shape y una transformaci&oacute;n (posici&oacute;n, escala y orientaci&oacute;n). Las
 *  formas que componen el cuerpo est&aacute;n en "parent space". Para obtener las
 *  formas que componen un cuerpos en "world space" es necesario aplicarles la
 *  transformaci&oacute;n del cuerpo. 
 *
 *  Proporciona un m&eacute;todo est&aacute;tico para hacer un test de colisi&oacute;n entre dos
 *  cuerpos independientemente de las formas que los compongan.
 *
 *  Ejemplo:
 *
 *  \code
 *  // Creamos las formas del cuerpo A
 *  Shape* sphereA = new Sphere("sphereA", Ogre::Vector3(0, 0, 0), 5.0);
 *  Shape* aabbA = new AxisAlignedBox("aabbA", Ogre::Vector3(5, 5, 5), Ogre::Vector3(6, 6, 6));
 *
 *  // Creamos las formas del cuerpo B
 *  Shape* sphereB = new Sphere("sphereB", Ogre::Vector3(5, 5, 5), 5.0);
 *  Shape* aabbB = new AxisAlignedBox("aabbB", Ogre::Vector3(10, 10, 10), Ogre::Vector3(11, 11, 11));
 *  
 *  // Creamos el cuerpo A
 *  Body bodyA;
 *  bodyA.addShape(sphereA);
 *  bodyA.addShape(aabbA);
 *
 *  // Creamos el cuerpo B 
 *  Body bodyB;
 *  bodyB.addShape(sphereB);
 *  bodyA.addShape(aabbB);
 *
 *  // Transformamos el cuerpo B
 *  bodyB.setPosition(Ogre::Vector3(4, 4, 4);
 *  bodyB.setScale(Ogre::Vector3(1, 4, 1));
 *
 *  // Hacemos un test de colisi&oacute;n
 *  if (Body::getCollision(bodyA, bodyB))
 *      cout << "Hay colisi&oacute;n entre bodyA y bodyB" << endl;
 *
 *  \endcode
 */
class Body {
    public:
        /**
         *  Constructor
         *
         *  @param gameObject objeto de juego al que pertenece el body
         *  @param shapes vector de punteros a formas que compondr&aacute;n el cuerpo
         *  @param position posici&oacute;n del cuerpo
         *  @param scale escala del cuerpo
         *  @param orientation orientaci&oacute;n del cuerpo
         */
        Body(GameObject* gameObject,
             const std::vector<Shape*>& shapes,
             const Ogre::Vector3& position = Ogre::Vector3::ZERO,
             const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
             const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
             int type = 0);
        
        /**
         *  Constructor
         *
         *  @param gameObject objeto de juego al que pertenece el body
         *  @param position posici&oacute;n del cuerpo
         *  @param scale escala del cuerpo
         *  @param orientation orientaci&oacute;n del cuerpo
         */
        Body(GameObject* gameObject = 0,
             const Ogre::Vector3& position = Ogre::Vector3::ZERO,
             const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
             const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
             int type = 0);

        /**
         *  Destructor.
         *
         *  No destruye las formas que lo componen, debe hacerse de forma
         *  manual.
         */
        ~Body();

        /**
         *  @return objeto de juego al que pertenece el body
         */
        GameObject* getGameObject();

        /**
         *  @param gameObject nuevo gameObject al que pertenece el body
         */
        void setGameObject(GameObject* gameObject);

        /**
         *  @return vector de punteros a formas que componen el cuerpo
         */
        const std::vector<Shape*> getShapes() const;
        
        /**
         *  @return vector de punteros a formas que componen el cuerpo en world
         *  space
         */
        const std::vector<Shape*> getWorldSpaceShapes() const;

        /**
         *  @param shape nueva forma
         *
         *  A&ntilde;ade la forma al vector de punteros a formas que componen el
         *  cuerpo. No hace comprobaciones de duplicidad,
         */
        void addShape(Shape* shape);

        /**
         *  @param shape forma a eliminar
         *  @return true si se elimina la forma, false si no formaba parte del
         *  cuerpo
         *
         *  Elimina la forma de la lista que compone el cuerpo si existe. No
         *  destruye la forma.
         */
        bool removeShape(Shape* shape);

        /**
         *  @param name nombre de la forma a eliminar
         *  @return true si se elimina la forma, false en caso de que no
         *  formase parte del cuerpo
         *
         *  Elimina la forma del vector de formas que componen el cuerpo, no
         *  destruye la forma.
         */
        bool removeShape(const Ogre::String& name);

        /**
         *  @return matrix de transformaci&oacute;n del cuerpo
         */
        Ogre::Matrix4 getTransform() const;

        /**
         *  @param nueva matriz de transformaci&oacute;n del cuerpo
         */
        void setTransform(const Ogre::Matrix4& transform);

        /**
         *  @return posici&oacute;n del cuerpo en "world space"
         */
        const Ogre::Vector3& getPosition() const;

        /**
         *  @param position nueva posici&oacute;n del cuerpo en "world space"
         */
        void setPosition(const Ogre::Vector3& position);

        /**
         *  @return escala del cuerpo
         */
        const Ogre::Vector3& getScale() const;

        /**
         *  @param scale nueva escala del cuerpo
         */
        void setScale(const Ogre::Vector3& scale);

        /**
         *  @return orientaci&oacute;n del cuerpo en "world space"
         */
        const Ogre::Quaternion& getOrientation() const;

        /**
         *  @param orientation nueva orientaci&oacute;n del cuerpo en "world space"
         */
        void setOrientation(const Ogre::Quaternion& orientation);

        /**
         *  @return tipo de cuerpo, utilizado en el filtrado de colisiones
         */
        int getType() const;

        /**
         *  @param type nuevo tipo de cuerpo, utilizado en el filtrado de
         *  colisiones
         */
        void setType(int type);

        /**
         *  @param bodyA primer cuerpo del test de colisi&oacute;n
         *  @param bodyB segundo cuerpo del test de colisi&oacute;n
         *  @return true si los dos cuerpos colisionan, false en caso contrario
         *
         *  El m&eacute;todo se encarga de aplicar las transformaciones a las formas
         *  que componen a los dos cuerpos.
         */
        static bool getCollision(Body* bodyA, Body* bodyB);
    private:
        GameObject* _gameObject;
        std::vector<Shape*> _shapes;
        std::vector<Shape*> _worldShapes;
        Ogre::Vector3 _position;
        Ogre::Vector3 _scale;
        Ogre::Quaternion _orientation;
        int _type;

        void updateWorldShapes();
        void createWorldShapes();
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_BODY_H_

