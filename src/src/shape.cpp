/*
 * This file is part of SionTower.
 *
 * 
 * David Saltares Márquez (C) 2011
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
 *  @file shape.cpp
 *  @author David Saltares Márquez
 *  @date 25-01-2011
 */

#include <iostream>
#include <algorithm>

#include <boost/bind.hpp>


#include "shape.h"

using std::cerr;
using std::cout;
using std::endl;

Shape::CollisionDispatchTable Shape::_collisionDispatcher;

Shape::Shape(const Ogre::String& name): _name(name) {
}

Shape::~Shape() {
}

const Ogre::String& Shape::getName() const {
    return _name;
}

void Shape::configureCollisionDispatching() {
    // Completamos la tabla de chequeos de colisión
    _collisionDispatcher[SPHERE][SPHERE] = boost::bind(&Shape::getCollisionSphereSphere, _1, _2);
    _collisionDispatcher[AABB][AABB] = boost::bind(&Shape::getCollisionAABBAABB, _1, _2);
    _collisionDispatcher[PLANE][PLANE] = boost::bind(&Shape::getCollisionPlanePlane, _1, _2);
    _collisionDispatcher[OBB][OBB] = boost::bind(&Shape::getCollisionOBBOBB, _1, _2);

    _collisionDispatcher[AABB][SPHERE] = boost::bind(&Shape::getCollisionSphereAABB, _1, _2);
    _collisionDispatcher[AABB][PLANE] = boost::bind(&Shape::getCollisionPlaneAABB, _1, _2);
    _collisionDispatcher[AABB][OBB] = boost::bind(&Shape::getCollisionOBBAABB, _1, _2);

    _collisionDispatcher[SPHERE][AABB] = boost::bind(&Shape::getCollisionSphereAABB, _1, _2);
    _collisionDispatcher[SPHERE][PLANE] = boost::bind(&Shape::getCollisionPlaneSphere, _1, _2);
    _collisionDispatcher[SPHERE][OBB] = boost::bind(&Shape::getCollisionSphereOBB, _1, _2);

    _collisionDispatcher[PLANE][AABB] = boost::bind(&Shape::getCollisionPlaneAABB, _1, _2);
    _collisionDispatcher[PLANE][SPHERE] = boost::bind(&Shape::getCollisionPlaneSphere, _1, _2);
    _collisionDispatcher[PLANE][OBB] = boost::bind(&Shape::getCollisionOBBPlane, _1, _2);
    
    _collisionDispatcher[OBB][SPHERE] = boost::bind(&Shape::getCollisionSphereOBB, _1, _2);
    _collisionDispatcher[OBB][PLANE] = boost::bind(&Shape::getCollisionOBBPlane, _1, _2);
    _collisionDispatcher[OBB][AABB] = boost::bind(&Shape::getCollisionOBBAABB, _1, _2);
}
        
void Shape::addCollisionTest(CollisionCheckFunction test, int typeA, int typeB) {
    // Añadimos/Actualizamos el test de colisión (sin importar el orden entre
    // formas)
    _collisionDispatcher[typeA][typeB] = test;
    _collisionDispatcher[typeB][typeA] = test;
}

bool Shape::getCollision(Shape* shapeA, Shape* shapeB) {
    // Comprobamos si la forma A está registrada
    CollisionDispatchTable::iterator itA = _collisionDispatcher.find(shapeA->getType());
    if (itA == _collisionDispatcher.end()) {
        cout << "Shape::getCollision(): no existe el tipo " << shapeA->getType() << endl;
        return 0;
    }
    
    // Comprobamos que la forma B está registrada
    CollisionDispatchTable::iterator itB = _collisionDispatcher.find(shapeB->getType());
    if (itB == _collisionDispatcher.end()) {
        cout << "Shape::getCollision(): no existe el tipo " << shapeB->getType() << endl;
        return 0;
    }

    // Comprobamos que hay un método de comprobación del tipo A - B
    boost::unordered_map<int, CollisionCheckFunction>::iterator itC = _collisionDispatcher[shapeA->getType()].find(shapeB->getType());
    if (itC == itA->second.end()) {
        cout << "Shape::getCollision(): no existe un método de comprobación entre" << shapeA->getType() << " y " << shapeB->getType() << endl;
        return 0;
    }

    // Llamamos al método de comprobación
    return itC->second(shapeA, shapeB);
}


