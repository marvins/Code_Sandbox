#    File:    ConfigurationPane.py
#    Author:  Marvin Smith
#    Date:    7/12/2016
#
#    Purpose: View and manage logs
#

#   PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Project
from core.SysLogger import SysLogger

class LogViewerPane(QtGui.QWidget):
    '''
    Log Viewer Panel
    '''

    #  Close panel signal
    close_panel = pyqtSignal()

    #  Default Constructor
    def __init__(self, options, parent = None):

        #  Create parent
        QtGui.QWidget.__init__(self,parent)

        #  Set Preferences
        self.m_options = options

        #  Set title
        self.setWindowTitle('Log Viewer')

        # Initialize the GUI
        self.Initialize_UI()

        #  Load initial data
        self.Update_Log_Window()


    def Initialize_UI(self):

        #  Create the main widget
        self.m_main_layout = QtGui.QVBoxLayout()

        #  Create Log Edit
        self.Build_Log_Window()

        #  Create Toolbar
        self.Build_Toolbar()

        #  Set the layout
        self.setLayout(self.m_main_layout)

    def Build_Log_Window(self):

        #  Create Text Edit
        self.m_log_edit = QtGui.QTextEdit()

        #  Add to main layout
        self.m_main_layout.addWidget(self.m_log_edit)

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

    def Update_Log_Window(self):

        #  Get text from logger
        self.m_log_edit.setText(SysLogger.Get_Log_Output())

    def Close_Panel(self):

        self.close_panel.emit()