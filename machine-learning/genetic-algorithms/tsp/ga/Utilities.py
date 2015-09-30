__author__ = 'Marvin Smith'

#  Python Libraries
import math

#-----------------------------------------#
#-          Haversine Distance           -#
#-----------------------------------------#
def Haversine_Distance_Meters(lat1, long1, lat2, long2):

    #  Distance of Earth's Radius in meters
    radius_meters = 6378137

    # Convert latitude and longitude to
    # spherical coordinates in radians.
    degrees_to_radians = math.pi/180.0

    # phi = 90 - latitude
    phi1 = (90.0 - lat1)*degrees_to_radians
    phi2 = (90.0 - lat2)*degrees_to_radians

    # theta = longitude
    theta1 = long1*degrees_to_radians
    theta2 = long2*degrees_to_radians

    # Compute spherical distance from spherical coordinates.

    # For two locations in spherical coordinates
    # (1, theta, phi) and (1, theta', phi')
    # cosine( arc length ) =
    #    sin phi sin phi' cos(theta-theta') + cos phi cos phi'
    # distance = rho * arc length

    cos = (math.sin(phi1)*math.sin(phi2)*math.cos(theta1 - theta2) +
           math.cos(phi1)*math.cos(phi2))
    arc = math.acos( cos )

    # Remember to multiply arc by the radius of the earth
    # in your favorite set of units to get length.
    return arc * radius_meters


def Build_Google_Map_Coordinate_List(alg_name,
                                     alg_list,
                                     xoff = 0,
                                     yoff = 0):
    """
    Build Google Map Coordinate List
    :param alg_name:
    :param alg_list:
    :param xoff:
    :param yoff:
    :return:
    """

    # Gap
    gap = '                                       '

    #  Create output
    output =  'var ' + alg_name + ' = ['

    #  Build the initial values for the center lat and lon
    center_lat = alg_list.cities[0].latitude
    center_lon = alg_list.cities[0].longitude

    #  Start building the output text
    output += "{lat: " + str(alg_list.cities[0].latitude+yoff) + ', lng: ' + str(alg_list.cities[0].longitude+xoff) + "}"

    #  Iterate over each city in list
    for city in alg_list.cities[1:]:
        output += ",\n" + gap + "{lat: " + str(city.latitude) + ", lng: " + str(city.longitude) + "}"

        #  Sum the latitudes and longitudes
        center_lat += city.latitude
        center_lon += city.longitude

    #  Append the closing bracket for the output
    output += "];\n"

    #  Divide the center values
    center_lat /= len(alg_list.cities)
    center_lon /= len(alg_list.cities)

    #  Return output
    return output, center_lat, center_lon

#----------------------------------#
#-      Write the output file     -#
#----------------------------------#
def Write_Output( ga_solution,
                  gd_solution,
                  bf_solution ):

    #  Select the input HTML Path
    filename = 'ga/index.html'


    #  Load the html file
    data = None
    with open(filename, 'r') as f:
        data = f.read()

    #  Set our offsets
    xoff_ga = 0
    yoff_ga = 0

    xoff_gd = -0.1
    yoff_gd = -0.1

    xoff_bf = 0.1
    yoff_bf = 0.1

    #  Construct the output
    ga_point_text, ga_center_lat, ga_center_lon = Build_Google_Map_Coordinate_List('ga_path_coordinates', ga_solution['solution'], xoff_ga, yoff_ga)
    gd_point_text, gd_center_lat, gd_center_lon = Build_Google_Map_Coordinate_List('gd_path_coordinates', gd_solution['solution'], xoff_gd, yoff_gd)
    bf_point_text, bf_center_lat, bf_center_lon = Build_Google_Map_Coordinate_List('bf_path_coordinates', bf_solution['solution'], xoff_bf, yoff_bf)


    #  Update the Point Info
    data = data.replace('__GA_POINTS_HERE__', ga_point_text)
    data = data.replace('__GD_POINTS_HERE__', gd_point_text)
    data = data.replace('__BF_POINTS_HERE__', bf_point_text)

    #   Update the center map position
    data = data.replace('__CENTER_LAT__', str(ga_center_lat))
    data = data.replace('__CENTER_LON__', str(ga_center_lon))

    #  Update the performance times
    data = data.replace('__GA_TIME__', "%f" % ga_solution['time'])
    data = data.replace('__GD_TIME__', "%f" % gd_solution['time'])
    data = data.replace('__BF_TIME__', "%f" % bf_solution['time'])

    #  Update distnaces
    data = data.replace('__GA_DIST__', "%f" % ga_solution['dist'])
    data = data.replace('__GD_DIST__', "%f" % gd_solution['dist'])

    #   Add the listeners
    gap = '       '
    output_text = ''
    for x in range(0,len(ga_solution['solution'].cities)):
        output_text += gap + 'ga_markers[' + str(x) + '].addListener(\'click\',function(){ga_windows[' + str(x) + '].open(map,ga_markers[' + str(x) + ']);});\n'
    data = data.replace('__ADD_LISTENERS__', output_text)


    #  Add the Window Titles
    output_text = ''
    gap = '            '
    output_text += 'ga_titles[' + str(x) + ']=\'Start and Finish\'\n'
    for x in range(1,len(ga_solution['solution'].cities)-1):
        bf_marker = 0
        for y in range(1,len(bf_solution['solution'].cities)-1):
            if ga_solution['solution'].cities[x].name == bf_solution['solution'].cities[y].name:
                bf_marker = y
                break
        output_text += gap + 'ga_titles[' + str(x) + ']=\'GA Marker ' + str(x) + ', BF Marker ' + str(bf_marker) + '\'\n'

    data = data.replace('__WINDOW_TITLES_HERE__', output_text)

    #  Write the output
    with open('output.html', 'w') as f:
        f.write(data)
