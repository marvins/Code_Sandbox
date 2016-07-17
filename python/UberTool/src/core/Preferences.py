#    File:    Preferences.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#
#    Purpose:  Store preferences for the GUI
#  

#  Python Libraries
import os, argparse, ConfigParser, datetime, sys

#  UberTool Libraries
from core.AddOnLoader import *
from core.Preference_Utilities import *
from core.SysLogger import SysLogger


#  Preferences Class
class Preferences:


    #  Constructor
    def __init__( self ):

        #  Log Entry
        SysLogger.Log('Loading the Configuration.')

        #  Set the UberTool Root Path
        self.ubertool_root = Get_UberTool_Root()


        #  Set Default Parameters
        self.Set_Default_Parameters()


        #  Parse Command-Line Arguments
        self.Parse_Command_Line()


        #  Parse Configuration File
        self.Parse_Configuration_File()


        #  Load Plugins
        self.Load_Plugin_Configurations()


        #  Write Configuration if Requested
        if self.Query('CORE','WRITE_CONFIGURATION','False') == 'True':
            SysLogger.Log('Writing Configuration')
            self.Write_Configuration_File()

        #  Log Exit
        SysLogger.Log('Finish loading configuration')


    def Set_Default_Parameters(self):

        #  Initialize Containers
        self.options = Initialize_Default_Options(self.ubertool_root)

        #  Initialize Plugin Options
        self.plugin_options = {}


    def Parse_Command_Line(self):

        #  Create argument parser
        parser = argparse.ArgumentParser(description='UberTool Application')

        #  Specify unique configuration file
        parser.add_argument('-c','--config',
                            dest='config_pathname',
                            required=False,
                            default=self.Query('CORE','DEFAULT_CONFIG_PATHNAME'),
                            help='Override the default configuration file.')

        #  Force writing the configuration path
        parser.add_argument('-w','--write-config',
                            dest='write_config',
                            required=False,
                            action='store_true',
                            default=False,
                            help='Write configuration file regardless if exists.  Will overwrite.')


        #  Return Parser
        self.cmd_parser = parser.parse_args()

        #  Check options
        if self.cmd_parser.write_config == True:
            self.options['CORE']['WRITE_CONFIGURATION'][0] = 'True'

    def Parse_Configuration_File(self):

        #  Create Config File Parser
        self.cfg_parser = ConfigParser.ConfigParser()

        #  Check if config file already exists
        config_pathname = self.Query('CORE','DEFAULT_CONFIG_PATHNAME')

        if not self.cmd_parser.config_pathname is None:
            config_pathname = self.cmd_parser.config_pathname

        print('Loading: ' + str(config_pathname))

        if os.path.exists(config_pathname) == False:
            self.options['CORE']['WRITE_CONFIGURATION'][0] = 'True'
            return


        #  Otherwise, load
        self.cfg_parser.read(config_pathname)


        #  Get key list
        key_list = self.cfg_parser.sections()
        if 'plugins' in key_list:
            key_list.remove('plugins')

        #  Iterate over keys
        for key in key_list:

            #  Grab the items
            item_list = self.cfg_parser.items(key)

            #  Iterate over items
            for item in item_list:

                # Grab the value
                self.Set(key, item[0], item[1])


    def Write_Configuration_File(self):

        #  Determine output path
        output_pathname = self.Query('CORE','DEFAULT_CONFIG_PATHNAME')

        if not self.cmd_parser.config_pathname is None:
            output_pathname = self.cmd_parser.config_pathname


        #  Check if main directory exists
        output_dir = os.path.dirname(output_pathname)
        if os.path.exists(output_dir) is False:
            os.makedirs(output_dir)

        #  Open the output file
        with open(output_pathname, 'w') as fout:

            #  Construct Date String
            date_string = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


            #  Write Header
            fout.write('#  UberTool Configuration File\n')
            fout.write('#  Date: ' + date_string + '\n\n')

            #  Iterate over modules
            keys = self.QueryModules()
            for module in keys:

                #  Write the header
                fout.write('[' + module + ']\n')

                #  Get list of items
                mod_items = self.QueryModuleItems(module)
                for item in mod_items:

                    #  Fetch the info
                    pair = self.QueryFull(module, item)

                    fout.write('# ' + pair[1] + '\n')
                    fout.write(item + '=' + pair[0] + '\n\n')

            #  Iterate over plugin options
            fout.write('[plugins]\n')


    def Load_Plugin_Configurations(self):

        #  Load the Default Config Options
        results = AddOnLoader.Load_Default_Config_Options(self)

        for key in results.keys():

            #  Check if key exists in plugin options
            if key in self.plugin_options.keys():
                self.plugin_options[key][0] = results[key][0]
            else:
                self.plugin_options[key] = results[key]

    #  Retrieve a setting
    def QueryFull(self, module, name, default = None):

        #  Check if module exists
        if not module in self.options.keys():
            print('Warning: ' + module + ' not found in options.')
            return [default,'']

        #  Check if name exists
        if not name in self.options[module].keys():
            print('Warning: ' + name + ' not found in module.')
            return [default,'']

        output = (self.options[module][name][0],self.options[module][name][1])
        return output

    #  Retrieve a setting
    def Query(self, module, name, default = None):
        return self.QueryFull(module, name, default)[0]

    def QueryModules(self):
        return self.options.keys()

    def QueryModuleItems(self, module):

        if not module in self.options.keys():
            return []
        return self.options[module].keys()

    #  Retrieve a setting
    def QueryPluginFull(self, name, default = None):

        #  Check if name exists
        if not name in self.plugin_options.keys():
            print('Warning: ' + name + ' not found in plugin module.')
            return [default,'']

        output = (self.plugin_options[name][0],self.plugin_options[name][1])
        return output

    #  Retrieve a setting
    def QueryPlugin(self, name, default = None):
        return self.QueryPluginFull(name, default)[0]

    def QueryPluginItems(self):
        return self.plugin_options.keys()

    def Set(self, module, name, value, comment = '',
            create_module_if_missing = False,
            create_name_if_missing = False):

        #  Check if module exists
        if not module in self.options.keys():
            if create_module_if_missing == True:
                self.options[module] = {}
            else:
                return

        #  Check if name exists
        if not name in self.options[module].keys():
            if create_name_if_missing == True:
                self.options[module][name] = [value, comment]
            else:
                return

        self.options[module][name][0] = value
        if not comment == '':
            self.options[module][name][1] = comment
