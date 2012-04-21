/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JComponent;
import javax.swing.JFileChooser;

import ij.IJ;
import ij.ImagePlus;
import ij.plugin.filter.PlugInFilter;
import ij.process.ImageProcessor;

import java.io.*;
import java.util.Scanner;

/**
 *
 * @author ms6401
 */
public class Save_FileChooser {

    static public void loadHeaderData(JComponent frame, String currentDirectory, GDAL_Header header_data) {

        JFileChooser fc = new JFileChooser();
        fc.setMultiSelectionEnabled(false);
        fc.setCurrentDirectory(new File(currentDirectory));
        fc.setFileHidingEnabled(false);
        File file;

        int returnVal = fc.showOpenDialog(frame);

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            file = fc.getSelectedFile();

            try {
                //open the file and evaluate the contents
                Scanner loader = new Scanner(file);
                
                header_data.original_filename = loader.next();
                header_data.image_filename = header_data.original_filename;
                
                String ptype = loader.next();
                header_data.pixel = PixelType.getType( ptype );
                
            } catch (IOException ex) {
                IJ.error("Error: File error occured, cancelling load");
                System.out.println(ex.getMessage());
            }

        } else {
            IJ.error("ERROR: No File Chosen, quitting");
            file = null;
        }

    }
    public static void selectImageName( JComponent frame, GDAL_Header header_data ){
        
        JFileChooser fc = new JFileChooser();
        fc.setMultiSelectionEnabled(false);
        fc.setCurrentDirectory(new File(header_data.original_filename));
        fc.setFileHidingEnabled(false);
        fc.setDialogTitle("Select file to save");
        File file;

        int returnVal = fc.showOpenDialog(frame);

        if (returnVal == JFileChooser.APPROVE_OPTION) {
            header_data.image_filename = fc.getSelectedFile().getAbsolutePath();

        } else {
            IJ.error("ERROR: No File Chosen, quitting");
            
        }
        
    }
}