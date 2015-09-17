#!/usr/bin/env python3

__author__ = 'Marvin Smith'


import argparse, csv, random, math, configparser


def str2bool(s):
    return s.lower() in ['true','True','1',1,]

def Parse_Config_File(options):

    #  Output options
    output = {'input_pathname': options.input_pathname}

    #  Create parser
    config = configparser.ConfigParser()
    config.read(options.config_pathname)

    #  Check the train ratio
    if options.train_ratio is None:
        output['train_ratio'] = config.getfloat('general','train_ratio')
    else:
        output['train_ratio'] = options.train_ratio

    #  Check the skip shuffle
    output['skip_shuffle'] = config.getboolean('general','skip_shuffle')

    #  Check flags
    output['field_masks'] = [ str2bool(x) for x in config.get('general','field_masks').split(',')]

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

    #  Add train / test ratio
    parser.add_argument('-tr','--train-rate',
                        dest='train_ratio',
                        required=False,
                        default=None,
                        type=float,
                        help='Set the amount of data to train from.')

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

    return train_results

class Classifier(object):

    #  Trained descriptors
    descriptors = None

    def __init__(self, descriptors):

        #  Set the descriptors
        self.descriptors = descriptors


    def Classify(self, sample ):


        total_score = 0

        #  Iterate over each dimension
        for x in range(0, len(sample)):

            minScore = None

            #  Iterate over each party
            for party in self.descriptors.keys():

                #  Compute the score
                score = math.fabs(self.descriptors[party][x][0] - sample[x])/self.descriptors[party][x][1]

                #  Compute the closest
                if minScore is None or minScore > score:
                    minScore = score
                    minParty = party

            if minParty == 'democrat':
                total_score += minScore
            elif minParty == 'republican':
                total_score -= minScore
            else:
                raise Exception("Unknown Party: " + str(minParty))

        print('zscore: ' + str(total_score))
        if total_score < 0:
            return 'republican'
        else:
            return 'democrat'



def Main():

    #  Parse Command-Line Options
    cmd_options = Parse_Command_Line()

    #  Parse Configuration File
    options = Parse_Config_File( cmd_options )
    print(options['field_masks'])

    #  Load dataset
    datasets = Load_Dataset( options )

    #  Descriptors
    descriptors = {}

    #  Test Datasets
    test_sets = {}

    #  Iterate over each party
    for party in datasets.keys():

        #  Randomly Shuffle both datasets
        if options['skip_shuffle'] is not False:
            random.shuffle(datasets[party])


        #  Partition datasets
        train_n = int(options['train_ratio'] * len(datasets[party]))
        training_set     = datasets[party][0:train_n]
        test_sets[party] = datasets[party][train_n:]

        #  Get training data
        descriptors[party] = Train(training_set, options)

    print(descriptors['republican'])
    print(descriptors['democrat'])
    input('pause')

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
            input('result: ' + result + ', actual: ' + party)


    print('valid  : ' + str(valid_matches))
    print('invalid: ' + str(invalid_matches))
    print('total  : ' + str(total_matches))


if __name__ == '__main__':
    Main()