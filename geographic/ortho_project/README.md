This is the readme for the test orthorectification pipeline. 

To use, modify the config file in data/options.cfg

Build and run

make clean
make
./bin/orthoproject


To change the parameters, change the rotation angle of the camera and the axis.  Also, feel free to change the camera position, but that won't have any effect until I start modifying geographic imagery.

OBJECTIVES:
 - Need to validate the detection of image corners
    * Change the image view given test image. 
    * Change the image view given geo image.



