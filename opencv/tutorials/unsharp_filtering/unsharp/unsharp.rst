.. _unsharp:

Image Enhancement
********************

Goal
=====

In this tutorial you will learn how to:

.. container:: enumeratevisibleitemswithsquare

   * Use basic filtering operations to enhance image details

Theory
=======

#. *Unsharp Masking* is an image enhancement technique commonly applied in modern photo 
    manipulation software.  Image enhancement or `image sharpening`, seeks to exaggerate
    image details such as edges and color gradients. 

    a. Unsharp masking achieves this by detecting regions with low detail and removing that detail
       from the image. Regions with high amounts of change will remain, allowing for the remaining 
       detail to be returned to the original image. The figure below illustrates this.

       +----------+----------+---------+
       |  |TXT01| |  |TXT02| | |TXT03| |
       +----------+----------+---------+
        Image




.. |TXT01| image:: images/original.jpg

.. |TXT02| image:: images/highpass.jpg

.. |TXT03| image:: images/sharpened.jpg




    This image is then subtracted from the original image, leaving high-frequency or "high-pass" information. By adding the remaining high-frequency information back to the original image, you can sharpen the image.

    .. math::

        \textup{Highpass} = \textup{Original} - \textup{Lowpass}
   
    .. math::
        \textup{Sharpened Image} = \textup{Original} + \textup{Highpass}

    
        
Algorithm
---------

#. Blur an image using a Gaussian Blur. This will produce the "low-pass" image.

    .. math::
        G(x,y) = \frac{1}{{\sigma \sqrt {2\pi } }}   e^{-\frac{x^2 + y^2}{2\sigma^2}} 
        
    * This equation will produce a kernel shown in the example below with :math:`\sigma = \sqrt{2}`        
    
    .. math::

        \textup{Kernel} = \begin{bmatrix}
        0.0382 & 0.0808 & 0.1038 & 0.0808 & 0.0382 \\
        0.0808 & 0.1711 & 0.2197 & 0.1711 & 0.0808 \\
        0.1038 & 0.2196 & 0.2821 & 0.2197 & 0.1038 \\
        0.0808 & 0.1711 & 0.2197 & 0.1711 & 0.0808 \\
        0.0382 & 0.0808 & 0.1038 & 0.0808 & 0.0382 
        \end{bmatrix}


#. Subtract the original image with the blurred image.  This will create a high-pass image.
    
#. Add the high-pass image back into the original image to produce the sharpened image.

    a. Because the high-pass image may not be enough, use an optional :math:`\alpha` scale factor. If :math:`\alpha` = 1, then it will be a simple high-boost filter.  If :math:`\alpha > 1`, then you will notice more sharpening effects. 
        
        .. math::
            \textup{Sharpened Image} = \textup{Original Image} + \alpha \cdot \textup{High-Pass Image}

#. For more information, refer to an Image Processing textbook or a popular Photoshop reference.

Code
=====

#.  Code Overview
    
    * Takes an image as input, which can be either 1 or 3 channel.
    * Takes an additional sigma value for blurring and sets the kernel width to 5 times sigma.
    * Applies the *Unsharp Mask* to produce a sharpened image.
    * Writes the sharpened image to file given the output filename. 



#. The tutorial code shown below. You can also download it from **HERE** 

.. `here <http://code.opencv.org/svn/opencv/trunk/opencv/samples/cpp/tutorial_code/ImgTrans/UnsharpMask_Demo.cpp>`_


