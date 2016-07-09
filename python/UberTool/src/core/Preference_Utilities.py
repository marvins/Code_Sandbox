#    File:    Preference_Utilities.py
#    Author:  Marvin Smith
#    Date:    7/9/2016
#
#    Purpose: Contains helper functions for the preference module.
#

#  Python Libraries
import os

def Get_UberTool_Root():

    #  Get this file path
    root = os.path.dirname(__file__) + '/..'

    return root

def Initialize_Default_Options(ubertool_root):
    '''
    Initialize Default Configuration Options for UberTool
    '''

    options = {}

    #  Set CORE Default Values
    options['CORE'] = {}
    options['CORE']['DEFAULT_CONFIG_PATHNAME'] = [os.environ['HOME'] + '/.ubertool/options.cfg', 'Path to default config file.']
    options['CORE']['MODULE_PATH']             = [ubertool_root + '/plugins/module-list.txt', 'Path to module list.']
    options['CORE']['ICON_HOME']               = [ubertool_root + '/icons', 'Path to icons.']
    options['CORE']['WRITE_CONFIGURATION']     = ['False', 'Flag to write config each time you restart.']


    #  Set GUI Default Values
    options['GUI'] = {}
    options['GUI']['MAIN_WINDOW_TITLE']          = ['UberTool', 'Main GUI Window Name.']
    options['GUI']['MainWindowButtonWidth']      = ['100', 'Width of Main Window Buttons']
    options['GUI']['MainWindowButtonHeight']     = ['100', 'Height of Main Window Buttons']
    options['GUI']['MainWindowButtonIconWidth']  = ['70', 'Width of Icons on the Main Window.']
    options['GUI']['MainWindowButtonIconHeight'] = ['70', 'Height of Icons on the Main Window.']
    options['GUI']['MainWindowButtonsPerRow']    = ['3', 'Number of Buttons per Main Window Row']


    return options