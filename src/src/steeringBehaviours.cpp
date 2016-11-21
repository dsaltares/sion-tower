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

#include <iostream>

#include "steeringBehaviours.h"
#include "enemy.h"


using std::cout;
using std::endl;

// STEERING BEHAVIOUR

SteeringBehaviour::SteeringBehaviour(Kinematic* character) {
    this->character = character;
}


// SEEK

Seek::Seek(Kinematic* character, Kinematic* target): SteeringBehaviour(character) {
    this->target = target;
    maxAcceleration = 1.0f;
}

void Seek::getSteering(Steering& steering) {
    // Dirección hacia el objetivo
    Ogre::Vector3 linear = target->getPosition() - character->getPosition();
    
    // Nos limitamos a la máxima aceleración
    linear.normalise();
    linear *= maxAcceleration;
    
    // Actualizamos steering
    steering.setLinear(linear);
    steering.setAngular(0.0f);
}


// FLEE

Flee::Flee(Kinematic* character, Kinematic* target): SteeringBehaviour(character) {
    this->target = target;
    maxAcceleration = 2.0f;
}

void Flee::getSteering(Steering& steering) {
    if (character != 0 && target != 0) {
        // Dirección huyendo del objetivo
        Ogre::Vector3 linear = character->getPosition() - target->getPosition();
        
        // Nos limitamos a la máxima aceleración
        linear.normalise();
        linear *= maxAcceleration;
        
        // Actualizamos steering
        steering.setLinear(linear);
        steering.setAngular(0.0f);
    }
}


// ARRIVE

Arrive::Arrive(Kinematic* character, Kinematic* target): SteeringBehaviour(character) {
    this->target = target;
    maxAcceleration = 5.0f;
    targetRadius = 2.0f;
    slowRadius = 4.0f;
    timeToTarget = 0.1f;
}

void Arrive::getSteering(Steering& steering) {
    if (character != 0 && target != 0) {
        // Dirección hacia el objetivo
        Ogre::Vector3 direction = target->getPosition() - character->getPosition();
        Ogre::Real distance = direction.length();
        
        // Comprobar si hemos llegado
        if (distance <= targetRadius) {
            steering.setNone();
            return;
        }
        
        Ogre::Real targetSpeed;
        
        // Si estamos fuera del área de frenado, vamos a máxima velocidad
        if (distance > slowRadius)
            targetSpeed = character->getMaxSpeed();
            
        // Si no, calculamos una velocidad nueva
        else
            targetSpeed = character->getMaxSpeed() * distance / slowRadius;
            
        // Velocidad objetivo
        Ogre::Vector3 targetVelocity;
        targetVelocity = direction;
        targetVelocity.normalise();
        targetVelocity *= targetSpeed;
        
        // La aceleración trata de alcanzar la velocidad objetivo
        Ogre::Vector3 linear = (targetVelocity - character->getVelocity()) / timeToTarget;

        // Comprobamos si sobrepasamos la aceleración máxima
        if (linear.length() > maxAcceleration) {
            linear.normalise();
            linear *= maxAcceleration;
        }
        
        // Actualizamos steering
        steering.setLinear(linear);
        steering.setAngular(0.0f);
    }
}



// ALIGN

Align::Align(Kinematic* character, Kinematic* target): SteeringBehaviour(character) {
    this->target = target;
    maxAngularAcceleration = 4.0f;
    maxRotation = 3.0f;
    targetRadius = 0.1f;
    slowRadius = 0.5f;
    timeToTarget = 0.1f;
}

