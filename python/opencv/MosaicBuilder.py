#!/usr/bin/env python

__author__ = 'marvinsmith'


#  Python Imports
import argparse, cv2
import numpy as np

# ---------------------------------------- #
# -       FLANN Indexing Arguments       - #
# ---------------------------------------- #
FLANN_INDEX_ARGS = { 'FLANN_INDEX_LINEAR':0,
                     'FLANN_INDEX_KDTREE':1,
                     'FLANN_INDEX_COMPOSITE':2,
                     'FLANN_INDEX_KMEANS':3,
                     'FLANN_INDEX_KDTREE_SINGLE':4}


# ---------------------------------------- #
# -     Parse Command-Line Arguments     - #
# ---------------------------------------- #
def Parse_Arguments():
    """
    Parses the command-line arguments
    :return: Argument parser.
    """

    #  Build the main argument parser
    parser = argparse.ArgumentParser(description='Stitch images together')

    #  Get the anchor image
    parser.add_argument( '--anchor-image',
                         dest='anchor_image',
                         required=True,
                         help='Anchor image to be used to map other images to.')

    #  Get the output image name
    parser.add_argument('--output-image',
                        dest='output_image',
                        required=True,
                        help='Path of image to write results to.')

    #  Get the remaining image list
    parser.add_argument('--image',
                        dest='image_list',
                        action='append',
                        required=True,
                        help='Images to stitch to the anchor image.')

    #  Set the minimum matches required to stich
    parser.add_argument( "--min-match-count",
                         dest="min_match_count",
                         type=int,
                         required=False,
                         default=10,
                         help="Minimum number of matches required to stitch")

    #  Set the flann index argument
    parser.add_argument( '--flann-index-algorithm',
                         dest='flann_index_algorithm',
                         required=False,
                         default=FLANN_INDEX_ARGS['FLANN_INDEX_KMEANS'],
                         choices=FLANN_INDEX_ARGS.keys(),
                         help='FLANN indexing algorithm to perform.')

    #  Set the flann number of trees
    parser.add_argument( '--flann-number-trees',
                         dest='flann_index_trees',
                         required=False,
                         default=5,
                         help="Number of FLANN indexing trees.")

    #  Skip display of images
    parser.add_argument( '--skip-display',
                         dest='skip_display',
                         required=False,
                         default=False,
                         type=bool,
                         help="Skip display of images. Useful for huge images.")

    #   FLANN Search Check Parameters
    parser.add_argument( '--flann-search-checks',
                         dest='flann_search_checks',
                         type=int,
                         required=False,
                         default=50,
                         help='Number of FLANN checks to conduct when performing a Nearest-Neighbor search.')

    #  Return the parser
    return parser.parse_args()

# --------------------------- #
# -      Main Function      - #
# --------------------------- #
def main():

    #  Parse Command-Line Arguments
    options = Parse_Arguments()


    #  Load each image
    images = [ cv2.imread(img_path) for img_path in options.image_list ]
    images_gray = [ cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) for img in images]


    #  Load the anchor image
    anchor_image = cv2.imread( options.anchor_image )
    anchor_image_gray = cv2.cvtColor( anchor_image, cv2.COLOR_BGR2GRAY )
    

    #  Initialize the SIFT detector
    sift = cv2.SIFT()


    #  Compute Keypoints and Descriptors
    anchor_keypoints, anchor_descriptors = sift.detectAndCompute( anchor_image_gray, None )
    image_keypoints = []
    image_descriptors = []

    for img in images_gray:
        img_keypoints,  img_descriptors  = sift.detectAndCompute( img, None )
        image_keypoints.append(img_keypoints)
        image_descriptors.append(img_descriptors)



    # Initialize parameters for Flann based matcher
    index_params = dict(algorithm = options.flann_index_algorithm,
                        trees     = options.flann_index_trees)

    search_params = dict(checks = options.flann_search_checks)

    # Initialize the Flann based matcher object
    flann = cv2.FlannBasedMatcher(index_params, search_params)


    #  Match between images



# ---------------------------- #
# -       Main Program       - #
# ---------------------------- #
if __name__ == '__main__':
    main()

