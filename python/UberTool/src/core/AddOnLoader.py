#    File:    AddOnLoader.py
#    Author:  Marvin Smith
#    Date:    4/20/2014
#
#    Purpose:  Load all plugins in the add-on directory
#

#  Python Libraries
import imp, sys, os

#  Load Plugin Base
import plugins.PluginBase

#   Add-On Loader
class AddOnLoader:

    plugins = []

    #  Constructor
    def __init__(self, preferences, configurationFile):

        #  Open the module configuration file
        modules = open(configurationFile,'r')

        #  Grab the base path
        plugin_path = os.path.dirname(__file__) + '/../plugins/'

        #  Read Each List
        for line in modules:
            if not line.strip() == '':

                #  Split up the module name and the class name
                components = line.strip().split(':')

                mod_path = plugin_path + components[0]
                cls_path = plugin_path + components[1]
                py_mod = imp.load_source(components[1], mod_path)

                class_inst = getattr(py_mod, components[1])
                self.plugins += [class_inst(preferences)]