bool Shape::getCollisionSphereSphere(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que son esferas)
    Sphere* sphereA = static_cast<Sphere*>(shapeA);
    Sphere* sphereB = static_cast<Sphere*>(shapeB);

    // Hacemos el test
    Ogre::Vector3 s = sphereA->getCenter() - sphereB->getCenter();
    Ogre::Real totalRadius = sphereA->getRadius() + sphereB->getRadius();

    return (s.squaredLength() <= totalRadius * totalRadius);
}


bool Shape::getCollisionAABBAABB(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que son AABBs)
    AxisAlignedBox* aabb1= static_cast<AxisAlignedBox*>(shapeA);
    AxisAlignedBox* aabb2 = static_cast<AxisAlignedBox*>(shapeB);

    // Hacemos el test
    return (aabb1->getMaxPos().x > aabb2->getMinPos().x &&
            aabb1->getMinPos().x < aabb2->getMaxPos().x &&
            aabb1->getMaxPos().y > aabb2->getMinPos().y &&
            aabb1->getMinPos().y < aabb2->getMaxPos().y &&
            aabb1->getMaxPos().z > aabb2->getMinPos().z &&
            aabb1->getMinPos().z < aabb2->getMaxPos().z);
}

bool Shape::getCollisionSphereAABB(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que A es Sphere y B es AABB)
    Sphere* sphere;
    AxisAlignedBox* aabb;
    if (shapeA->getType() == SPHERE) {
        sphere = static_cast<Sphere*>(shapeA);
        aabb = static_cast<AxisAlignedBox*>(shapeB);
    } else {
        sphere = static_cast<Sphere*>(shapeB);
        aabb = static_cast<AxisAlignedBox*>(shapeA);
    }

    // Hacemos el test
    Ogre::Real s = 0;
    Ogre::Real d = 0;
    Ogre::Vector3 center = sphere->getCenter();
    Ogre::Vector3 minPos = aabb->getMinPos();
    Ogre::Vector3 maxPos = aabb->getMaxPos();

    // Comprobamos si el centro de la esfera está dentro del AABB
    bool centerInsideAABB = (center.x <= maxPos.x &&
                             center.x >= minPos.x &&
                             center.y <= maxPos.y &&
                             center.y >= minPos.y &&
                             center.z <= maxPos.z &&
                             center.z >= minPos.z);

    if (centerInsideAABB)
        return true;

    // Comprobamos si la esfera y el AABB se intersectan
    for (int i = 0; i < 3; ++i) {
        if (sphere->getCenter()[i] < aabb->getMinPos()[i]) {
            s = sphere->getCenter()[i] - aabb->getMinPos()[i];
            d += s * s;
        } else if (sphere->getCenter()[i] > aabb->getMaxPos()[i]) {
            s = sphere->getCenter()[i] - aabb->getMaxPos()[i];
            d += s * s;
        }
    }

    return (d <= sphere->getRadius() * sphere->getRadius());
}

bool Shape::getCollisionPlanePlane(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que son Planes)
    Plane* planeA = static_cast<Plane*>(shapeA);
    Plane* planeB = static_cast<Plane*>(shapeB);

    // Hacemos el test
    Ogre::Vector3 normalA = planeA->getNormal().normalisedCopy();
    Ogre::Vector3 normalB = planeB->getNormal().normalisedCopy();
    return normalA.dotProduct(normalB) != 1 || planeA->getPosition() == planeB->getPosition();
}


