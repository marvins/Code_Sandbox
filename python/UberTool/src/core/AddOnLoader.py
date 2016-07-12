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
    def __init__(self, options):

        #  Open the module configuration file
        modules = open(options.Query('CORE','MODULE_PATH'),'r')

        #  Grab the base path
        plugin_path = options.Query('CORE','MODULE_BASE')

        #  Read Each List
        for line in modules:
            if not line.strip() == '':

                #  Split up the module name and the class name
                components = line.strip().split(':')

                mod_path = plugin_path + '/' + components[0]
                py_mod = imp.load_source(components[1], mod_path)

                class_inst = getattr(py_mod, components[1])
                self.plugins += [class_inst(options)]

    @staticmethod
    def Load_Default_Config_Options(options):

        #  Create Output
        output = {}

        #  Open the module configuration file
        modules = open(options.Query('CORE','MODULE_PATH'),'r')

        #  Grab the base path
        plugin_path = options.Query('CORE','MODULE_BASE')

        #  Read Each List
        for line in modules:
            if not line.strip() == '':

                #  Compute paths
                components = line.strip().split(':')

                mod_path = plugin_path + '/' + components[0]
                py_mod = imp.load_source(components[1], mod_path)

                #  Call Static Method
                class_inst = getattr(py_mod, components[1])
                options = class_inst.Get_Default_Configuration_Options()

                if len(options.keys()) > 0:
                    output.update(options)

        return output