.. code-block::  cpp

    //OpenCV Headers
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/imgproc/imgproc.hpp>

    //STL Headers
    #include <iostream>
    #include <sstream>
    #include <vector>
    
    using namespace cv;

    /**
     *  Compute the Unsharp Masked image. 
     *
     * @param[in] input_image  Image to be transformed
     * @param[in/out] output_image Transformed Image.  Will retain same CV Type.
     * @param[in] sigma Strength of Gaussian Blur.
     * @param[in] gauss_window Width of Gaussian Window.
     * @param[in] alpha Scale factor for high-pass image
     */
    void UnsharpFilter( Mat const& input_image, Mat& output_image, double sigma, Size const& gauss_window, double alpha ){

        //split image into channels
        std::vector<Mat> channels;
        split( input_image, channels );

        //iterate over each channel
        Mat gaussian, diff;
        for( size_t i=0; i<channels.size(); i++ ){

            //compute gaussian kernel
            GaussianBlur( channels[i], gaussian, gauss_window, sigma);

            //subtract images
            diff = channels[i] - gaussian;
        
            //add images back in
            channels[i] = channels[i] + alpha * diff;
        }

        //merge channels together
        Mat merged_image;
        merge( channels, merged_image );

        //return image to uchar
        merged_image.convertTo( output_image, input_image.type());
    }
    
    /**
     * Main Driver
     */
    int main( int argc, char* argv[] ){

        //simple error checking
        if( argc < 5 ){
            std::cout << "usage:  ./Unsharp  <input image> <output image> <sigma> <alpha>" << std::endl;
            return 0;
        }

        //load sigma
        double sigma = 1;
        std::stringstream sin;
        sin >> argv[3];
        sin << sigma;
        sin.clear();
        sin.str("");

        //load width
        int    width = 5*sigma;
    
        //load alpha
        double alpha = 9;
        sin >> argv[4];
        sin << alpha;
        sin.clear();
        sin.str("");

        //read image
        Mat image = imread( argv[1] );

        //apply Unsharp Mask Filter
        Mat imageout;
        UnsharpFilter( image, imageout, sigma, Size(width,width), alpha );

        //write result to file
        imwrite(argv[2], imageout);

        return 0;
    }



Explanation
============

#. Load parameters, load the image, and do simple error-checking

   .. code-block:: cpp
        
        //simple error checking
        if( argc < 5 ){
            std::cout << "usage:  ./Unsharp  <input image> <output image> <sigma> <alpha>" << std::endl;
            return 0;
        }

        //load sigma
        double sigma = 1;
        std::stringstream sin;
        sin >> argv[3];
        sin << sigma;
        sin.clear();
        sin.str("");

        //load width
        int    width = 5*sigma;
    
        //load alpha
        double alpha = 9;
        sin >> argv[4];
        sin << alpha;
        sin.clear();
        sin.str("");

        //read image
        Mat image = imread( argv[1], 0);

    Notes:
    
    a.  The kernel width is implied to be :math:`5 \times \sigma`


#. Call function
 
   .. code-block:: cpp
        
        //apply Unsharp Mask Filter
        Mat imageout;
        UnsharpFilter( image, imageout, sigma, Size(width,width), alpha );

#.  Break image into separate channels and process each channel individually
    
    .. code-block:: cpp

        //split image into channels
        std::vector<Mat> channels;
        split( input_image, channels );

        //iterate over each channel
        Mat gaussian, diff;
        for( size_t i=0; i<channels.size(); i++ ){



#.  Compute the Unsharp Image for each channel
    
    a. Compute the gaussian blur...
        
        .. code-block:: cpp


            //compute gaussian kernel
            GaussianBlur( channels[i], gaussian, gauss_window, sigma);
   
   b.  Subtract the blurred image from the original
        
        .. code-block:: cpp

            //subtract images
            diff = channels[i] - gaussian;
        
   c.  Add the high-pass image back with the original using the :math:`\alpha` scale factor

        .. code-block:: cpp

            //add images back in
            channels[i] = channels[i] + alpha * diff;
        

    d. Merge channels together and return image

        .. code-block:: cpp

            //merge channels together
            Mat merged_image;
            merge( channels, merged_image );

            //return image to uchar
            merged_image.convertTo( output_image, input_image.type());

    e.  Write results to file
        
        .. code-block:: cpp
            
            //write result to file
            imwrite(argv[2], imageout);
            


Result
=======

* To compile the program, simply use the command...

    .. code-block:: bash
        
        g++ UnsharpMash_Demo.cpp `pkg-config opencv --cflags --libs` -Wall

* To run, use
      
    .. code-block:: bash
        
        ./a.out input.png output.png 1.44 9

    a.  given no input, the program will output usage instructions.

*  Image results
    
    a.  Input Image
        
        .. image:: images/butterfly.jpg
            :alt: Original test image
            :width: 400pt
            :align: center
    
    b.  Blurred Image
        
        .. image:: images/low.png
            :alt: Blurred Image (low-pass)
            :width: 400pt
            :align: center
    
    c.  High-Pass Image (Scaled by factor of ten for demonstration purposes)
        
        .. image:: images/high.png
            :alt: Difference Image (high-pass)
            :width: 400pt
            :align: center

    d.  Result Image
        
        .. image:: images/result.png
            :alt: Resulting Image
            :width: 400pt
            :align: center

  


