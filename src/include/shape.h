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
 
#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_SHAPE_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_SHAPE_H_

#include <OGRE/Ogre.h>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

class Plane;
class Sphere;
class AxisAlignedBox;
class OrientedBox;

//! Clase abstracta que modela formas b&aacute;sicas, proporciona tests de colisi&oacute;n para dichas formas

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 25-01-2011
 *
 *  Shape es una clase virtual pura que modela el concepto de forma: esfera,
 *  plano, AABB, OBB...
 *
 *  Las formas compondr&aacute;n los cuerpos (clase Body). Un objeto del juego
 *  est&aacute; compuesto por una lista de formas y una matriz de transformaci&oacute;n.
 *
 *  Es la encargada del collision dispatching. Dados dos punteros a dos formas
 *  se encarga de seleccionar el m&eacute;todo que comprueba la colisi&oacute;n
 *  entre ambas seg&uacute;n su clase concreta. Cada tipo de forma cuenta con
 *  un identificador y la clase Shape lleva internamente una tabla que
 *  relaciona dos tipos de formas con el m&eacute;todo que detecta colisiones entre
 *  ellos. Por supuesto, debe haber una funci&oacute;n para los tipos dados, en caso
 *  contrario se produce un error.
 *
 *  Tipos de formas y colisiones implementadas:
 * 
 *  <ul>
 *      <li><strong>Sphere</strong>
 *          <ul>
 *              <li>Sphere: s&iacute;</li>
 *              <li>Plane: s&iacute;</li>
 *              <li>AxisAlignedBox: s&iacute;</li>
 *              <li>OrientedBox: sí</li>
 *          </ul>
 *      </li>
 *      <li><strong>Plane</strong>
 *          <ul>
 *              <li>Sphere: s&iacute;</li>
 *              <li>Plane: s&iacute;</li>
 *              <li>AxisAlignedBox: s&iacute;</li>
 *              <li>OrientedBox: sí</li>
 *          </ul>
 *      </li>
 *      <li><strong>AxisAlignedBox</strong>
 *          <ul>
 *              <li>Sphere: s&iacute;</li>
 *              <li>Plane: s&iacute;</li>
 *              <li>AxisAlignedBox: s&iacute;</li>
 *              <li>OrientedBox: sí</li>
 *          </ul>
 *      </li>
 *      <li><strong>OrientedBox</strong>
 *          <ul>
 *              <li>Sphere: sí</li>
 *              <li>Plane: sí</li>
 *              <li>AxisAlignedBox: sí</li>
 *              <li>OrientedBox: sí</li>
 *          </ul>
 *      </li>
 *  </ul>
 *  
 * 
 *
 *  Ejemplo de uso:
 *
 *  \code
 *  // Preparamos la tabla del collision dispatching
 *  Shape::configureCollisionDispatching();
 *  
 *  // Creamos dos formas    
 *  Sphere* sphereA = new Sphere("sphereA", Ogre::Vector3(0, 0, 0), 5.0);
 *  Sphere* sphereB = new Sphere("sphereB", Ogre::Vector3(1, 1, 1), 2.1);
 *  
 *  // Comprobamos colisiones entre las formas
 *  if (Shape::getCollision(sphereA, sphereB))
 *      cout << sphereA->getName() << " ha colisionado con " << sphereB->getName() << endl;
 *
 *  \endcode
 */
class Shape {
    public:
        /**
         *  Tipos de formas
         */
        enum Type {SPHERE = 1, AABB = 2, PLANE = 3, OBB = 4};
        
        /**
         *  Test de colisión entre dos tipos de formas. Objeto función de boost
         *  que recibe dos punteros a Shape y devuelve bool.
         */
        typedef boost::function<bool(Shape*, Shape*)> CollisionCheckFunction;

        /**
         *  Constructor
         *
         *  @param name nombre de la forma
         *
         *  No puede llamarse directamente ya que es una clase abstracta. Crea
         *  una forma con el nombre dado.
         */
        Shape(const Ogre::String& name = "");

