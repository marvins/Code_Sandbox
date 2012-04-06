/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvl.image_type;

import jvl.channel_type.*;

import java.lang.*;
import java.util.*;

/**
 *
 * @author marvin_smith1
 */
public class ImageType< CType extends ChannelType> {

    ArrayList<ArrayList<CType>> pixels;
    CType ptype;
    public int rows;
    public int cols;

    public ImageType( CType val) {

        
            //initialize variables
            cols = 1;
            rows = 1;
            ptype = val;

            //initialize pixel data
            pixels = new ArrayList<ArrayList<CType>>();
            for (int i = 0; i < cols; i++) {

                pixels.add(new ArrayList<CType>());
                for (int j = 0; j < rows; j++) {
                    pixels.get(i).add(ptype);
                }
            }
    }

    /**
     * Parameterized constructor
     *
     * @param width image width in pixels
     * @param height image height in pixels
     */
    public ImageType(int width, int height, CType val ) {

        
        //initialize variables
        cols = width;
        rows = height;
        ptype = val;

        //initialize pixel data
        pixels = new ArrayList<ArrayList<CType>>();
        for (int i = 0; i < width; i++) {

            pixels.add(new ArrayList<CType>());
            for (int j = 0; j < height; j++) {
                pixels.get(i).add(ptype);
            }
        }
        
    }

    /**
     * Access the pixel at location c, r
     *
     * @param c column in image
     * @param r row in image
     * @return pixel value
     */
    public CType getPixel(int c, int r) {
        return (CType)pixels.get(c).get(r).copy();
    }
    
    public void setPixel(int c, int r, CType pix ){
        pixels.get(c).set(r, (CType)pix.copy());
    }
}
