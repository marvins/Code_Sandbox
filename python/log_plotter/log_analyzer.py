__author__ = 'Marvin Smith'

#  Python Libraries
import matplotlib.pyplot as plt
import argparse
import networkx as nx


def Parse_Log_File( log_pathname ):

    with open(log_pathname, 'r') as logfile:

        lines = logfile.readlines()


def Parse_Command_Line():

    # Create parser
    parser = argparse.ArgumentParser(description='Log Analysis Utility')

    #  Define input log file
    parser.add_argument('-i','--input',
                        dest='input_pathname',
                        required=True,
                        help='Input log file to parse.')

    #  Parse and return
    return parser.parse_args()


def Render_Log_Timeline(options, log_data):

    pass


def Main():

    #  Parse command-line args
    options = Parse_Command_Line()

    #  Read Log File
    log_data = Parse_Log_File(options.input_pathname)


    #  Render nodes
    Render_Log_Timeline(options, log_data)



if __name__ == '__main__':
    Main()
