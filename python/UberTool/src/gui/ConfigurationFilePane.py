#    File:    ConfigurationFilePane.py
#    Author:  Marvin Smith
#    Date:    7/12/2016
#
#    Purpose:  Display the Configuration File Widget
#


#   PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *


#  Configuration File Pane
class ConfigurationFilePane(QtGui.QWidget):

    #  Close panel signal
    close_panel = pyqtSignal()

    def __init__(self, options, parent = None):

        #  Build Parent
        QtGui.QWidget.__init__(self, parent)

        #  Set config options
        self.m_options = options

        #  Set Preferences
        self.m_options = options

        #  Set title
        self.setWindowTitle('Config Editor')

        #  Set Defaults
        self.m_tree_labels = ['Module','Name','Value','Description']

        # Initialize the GUI
        self.Initialize_UI()

        #  Update the GUI
        self.Update_Configuration_Tree()


    def Initialize_UI(self):

        #  Create the main widget
        self.m_main_layout = QtGui.QVBoxLayout()


        #  Create Configuration Panel
        self.Build_Configuration_Panel()

        #  Create Toolbar
        self.Build_Toolbar()

        #  Set the layout
        self.setLayout(self.m_main_layout)

    def Build_Configuration_Panel(self):

        #  Create configuration widget
        self.m_config_widget = QtGui.QGroupBox('Configuration Settings',self)

        # Create layout
        self.m_config_layout = QtGui.QHBoxLayout()

        #  Create Tree Widget
        self.m_config_tree = QtGui.QTreeWidget()
        self.m_config_tree.setColumnCount(len(self.m_tree_labels))
        self.m_config_layout.addWidget( self.m_config_tree )

        #  Create Panel Toolbar
        self.Build_Configuration_Panel_Toolbar()

        #  Set the config layout
        self.m_config_widget.setLayout(self.m_config_layout)

        #  Add to main layout
        self.m_main_layout.addWidget(self.m_config_widget)


    def Build_Configuration_Panel_Toolbar(self):

        #  Create Panel Button Widget
        self.m_config_button_panel = QtGui.QWidget()

        #  Create Panel Button Layout
        self.m_config_button_layout = QtGui.QVBoxLayout()

        #  Get Icon Base Path
        icon_base = self.m_options.Query('CORE','ICON_HOME')
        add_item_icon_path  = icon_base + '/add.png'
        rm_item_icon_path   = icon_base + '/trashcan.png'
        edit_item_icon_path = icon_base + '/pencil.png'

        #  Create Add Button
        self.m_config_add_button  = self.Create_Button(icon_path=add_item_icon_path)
        self.m_config_button_layout.addWidget(self.m_config_add_button)

        #  Create RM Button
        self.m_config_rm_button   = self.Create_Button(icon_path=rm_item_icon_path)
        self.m_config_button_layout.addWidget(self.m_config_rm_button)

        #  Create Edit Button
        self.m_config_edit_button = self.Create_Button(icon_path=edit_item_icon_path)
        self.m_config_button_layout.addWidget(self.m_config_edit_button)


        self.m_config_button_panel.setLayout(self.m_config_button_layout)
        self.m_config_layout.addWidget(self.m_config_button_panel)

    def Build_Toolbar(self):

        #  Create Widget
        self.m_toolbar_widget = QtGui.QGroupBox('Toolbar',self)

        #  Create Layout
        self.m_toolbar_layout = QtGui.QHBoxLayout()
        self.m_toolbar_layout.setAlignment(Qt.AlignLeft)


        #  Create Close Button
        icon_path=self.m_options.Query('CORE','ICON_HOME')+'/close.png'
        self.m_toolbar_close_button = self.Create_Button(action=self.Close_Panel,
                                                         tooltip='Close Window.',
                                                         icon_path=icon_path)
        self.m_toolbar_layout.addWidget(self.m_toolbar_close_button)

        #  Set layout
        self.m_toolbar_widget.setLayout(self.m_toolbar_layout)

        #  Add to main layout
        self.m_main_layout.addWidget(self.m_toolbar_widget)


    def Update_Configuration_Tree(self):

        #  Clear the tree
        self.m_config_tree.clear()

        #  Update the headers
        self.m_config_tree.setColumnCount(len(self.m_tree_labels))
        self.m_config_tree.setHeaderLabels(self.m_tree_labels)

        #  Add items
        for key in self.m_options.QueryModules():

            #  create module widget
            module_widget = QtGui.QTreeWidgetItem()
            module_widget.setText(0, key)

            #  Add Child Widgets
            child_key_list = self.m_options.QueryModuleItems(key)
            child_key_list.sort()

            for child in child_key_list:

                value = self.m_options.QueryFull(key, child)

                #  Create widget
                item_widget = QtGui.QTreeWidgetItem()
                item_widget.setText(0, key )
                item_widget.setText(1, child)
                item_widget.setText(2, value[0])
                item_widget.setText(3, value[1])

                module_widget.addChild(item_widget)

            #  Add the module widget
            self.m_config_tree.addTopLevelItem(module_widget)

        #  Add Plugin Widgets
        plugin_widget = QtGui.QTreeWidgetItem()
        plugin_widget.setText(0, 'PLUGINS')

        for child in self.m_options.QueryPluginItems():

            value = self.m_options.QueryPluginFull(child)

            item_widget = QtGui.QTreeWidgetItem()
            item_widget.setText(0, 'PLUGINS')
            item_widget.setText(1, child)
            item_widget.setText(2, value[0])
            item_widget.setText(3, value[1])
            plugin_widget.addChild(item_widget)

        self.m_config_tree.addTopLevelItem(plugin_widget)


    def Create_Button( self, name = None,
                             parent = None,
                             action=None,
                             icon_path = None,
                             icon_size = None,
                             tooltip = None):

        #  Create ToolButton
        button = QtGui.QToolButton(parent)

        #  Set attributes
        if not name is None:
            button.setText(name)
            button.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        if not action is None:
            button.clicked.connect(action)

        button.setFixedWidth(int(self.m_options.Query('GUI','ToolbarButtonWidth',100)))
        button.setFixedHeight(int(self.m_options.Query('GUI','ToolbarButtonHeight',100)))

        if not icon_path is None:
            button.setIcon(QtGui.QIcon(icon_path))

        if icon_size is None:
            button.setIconSize(QSize(int(self.m_options.Query('GUI','ToolbarButtonIconWidth')),
                                     int(self.m_options.Query('GUI','ToolbarButtonIconHeight'))))
        else:
            button.setIconSize(icon_size)


        if not tooltip is None:
            self.setToolTip(tooltip)

        return button

    @pyqtSlot()
    def Close_Panel(self):

        print('Closing Config File Pane')
        self.close_panel.emit()

    @pyqtSlot()
    def AddConfigOption(self):
        pass

    @pyqtSlot()
    def RemoveConfigOption(self):
        pass

    @pyqtSlot()
    def EditConfigOption(self):
        pass
