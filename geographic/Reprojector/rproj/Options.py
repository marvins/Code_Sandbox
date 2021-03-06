#    File:    Options.py
#    Author:  Marvin Smith
#    Date:    11/28/2015
#

#  Python Libraries
import sys, argparse, ConfigParser, logging
from osgeo import osr


class Options(object):

    #  Configuration Options
    config = None


    #--------------------------#
    #-      Constructor       -#
    #--------------------------#
    def __init__(self):
        '''
        Constructor
        :param args: List of command-line arguments
        :return:
        '''

        #  Setup options
        self.config = {}

        #  Create Command-Line Parser
        self.Parse_Command_Line()

        #  Configure Logging
        self.Configure_Logging()

        #  Parse the Configuration File
        self.Parse_Config_File()


    def Parse_Command_Line(self):

        # Create command-line parser
        parser = argparse.ArgumentParser(description='Reproject Map Data to Various Types.')

        #  Config Path Arguments
        parser.add_argument('-c','--config',
                            dest='config_path',
                            default='data/options.cfg',
                            required=False,
                            help='Set the default configuration file path.')

        #  Program Mode
        parser.add_argument('-m','--mode',
                            dest='program_mode',
                            default='project',
                            required=False,
                            choices=['project','convert'],
                            help='Set the mode for the operation.')

        #   Shapefile
        parser.add_argument('-s','--shapefile',
                            dest='shapefile',
                            required=False,
                            default=None,
                            help='Provide shapefile to process.')

        #  Logging
        parser.add_argument('-v','--verbose',
                            dest='log_level',
                            required=False,
                            action='store_const',
                            const='DEBUG',
                            default='INFO',
                            help='Set log level.')

        #  Set the Default Projection Type
        parser.add_argument('-p','--projection',
                            dest='output_projection',
                            required=False,
                            default=None,
                            help='Override the output projection type.')

        #  Parse Arguments
        self.cmd_options = parser.parse_args()


    def Parse_Config_File(self):

        #  Log Entry
        logging.debug('Parsing Configuration File.')

        #  Create Config Parser
        self.cfg_parser = ConfigParser.ConfigParser()

        #  Check if Config File Overloaded
        if self.cmd_options.config_path is not None:
            logging.debug('Loading Config Path: ' + self.cmd_options.config_path)
            self.cfg_parser.read(self.cmd_options.config_path)

        #  Check Mode
        self.config['program_mode'] = self.cmd_options.program_mode

        #  Process Shapefile
        if self.cmd_options.program_mode == 'convert':
            self.config['shapefile_path'] = self.cmd_options.shapefile

        #  Get the Projections
        self.config['in_srs']  = self.cfg_parser.get('shapefile','projection')
        self.config['out_srs'] = self.cfg_parser.get('general','projection')

        if self.cfg_parser.get('shapefile','projection') == '':
            self.config['in_srs_params']  = { 'cs'          : self.cfg_parser.get('shapefile','cs'),
                                              'is_northern' : self.cfg_parser.getboolean('shapefile','is_northern'),
                                              'is_utm'      : self.cfg_parser.getboolean('shapefile','is_utm'),
                                              'grid_zone'   : self.cfg_parser.getint('shapefile','grid_zone') }

        if self.cfg_parser.get('general','projection') == '':
            self.config['out_srs_params'] = { 'cs'          : self.cfg_parser.get('general','cs'),
                                              'is_northern' : self.cfg_parser.getboolean('general','is_northern'),
                                              'is_utm'      : self.cfg_parser.getboolean('general','is_utm'),
                                              'grid_zone'   : self.cfg_parser.getint('general','grid_zone') }

        #  Set the reference shapefile
        self.reference_shapefile = self.cfg_parser.get('general','pathname')

        #  Get the Feature Point Min Threshold
        self.config['feature_point_min_threshold'] = self.cfg_parser.getint('general','feature_point_min_threshold')

        #  Process Rendering parameters
        render_config = {}
        if self.cmd_options.output_projection is None:
            render_config['output_projection'] = self.cfg_parser.get('rendering','output_projection')
        else:
            render_config['output_projection'] = self.cmd_options.output_projection
        
        projection_name = render_config['output_projection']
        render_config['output_image_path'] = self.cfg_parser.get(projection_name,'output_image_path')
        

        #  Get Projection Specific Parameters
        render_config['min_x_padding'] = self.cfg_parser.getfloat(projection_name,'min_x_padding')
        render_config['max_x_padding'] = self.cfg_parser.getfloat(projection_name,'max_x_padding')
        render_config['min_y_padding'] = self.cfg_parser.getfloat(projection_name,'min_y_padding')
        render_config['max_y_padding'] = self.cfg_parser.getfloat(projection_name,'max_y_padding')
        render_config['window_gsd'] = self.cfg_parser.getfloat(projection_name,'window_gsd')

        if projection_name == 'armadillo':
            render_config['central_meridian'] = self.cfg_parser.getfloat(projection_name,'central_meridian')
            render_config['radius_sphere'] = self.cfg_parser.getfloat(projection_name,'radius_sphere')

        elif projection_name == 'bonne':
            render_config['proj4_def'] = self.cfg_parser.get(projection_name,'proj4_def')

        elif projection_name == 'utm':

            #  Add Grid Zone Data
            render_config['grid_zone'] = self.cfg_parser.getint(projection_name,'grid_zone')
            render_config['is_north']  = self.cfg_parser.getboolean(projection_name,'is_north')
            render_config['datum']     = self.cfg_parser.get(projection_name,'datum')


        self.config['rendering'] = render_config

    def Configure_Logging(self):

        logging.basicConfig(level=getattr(logging,self.cmd_options.log_level))


    def Get_Input_SRS(self):

        #  Create SRS
        prj = osr.SpatialReference()

        #  Check if SRS is empty
        if self.config['in_srs'] == '':

            #  Get the Datum
            cs = self.config['in_srs_params']['cs']
            prj.SetWellKnownGeogCS(cs)

            if self.config['in_srs_params']['is_utm'] is True:

                #  Get the Grid Zone
                grid_zone = self.config['in_srs_params']['grid_zone']

                #  Check if Northern
                is_northern = self.config['in_srs_params']['is_northern']

                #  Set info
                prj.SetUTM(grid_zone, is_northern)
            else:
                raise Exception('Not enough info.')
        else:
            prj.SetWellKnownGeogCS(str(self.config['in_srs']))

        return prj

    def Get_Output_SRS(self):

        #  Create SRS
        prj = osr.SpatialReference()

        #  Check if SRS is empty
        if self.config['out_srs'] == '':

            #  Get the Datum
            cs = self.config['out_srs_params']['cs']
            prj.SetWellKnownGeogCS(cs)

            if self.config['out_srs_params']['is_utm'] is True:

                #  Get the Grid Zone
                grid_zone = self.config['out_srs_params']['grid_zone']

                #  Check if Northern
                is_northern = self.config['out_srs_params']['is_northern']

                #  Set info
                prj.SetUTM(grid_zone, is_northern)
            else:
                raise Exception('Not enough info.')
        else:
            prj.SetWellKnownGeogCS(str(self.config['out_srs']))

        return prj
