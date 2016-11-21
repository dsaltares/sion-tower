#!/usr/bin/env python
# -*- coding: utf-8  -*-

###############################################################################
# This file is part of Sion Tower.                                            #
#                                                                             #
# This program is free software: you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation, either version 3 of the License, or           #
# any later version.                                                          #
#                                                                             #
# This program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License           #
# along with this program.  If not, see <http://www.gnu.org/licenses/>.       #
#                                                                             #
# Copyright (C) 2011, David Saltares Márquez, <david.saltares@gmail.com>      #
###############################################################################

import os
import sys

from xml.dom import minidom
from xml.etree.ElementTree import ElementTree

def syntax():
    print 'Syntax error, the correct syntax is:'
    print '    python translateLayout.py layoutsDir file.pot'
    print ''

def translateLayout(layoutFile, potFile, codes):
    # Load XML Document
    print '    * Loading XML document...'
    doc = ElementTree()
    
    #try:
    doc.parse(layoutFile)
    #except IOError:
    #    print '    * ERROR parsing MyGUI layout'
    #    exit(1)

    root = doc.getroot()
    
    # Retrieve every "Property" elements
    print '    * Procesing properties...'
    properties = root.findall('.//Widget/Property')
    
    # Open pot file
    try:
        f = open(potFile, 'w')
    except IOError:
        print '    * ERROR opening pot file'
        exit(1)
    
    # Iterate through every Property
    for p in properties:
        # Check if it has a Caption property
        key = p.get('key')
        if key == 'Caption':
            # Get Caption
            value = p.get('value')
            if not value.isdigit() and not value == '' and not value in codes:
                print '        - Caption found: ' + value
                
                # Write caption into pot file
                f.write('msgid "' + value.encode('utf-8') + '"\n')
                f.write('msgstr ""\n\n')
                codes.append(value)
            
            
    # Save file
    print '    * Saving .pot file...'
    f.close()
    

def main():
    # Header
    print ''
    print 'MyGUI Layout - POT converter'
    print '============================'
    print ''
    
    # Check arguments
    if len(sys.argv) < 3:
        syntax()
        sys.exit(1)
    
    # Iterate through layout dirs converting every .layout file
    dirName = sys.argv[1]
    potFiles = []
    codes = []
    
    for f in os.listdir(dirName):
        if os.path.isfile(os.path.join(dirName, f)) and f.endswith('.layout'):
            baseName = f[:-7]
            potFile = baseName + '.pot'
            print '* Converting ' + f + '...'
            translateLayout(os.path.join(dirName, f), os.path.join(dirName, potFile), codes)
            potFiles.append(os.path.join(dirName, potFile))
    
    # Merge all pot files together
    print '* Merging pot files together...'
    command = 'cat ' + " ".join(potFiles) + ' > ' + sys.argv[2]
    os.system(command)
    
    # Delete all temporary pot files
    print '* Deleting temporary POT files...'
    #os.system('rm ' + " ".join(potFiles))
    
    print ''


if __name__ == "__main__":
	main()
