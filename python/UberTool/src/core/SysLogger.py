#    File:    SysLogger.py
#    Author:  Marvin Smith
#    Date:    7/12/2016
#
#    Purpose:  Provide system-wide logging for GUI
#

from core.Singleton import Singleton


class SysLogger:

    #  Set the singleton pattern
    __metaclass__ = Singleton


    def __init__(self):

        #  Set default log output
        self.m_log_output = ''

    @staticmethod
    def Log(message):

        #  Get instance
        inst = SysLogger()
        inst.m_log_output += message + '\n'

    @staticmethod
    def Get_Log_Output():

        #  Get instance
        inst = SysLogger()
        return inst.m_log_output
