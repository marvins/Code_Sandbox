#!/usr/bin/env python
__author__ = 'marvinsmith'

#  Python Libraries
import csv, os, sys, ConfigParser
import matplotlib.pyplot as plt


#   Load CSV Data
def Load_Data(pathname):

    #  Create output container
    output = {'ga':[],
              'gd':[],
              'bf':[]}

    #  Open and parse CSV Data
    with open(pathname, 'r') as csvfile:

        #  Create csv reader
        reader = csv.DictReader(csvfile)

        for line in reader:

            #  Get the Algorithm
            algo = line['Algorithm']

            #  Get the Distance
            dist = float(line['Distance'])

            #  Get the Time
            runtime = float(line['Time'])

            #  Get the number of cities
            numCities = int(line['Number Cities'])

            #  Update output
            output[algo].append((dist,runtime,numCities))


    return output


def Build_Plot_Data_Time_Vs_Cities( data, algorithm ):

    #  Create output data
    maxVal = 15
    entries = [[x for x in range(0,maxVal)],[0 for x in range(0,maxVal)],[0 for x in range(0,maxVal)]]
    output = [[],[]]

    # Get the subset
    subset = data[algorithm]

    #  Iterate over values
    for field in subset:

        x = field[2]
        entries[1][x] += field[1]
        entries[2][x] += 1

    for x in range(0,maxVal):
        if entries[2][x] > 0:
            output[0].append(x)
            output[1].append(entries[1][x] / float(entries[2][x]))

    return output

def Build_Plot_Data_Distance_Vs_Cities( data, algorithm ):

    #  Create output data
    maxVal = 15
    entries = [[x for x in range(0,maxVal)],[0 for x in range(0,maxVal)],[0 for x in range(0,maxVal)]]
    output = [[],[]]

    # Get the subset
    subset = data[algorithm]

    #  Iterate over values
    for field in subset:

        x = field[2]
        entries[1][x] += field[0]/1000.0
        entries[2][x] += 1

    for x in range(0,maxVal):
        if entries[2][x] > 0:
            output[0].append(x)
            output[1].append(entries[1][x] / float(entries[2][x]))

    return output


def Plot_Time_Vs_Cities( data, color_list, algo, config ):

    counter = 0

    #  Iterate over each entry
    for entry in data:

        #  Plot Line
        plt.plot(entry[0],entry[1], color_list[counter], label=algo[counter])

        counter += 1

    #  Configure Plot
    plt.xlim([config.getint('timing_plots','num_cities_min_x'),
              config.getint('timing_plots','num_cities_max_x')])


    if config.getboolean('timing_plots','num_cities_use_log') is True:
        plt.yscale('log')
        plt.ylim([0,100000])
    else:
        plt.ylim([config.getfloat('timing_plots','num_cities_min_y'),
                  config.getfloat('timing_plots','num_cities_max_y')])

    plt.title('Completion Time vs Number of Cities')
    plt.xlabel('Number of Cities')
    plt.ylabel('Time [s]')
    plt.legend(loc='upper left')
    plt.grid()

    #  Plot
    plt.show()

def Plot_Accuracy_Over_Number_Of_Cities(data, color_list, algo, config):

    counter = 0

    #  Iterate over each entry
    for entry in data:

        #  Plot Line
        plt.plot(entry[0],entry[1], color_list[counter], label=algo[counter])

        counter += 1

    #  Configure Plot
    plt.xlim([config.getint('timing_plots','distance_plot_min_x'),
              config.getint('timing_plots','distance_plot_max_x')])


    if config.getboolean('timing_plots','num_cities_use_log') is True:
        plt.yscale('log')
        plt.ylim([0,100000])
    else:
        plt.ylim([config.getfloat('timing_plots','distance_plot_min_y'),
                  config.getfloat('timing_plots','distance_plot_max_y')])

    plt.title('Distance from Solution vs Number of Cities')
    plt.xlabel('Number of Cities')
    plt.ylabel('Distance from optimal solution [km]')
    plt.legend(loc='upper left')
    plt.grid()

    #  Plot
    plt.show()

if __name__ == '__main__':

    data = Load_Data(sys.argv[1])

    configuration = ConfigParser.ConfigParser()
    configuration.read(sys.argv[2])

    #  Get the GA Count
    ga_fields = Build_Plot_Data_Time_Vs_Cities( data, 'ga')
    gd_fields = Build_Plot_Data_Time_Vs_Cities( data, 'gd')
    bf_fields = Build_Plot_Data_Time_Vs_Cities( data, 'bf')

    #  Plot Time vs Cities
    Plot_Time_Vs_Cities([ga_fields, gd_fields, bf_fields],
                        ['b','r','g'],
                        ['Genetic','Greedy','Brute-Force'],
                        configuration)

    ga_fields = Build_Plot_Data_Distance_Vs_Cities( data, 'ga')
    gd_fields = Build_Plot_Data_Distance_Vs_Cities( data, 'gd')
    bf_fields = Build_Plot_Data_Distance_Vs_Cities( data, 'bf')

    #  Plot Accuracy over Number of Cities
    Plot_Accuracy_Over_Number_Of_Cities([ga_fields, gd_fields, bf_fields],
                                        ['b','r','g'],
                                        ['Genetic','Greedy','Brute-Force'],
                                        configuration)