void Align::getSteering(Steering& steering) {
    // Tomamos la dirección hacia el objetivo
    Ogre::Real rotation = target->getOrientation() - character->getOrientation();
    
    
    // Mapeamos el resultado al intervalo (-pi, pi)
    rotation = mapToRange(rotation);
    
    Ogre::Real rotationSize = std::abs(rotation);
    
    // Si hemos llegado no devolvemos steering
    if (rotationSize < targetRadius) {
        steering.setNone();
        return;
    }
    
    Ogre::Real targetRotation;
    
    // Si estamos fuera de slowRadius, utilizamos rotación maxima
    if (rotationSize > slowRadius)
        targetRotation = maxRotation;
    else 
        targetRotation = rotationSize / slowRadius;
    
    // La rotación final combina velocidad angular y dirección
    targetRotation *= rotation / rotationSize;
    
    // La aceleración trata de llegar a la rotacion objetivo
    Ogre::Real angular;
    angular = (targetRotation - character->getRotation()) / timeToTarget;
    
    // Comprobamos si la aceleración es demasiado grande
    Ogre::Real angularAcceleration = std::abs(angular);
    
    if (angularAcceleration > maxAngularAcceleration) {
        angular /= angularAcceleration;
        angular *= maxAngularAcceleration;
    }
    
    // Actualizamos el steering
    steering.setAngular(angular);
}

Ogre::Real Align::mapToRange(Ogre::Real angle) {
    Ogre::Real pi = Ogre::Math::PI;
    
    if (angle < -pi || angle > pi) {
        Ogre::Real times = abs(floor(angle / pi)) + 1.0f;
        angle = angle / times;
    }
    
    return angle;
}



// VELOCITYMATCH

VelocityMatch::VelocityMatch(Kinematic* character, Kinematic* target): SteeringBehaviour(character) {
    this->target = target;
    maxAcceleration = 0.2f;
    timeToTarget = 0.1f;
}

void VelocityMatch::getSteering(Steering& steering) {
    if (character != 0 && target != 0) {
        // La aceleración trata de llegar a la velocidad del objetivo
        Ogre::Vector3 linear = target->getVelocity() - character->getVelocity();
        linear /= timeToTarget;
        
        // Comprobamos si llevamos demasiada aceleración
        if (linear.length() > maxAcceleration) {
            linear.normalise();
            linear *= maxAcceleration;
        }
        
        // Actualizamos steering
        steering.setLinear(linear);
        steering.setAngular(0.0f);
    }
}



// PURSUE

Pursue::Pursue(Kinematic* character, Kinematic* target): Seek(character) {
    this->target = target;
    maxPrediction = 1.0f;
}

void Pursue::getSteering(Steering& steering) {
    if (character != 0 && target != 0) {
        // 1. Calculamos el objetivo para delegar en seek
        
        // Distancia hacia el objetivo
        Ogre::Vector3 direction = target->getPosition() - character->getPosition();
        Ogre::Real distance = direction.length();
        
        // Recuperamos la velocidad del personaje
        Ogre::Real speed = character->getVelocity().length();
        
        Ogre::Real prediction;
        
        // Comprobar si la velocidad es demasiado pequeña para predecir
        if (speed <= distance / maxPrediction)
            prediction = maxPrediction;
        else
            prediction = distance / speed;
            
        // Creamos un Steering nuevo para Seek
        Kinematic explicitTarget = *target;
        Ogre::Vector3 position = target->getPosition() + target->getVelocity() * prediction;
        Seek::target = &explicitTarget;
        Seek::target->setPosition(position);
        
        // 2. Delegamos en Seek
        Seek::getSteering(steering);
    }
}

Evade::Evade(Kinematic* character, Kinematic* target): Flee(character) {
    this->target = target;
    maxPrediction = 1.0f;
}

void Evade::getSteering(Steering& steering) {
    if (character != 0 && target != 0) {
        // 1. Calculamos el objetivo para delegar en flee
        
        // Distancia hacia el objetivo
        Ogre::Vector3 direction = target->getPosition() - character->getPosition();
        Ogre::Real distance = direction.length();
        
        // Recuperamos la velocidad del personaje
        Ogre::Real speed = character->getVelocity().length();
        
        Ogre::Real prediction;
        
        // Comprobar si la velocidad es demasiado pequeña para predecir
        if (speed <= distance / maxPrediction)
            prediction = maxPrediction;
        else
            prediction = distance / speed;
            
        // Creamos un target nuevo para Seek
        Kinematic explicitTarget = *target;
        Ogre::Vector3 position = target->getPosition() + target->getVelocity() * prediction;
        Flee::target = &explicitTarget;
        Flee::target->setPosition(position);
        
        // 2. Delegamos en Seek
        Flee::getSteering(steering);
    }
}



