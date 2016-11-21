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
 *  @file main.cpp
 *  @author David Saltares Márquez
 *  @date 26-11-2010
 */

#include <iostream>
#include <libintl.h>
#include <locale.h>

#include "game.h"


using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char **argv) {
    // Gettext
    bind_textdomain_codeset("siontower", "UTF-8");
    setlocale(LC_MESSAGES, "");
    bindtextdomain("siontower", "lang" );
    textdomain("siontower");
    
    Game* sionTower = new Game();
    
    sionTower->start();
    
    delete sionTower;

    return 0;
}

/**
 *  @mainpage Sion Tower v1.0
 *
 *  <p style="text-align: center;"><img src=siontower.jpg width=550px></p>
 *
 *  <h2>Información</h2>
 *
 *  <ul>
 *      <li><a href="http://cusl5-iberogre.forja.rediris.es" target="_blank">Web oficial</a></li>
 *      <li><a href="http://siondream.com/blog/category/proyectos/pfc" target="_blank">Blog de desarrollo</a></li>
 *      <li><a href="https://forja.rediris.es/projects/cusl5-iberogre" target="_blank">Forja Red Iris</a></li>
 *      <li><a href="http://twitter.com/iberogre" target="_blank">Twitter</a></li>
 *      <li><a href="http://www.youtube.com/user/davidsaltares" target="_blank">Canal de Youtube</a></li>
 *  </ul>
 *
 *  <h2>¿Qué es Sion Tower?</h2>
 *
 *  <p style="text-align: justify;">
 *  Sion Tower es un videojuego de estrategia y acción en el que un joven mago
 *  se ve obligado a defender la Torre Sagrada de una invasión enemiga. Cada
 *  nivel es un piso de la torre, nuestra misión es impedir que los adversarios
 *  sigan avanzando y roben la reliquia sagrada. Para ello tendremos que acabar
 *  con ellos lanzando hechizos administrando el maná disponible.
 *  </p>
 *
 *  <p style="text-align: justify;">
 *  Sion Tower forma parte del proyecto <a href="http://osl2.uca.es/iberogre"
 *  target="_blank"><b>IberOgre</b></a>, la wiki de Ogre en español. El
 *  videojuego ejemplifica y amplía todos los contenidos de sus artículos.
 *  En IberOgre encontrarás artículos en español sobre los distintos sistemas
 *  de Ogre y una traducción completa del manual de referencia oficial.
 *  </p>
 *
 *  <h2>Autores</h2>
 *
 *  <ul>
 *      <li>
 *          <strong><a href="http://siondream.com" target="_blank">David Saltares Márquez</a></strong>:
 *          <ul>
 *              <li>Diseño del juego</li>
 *              <li>Implementación</li>
 *              <li>Diseño de niveles</li>
 *              <li>Arte 2D y 3D de escenarios</li>
 *          </ul>
 *      </li>
 *      <li>
 *          <strong><a href="http://ajr-portafolio.blogspot.com" target="_blank">Antonio Jiménez Rodríguez (AJR)</a></strong>:
 *          <ul>
 *              <li>Diseño de personajes</li>
 *              <li>Arte 3D de personajes</li>
 *              <li>Animación</li>
 *          </ul>
 *      </li>
 *      <li>
 *          <strong><a href="http://estudioevergreen.wordpress.com/" target="_blank">Estudio Evergreen</a></strong>: Antonio Caro Oca y Daniel Pellicer García
 *          <ul>
 *              <li>Banda sonora original</li>
 *          </ul>
 *      </li>
 *      <li>
 *          Francisco Martín Márquez:
 *          <ul>
 *              <li>Efectos de sonido</li>
 *          </ul>
 *      </li>
 *      <li>
 *          <strong><a href="http://blendershop.blogspot.com" target="blank">Daniel Belohlavek</a></strong>:
 *          <ul>
 *              <li>Iconos 2D para los hechizos</li>
 *          </ul>
 *      </li>
 *  </ul>
 *
 *  <h2>Licencia</h2>
 *
 *  <ul>
 *      <li><b>Código fuente</b>: GPL v3, para más detalles ver fichero LICENSE distribuido con
 *      el juego</li>
 *      <li><b>Ficheros multimedia</b>: Creative Commons 3.0 Atribución -
 *      No comercial - Compartir igual. Para más detalles ver el fichero
 *      MEDIA-LICENSE distribuido con el juego.</li>
 *  </ul>
 *
 */
