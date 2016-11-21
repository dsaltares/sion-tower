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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_SONGMANAGER_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_SONGMANAGER_H_

#include <OGRE/Ogre.h>

#include "song.h"

//! Gestor de recursos de tipo Song

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 29-12-2010
 *
 *  Gestor de recursos de tipo m&uacute;sica que hereda de Ogre::ResourceManager y
 *  Ogre::Singleton. Se utiliza para integrar el subsistema de audio con el
 *  sistema de gesti&oacute;n de recursos de Ogre.
 *
 *  Utilizaremos esta clase para obtener los objetos SongPtr para reproducir
 *  m&uacute;sica.
 *
 *  Ejemplo:
 *
 *  \code
 *      // Creamos el ResourceManager
 *      SongManager* songManager = new SongManager(); 
 *
 *      ...
 *
 *      // Cargamos el recurso
 *      SongPtr levelMusic = songManager->load("musicaNivel1.ogg", "Nivel1");
 *      levelMusic->play();
 *
 *      ...
 *
 *      // Destruimos el ResourceManager
 *      delete songManager;
 *  \endcode
 */
class SongManager: public Ogre::ResourceManager,
                     public Ogre::Singleton<SongManager> {
    public:
        /**
         *  Constructor predeterminado
         *
         *  Crea el ResourceManager y lo registra como un nuevo tipo de gestor
         *  de recursos de Ogre.
         */
        SongManager();

        /**
         *  Destructor
         *
         *  Libera la memoria del ResourceManager y lo borra de la lista de
         *  gestores de recursos de Ogre.
         */
        virtual ~SongManager();

        /**
         *  @param name nombre del recurso Song.
         *  @group grupo del recurso. Si no se indica, se toma el grupo por
         *  defecto (General).
         *  @return  un shared pointer al recurso indicado listo para utilizar
         *  normalmente.
         */
        virtual SongPtr load(const Ogre::String& name, const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        /**
         *  @return referencia a la &uacute;nica instancia del gestor de recursos de
         *  m&uacute;sica.
         */
        static SongManager& getSingleton();

        /**
         *  @return puntero a la &uacute;nica instancia del gestor de recursos de
         *  m&uacute;sica.
         */
        static SongManager* getSingletonPtr();

    protected:
        /**
         *  @param name nombre del recurso
         *  @param handle identificador &uacute;nico del recurso.
         *  @param group grupo del recurso.
         *  @param isManual true si el recurso se est&aacute; cargando de forma
         *  manual.
         *  @param loader en el caso de que se est&eacute; cargando el recurso de
         *  forma manual, este es el cargador.
         *  @param createParams par&aacute;metros para configurar la creaci&oacute;n del
         *  recurso (no se utiliza)
         */
        Ogre::Resource* createImpl(const Ogre::String& name,
                                   Ogre::ResourceHandle handle,
                                   const Ogre::String& group,
                                   bool isManual,
                                   Ogre::ManualResourceLoader* loader,
                                   const Ogre::NameValuePairList* createParams);
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_SONGMANAGER_H_