// FACE

Face::Face(Kinematic* character, Kinematic* target): Align(character) {
    this->target = target;
}

void Face::getSteering(Steering& steering) {
    if (character != 0 && target != 0) {
        // 1. Calculamos el objetivo para delegar en Align
        
        // Dirección del objetivo
        Ogre::Vector3 direction = target->getPosition() - character->getPosition();
        
        // Si la dirección es 0, no hacemos nada
        if (direction.squaredLength() == 0)
            return;
        
        // Creamos un target nuevo para Align
        Kinematic explicitTarget = *target;
        Ogre::Real orientation = atan2(direction.x, direction.z);
        Align::target = &explicitTarget;
        Align::target->setOrientation(orientation);
        
        // 2. Delegamos en Align
        Align::getSteering(steering);
    }
}



// WANDER

Wander::Wander(Kinematic* character): Face(character) {
    wanderOffset = 3.0f;
    wanderRadius = 6.0f;
    wanderRate = 500.0f;
    wanderOrientation = 0.0f;
    maxAcceleration = 10.0f;
}

void Wander::getSteering(Steering& steering) {
    if (character != 0) {
        // 1. Calculamos el objetivo para delegar en Face
        
        // Calculamos la orientación wander
        wanderOrientation = character->getOrientation() + randomBinomial() * wanderRate;
        Ogre::Vector3 wanderDirection = angleToVector(wanderOrientation);
        wanderDirection.normalise();
        
        // Centro del círculo wander
        Ogre::Vector3 characterDirection = angleToVector(character->getOrientation());
        characterDirection.normalise();
        Ogre::Vector3 wanderTarget =  characterDirection * wanderOffset + character->getPosition();
        
        // Punto de la circunferencia
        wanderTarget += wanderDirection * wanderRadius;
        
        // 2. Delegamos en Face
        target = new Kinematic(wanderTarget);
        Face::getSteering(steering);
        
        // 3. Linear al máximo
        
        // Actualizamos steering
        steering.setLinear(characterDirection * maxAcceleration);
        
        
        delete target;
    }
}



// COLLISION AVOIDANCE

CollisionAvoidance::CollisionAvoidance(Kinematic* character, const std::vector<Enemy*>& targets, Enemy* myself): Flee(character) {
    this->targets = targets;
    this->myself = myself;
    maxAcceleration = 2.0f;
    radius = 1.0f;
}

