/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import ij.IJ;
import ij.ImagePlus;
import ij.plugin.filter.PlugInFilter;
import ij.process.ImageProcessor;

import javax.swing.*;

/**
 *
 * @author ms6401
 */
public class SaveImagePlugin implements PlugInFilter{
    
    @Override
    public int setup( String arg, ImagePlus imp){
        
        return DOES_ALL;
        
    }
    
    @Override
    public void run( ImageProcessor ip){
        
        
        /** We need to load the image header info */
        /** Prompt user to re-select original file */
        final ImageProcessor IP = ip;
        
        javax.swing.SwingUtilities.invokeLater(new Runnable(){
            public void run(){
                createAndShowGui( IP );
            }
        });
        
    }
    
    
    public void createAndShowGui( ImageProcessor ip ){
        
        JFrame frame = new JFrame("Save NITF Utility");
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        
        NITF_Writer writer = new NITF_Writer( ip );
        writer.setOpaque(true);
        frame.setContentPane(writer);
      
        frame.pack();
        frame.setVisible(true);
    }
    
}