        /**
         *  Destructor
         */
        virtual ~Shape();

        /**
         *  @return nombre de la forma
         */
        const Ogre::String& getName() const;

        /**
         *  @return tipo de la forma
         *
         *  Cada clase que herede de Shape debe implementar este m&eacute;todo
         *  devolviendo el valor correspondiente del enumerado.
         */
        virtual int getType() const = 0;

        /**
         *  @param localShape shape en local space a partir de la cual de
         *  aplica la transformaci&oacute;n. Es imprescindible las dos shapes sean del
         *  mismo tipo.
         *  @param position distancia con respecto al origen en world space
         *  @param scale tama&ntilde;o a aplicar
         *  @param orientation rotaci&oacute;n a aplicar
         *
         *  La forma ahora ser&aacute; la localShape transformada a partir del resto
         *  de par&aacute;metros. Cada clase Shape debe implementar este m&eacute;todo. Las
         *  shapes deben ser del mismo tipo, en otro caso, el resultado es
         *  indeterminado. La transformaci&oacute;n no se puede deshacer.
         */
        virtual void applyTransform(Shape* localShape,
                                    const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                    const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                    const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY) = 0;



        /**
         *  @param traslation desplazamiento a aplicar
         *  @param scale escala de la forma
         *  @param orientation orientaci&oacute;n de la forma
         *  @return una copia de la forma transformada seg&uacute;n los par&aacute;metros
         *  pasados. Se reserva memoria con new, no es conveniente abusar de
         *  esta funci&oacute;n. El usuario del m&eacute;todo es <strong>responsable de
         *  liberar la memoria</strong> de la nueva forma.
         */
        virtual Shape* getTransformedCopy(const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                          const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                          const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY) = 0;

        /**
         *  Construye la tabla que relaciona dos clases concretas  de formas
         *  con un m&eacute;todo que hace la comprobaci&oacute;n de colisiones. Es necesario
         *  llamar a este m&eacute;todo antes de comprobar cualquier tipo de colisi&oacute;n.
         *  La clase CollisionManager deber&iacute;a ser la que llame a este m&eacute;todo.
         */
        static void configureCollisionDispatching();

        /**
         *  @param test objeto función de boost que recibe dos punteros a Shape
         *  y devuelve bool. Test de colisión entre las dos formas.
         *  @param typeA tipo de la primera forma.
         *  @param typeB tipo de la segunda forma.
         *
         *  Añade un test de colisión entre dos tipos de formas. Si ya existía
         *  uno para dicha combinación de formas, se sobreescribe. Sólo es
         *  necesario llamar al método una vez por cada pareja de formas.
         *  Internamente ya hace que el test sea recíproco (A-B y B-A).
         */
        static void addCollisionTest(CollisionCheckFunction test, int typeA, int typeB);

        /**
         *  @param shapeA forma A
         *  @param shapeB forma B
         *  @return true si existe colisi&oacute;n entre A y B, false en caso
         *  contrario.
         *
         *  Recibe dos formas de un tipo indeterminado y devuelve si existe
         *  colisi&oacute;n entre ambas. Si no existe un m&eacute;todo que gestione la
         *  colisi&oacute;n entre los tipos concretos de formas dadas se devuelve
         *  falso y se avisa del problema.
         */
        static bool getCollision(Shape* shapeA, Shape* shapeB);

    protected:
        Ogre::String _name;

    private:
        typedef boost::unordered_map<int, boost::unordered_map<int, CollisionCheckFunction> > CollisionDispatchTable;

        static CollisionDispatchTable  _collisionDispatcher;
        
        static bool getCollisionSphereSphere(Shape* shapeA, Shape* shapeB);
        static bool getCollisionAABBAABB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionPlanePlane(Shape* shapeA, Shape* shapeB);
        static bool getCollisionOBBOBB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionSphereAABB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionSphereOBB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionPlaneSphere(Shape* shapeA, Shape* shapeB);
        static bool getCollisionPlaneAABB(Shape* shapeA, Shape* shapeB);
        static bool getCollisionOBBPlane(Shape* shapeA, Shape* shapeB);
        static bool getCollisionOBBAABB(Shape* shapeA, Shape* shapeB);
};


