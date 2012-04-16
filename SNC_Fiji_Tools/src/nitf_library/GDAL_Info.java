/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import java.io.*;
import java.util.*;

import org.gdal.gdal.Band;
import org.gdal.gdal.Dataset;
import org.gdal.gdal.Driver;
import org.gdal.gdal.gdal;
import org.gdal.gdalconst.gdalconstConstants;

import ij.*;
import ij.process.*;
import ij.process.ByteProcessor;
import ij.process.ShortProcessor;
import ij.process.ColorProcessor;
import org.gdal.gdalconst.gdalconst;

import javax.swing.*;
import javax.swing.filechooser.*;

/**
 *
 * @author ms6401
 */
public class GDAL_Info {

    //GDAL Dataset info
    public Dataset hDataset;
    public Driver hDriver;
    public String filename;
    public int rasterXSize;
    public int rasterYSize;
    public int rasterCount;
    public PixelType ptype;

    public GDAL_Info(String ifilename) {

        //set filename
        filename = ifilename;

        //open GDAL
        gdal.AllRegister();

        //load dataset
        hDataset = gdal.Open(filename, gdalconstConstants.GA_ReadOnly);

        //load driver
        hDriver = hDataset.GetDriver();

        //load number of bands
        rasterCount = hDataset.getRasterCount();
        if (rasterCount < 1) {
            throw new IllegalArgumentException("ERROR: NITF Image must have at least 1 band");
        }

        //load raster size
        rasterXSize = hDataset.getRasterXSize();
        rasterYSize = hDataset.getRasterYSize();

        //get max pixel values for each raster
        double adfCMinMax[] = new double[2];
        double maxP = 0, minP = 0;
        for (int i = 0; i < rasterCount; i++) {

            //pull band and compute max/min
            hDataset.GetRasterBand(i + 1).ComputeRasterMinMax(adfCMinMax);
            if (i == 0) {
                minP = adfCMinMax[0];
                maxP = adfCMinMax[1];
            }
            if (adfCMinMax[0] < minP) {
                minP = adfCMinMax[0];
            }
            if (adfCMinMax[1] > maxP) {
                maxP = adfCMinMax[1];
            }
        }

        ptype = PixelType.getType(gdal.GetDataTypeName(hDataset.GetRasterBand(1).getDataType()), minP, maxP);

    }

    /**
     * @return ImageJ ImagePlus object
     */
    ImagePlus loadImageByte(JComponent panel) {

        /**
         * Iterate through the first band, setting values
         */
        Band channel = hDataset.GetRasterBand(1);
        if (channel.GetXSize() != rasterXSize) {
            throw new IllegalArgumentException("Error: Band X does not match overall size");
        }
        if (channel.GetYSize() != rasterYSize) {
            throw new IllegalArgumentException("Error: Band Y does not match overall size");
        }

        //iterate over each column
        ByteProcessor pixels = new ByteProcessor(rasterXSize, rasterYSize);
        ImagePlus imgOut = new ImagePlus(filename, pixels);

        for (int i = 0; i < rasterYSize; i++) {

            //load raster for column
            int[] data = new int[rasterXSize];
            channel.ReadRaster(0, i, rasterXSize, 1, data);

            for (int j = 0; j < rasterXSize; j++) {

                pixels.putPixel(j, i, (byte) data[j]);
            }
        }

        write_header_info(panel);
        return imgOut;
    }

    /**
     * @return ImageJ ImagePlus object
     */
    public ImagePlus loadImageShort(PixelType pix, JComponent panel) {

        /**
         * Iterate through the first band, setting values
         */
        Band channel = hDataset.GetRasterBand(1);
        if (channel.GetXSize() != rasterXSize) {
            throw new IllegalArgumentException("Error: Band X does not match overall size");
        }
        if (channel.GetYSize() != rasterYSize) {
            throw new IllegalArgumentException("Error: Band Y does not match overall size");
        }

        //iterate over each column
        ShortProcessor pixels = new ShortProcessor(rasterXSize, rasterYSize);
        ImagePlus imgOut = new ImagePlus(filename, pixels);

        for (int i = 0; i < rasterYSize; i++) {

            //load raster for column
            int[] data = new int[rasterXSize];
            channel.ReadRaster(0, i, rasterXSize, 1, data);

            for (int j = 0; j < rasterXSize; j++) {
                if (pix == PixelType.UInt12) {
                    pixels.putPixel(j, i, data[j] * 8);
                } else if (pix == PixelType.UInt14) {
                    pixels.putPixel(j, i, data[j] * 4);
                } else {
                    pixels.putPixel(j, i, data[j]);
                }
            }
        }

        write_header_info(panel);
        return imgOut;
    }

