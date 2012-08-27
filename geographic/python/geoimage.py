import cv2, gdal, struct, numpy as np
from gdalconst import *


class GeoImage:
	
	# Image Filename
	filename = '__NONE__'

	# Actual OpenCV Image
	image = None

	def __init__ ( self, filename ):
		
		# Load the GDAL Dataset
		self.filename = filename
		dataset = gdal.Open( filename, gdal.GA_ReadOnly )
		
		# Throw exception if dataset is null
		if dataset is None:
			raise Exception('Dataset does not exist')
		
		# Load the image data
		else:

			# Compute the Datatype
			self.datatype = GDAL2OpenCVDatatype( dataset )

			# Create the output image type
			self.image = np.zeros( (dataset.RasterYSize,dataset.RasterXSize,dataset.RasterCount), self.datatype)
			
			# iterate through each band
			for channel in xrange(0, dataset.RasterCount):
				
				# pull band
				band = dataset.GetRasterBand(channel+1)

				# iterate through each row
				for row in xrange(0, dataset.RasterYSize):

					# pull entire row
					scanline = band.ReadRaster( 0, row, band.XSize, 1, band.XSize, 1, GDT_Float32 )
					float_row = struct.unpack('f'*band.XSize, scanline)
					
					# iterate over each column
					for col in xrange(0, dataset.RasterXSize):
						self.image[row][col][channel] = float_row[col]
		
		# Make sure that if image is 16 bit and ABPP is 12, then scale
		if self.image.dtype == np.uint16 and int(dataset.GetMetadataItem('NITF_ABPP')) == 12:
			self.image = np.dot( self.image, 2**4 )



def GDAL2OpenCVDatatype( dataset ):
	
	# get the first data type
	datatype = gdal.GetDataTypeName( dataset.GetRasterBand(1).DataType)
	
	# iterate through each datatype, ensuring they are all the same
	for x in xrange( 2, dataset.RasterCount+1 ):
		if datatype is not gdal.GetDataTypeName( gdalGetRasterBand(x)):
			raise Exception('Image has bands with different datatypes')

	
	# take the datatype and convert to opencv type
	if   datatype == 'UInt8':
		if   dataset.RasterCount == 1:
			return cv2.CV_8UC1
		elif dataset.RasterCount == 3:
			return cv2.CV_8UC3

	elif datatype == 'UInt16':
		if   dataset.RasterCount == 1:
			return np.uint16
		elif dataset.RasterCount == 3:
			return cv2.CV_16UC3
		
	else:
		raise Exception('Unsupported OpenCV Datatype, GDAL input: ' + str(datatype))



	raw_input('hello')
		
	

