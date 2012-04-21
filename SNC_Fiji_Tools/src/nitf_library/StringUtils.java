/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import java.io.File;
/**
 *
 * @author marvin_smith1
 */
public class StringUtils {
 
    public final static String ntf = "ntf";
    public final static String NTF = "NTF";
    public final static String jpg = "jpg";
    
    /*
     * Get the extension of a file.
     */  
    public static String getExtension(File f) {
        String ext = null;
        String s = f.getName();
        int i = s.lastIndexOf('.');

        if (i > 0 &&  i < s.length() - 1) {
            ext = s.substring(i+1).toLowerCase();
        }
        return ext;
    }
    
}