    void write_header_info(JComponent panel) {

        JFileChooser fc = new JFileChooser();
        fc.setMultiSelectionEnabled(false);
        fc.setCurrentDirectory(new File("."));
        fc.setFileHidingEnabled(false);
        fc.setDialogTitle("Select a file to save current header information to");
        fc.setApproveButtonText("SAVE");
        File file;

        int returnVal = fc.showOpenDialog(panel);

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            file = fc.getSelectedFile();

            //create list of metadata
            Vector metaDataList = hDataset.GetMetadata_List();
            Enumeration keys = metaDataList.elements();
            System.out.println("MetaData: ");

            try {

                //create filewriter
                FileWriter fw = new FileWriter(file);

                //write the filename
                fw.write((new File(filename)).getAbsolutePath() + "\n");

                //write the pixeltype
                fw.write(ptype.getName() + "\n");

                //write the number of pixels
                fw.write(hDataset.GetRasterCount() + "\n");

                for (int i = 0; i < metaDataList.size(); i++) {
                    fw.write((String) keys.nextElement() + "\n");
                }


                //close
                fw.close();

            } catch (IOException ex) {
                System.out.println("Could not write to file");
                System.out.println(ex.getMessage());
                IJ.error("ERROR: Cound not write to file");
                IJ.error(ex.getMessage());
            }
        } else {
            IJ.error("File not selected, not saving header information");
        }

    }

    ImagePlus loadImageByteC3(PixelType ptype, JComponent panel) {

        /**
         * Iterate through the first band, setting values
         */
        Band channelR = hDataset.GetRasterBand(1);
        Band channelG = hDataset.GetRasterBand(2);
        Band channelB = hDataset.GetRasterBand(3);
        if (channelR.GetXSize() != rasterXSize || channelR.GetYSize() != rasterYSize) {
            throw new IllegalArgumentException("Error: Red Band does not match overall size");
        }
        if (channelG.GetXSize() != rasterXSize || channelG.GetYSize() != rasterYSize) {
            throw new IllegalArgumentException("Error: Green Band does not match overall size");
        }
        if (channelB.GetXSize() != rasterXSize || channelB.GetYSize() != rasterYSize) {
            throw new IllegalArgumentException("Error: Blue Band does not match overall size");
        }

        //iterate over each column
        ColorProcessor pixels = new ColorProcessor(rasterXSize, rasterYSize);
        ImagePlus imgOut = new ImagePlus(filename, pixels);

        for (int i = 0; i < rasterYSize; i++) {

            //load raster for column
            int[] dataR = new int[rasterXSize];
            int[] dataG = new int[rasterXSize];
            int[] dataB = new int[rasterXSize];

            channelR.ReadRaster(0, i, rasterXSize, 1, dataR);
            channelG.ReadRaster(0, i, rasterXSize, 1, dataG);
            channelB.ReadRaster(0, i, rasterXSize, 1, dataB);

            int[] PIX = new int[3];
            for (int j = 0; j < rasterXSize; j++) {
                PIX[0] = dataR[j];
                PIX[1] = dataG[j];
                PIX[2] = dataB[j];

                pixels.putPixel(j, i, PIX);
            }
        }

        write_header_info(panel);
        return imgOut;
    }

    public static boolean write(GDAL_Header header_info, ImageProcessor ip) {

        System.out.println("Output Filename: " + header_info.image_filename);
        System.out.println("Input N Channels: " + header_info.fijiNChannels);
        System.out.println("OUtput N Channel: " + header_info.nChannels);
        
        System.out.println("Input pixeltype: " + header_info.fijiPixelType.getName());
        System.out.println("Output pixeltype: " + header_info.pixel.getName());
        
        //make sure you don't allow unknown pixel types
        if (header_info.pixel == PixelType.Unknown) {
            IJ.error("Error: Pixeltype is unknown, cancelling");
            return false;
        }

        //load gdal headers
        gdal.AllRegister();
        Driver driver = gdal.GetDriverByName("NITF");

        //check for the pixeltype
        Dataset dataset;
        if (header_info.pixel == PixelType.UInt16
                || header_info.pixel == PixelType.UInt14
                || header_info.pixel == PixelType.UInt12) {

            dataset = driver.Create(header_info.image_filename, ip.getWidth(), ip.getHeight(), header_info.fijiNChannels, gdalconst.GDT_UInt16);
        } else {
            dataset = driver.Create(header_info.image_filename, ip.getWidth(), ip.getHeight(), header_info.fijiNChannels, gdalconst.GDT_Byte);
        }

        //iterate through each channel band
        for (int i = 1; i < (header_info.fijiNChannels + 1); i++) {

            //pull raster
            Band band = dataset.GetRasterBand(i);
            float[] floatArray = new float[ip.getWidth()];
            int[] pixelAt = new int[3];
            
            for (int j = 0; j < ip.getHeight(); j++) {

                for (int k = 0; k < ip.getWidth(); k++) {
                    
                    ip.getPixel(k,j,pixelAt);
                    
                    if( header_info.fijiNChannels == header_info.nChannels ){
                        floatArray[k] = PixelType.setPixelGivenTypes( pixelAt[i-1], header_info.fijiPixelType, header_info.pixel);
                    }
                    else if( header_info.fijiNChannels == 3 && header_info.nChannels == 1 ){
                        floatArray[k]  = PixelType.setPixelGivenTypes( pixelAt[0], header_info.fijiPixelType, header_info.pixel);
                        floatArray[k] += PixelType.setPixelGivenTypes( pixelAt[1], header_info.fijiPixelType, header_info.pixel);
                        floatArray[k] += PixelType.setPixelGivenTypes( pixelAt[2], header_info.fijiPixelType, header_info.pixel);
                        floatArray[k] /= 3;
                        
                    }
                    else if( header_info.fijiNChannels == 1 && header_info.nChannels == 3 )
                        floatArray[k]  = PixelType.setPixelGivenTypes( pixelAt[0], header_info.fijiPixelType, header_info.pixel);
                    else 
                        throw new IllegalArgumentException("Error: invalid conversion");
                    
                }

                band.WriteRaster(0, j, ip.getWidth(), 1, floatArray);

            }
        }
        System.out.println("Write finished to " + header_info.image_filename);
        dataset.delete();
        //gdal.Unlink(header_info.image_filename);

        return true;

    }
}
