#! /usr/bin/env python
import xml.etree.ElementTree as ET 
from xml.etree.ElementTree import Element, SubElement

spacer = '   '

# Open an existing xml file
tree = ET.parse("books.xml")

# get root
root = tree.getroot()

print root

for child in root.getchildren():
	print spacer + str(child)
	print spacer + spacer     + str(child.attrib.get('id'))
	

# Create a new xml file




