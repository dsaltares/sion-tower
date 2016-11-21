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


#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_SPELL_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_SPELL_H_

#include <boost/unordered_map.hpp>
#include <OGRE/Ogre.h>

#include "gameObject.h"
#include "soundFX.h"

//! Clase hija de GameObject que modela los hechizos del juego

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 23-05-2011
 * 
 * Spell hereda de GameObject e incorpora sistemas de partículas de Ogre
 * para representar hechizos y sus explosiones. Así mismo, incluyen efectos
 * de sonido para el lanzamiento y el impacto.
 */

class Spell: public GameObject {
    public:
        
        /**
         * Tipo de hechizo, según el tipo tiene un sistema de partículas
         * asociado y ciertas propiedades personalizadas.
         */
        enum Type {
            FIRE,
            GEA,
            BLIZZARD
        };
        
        /**
         * Estado en el que se encuentra el hechizo.
         */
        enum State {
            MOVE,
            EXPLODE,
            ERASE
        };
        
        
        //! Propiedades de cada tipo de hechizo
        struct SpellData {
            Ogre::String name;
            Ogre::String description;
            int power;
            int mana;
            Ogre::Real speed;
            Ogre::Real explosionTime;
            Ogre::String particleMove;
            Ogre::String particleExplode;
            Ogre::String soundCast;
            Ogre::String soundExplode;
        };
        
        /**
         * Constructor
         * 
         * @param sceneManager gestor de la escena
         * @param type tipo de hechizo que deseamos crear
         * @param position posición inicial del hechizo
         * @parma direction dirección hacia la que se moverá el hechizo
         */
        Spell(Ogre::SceneManager* sceneManager,
              Type type,
              const Ogre::Vector3& position,
              const Ogre::Vector3& direction);
              
        /**
         * Destructor
         * 
         * Libera la memoria ocupada por el hechizo
         */
        ~Spell();
        
        /**
         * Inicializa los datos de los distintos tipos de hechizo. Es necesario
         * llamar a este método durante la inicialización del sistema.
         */
        static void initialiseSpellData();
        
        /**
         * @return tipo de objeto de juego, en este caso GameObject::SPELL
         */
        GameObject::Type getType() const;
        
        /**
         * @return tipo de hechizo
         */
        Type getSpellType() const;
        
        /**
         * @return estado actual
         */
        State getState() const;
        
        /**
         * @return daño que inflinge el hechizo
         */
        int getPower() const;
        
        /**
         * @return Maná necesario
         */
        int getMana() const;
        
        /**
         * @param type tipo de hechizo
         * @return Maná necesario según el tipo de hechizo
         */
        static int getMana(Type type);
        
        /**
         * @return velocidad a la que viaja el hechizo
         */
        Ogre::Real getSpeed() const;
        
        /**
         * @return dirección que lleva el hechizo
         */
        const Ogre::Vector3& getDirection() const;
        
        /**
         * @return nombre del hechizo traducido
         */
        const Ogre::String& getName() const;
        
        /**
         * @return descripción del hechizo traducida
         */
        const Ogre::String& getDescription() const;
        
        static SpellData getSpellData(Type type);
        
        /**
         * El hechizo ha impactado y debe iniciar su secuencia de explosión y destrucción
         */
        void explode();
        
        /**
         * @param deltaT diferencia de tiempo en ms desde el último frame
         * 
         * Actualiza el hechizo según su estado
         */
        virtual void update(Ogre::Real deltaT);
    
    private:
        Type _type;
        State _state;
        Ogre::ParticleSystem* _particleMove;
        Ogre::ParticleSystem* _particleExplode;
        Ogre::Vector3 _direction;
        Ogre::Timer* _explosionTimer;
        SoundFXPtr _soundExplode;
        SoundFXPtr _soundCast;
        
        SpellData _spellData;
        
        static int _particleCounter;
        
        static boost::unordered_map<Type, SpellData> _spells;
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_SPELL_H_
