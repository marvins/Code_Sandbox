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


/**
 *
 * @author marvin_smith1
 */
public class GDAL_FileChooser {
    
    static public List<String> getImageFilenames( JComponent frame, boolean multipleFiles, String currentDirectory ){
        
        List<String> list = new ArrayList<>();
                
        JFileChooser fc = new JFileChooser();
        fc.setMultiSelectionEnabled(multipleFiles);
        fc.setFileFilter(new GDAL_FileFilter());
        fc.setCurrentDirectory(new File(currentDirectory));
        fc.setDialogTitle("Select Images to Load");
        
        fc.showOpenDialog(frame);
        
        if( multipleFiles){
            File[] filelist = fc.getSelectedFiles();
            
            for(int i=0; i<filelist.length; i++)
                list.add(filelist[i].getAbsolutePath());
        }
        else{
            File file = fc.getSelectedFile();
            
            list.add(file.getName());
        }
        
        
        return list;
    }
    
}
