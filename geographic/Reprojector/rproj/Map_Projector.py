#    File:    Map_Projector.py
#    Author:  Marvin Smith
#    Date:    11/25/2015
#

#  Python Libraries
import cv2
import numpy as np

#  Project Libraries
from GDAL_Utilities import *
from Projections    import *


def Render_Map( options ):

    #  Get the Rendering config parameters
    render_config = options.config['rendering']

    #  Open the shapefile and extract shapes
    vec_data = Load_Vector_Data( options.reference_shapefile,
                                 options )

    #  Make sure the data has points to process
    if len(vec_data.keys()) <= 0:
        raise Exception('No points to process in shapefile.')


    #  Create the output projection range
    min_range = None
    max_range = None

    #  Get the projection parameters
    projected_vectors = {}
    projector         = Get_Projector( render_config['output_projection'],
                                       render_config ) 

    #  Iterate over pixels
    vec_ids = vec_data.keys()
    vec_ids.sort()
    for vid in vec_ids:

        #  Init the output
        projected_vectors[vid] = []

        #  Get the range of values
        for feature in vec_data[vid]:
            
            #  Create projected feature
            proj_feat = []

            #  Iterate over points
            for point in feature:
                
                # Project Point
                proj_point = projector.Transform_Forward(point)
                if proj_point is None:
                    continue

                #  Check against min and max
                if min_range is None:
                    min_range = [proj_point[0], proj_point[1]]
                if max_range is None:
                    max_range = [proj_point[0], proj_point[1]]

            
                #  Check the x/y values
                if proj_point[0] < min_range[0]:
                    min_range[0] = proj_point[0]
                if proj_point[1] < min_range[1]:
                    min_range[1] = proj_point[1]

                if proj_point[0] > max_range[0]:
                    max_range[0] = proj_point[0]
                if proj_point[1] > max_range[1]:
                    max_range[1] = proj_point[1]

                #  Add to output
                proj_feat.append( projector.Transform_Forward(point))

            projected_vectors[vid].append(proj_feat)

    print('Projected Vectors: ' + str(len(projected_vectors.keys())))

    #  Add bounding padding
    min_render_bbox = [min_range[0] - render_config['min_x_padding'],
                       min_range[1] - render_config['min_y_padding']]
    max_render_bbox = [max_range[0] + render_config['max_x_padding'],
                       max_range[1] + render_config['max_y_padding']]
    
    bbox_diff = [max_render_bbox[0] - min_render_bbox[0],
                 max_render_bbox[1] - min_render_bbox[1]]

    logging.debug('Min Range: ' + str(min_range))
    logging.debug('Max Range: ' + str(max_range))
    
    gsd = render_config['window_gsd']

    #  Set the rows and cols
    window_rows = (max_render_bbox[1] - min_render_bbox[1])/gsd
    window_cols = (max_render_bbox[0] - min_render_bbox[0])/gsd

    #  Build Image
    logging.debug(' - Building Output Image With Size: ' + str(window_rows) + ' rows by ' + str(window_cols) + ' cols')
    image = np.zeros((window_rows,window_cols,3), dtype=np.uint8)
    image[:] = [255,255,255]

    thickness = 3

    #  Write Each Polygon
    projected_keys = projected_vectors.keys()
    projected_keys.sort()
    for kidx in projected_keys:

        #  Fetch the Layer
        layer = projected_vectors[kidx]
        

        for feature in layer:

            #  Create the polygon shape
            polygon = np.zeros( (len(feature), 2), dtype=np.int32)
            counter = 0
                        
            # Convert each feature to pixel
            for point in feature:
                xVal = int(math.floor((point[0] - min_render_bbox[0])/bbox_diff[0] * window_cols + 0.5))
                yVal = window_rows - int(math.floor((point[1] - min_render_bbox[1])/bbox_diff[1] * window_rows + 0.5))
                polygon[counter,:] = [xVal,yVal]
                counter += 1

            #cv2.fillPoly( image, [polygon], (255,25,25))
            cv2.polylines( image, [polygon], True, (0,0,0), thickness)

    #  Write Image
    cv2.imwrite( render_config['output_image_path'], image )
    logging.info('Writing output to ' + render_config['output_image_path'])

