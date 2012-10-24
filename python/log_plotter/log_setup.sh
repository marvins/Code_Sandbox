#! /bin/bash 


#
# This is a configuration utility for the log parsing script
#

LOGFILE_NAME=gs2podcontroller_20121020_183925.metrics

CSV_OUTPUT_FILE=slice1_temps.csv


########################################################
# This is the component to parse for
#
# Available options
#
# - SLICE_1_PROCESSOR  (1-8)
# - CENTRAL_PROCESSOR
#
COMPONENT=SLICE_1_PROCESSOR


############################################################
# These are the tags to watch for and parse
#
KEYS="Thermal_sensor_--_core_0  Thermal_sensor_--_core_1  Thermal_sensor_--_core_2 Thermal_sensor_--_core_3"



#############################################
#          Do not change !!!!!!!!!!         #
#############################################
SCRIPT_NAME=log_utility.py

# Command
./${SCRIPT_NAME} -log=${LOGFILE_NAME} -csv=${CSV_OUTPUT_FILE} -comp=${COMPONENT} -keys=${KEYS}