bool Shape::getCollisionOBBOBB(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión, estamos seguros de que son OBB
    OrientedBox* obbA = static_cast<OrientedBox*>(shapeA);
    OrientedBox* obbB = static_cast<OrientedBox*>(shapeB);

    
    // FUENTE: Real Time Collision Detection pág 101


    // Obtenemos B en función de los ejes locales de A
    Ogre::Real ra, rb;
    Ogre::Matrix3 R, absR;
    Ogre::Matrix3 axesA = obbA->getAxes();
    Ogre::Matrix3 axesB = obbB->getAxes();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            Ogre::Vector3 vA(axesA[i][0], axesA[i][1], axesA[i][2]);
            Ogre::Vector3 vB(axesB[j][0], axesB[j][1], axesB[j][2]);
            R[i][j] = vA.dotProduct(vB);
        }
    }

    // Vector de translación t en los ejes de A
    Ogre::Vector3 t = obbB->getCenter() - obbA->getCenter();
    t = Ogre::Vector3(t.dotProduct(Ogre::Vector3(axesA[0][0], axesA[0][1], axesA[0][2])),
                      t.dotProduct(Ogre::Vector3(axesA[1][0], axesA[1][1], axesA[1][2])),
                      t.dotProduct(Ogre::Vector3(axesA[2][0], axesA[2][1], axesA[2][2])));
        
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 3; ++j)
            absR[i][j] = std::abs(R[i][j]);

    // Test ejes L = A0 L = A1 L = A2
    for (int i = 0; i < 3; ++i) {
        ra = obbA->getExtent()[i];
        rb = obbB->getExtent()[0] * absR[i][0] +
             obbB->getExtent()[1] * absR[i][1] +
             obbB->getExtent()[2] * absR[i][2];

        if (std::abs(t[i]) > ra + rb) return false;
    }

    // Test ejes L = B0 L = B1 L = B2
    for (int i = 0; i < 3; ++i) {
        ra = obbA->getExtent()[0] * absR[0][i] +
             obbA->getExtent()[1] * absR[1][i] +
             obbA->getExtent()[2] * absR[2][i];
        rb = obbB->getExtent()[i];

        if (std::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return false;
    }
   
    // Test eje L = A0 x B0
    ra = obbA->getExtent()[1] * absR[2][0] + obbA->getExtent()[2] * absR[1][0];
    rb = obbB->getExtent()[1] * absR[0][2] + obbB->getExtent()[2] * absR[0][1];
    if (std::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

    // Test eje L = A0 x B1
    ra = obbA->getExtent()[1] * absR[2][1] + obbA->getExtent()[2] * absR[1][1];
    rb = obbB->getExtent()[0] * absR[0][2] + obbB->getExtent()[2] * absR[0][0];
    if (std::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

    // Test eje L = A0 x B2
    ra = obbA->getExtent()[1] * absR[2][2] + obbA->getExtent()[2] * absR[1][2];
    rb = obbB->getExtent()[0] * absR[0][1] + obbB->getExtent()[1] * absR[0][0];
    if (std::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

    // Test eje L = A1 x B0
    ra = obbA->getExtent()[0] * absR[2][0] + obbA->getExtent()[2] * absR[0][0];
    rb = obbB->getExtent()[1] * absR[1][2] + obbB->getExtent()[2] * absR[1][1];
    if (std::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

    // Test eje L = A1 x B1
    ra = obbA->getExtent()[0] * absR[2][1] + obbA->getExtent()[2] * absR[0][1];
    rb = obbB->getExtent()[0] * absR[1][2] + obbB->getExtent()[2] * absR[1][0];
    if (std::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;
   
    // Test eje L = A1 x B2
    ra = obbA->getExtent()[0] * absR[2][2] + obbA->getExtent()[2] * absR[0][2];
    rb = obbB->getExtent()[0] * absR[1][1] + obbB->getExtent()[1] * absR[1][0];
    if (std::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

    // Test eje L = A2 x B0
    ra = obbA->getExtent()[0] * absR[1][0] + obbA->getExtent()[1] * absR[0][0];
    rb = obbB->getExtent()[1] * absR[2][2] + obbB->getExtent()[2] * absR[2][1];
    if (std::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

    // Test eje L = A2 x B1
    ra = obbA->getExtent()[0] * absR[1][1] + obbA->getExtent()[1] * absR[0][1];
    rb = obbB->getExtent()[0] * absR[2][2] + obbB->getExtent()[2] * absR[2][0];
    if (std::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

    // Test eje L = A2 x B2
    ra = obbA->getExtent()[0] * absR[1][2] + obbA->getExtent()[1] * absR[0][2];
    rb = obbB->getExtent()[0] * absR[2][1] + obbB->getExtent()[1] * absR[2][0];
    if (std::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;


    return true;
}


bool Shape::getCollisionPlaneSphere(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que A es Plane y B es Sphere)
    Plane* plane;
    Sphere* sphere;
    if (shapeA->getType() == PLANE) {
        plane = static_cast<Plane*>(shapeA);
        sphere = static_cast<Sphere*>(shapeB);
    } else {
        plane = static_cast<Plane*>(shapeB);
        sphere = static_cast<Sphere*>(shapeA);
    }

    // Hacemos el test
    
    // Distancia del centro de la esfera al plano
    Ogre::Vector3 v = sphere->getCenter() - plane->getPosition();
    Ogre::Vector3 n = plane->getNormal().normalisedCopy();
    Ogre::Real d = abs(n.dotProduct(v));

    // Si d <= radio, hay colisión
    return d <= sphere->getRadius();
}

bool Shape::getCollisionPlaneAABB(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que A es Plane y B es AABB)
    Plane* plane;
    AxisAlignedBox* aabb;
    if (shapeA->getType() == PLANE) {
        plane = static_cast<Plane*>(shapeA);
        aabb = static_cast<AxisAlignedBox*>(shapeB);
    } else {
        plane = static_cast<Plane*>(shapeB);
        aabb = static_cast<AxisAlignedBox*>(shapeA);
    }


    // Hacemos el test
    Ogre::Vector3 p;
    Ogre::Vector3 n;

    for (int i = 0; i < 3; ++i) {
        if (plane->getNormal()[i] >= 0) {
            p[i] = aabb->getMaxPos()[i];
            n[i] = aabb->getMinPos()[i];
        } else {
            p[i] = aabb->getMaxPos()[i];
            n[i] = aabb->getMinPos()[i];
        }
    }

    // Si p está en un lado diferente del plano que n, hay intersección
    Ogre::Real d1 = plane->getNormal().dotProduct(p - plane->getPosition());
    Ogre::Real d2 = plane->getNormal().dotProduct(n - plane->getPosition());

    return ((d1 <= 0 && d2 >= 0) || (d1 >= 0 && d2 <= 0));
}


static Ogre::Vector3 closestPointToOBB(const Ogre::Vector3& p, const OrientedBox* obb) {
    Ogre::Vector3 d = p - obb->getCenter();
    Ogre::Vector3 closest = obb->getCenter();
    Ogre::Matrix3 axes = obb->getAxes();

    for (int i = 0; i < 3; ++i) {
        Ogre::Real dist = d.dotProduct(Ogre::Vector3(axes[i][0], axes[i][1], axes[i][2]));
        if (dist > obb->getExtent()[i]) 
            dist = obb->getExtent()[i];
        if (dist < -obb->getExtent()[i]) 
            dist = -obb->getExtent()[i];

        closest += dist * Ogre::Vector3(axes[i][0], axes[i][1], axes[i][2]);
    }

    return closest;
}

bool Shape::getCollisionSphereOBB(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que uno es Sphere y otro OBB
    Sphere* sphere;
    OrientedBox* obb;
    if (shapeA->getType() == OBB) {
        obb = static_cast<OrientedBox*>(shapeA);
        sphere = static_cast<Sphere*>(shapeB);
    } else {
        obb = static_cast<OrientedBox*>(shapeB);
        sphere = static_cast<Sphere*>(shapeA);
    }

    Ogre::Vector3 closest = closestPointToOBB(sphere->getCenter(), obb);

    Ogre::Vector3 v = closest - sphere->getCenter();

    return v.dotProduct(v) <= sphere->getRadius() * sphere->getRadius();
}

        
bool Shape::getCollisionOBBPlane(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que uno es Plane y otro OBB
    Plane* plane;
    OrientedBox* obb;
    if (shapeA->getType() == OBB) {
        obb = static_cast<OrientedBox*>(shapeA);
        plane = static_cast<Plane*>(shapeB);
    } else {
        obb = static_cast<OrientedBox*>(shapeB);
        plane = static_cast<Plane*>(shapeA);
    }

    Ogre::Matrix3 axes = obb->getAxes();
    Ogre::Vector3 extent = obb->getExtent();
    Ogre::Vector3 normal = plane->getNormal();

    // Radio de la proyección de obb en el plano L(t) = obb.center + t * plane.normal
    Ogre::Real r = extent[0] * std::abs(normal.dotProduct(Ogre::Vector3(axes[0][0], axes[0][1], axes[0][2]))) +
                   extent[1] * std::abs(normal.dotProduct(Ogre::Vector3(axes[1][0], axes[1][1], axes[1][2]))) +
                   extent[2] * std::abs(normal.dotProduct(Ogre::Vector3(axes[2][0], axes[2][1], axes[2][2])));

    // Distancia del centro de la caja al plano
    Ogre::Real s = normal.dotProduct(obb->getCenter() - plane->getPosition());
    
    return abs(s) <= r;
}

        
bool Shape::getCollisionOBBAABB(Shape* shapeA, Shape* shapeB) {
    // Hacemos la conversión (estamos seguros de que uno es AABB y otro OBB
    AxisAlignedBox* aabb;
    OrientedBox* obb;
    if (shapeA->getType() == OBB) {
        obb = static_cast<OrientedBox*>(shapeA);
        aabb = static_cast<AxisAlignedBox*>(shapeB);
    } else {
        obb = static_cast<OrientedBox*>(shapeB);
        aabb = static_cast<AxisAlignedBox*>(shapeA);
    }

    // Convertimos aabb en obb
    Ogre::Vector3 minPos = aabb->getMinPos();
    Ogre::Vector3 maxPos = aabb->getMaxPos();
    Ogre::Vector3 extent = (maxPos - minPos) * 0.5f;
    Ogre::Vector3 center = (maxPos + minPos) * 0.5f;

    OrientedBox convertedOBB("convertedOBB", center, extent, Ogre::Matrix3::IDENTITY);

    return getCollisionOBBOBB(obb, &convertedOBB);
}


/*
 *  SPHERE
 */


Sphere::Sphere(const Ogre::String& name,
               const Ogre::Vector3& center,
               Ogre::Real radius): Shape(name), _center(center), _radius(radius) {
}

Sphere::~Sphere() {
}

int Sphere::getType() const {
    return SPHERE;
}

void Sphere::applyTransform(Shape* localShape,
                            const Ogre::Vector3& traslation,
                            const Ogre::Vector3& scale,
                            const Ogre::Quaternion& orientation) {
    // localShape debería ser una esfera
    Sphere* localSphere = static_cast<Sphere*>(localShape);

    // Aplicamos traslación y escala (no es necesario rotar la esfera)
    _center = localSphere->_center + traslation;
    // No podemos escalar una esfera teniendo en cuenta x, y, z porque no
    // podemos deformarla.
    _radius = localSphere->_radius * scale.x;
}
        
Shape* Sphere::getTransformedCopy(const Ogre::Vector3& traslation,
                                  const Ogre::Vector3& scale,
                                  const Ogre::Quaternion& orientation) {
    // Creamos la nueva esfera
    Sphere* sphere = new Sphere(_name, _center, _radius);

    // Aplicamos la transformacion
    sphere->applyTransform(this, traslation, scale, orientation);

    return sphere;
}

Ogre::Real Sphere::getRadius() const {
    return _radius;
}
        
void Sphere::setRadius(Ogre::Real radius) {
    _radius = radius;
}

const Ogre::Vector3& Sphere::getCenter() const {
    return _center;
}

void Sphere::setCenter(const Ogre::Vector3& center) {
    _center = center;    
}





/*
 *  AXISALIGNEDBOX
 */


AxisAlignedBox::AxisAlignedBox(const Ogre::String& name,
                               const Ogre::Vector3& minPos,
                               const Ogre::Vector3& maxPos): Shape(name), _minPos(minPos), _maxPos(maxPos) {
}

AxisAlignedBox::~AxisAlignedBox() {
}

int AxisAlignedBox::getType() const {
    return AABB;
}
        
void AxisAlignedBox::applyTransform(Shape* localShape,
                                    const Ogre::Vector3& traslation,
                                    const Ogre::Vector3& scale,
                                    const Ogre::Quaternion& orientation) {
    // localShape debería ser AxisAlignedBox
    AxisAlignedBox* localAABB = static_cast<AxisAlignedBox*>(localShape);

    // Aplicamos traslación
    _minPos = localAABB->_minPos + traslation;
    _maxPos = localAABB->_maxPos + traslation;

    // Aplicamos la escala
    _minPos *= scale;
    _maxPos *= scale;

    // No podemos rotar un AABB
}

Shape* AxisAlignedBox::getTransformedCopy(const Ogre::Vector3& traslation,
                                          const Ogre::Vector3& scale,
                                          const Ogre::Quaternion& orientation) {
    // Creamos el nuevo AABB
    AxisAlignedBox* aabb = new AxisAlignedBox(_name, _minPos, _maxPos);

    // Aplicamos la transformacion
    aabb->applyTransform(this, traslation, scale, orientation);

    return aabb;
}

const Ogre::Vector3& AxisAlignedBox::getMinPos() const {
    return _minPos;
}

void AxisAlignedBox::setMinPos(const Ogre::Vector3& minPos) {
    _minPos = minPos;
}

const Ogre::Vector3& AxisAlignedBox::getMaxPos() const {
    return _maxPos;
}

void AxisAlignedBox::setMaxPos(const Ogre::Vector3& maxPos) {
    _maxPos = maxPos;
}



/*
 *  PLANE
 */


Plane::Plane(const Ogre::String& name,
             const Ogre::Vector3& position,
             const Ogre::Vector3& normal): Shape(name), _position(position), _normal(normal) {
}

Plane::~Plane() {
}

int Plane::getType() const {
    return PLANE;
}
        
void Plane::applyTransform(Shape* localShape,
                           const Ogre::Vector3& traslation,
                           const Ogre::Vector3& scale,
                           const Ogre::Quaternion& orientation) {
    
    // localShape debería ser Plane
    Plane* plane = static_cast<Plane*>(localShape);

    // Aplicamos traslacion
    _position = plane->_position + traslation;

    // No tiene sentido aplicar escala
    
    // Aplicamos la rotación a la normal
    _normal = orientation * plane->_normal;
}

Shape* Plane::getTransformedCopy(const Ogre::Vector3& traslation,
                                 const Ogre::Vector3& scale,
                                 const Ogre::Quaternion& orientation) {
    // Creamos un nuevo plano
    Plane* plane = new Plane(_name, _position, _normal);

    // Aplicamos la transformacion
    plane->applyTransform(this, traslation, scale, orientation);

    return plane;
}

void Plane::setPoints(const Ogre::Vector3& pointA,
                      const Ogre::Vector3& pointB,
                      const Ogre::Vector3& pointC) {
    // Tomamos los vectores AB y AC
    Ogre::Vector3 AB = pointB - pointA;
    Ogre::Vector3 AC = pointC - pointA;
    
    // Su producto vectorial será la normal
    _normal = AB.crossProduct(AC);
    
    // La posición puede ser cualquier punto
    _position = pointA;
}


const Ogre::Vector3& Plane::getPosition() const {
    return _position;
}

void Plane::setPosition(const Ogre::Vector3& position) {
    _position = position;
}

const Ogre::Vector3& Plane::getNormal() const {
    return _normal;
}

void Plane::setNormal(const Ogre::Vector3& normal) {
    _normal = normal;
}

Ogre::Real Plane::getX(Ogre::Real y, Ogre::Real z) {    
    Ogre::Real distance = -(_position.dotProduct(_normal));
    
    if (_normal[0])
        return (-(_normal[1] * y + _normal[2] * z + distance) / _normal[0]);
        
    return 0.0f;
}   
        
Ogre::Real Plane::getY(Ogre::Real x, Ogre::Real z) {
    Ogre::Real distance = -(_position.dotProduct(_normal));
    
    if (_normal[1])
        return (-(_normal[0] * x + _normal[2] * z + distance) / _normal[1]);
    
    return 0.0;
}

Ogre::Real Plane::getZ(Ogre::Real x, Ogre::Real y) {
    Ogre::Real distance = -(_position.dotProduct(_normal));
    
    if (_normal[2])
        return (-(_normal[0] * x + _normal[1] * y + distance) / _normal[2]);
        
    return 0.0f;
}



/*
 *  ORIENTED BOX
 *
 */

OrientedBox::OrientedBox(const Ogre::String& name,
                         const Ogre::Vector3& center,
                         const Ogre::Vector3& extent,
                         const Ogre::Matrix3& axes): Shape(name), _center(center), _extent(extent), _axes(axes) {
}

OrientedBox::~OrientedBox() {
}

int OrientedBox::getType() const {
    return OBB;
}

void OrientedBox::applyTransform(Shape* localShape,
                                 const Ogre::Vector3& traslation,
                                 const Ogre::Vector3& scale,
                                 const Ogre::Quaternion& orientation) {

    // localShape es OBB
    OrientedBox* obb = static_cast<OrientedBox*>(localShape);

    // Transformamos el OBB
    _center = obb->_center + traslation;
    Ogre::Matrix3 rotation;
    orientation.ToRotationMatrix(rotation);
    _axes = obb->_axes * rotation;
    _extent = obb->_extent * scale;
}

Shape* OrientedBox::getTransformedCopy(const Ogre::Vector3& traslation,
                                       const Ogre::Vector3& scale,
                                       const Ogre::Quaternion& orientation) {
    OrientedBox* obb = new OrientedBox(_name, _center, _extent, _axes);

    obb->applyTransform(this, traslation, scale, orientation);

    return obb;
}

const Ogre::Vector3& OrientedBox::getCenter() const {
    return _center;
}

void OrientedBox::setCenter(const Ogre::Vector3& center) {
    _center = center;
}

const Ogre::Vector3& OrientedBox::getExtent() const {
    return _extent;
}

void OrientedBox::setExtent(const Ogre::Vector3& extent) {
    _extent = extent;
}

const Ogre::Matrix3& OrientedBox::getAxes() const {
    return _axes;
}

void OrientedBox::setAxes(const Ogre::Matrix3& axes) {
    _axes = axes;
}

