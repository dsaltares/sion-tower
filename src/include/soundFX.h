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
#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_SOUNDFX_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_SOUNDFX_H_

#include <SDL/SDL_mixer.h>
#include <OGRE/Ogre.h>

//! Recurso que representa a los ficheros con efectos de sonido .wav

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 29-12-2010
 *
 *  La clase SoundFX es un tipo de Recurso utilizado para reproducir efectos de
 *  sonido el formato .wav. Para pistas de audio largas es recomendable
 *  utilizar la clase Musica.
 *
 *  Para acceder a un recurso de este tipo se utiliza la clase SoundFXManager.
 *  Lo usual es obtener un objeto de tipo SoundFXPtr a trav&eacute;s de SoundFXManager.
 */
class SoundFX: public Ogre::Resource {
    public:
        /**
         *  Constructor
         *  
         *  @param creator gestor de recursos que crea el sonido.
         *  @param name nombre que identifica al recurso.
         *  @param handle identificador para obtener el recurso de forma m&aacute;s
         *  eficiente.
         *  @param group nombre del grupo al que pertenece el sonido.
         *  @param isManual si su valor es tru indica que el recurso est&aacute;
         *  siendo cargado mediante un Ogre::ManualResourceLoader.
         *  @param loader Ogre::ManualResourceLoader encargado de crear el
         *  recurso. Solo tiene sentido si isManual vale true.
         *
         *  Crea pero no carga el recurso.
         */
        SoundFX(Ogre::ResourceManager* creator,
               const Ogre::String& name,
               Ogre::ResourceHandle handle,
               const Ogre::String& group,
               bool isManual = false,
               Ogre::ManualResourceLoader* loader = 0);

        /**
         *  Destruye el recurso y libera la memoria ocupada.
         */
        ~SoundFX();

        /**
         *  @param loop n&uacute;mero de veces que se repite el sonido. Su valor por
         *  defecto es 0, s&oacute;lo se reproduce una vez. Si vale -1 se reproducir&aacute;
         *  infinitas veces. Para valores n, se reproduce n+1 veces.
         *  @return canal en el que se est&aacute; reproduciendo el efecto.
         *
         *  Reproduce el efecto de sonido
         */
        int play(int loop = 0);
        
    protected:
        /**
         *  Carga el efecto de sonido
         */
        void loadImpl();

        /**
         *  Libera la memoria ocupada por el efecto de sonido
         */
        void unloadImpl();

        /**
         *  @return espacio en memoria que ocupa el objeto
         */
        size_t calculateSize() const;
    private:
        Mix_Chunk* _sound;
        Ogre::String _path;
        size_t _size;
};


//! Puntero a un recurso SoundFX

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 29-12-2010
 *
 *  Hereda de Ogre::SharedPointer<SoundFX> de forma que permite contabilizar
 *  cuantas referencias existen al recurso. As&iacute;, nos aseguramos un borrado y
 *  cargado eficiente de parte del sistema de gesti&oacute;n de recursos de Ogre.
 */
class SoundFXPtr: public Ogre::SharedPtr<SoundFX> {
    public:
        /**
         *  Constructor predeterminado
         */
        SoundFXPtr(): Ogre::SharedPtr<SoundFX>() {}

        /**
         *  Constructor
         *
         *  @param s puntero al recurso sonido a partir del cual construir el puntero
         *  inteligente.
         */
        explicit SoundFXPtr(SoundFX* s): Ogre::SharedPtr<SoundFX>(s) {}
        
        /**
         *  Constructor
         *
         *  @param s referencia al recurso sonido a partir del cual construir el puntero
         *  inteligente.
         */
        SoundFXPtr(const SoundFXPtr& s): Ogre::SharedPtr<SoundFX>(s) {}

        /**
         *  Constructor
         *
         *  @param r referencia a un recurso gen&eacute;rico a partir del cual se
         *  construye el puntero inteligente.
         */
        SoundFXPtr(const Ogre::ResourcePtr& r);

        /**
         *  Operador de asignaci&oacute;n.
         *
         *  @param r puntero a un recurso gen&eacute;rico
         *  @return objeto SoundFXPtr resultante
         */
        SoundFXPtr& operator= (const Ogre::ResourcePtr& r);
};

#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_SOUNDFX_H_
