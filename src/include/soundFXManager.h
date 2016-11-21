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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_SOUNDFXMANAGER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_SOUNDFXMANAGER_H_

#include <OGRE/Ogre.h>

#include "soundFX.h"

//! Gestor de recursos de tipo Musica

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 29-12-2010
 *
 *  Gestor de recursos de tipo sonido que hereda de Ogre::ResourceManager y
 *  Ogre::Singleton. Se utiliza para integrar el subsistema de audio con el
 *  sistema de gesti&oacute;n de recursos de Ogre.
 *
 *  Utilizaremos esta clase para obtener los objetos SoundFXPtr para reproducir
 *  efectos.
 *
 *  Ejemplo:
 *
 *  \code
 *      // Creamos el ResourceManager
 *      SoundFXManager* soundFXManager = new SoundFXManager(); 
 *
 *      ...
 *
 *      // Cargamos el recurso
 *      SoundFXPtr shoot = soundFXManager->load("disparo.wav", "Personaje");
 *      shoot->play();
 *
 *      ...
 *
 *      // Destruimos el ResourceManager
 *      delete soundFXManager;
 *  \endcode
 */
class SoundFXManager: public Ogre::ResourceManager,
                      public Ogre::Singleton<SoundFXManager> {
    public:
        /**
         *  Constructor predeterminado.
         *
         *  Crea el ResourceManager y lo registra como un nuevo tipo de gestor
         *  de recursos de Ogre.
         */
        SoundFXManager();

        /**
         *  Destructor.
         *
         *  Libera la memoria ocupada por el gesto y lo borra de la lista de
         *  gestores de recursos de Ogre.
         */
        virtual ~SoundFXManager();

        /**
         *  @param name nombre del efecto de sonido.
         *  @param group grupo del efecto de sonido.
         *  @return un shared pointer al recurso indicado listo para utilizar
         *  normalmente 
         */
        virtual SoundFXPtr load(const Ogre::String& name,
                                const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        /**
         *  @return referencia a la &uacute;nica instancia del gestor de recursos de
         *  efectos de sonido.
         */
        static SoundFXManager& getSingleton();

        /**
         *  @return puntero a la &uacute;nica instancia del gestor de recursos de
         *  efectos de sonido.
         */
        static SoundFXManager* getSingletonPtr();

        /**
         *  @return n&uacute;mero de canales disponibles.
         */
        static int getAvailableChannels();
    protected:
        Ogre::Resource* createImpl(const Ogre::String& name,
                                   Ogre::ResourceHandle handle,
                                   const Ogre::String& group,
                                   bool isManual,
                                   Ogre::ManualResourceLoader* loader,
                                   const Ogre::NameValuePairList* createParams);
    
    private:
        static int numChannels;
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_SOUNDFXMANAGER_H_