//! Clase que modela la forma "esfera"

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 27-01-2011
 *
 *  Clase que hereda de Shape y modela la esfera. Las esferas est&aacute;n compuestas
 *  de un punto (centro) y un radio. 
 */
class Sphere: public Shape {
    public:
        /**
         *  Constructor
         *
         *  @param name nombre de la esfera
         *  @param center punto que indica el centro de la esfera
         *  @param radius radio de la esfera
         */
        Sphere(const Ogre::String& name = "",
               const Ogre::Vector3& center = Ogre::Vector3::ZERO,
               Ogre::Real radius = 1.0);

        /**
         *  Destructor
         */
        ~Sphere();

        /**
         *  @return tipo de forma, en este caso Shape::SPHERE
         */
        int getType() const;
        
        void applyTransform(Shape* localShape,
                            const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                            const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        Shape* getTransformedCopy(const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                  const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                  const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);
        /**
         *  @return radio de la esfera
         */
        Ogre::Real getRadius() const;

        /**
         *  @param radius nuevo radio de la esfera
         */
        void setRadius(Ogre::Real radius);

        /**
         *  @return centro de la esfera
         */
        const Ogre::Vector3& getCenter() const;

        /**
         *  @param center nuevo centro de la esfera
         */
        void setCenter(const Ogre::Vector3& center);
    private:
        Ogre::Vector3 _center;
        Ogre::Real _radius;
};



//! Clase que modela la forma "caja alineada" 

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 28-01-2011
 *
 *  Clase que hereda de Shape y modela una caja alineada con los ejes.
 *  Las AABB (AxisAlignedBoxes) est&aacute;n compuestas del punto m&iacute;nimo y el m&aacute;ximo.
 */
class AxisAlignedBox: public Shape {
    public:
        /**
         *  Constructor
         *
         *  @param name nombre del AABB
         *  @param minPos punto m&iacute;nimo del AABB
         *  @param maxPos punto m&aacute;ximo del AABB
         */
        AxisAlignedBox(const Ogre::String& name = "",
                       const Ogre::Vector3& minPos = Ogre::Vector3(-1, -1, -1),
                       const Ogre::Vector3& maxPos = Ogre::Vector3(1, 1, 1));

        /**
         *  Destructor
         */
        ~AxisAlignedBox();

        /**
         *  @return tipo del AABB, es decir, el valor del enumerado
         *  Sphere::AABB
         */
        int getType() const;
        
