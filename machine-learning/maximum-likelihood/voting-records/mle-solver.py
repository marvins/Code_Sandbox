#!/usr/bin/env python2

__author__ = 'Marvin Smith'


import argparse, csv, random, math, ConfigParser, logging
import matplotlib.pyplot as plt


def str2bool(s):
    return s.lower() in ['true','True','1',1,]

def Parse_Config_File(options):

    #  Output options
    output = {'input_pathname': options.input_pathname}

    #  Create parser
    config = ConfigParser.ConfigParser()
    config.read(options.config_pathname)

    #  Check the train ratio
    output['train_ratio_start'] = config.getfloat('general','train_ratio_start')
    output['train_ratio_step']  = config.getfloat('general','train_ratio_step')

    #  Check the skip shuffle
    output['skip_shuffle'] = config.getboolean('general','skip_shuffle')

    #  Check flags
    output['field_masks'] = [ str2bool(x) for x in config.get('general','field_masks').split(',')]

    output['repetitions'] = config.getint('general','number_repetitions')

    #  Min Sigma
    output['min_sigma'] = config.getfloat('general','min_sigma')

    #  Log Severity
    output['log_severity'] = config.get('general','log_severity')

    # return configuration
    return output

def Parse_Command_Line():

    #  Create argument parser
    parser = argparse.ArgumentParser(description='Maximum Likelihood Example Application.')

    #  Add input data
    parser.add_argument('-i','--input-pathname',
                        dest='input_pathname',
                        required=True,
                        help='Input vote information.')

    #  Add config file
    parser.add_argument('-c','--config-pathname',
                        dest='config_pathname',
                        required=False,
                        default='options.cfg',
                        help='Override the default configuration file.')


    #  Return parsed arguments
    return parser.parse_args()

def Load_Dataset( options ):

    #  Output data
    output = {'republican':[],
              'democrat':[]}
    counter = 0

    #  Parties
    votes = {'y':1.0,
             'n':0.0,
             '?':0.5}

    #  Open CSV File
    with open(options['input_pathname'], 'r') as csvfile:

        for row in csv.reader(csvfile):

            #  Create new temp row
            temp_row = []

            #  grab the party
            party = row[0]

            for x in range(1,len(row)):
                if options['field_masks'][x-1] is True:
                    temp_row.append(votes[row[x]])

            output[party].append(temp_row)

    #  Return results
    return output

def Train( training_data, options ):

    #  Compute the sum of each vote
    train_results = [[0,0] for x in range(0,len(training_data[0]))]
    for training_list in training_data:
        for x in range(0,len(training_list)):
            train_results[x][0] += training_list[x]


    #  Find the mean
    for x in range(0, len(train_results)):
        train_results[x][0] /= len(training_data)

    #  Compute Variance
    for x in range(0, len(training_data)):
        for y in range(0, len(training_data[x])):
            train_results[y][1] += (training_data[x][y] - train_results[y][0]) ** 2

    for x in range(0, len(train_results)):
        train_results[x][1] /= len(training_data)

        if train_results[x][1] < options['min_sigma']:
            logging.debug("Below min sigma (" + str(train_results[x][1]) + '), using eps : ' + str(options['min_sigma']))
            train_results[x][1] = options['min_sigma']

    return train_results

class Classifier(object):

    #  Trained descriptors
    descriptors = None

    def __init__(self, descriptors):

        #  Set the descriptors
        self.descriptors = descriptors

    def To_String(self):

        output  = 'Classifier\n'
        output += '  - Republican: ' + str(self.descriptors['republican']) + '\n'
        output += '  - Democrat  : ' + str(self.descriptors['democrat']) + '\n'

        return output

    def Classify(self, sample ):


        total_score = 0


        #  Iterate over each dimension
        for x in range(0, len(sample)):

            minScore = None

            #  Iterate over each party
            for party in self.descriptors.keys():

                #  Grab sigma and mu
                mu    = self.descriptors[party][x][0]
                sigma = self.descriptors[party][x][1]

                #  Compute the score
                #score = math.fabs(self.descriptors[party][x][0] - sample[x])/self.descriptors[party][x][1]
                score = 1-(1/math.sqrt(2 * math.pi * sigma * sigma)) * math.exp(-(sample[x]-mu)**2/(2*sigma*sigma))

                #  Compute the closest
                if minScore is None or minScore > score:
                    minScore = score
                    minParty = party

            if minParty == 'democrat':
                total_score -= 1
            elif minParty == 'republican':
                total_score += 1
            else:
                raise Exception("Unknown Party: " + str(minParty))


        if total_score > 0:
            return 'republican'
        else:
            return 'democrat'

def Plot_Data(options, plot_data):

    #  Set the title
    plt.title('MLE Classification Accuracy')
    plt.xlim([0,1])
    plt.ylim([0,1])

    #  Plot the line data
    plt.plot( plot_data[0], plot_data[1] )

    #  Print the axis
    plt.xlabel('Ratio of data for training.')
    plt.ylabel('Percent of classified results correct (#accurate/#tests)')

    plt.grid(True)

    plt.show()

def Main():

    #  Parse Command-Line Options
    cmd_options = Parse_Command_Line()

    #  Parse Configuration File
    options = Parse_Config_File( cmd_options )

    #  Logging
    logging.basicConfig(level=getattr(logging,options['log_severity']))

    #  Load dataset
    datasets = Load_Dataset( options )



    #  Plot Data
    plot_data = [[],[]]


    #  Iterate over range of train rates
    train_ratio = options['train_ratio_start']
    while train_ratio < 1:

        total_accuracy = 0

        #  Repeat the desired number of times
        for x in range(0, options['repetitions']):

            #  Descriptors
            descriptors = {}

            #  Test Datasets
            test_sets = {}

            #  Iterate over each party
            for party in datasets.keys():

                #  Randomly Shuffle both datasets
                if options['skip_shuffle'] is False:
                    random.shuffle(datasets[party])

                #  Partition datasets
                train_n = int(train_ratio * len(datasets[party]))
                training_set     = datasets[party][0:train_n]
                test_sets[party] = datasets[party][train_n:]

                #  Get training data
                descriptors[party] = Train(training_set, options)


            #  Build the classifier
            classifier = Classifier(descriptors)


            valid_matches   = 0
            invalid_matches = 0
            total_matches   = 0

            #  Classify each test result
            for party in test_sets.keys():

                #  Iterate over each sample
                for sample in test_sets[party]:

                    #  Classify the sample
                    result = classifier.Classify(sample)

                    if result == party:
                        valid_matches += 1
                    else:
                        invalid_matches += 1
                    total_matches += 1


            #  Update the accuracy
            total_accuracy += float(valid_matches) / total_matches

        #  Update the total accuracy
        total_accuracy /= float(options['repetitions'])
        print('')
        print('Train rate: ' + str(train_ratio) + ', Percent Correct Classifications: ' + str(total_accuracy))

        plot_data[0].append(train_ratio)
        plot_data[1].append(total_accuracy)

        #  Increment the Training Ratio
        train_ratio += options['train_ratio_step']


    #  Plot
    Plot_Data( options, plot_data)

if __name__ == '__main__':
    Main()