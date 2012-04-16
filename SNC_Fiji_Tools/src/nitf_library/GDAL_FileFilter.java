/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import java.io.File;
import javax.swing.filechooser.FileFilter;

/**
 *
 * @author marvin_smith1
 */
public class GDAL_FileFilter extends FileFilter {
    
    @Override
    public boolean accept( File f ){
        if( f.isDirectory()){
            return true;
        }
        
        String extension = StringUtils.getExtension(f);
        
        if( extension == null)
            return false;
        
        if( extension.equals(StringUtils.ntf) ||
            extension.equals(StringUtils.NTF) ||
            extension.equals(StringUtils.jpg)){
            return true;
        }
        else
            return false;
    }
    
    public String getDescription() {
        return "Just Images";
    }
    
}