        void applyTransform(Shape* localShape,
                            const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                            const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        Shape* getTransformedCopy(const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                  const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                  const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        /**
         *  @return punto m&iacute;nimo del AABB
         */
        const Ogre::Vector3& getMinPos() const;

        /**
         *  @param minPos nuevo punto m&iacute;nimo del AABB
         */
        void setMinPos(const Ogre::Vector3& minPos);
        
        /**
         *  @return punto m&aacute;ximo del AABB
         */
        const Ogre::Vector3& getMaxPos() const;

        /**
         *  @param maxPos nuevo punto m&aacute;ximo del AABB
         */
        void setMaxPos(const Ogre::Vector3& maxPos);

    private:
        Ogre::Vector3 _minPos;
        Ogre::Vector3 _maxPos;
};



//! Clase que modela la forma "plano" 

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 28-01-2011
 *
 *  Clase que hereda de Shape y modela un plano. Un plano se compone de un
 *  vector normal y una distacia con respecto al origen.
 */
class Plane: public Shape {
    public:
        /**
         *  Constructor
         *
         *  @param name nombre de la forma
         *  @param position distancia del plano al origen de coordenadas
         *  @param normal vector normal al plano
         */
        Plane(const Ogre::String& name = "",
              const Ogre::Vector3& position = Ogre::Vector3(0, 0, 0),
              const Ogre::Vector3& normal = Ogre::Vector3(0, 1, 0));

        /**
         *  Destructor
         */
        ~Plane();

        /**
         *  @return tipo de forma, Shape::PLANE
         */
        int getType() const;
        
        void applyTransform(Shape* localShape,
                            const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                            const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        Shape* getTransformedCopy(const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                  const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                  const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);


        /**
         *  @return distancia del plano con respecto al origen.
         */
        const Ogre::Vector3& getPosition() const;

        /**
         *  @param position nueva distacia del plano con respecto al origen.
         */
        void setPosition(const Ogre::Vector3& position);

        /**
         *  @return vector normal al plano
         */
        const Ogre::Vector3& getNormal() const;

        /**
         *  @param normal nuevo vector normal al plano
         */
        void setNormal(const Ogre::Vector3& normal);
        
        /**
         *  @param pointA primer punto
         *  @param pointB segundo punto
         *  @param pointC tercer punto
         * 
         *  Configura el plano a partir de 3 puntos
         */
        void setPoints(const Ogre::Vector3& pointA,
                       const Ogre::Vector3& pointB,
                       const Ogre::Vector3& pointC);
        
        Ogre::Real getX(Ogre::Real y, Ogre::Real z);               
        Ogre::Real getY(Ogre::Real x, Ogre::Real z);
        Ogre::Real getZ(Ogre::Real x, Ogre::Real y);

    private:
        Ogre::Vector3 _position;
        Ogre::Vector3 _normal;
};


//! Clase que modela una caja orientada

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 10-02-2011
 *
 *  Clase que hereda de Shape y modela una caja orientada. Se compone de un
 *  centro, una extensi&oacute;n y unos ejes locales.
 */
class OrientedBox: public Shape {
    public:
        /**
         *  Constructor
         *
         *  @param name nombre de la forma
         *  @param center centro de la caja
         *  @param extent extensión de la caja (distancia del centro a cada
         *  plano (superior/inferior, laterales y frente).
         *  @param axes ejes locales que representan la rotación de la caja
         */
        OrientedBox(const Ogre::String& name = "",
                    const Ogre::Vector3& center = Ogre::Vector3::ZERO,
                    const Ogre::Vector3& extent = Ogre::Vector3::UNIT_SCALE,
                    const Ogre::Matrix3& axes = Ogre::Matrix3::IDENTITY);
        
        /**
         *  Destructor
         */
        ~OrientedBox();
        
        /**
         *  @return Shape::OBB
         */
        int getType() const;
        
        /**
         *  @param localShape forma que se quiere transformar, debe ser OBB
         *  @param traslation traslación a aplicar
         *  @param scale escala
         *  @param orientation rotación a aplicar
         */
        void applyTransform(Shape* localShape,
                            const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                            const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        /**
         *  @param traslation traslación a aplicar
         *  @param scale escala
         *  @param orientation rotación a aplicar
         *
         *  @return copia de la forma transformada según los parámetros
         */
        Shape* getTransformedCopy(const Ogre::Vector3& traslation = Ogre::Vector3::ZERO,
                                  const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
                                  const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY);

        /**
         *  @return centro del OBB
         */
        const Ogre::Vector3& getCenter() const;

        /**
         *  @param center nuevo centro el OBB
         */
        void setCenter(const Ogre::Vector3& center);

        /**
         *  @return extensión del OBB
         */
        const Ogre::Vector3& getExtent() const;

        /**
         *  @param extent nueva extensión del OBB
         */
        void setExtent(const Ogre::Vector3& extent);

        /**
         *  @return ejes locales del OBB
         */
        const Ogre::Matrix3& getAxes() const;

        /**
         *  @param axes nuevos ejes locales del OBB
         */
        void setAxes(const Ogre::Matrix3& axes);
    private:
        Ogre::Vector3 _center;
        Ogre::Vector3 _extent;
        Ogre::Matrix3 _axes;
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_SHAPE_H_
