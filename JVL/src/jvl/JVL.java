/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvl;

import jvl.image_type.*;
import jvl.channel_type.*;
//import jvl.pixel_type.*;

import java.util.*;

/**
 *
 * @author marvin_smith1
 */
public class JVL {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        
        //create an image
        //ImageType<RGB<Double>>  image1;// = new ImageType<RGB<Double>>(5,5);
        ImageType<RGB<Double>> image2 = new ImageType<RGB<Double>>(10, 10, new RGB<Double>(1d,1d,1d));
        
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                image2.setPixel(j, i, new RGB<Double>((double)i,(double)j,(double)(i+j)));
            }
        }
        
        //Show what we have
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                System.out.print(image2.getPixel(j,i) + ", ");
            }
            System.out.println("");
        }
        
    }
}
