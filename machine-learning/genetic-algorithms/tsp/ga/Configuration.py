__author__ = 'marvinsmith'


#  Python Libraries
import configparser, argparse, csv, os


class Configuration(object):

    #  Command-Line Arguments
    cmd_args = None

    #  Genetic Algorithm Configuration
    ga_params = {}

    #  Greedy Algorithm Configuration
    gd_params = {}

    #  Brute Force Configuration
    bf_params = {}

    #  General Parameters
    general_params = {}

    def __init__(self):

        #  Set defaults
        self.Set_Defaults()

        #  Parse the Command-Line
        self.Parse_Command_Line()

        #  Parse the Configuration File
        self.Parse_Configuration_File()

    def Set_Defaults(self):

        self.cmd_args = None
        self.general_params = {}
        self.ga_params = {}
        self.bf_params = {}
        self.gd_params = {}

    def Parse_Command_Line(self):

        #  Create parser
        parser = argparse.ArgumentParser(description='Solve the Traveling Salesman Problem.')


        #   Set the input dataset
        parser.add_argument('-i','--input',
                            dest='input_dataset',
                            required=True,
                            help='Dataset to solve for.')


        #  Configuration File Pathname
        parser.add_argument('-c','--config',
                            dest='config_pathname',
                            required=False,
                            default='datasets/options.cfg',
                            help='Configuration file params for different solvers.')


        parser.add_argument('-v','--verbose',
                            dest='log_level',
                            default=None,
                            action='store_const',
                            const='DEBUG',
                            help='Enable verbose logging.')
        #  Parse and return
        self.cmd_args = parser.parse_args()


    def Parse_Configuration_File(self):

        #  Open the Config file
        cfgParser = configparser.ConfigParser()
        cfgParser.read(self.cmd_args.config_pathname)

        #  Parse the Input Dataset
        self.general_params['input_dataset'] = self.cmd_args.input_dataset

        #  Parse the Log Level
        if self.cmd_args.log_level is not None:
            self.general_params['log_level'] = self.cmd_args.log_level
        elif cfgParser.has_option('general','log_level'):
            self.general_params['log_level'] = cfgParser.get('general','log_level')
        else:
            self.general_params['log_level'] = 'INFO'

        #  Parse Timing Info
        self.general_params['timing_info_pathname'] = cfgParser.get('general','timing_info_pathname')

        #  Parse if we are skipping anything
        self.general_params['skip_ga'] = cfgParser.getboolean('general','skip_ga')
        self.general_params['skip_gd'] = cfgParser.getboolean('general','skip_gd')
        self.general_params['skip_bf'] = cfgParser.getboolean('general','skip_bf')
        self.general_params['skip_html'] = cfgParser.getboolean('general','skip_html')
        self.general_params['skip_cities'] = cfgParser.getint('general','skip_cities')

        #  Parse the GA Config
        self.Parse_GA_Configuration_Parameters(cfgParser)


    def Parse_GA_Configuration_Parameters(self, cfgParser ):

        #  Read the Population Size
        self.ga_params['population_size'] = cfgParser.getint('genetic_algorithm','population_size')

        #  Read the Preservation Rate
        self.ga_params['preservation_rate'] = cfgParser.getfloat('genetic_algorithm','preservation_rate')

        #  Read the Mutation Rate
        self.ga_params['mutation_rate'] = cfgParser.getfloat('genetic_algorithm','mutation_rate')

        #  Read the Selection Rate
        self.ga_params['selection_rate'] = cfgParser.getfloat('genetic_algorithm','selection_rate')

        #  Read Max Iterations
        self.ga_params['max_iterations'] = cfgParser.getint('genetic_algorithm','max_iterations')

        #  Read Exit on Repeats
        self.ga_params['exit_on_repeats'] = cfgParser.getint('genetic_algorithm','exit_on_repeats')


    def Update_Performance_Information( self,
                                        ga_info,
                                        gd_info,
                                        bf_info,
                                        number_cities ):

        #  Check if file exists
        if os.path.exists(self.general_params['timing_info_pathname']) is False:
            with open(self.general_params['timing_info_pathname'], 'w') as csvfile:
                csvfile.write('Algorithm,Distance,Time,Number Cities,Population Size,Selection Rate,Mutation Rate\n')

        #  Create the output file
        with open(self.general_params['timing_info_pathname'], 'a') as csvfile:

            #  Write GA Performance
            csvfile.write('ga,' + str(ga_info['dist']) + ',' + str(ga_info['time']) + ',' + str(number_cities) + ',' + str(self.ga_params['population_size']) + '\n')

            #  Write GD Performance
            csvfile.write('gd,' + str(gd_info['dist']) + ',' + str(gd_info['time']) + ',' + str(number_cities) + '\n')

            #  Write BF Performance
            csvfile.write('bf,' + str(bf_info['dist']) + ',' + str(bf_info['time']) + ',' + str(number_cities) + '\n')

