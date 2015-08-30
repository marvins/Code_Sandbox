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


def Build_Google_Map_Coordinate_List(alg_name, alg_list, xoff = 0, yoff = 0):

    #  Create output
    output =  'var ' + alg_name + ' = ['

    #  Build the initial values for the center lat and lon
    center_lat = alg_list.cities[0].latitude
    center_lon = alg_list.cities[0].longitude

    #  Start building the output text
    output += "{lat: " + str(alg_list.cities[0].latitude+yoff) + ', lng: ' + str(alg_list.cities[0].longitude+xoff) + "}"

    #  Iterate over each city in list
    for city in alg_list.cities[1:]:
        output += ",\n{lat: " + str(city.latitude) + ", lng: " + str(city.longitude) + "}"

        #  Sum the latitudes and longitudes
        center_lat += city.latitude
        center_lon += city.longitude

    #  Append the closing bracket for the output
    output += "];\n"

    #  Divide the center values
    center_lat /= len(alg_list)
    center_lon /= len(alg_list)

    #  Return output
    return output, center_lat, center_lon

#----------------------------------#
#-      Write the output file     -#
#----------------------------------#
def Write_Output( best_fit_ga,
                  best_fit_bf,
                  best_fit_gd):

    #  Select the input HTML Path
    filename = 'ga/index.html'


    #  Load the html file
    data = None
    with open(filename, 'r') as f:
        data = f.read()

    #  Set our offsets
    xoff_ga = 0
    yoff_ga = 0

    xoff_gd = -0.01
    yoff_gd = -0.01

    xoff_bf = 0.01
    yoff_bf = 0.01

    #  Construct the output
    ga_point_text, ga_center_lat, ga_center_lon = Build_Google_Map_Coordinate_List('ga_path_coordinates', best_fit_ga, xoff_ga, yoff_ga)
    gd_point_text, gd_center_lat, gd_center_lon = Build_Google_Map_Coordinate_List('gd_path_coordinates', best_fit_gd, xoff_gd, yoff_gd)
    bf_point_text, bf_center_lat, bf_center_lon = Build_Google_Map_Coordinate_List('ga_path_coordinates', best_fit_bf, xoff_bf, yoff_bf)


    #  Update the Point Info
    data = data.replace('__GA_POINTS_HERE__', ga_point_text)
    data = data.replace('__GD_POINTS_HERE__', ga_point_text)
    data = data.replace('__BF_POINTS_HERE__', ga_point_text)

    #   Update the center map position
    data = data.replace('__CENTER_LAT__', str(ga_center_lat))
    data = data.replace('__CENTER_LON__', str(ga_center_lon))


    #   Add the listeners
    output_text = ''
    for x in range(0,len(best_fit_ga.cities)):
        output_text += 'ga_markers[' + str(x) + '].addListener(\'click\',function(){ga_windows[' + str(x) + '].open(map,ga_markers[' + str(x) + ']);});\n'
    data = data.replace('__ADD_LISTENERS__', output_text)


    #  Add the Window Titles
    output_text = ''
    output_text += 'ga_titles[' + str(x) + ']=\'Start and Finish\'\n'
    for x in range(1,len(best_fit_ga.cities)-1):
        bf_marker = 0
        for y in range(1,len(best_fit_bf.cities)-1):
            if best_fit_ga.cities[x].name == best_fit_bf.cities[y].name:
                bf_marker = y
                break
        output_text += 'ga_titles[' + str(x) + ']=\'GA Marker ' + str(x) + ', BF Marker ' + str(bf_marker) + '\'\n'

    data = data.replace('__WINDOW_TITLES_HERE__', output_text)

    #  Write the output
    with open('output.html', 'w') as f:
        f.write(data)