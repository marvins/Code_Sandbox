#    File:    Preferences.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose:  Store preferences for the GUI
#  

#  Python Libraries
import os, argparse, ConfigParser

#  UberTool Libraries
from core.Preference_Utilities import *


#  Preferences Class
class Preferences:


    #  Constructor
    def __init__( self ):


        #  Set the UberTool Root Path
        self.ubertool_root = Get_UberTool_Root()


        #  Set Default Parameters
        self.Set_Default_Parameters()


        #  Parse Command-Line Arguments
        self.cmd_parser = self.Parse_Command_Line()


        #  Parse Configuration File
        self.cfg_parser = self.Parse_Configuration_File()


        #  Load Plugins
        self.Load_Plugin_Configurations()


        #  Write Configuration if Requested
        if self.Query('CORE','WRITE_CONFIGURATION') == 'True':
            self.Write_Configuration_File()



    def Set_Default_Parameters(self):

        #  Initialize Containers
        self.options = Initialize_Default_Options(self.ubertool_root)

        #  Initialize Plugin Options
        self.plugin_options = {}


    def Parse_Command_Line(self):

        #  Create argument parser
        parser = argparse.ArgumentParser(description='UberTool Application')


        #  Return Parser
        return parser

    def Parse_Configuration_File(self):

        pass



    def Write_Configuration_File(self):

        pass

    def Load_Plugin_Configurations(self):

        pass

    #  Retrieve a setting
    def Query(self, module, name, default = None):

        #  Check if module exists
        if not module in self.options.keys():
            print('Warning: ' + module + ' not found in options.')
            return default

        #  Check if name exists
        if not name in self.options[module].keys():
            print('Warning: ' + name + ' not found in module.')
            return default

        return self.options[module][name][0]


