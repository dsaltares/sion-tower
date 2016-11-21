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

#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_SONG_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_SONG_H_

#include <SDL/SDL_mixer.h>
#include <OGRE/Ogre.h>


//! Recurso que representa a los ficheros de audio .mp3 u .ogg

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 28-12-2010
 *
 *  La clase Song es un tipo de Recurso utilizado para reproducir y manejar
 *  ficheros de audio tipo .mp3 u .ogg. Para efectos de sonido acude a la
 *  clase Sound.
 *
 *  Para acceder a un recurso de este tipo se utiliza la clase SongManager.
 *  Lo usual es obtener un objeto SongPtr a trav&eacute;s de SongManager.
 */
 
 class Song: public Ogre::Resource {
    public:
        /**
         *  Constructor
         *
         *  @param creator gestor de recursos que crea la m&uacute;sica.
         *  @param name nombre que identifica al recurso.
         *  @param handle identificador que permite un acceso m&aacute;s r&aacute;pido al
         *  recurso.
         *  @param group grupo de recursos al que pertenece el recurso.
         *  @param isManual si su valor es true indica que el recurso est&aacute;
         *  siendo creado mediante un Ogre::ManualResourceLoader
         *  @param loader Ogre::ManualResourceLoader encargado de crear el
         *  recurso.
         *
         *  Crea pero no carga el recurso.
         */
        Song(Ogre::ResourceManager* creator,
             const Ogre::String& name,
             Ogre::ResourceHandle handle,
             const Ogre::String& group,
             bool isManual = false,
             Ogre::ManualResourceLoader* loader = 0);

        /**
         *  Destruye el recurso y libera la memoria ocupada
         */
        ~Song();

        /**
         *  @param loop indica el n&uacute;mero de veces que debe repetirse la
         *  m&uacute;sica. Por defecto es -1 (bucle infinito).
         *
         *  Reproduce la m&uacute;sica y detiene la que est&eacute; actualmente en
         *  reproducci&oacute;n (si hay alguna). Si la m&uacute;sica estaba en pausa, la
         *  retoma por donde estaba.
         */
        void play(int loop = -1);

        /**
         *  Pausa la m&uacute;sica.
         */
        void pause();

        /**
         *  Para la m&uacute;sica. La pr&oacute;xima vez que se reproduzca, comenzar&aacute; desde
         *  el principio.
         */
        void stop();

        /**
         *  @param ms milisegundos que dura el efecto de suavizado
         *  @param loop n&uacute;mero de veces que se reproduce la m&uacute;sica. Por
         *  defecto es -1 (bucle infinito).
         *
         *  Es semejante a Song::play() pero aplica un efecto de suavizado a
         *  la entrada. La m&uacute;sica va aumentando de volumen hasta llegar a su
         *  estado normal.
         */
        void fadeIn(int ms, int loop = -1);

        /**
         *  @param ms milisegundos que dura el efecto de suavizado
         *
         *  Es semejante a Song::stop() pero aplica un efecto de suavizado a
         *  la salida. La m&uacute;sica va disminuyendo su volumen hasta que se para.
         */
        void fadeOut(int ms);

        /**
         *  @return true si existe alguna m&uacute;sica en reproducci&oacute;n, false en caso
         *  contrario.
         */
        static bool isPlaying();
    protected:
        /**
         *  Carga el fichero de audio.
         */
        void loadImpl();

        /**
         *  Libera la memoria ocupada por el audio.
         */
        void unloadImpl();

        /**
         *  @return espacio en memoria que ocupa el objeto.
         */
        size_t calculateSize() const;

    private:
        Mix_Music* _song;
        Ogre::String _path;
        size_t _size;
};

//! Puntero a un recurso Song

/**
 *  @author David Saltares M&aacute;rquez
 *  @date 28-12-2010
 *
 *  Hereda de Ogre::SharedPtr<Song> de forma que permite contabilizar cuantas
 *  referencias existen al recurso. As&iacute;, nos aseguramos un borrado y cargado
 *  eficiente de parte del sistema de gesti&oacute;n de recursos de Ogre.
 */
class SongPtr: public Ogre::SharedPtr<Song> {
    public:
        /**
         *  Constructor predeterminado
         */
        SongPtr(): Ogre::SharedPtr<Song>() {}

        /**
         *  Constructor
         *
         *  @param m recurso m&uacute;sica a partir del cual construir el puntero
         *  inteligente.
         */
        explicit SongPtr(Song* m): Ogre::SharedPtr<Song>(m) {}

        /**
         *  Constructor
         *
         *  @param m recurso m&uacute;sica a partir del cual construir el puntero
         *  inteligente.
         */
        SongPtr(const SongPtr &m): Ogre::SharedPtr<Song>(m) {}

        /**
         *  Constructor
         *
         *  @param r puntero a un recurso g&eacute;nerico
         */
        SongPtr(const Ogre::ResourcePtr &r);

        /**
         *  Operador de asignaci&oacute;n
         *
         *  @param r puntero a un recurso gen&eacute;rico
         *  @return objeto SongPtr resultante.
         */
        SongPtr& operator= (const Ogre::ResourcePtr& r);
};


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_SONG_H_
