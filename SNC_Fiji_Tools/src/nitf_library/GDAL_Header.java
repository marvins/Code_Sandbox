/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import java.io.File;

import java.awt.*;
import javax.swing.*;
import javax.swing.text.*;

import ij.*;
import ij.process.*;

/**
 *
 * @author ms6401
 */
public class GDAL_Header {
    
    public PixelType pixel;
    public String image_filename = "image.ntf";
    public File header_file;
    
    public final String GDALHeaderType = "NTIF";
    public String original_filename = ".";
    
    public int nChannels = 1;
    
    public PixelType fijiPixelType;
    public int fijiNChannels;
    
    public static String[] pixelTypes = {"Unknown","Byte", "UInt16", "UInt32"};
    public static String[] channelCnt = {"1 (GRAYSCALE)", "3 (RGB, Color)"};
    
    public GDAL_Header( ImageProcessor ip ){
        
        nChannels = ip.getNChannels();
        
        ImagePlus img = new ImagePlus("img", ip);
        if( img.getType() == ImagePlus.GRAY8 ){
            System.out.println("Gray 8");
            fijiPixelType = PixelType.Byte;
            fijiNChannels = 1;
        }
        else if( img.getType() == ImagePlus.GRAY16 ){
            System.out.println("Gray 16");
            fijiPixelType = PixelType.UInt16;
            fijiNChannels = 1;
        }
        else if( img.getType() == ImagePlus.GRAY32 ){
            System.out.println("Gray 32");
            fijiPixelType = PixelType.UInt32;
            fijiNChannels = 1;
        }
        else if( img.getType() == ImagePlus.COLOR_RGB ){
            System.out.println("Color RGB");
            fijiPixelType = PixelType.Byte;
            fijiNChannels = 3;
        }
        else{
            System.out.println("UNKNOWN at : " + img.getType() );
            pixel = PixelType.Unknown;
        }
        
        pixel = fijiPixelType;
        nChannels = fijiNChannels;
        
        
    }
    
    public int getPixelTypeIndex( ){
        if( pixel == PixelType.Byte )
            return 1;
        //if( pixel == PixelType.UInt12 )
        //    return 2;
        //if( pixel == PixelType.UInt14 )
        //    return 3;
        if( pixel == PixelType.UInt16 )
            return 2;
        if( pixel == PixelType.UInt32 )
            return 3;
        
        else
            return 0;
    }
    
}