void CollisionAvoidance::getSteering(Steering& steering) {
    
    // Parametros
    Ogre::Real neededSeparation = 0.0f;
    Ogre::Real posDistance = 0.0f;
    Ogre::Real velDistance = 0.0f;
    Ogre::Real separation = 0.0f;
    
    Ogre::Real time = 0.0f;
    Ogre::Real minTime = Ogre::Math::POS_INFINITY;
    
    Ogre::Vector3 posDifference;
    Ogre::Vector3 velDifference;
    
    Kinematic* selTarget = 0;
    Ogre::Real selSeparation = 0.0f;
    Ogre::Real selNeededSeparation = 0.0f;
    Ogre::Vector3 selPosDifference;
    Ogre::Vector3 selVelDifference;
    
    // Para cada enemigo
    std::vector<Enemy*>::iterator i;
    for (i = targets.begin(); i != targets.end(); ++i) {
        if (*i != myself) {
            target = &((*i)->getKinematic());
            
            // 1. Comprobamos que no colisionamos
            
            // Mínima distancia
            neededSeparation = 2 * radius;
            
            // Distancia al objetivo
            posDifference = target->getPosition() - character->getPosition();
            posDistance = posDifference.length();
            
            // Si estamos colisionando, huimos
            if (posDistance <= neededSeparation) {
                Flee::getSteering(steering);
                return;
            }
            
            // 2. Comprobamos si colisionamos en un futuro
            velDifference = target->getVelocity() - character->getVelocity();
            velDistance = velDifference.length();
            time = (posDifference.dotProduct(velDifference)) / (velDistance * velDistance);
            
            if (time > 0.5f)
                continue;
                
            // Solo si tenemos tiempo de colisión y es mínimo
            if (time > 0.0f && time < minTime)
                minTime = time;
            else
                continue;
            
            // Calculamos la separación en ese momento
            separation = posDistance - velDistance * minTime;
            
            // Si es el más corto y va a colisionar lo guardamos
            if (separation <= neededSeparation) {
                selTarget = target;
                selSeparation = separation;
                selNeededSeparation = neededSeparation;
            }
        }
    }

    if (selTarget == 0)
        return;
        
    // Cambiamos la dirección de la velocidad
    
    // 1. Tomamos un punto lo más lejano posible del punto de colisión
    // 2. Cambiamos la velocidad actual para dirigirnos a ese punto
    
    // La nueva posición del personaje al cabo del tiempo
    Ogre::Vector3 charPos = character->getPosition() + character->getVelocity() * minTime;
    Ogre::Vector3 targetPos = selTarget->getPosition() + selTarget->getVelocity() * minTime;
    
    // Dirección desde el objetivo al personaje
    posDifference = charPos - targetPos;
    posDifference.normalise();
    
    // Nueva posición deseada: en la dirección posDifference
    posDifference = charPos + posDifference * (selNeededSeparation - selSeparation);
    
    // Dirección deseada para llegar al punto seguro
    Ogre::Vector3 desiredDir = posDifference - character->getPosition();
    desiredDir.normalise();
    desiredDir = desiredDir * character->getVelocity().length();
    
    // Aceleracion deseada
    steering.setLinear(desiredDir - character->getVelocity());
}



// FOLLOW PATH

FollowPath::FollowPath(Kinematic* character, NavigationMesh::PointPath* path): Arrive(character) {
    this->path = path;
    pathOffset = 0.5f;
    
    // De Arribve
    targetRadius = 0.1f;
    slowRadius = 0.8f;
    maxAcceleration = 7.0f;
}

void FollowPath::getSteering(Steering& steering) {
    // 1. Calculamos el target para delegar en Seek
    target = new Kinematic(findTargetInPath());
    
    // 2. Delegamos en Seek
    Arrive::getSteering(steering);
    
    delete target;
}

Ogre::Vector3 FollowPath::findClosestPathPoint(const Ogre::Vector3& point) {
    Ogre::Real minDistance = Ogre::Math::POS_INFINITY;
    Ogre::Vector3 closestPoint = *(path->begin());
    
    // Recorremos la lista de puntos buscando el más cercano
    for (NavigationMesh::PointPath::iterator i = path->begin(); i != path->end(); ++i) {
        // Calculamos la nueva distancia
        Ogre::Vector3 direction = point - *i;
        
        if (direction.squaredLength() < minDistance) {
            closestPoint = *i;
            minDistance = direction.squaredLength();
        }
    }
    
    return closestPoint;
}

Ogre::Vector3 FollowPath::findTargetInPath() {
    Ogre::Real minDistance = Ogre::Math::POS_INFINITY;
    NavigationMesh::PointPath::iterator closestPointIt = path->begin();
    
    // Recorremos la lista de puntos buscando el más cercano
    for (NavigationMesh::PointPath::iterator i = path->begin(); i != path->end(); ++i) {
        // Calculamos la nueva distancia
        Ogre::Vector3 direction = character->getPosition() - *i;
        
        if (direction.squaredLength() < minDistance) {
            minDistance = direction.squaredLength();
            closestPointIt = i;
        }
    }
    
    NavigationMesh::PointPath::iterator targetIt = closestPointIt;
    ++targetIt;
    
    if (targetIt == path->end())
        return *closestPointIt;
    
    return *targetIt;
}
