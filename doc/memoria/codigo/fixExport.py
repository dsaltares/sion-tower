#!/usr/bin/env python
# -*- coding: utf-8  -*-

########################################################################
# This file is part of Sion Tower.                                     #
#                                                                      #
# This program is free software: you can redistribute it and/or modify #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# any later version.                                                   #
#                                                                      #
# This program is distributed in the hope that it will be useful,      #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with this program.  If not, see <http://www.gnu.org/licenses/>.#
#                                                                      #
# Copyright (C) 2011, David Saltares Marquez, david.saltares@gmail.com #
########################################################################

import os
import sys

from xml.dom import minidom
from xml.etree.ElementTree import ElementTree

# Ruta al OgreXMLConverter
converter = '/usr/local/bin/OgreXMLConverter'

def sintaxis():
    print 'Syntax error, the correct syntax is:'
    print '    python fixExport.py [mesh|skeleton] originXML destinyXML 
               scaleFactor [destinyBin]'
    print ''

def processMesh(root, scale):
    print '* Processing XML mesh...'

    vertices = root.findall('submeshes/submesh/geometry/vertexbuffer/vertex')
 
    for vertex in vertices:
        position = vertex.find('position')
        
        # Convertimos x
        x = float(position.attrib['x'])
        x = x * scale
        position.attrib['x'] = str(x)
        
        # Convertimos y
        y = float(position.attrib['y'])
        y = y * scale
        position.attrib['y'] = str(y)
        
        # Convertimos z
        z = float(position.attrib['z'])
        z = z * scale
        position.attrib['z'] = str(z)
        
    

def processSkeleton(root, scale):
    print '* Processing XML skeleton...'
    
    positions = root.findall('bones/bone/position')
    
    for position in positions:
        # Convertimos x
        x = float(position.attrib['x'])
        x = x * scale
        position.attrib['x'] = str(x)
        
        # Convertimos y
        y = float(position.attrib['y'])
        y = y * scale
        position.attrib['y'] = str(y)
        
        # Convertimos z
        z = float(position.attrib['z'])
        z = z * scale
        position.attrib['z'] = str(z)
    
    translations = root.findall('animations/animation/tracks/track/
                                 keyframes/keyframe/translate')
    
    for translation in translations:
        # Convertimos x
        x = float(translation.attrib['x'])
        x = x * scale
        translation.attrib['x'] = str(x)
        
        # Convertimos y
        y = float(translation.attrib['y'])
        y = y * scale
        translation.attrib['y'] = str(y)
        
        # Convertimos z
        z = float(translation.attrib['z'])
        z = z * scale
        translation.attrib['z'] = str(z)

def main():
    # Cabecera
    print ''
    print 'CINEMA4D - OGRE EXPORT SCALE FIX'
    print '================================'
    print ''
    
    # Comprobamos argumentos
    if len(sys.argv) < 5:
        sintaxis()
        sys.exit(1)
    
    # Cargamos documento XML
    print '* Loading XML document...'
    #doc = minidom.parse(sys.argv[2])
    doc = ElementTree()
    doc.parse(sys.argv[2])
    root = doc.getroot()
    
    # Procesamos XML
    print '* Checking document type...'
    if sys.argv[1] == 'mesh':
        processMesh(doc, float(sys.argv[4]))
    elif sys.argv[1] == 'skeleton':
        processSkeleton(doc, float(sys.argv[4]))
    else:
        print 'Unknown document type'
        print ''
        sys.exit(1)
    
    # Guardamos XML
    print '* Saving XML document...'
    doc.write(sys.argv[3])
    
    # Convertimos XML si definimos OgreXMLConverter
    if converter != '' and len(sys.argv) == 6:
        print '* Converting XML document to binary...'
        command = converter + ' ' + sys.argv[3] + ' ' + sys.argv[5]
        os.system(command)


if __name__ == "__main__":
	main()
