/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import ij.ImagePlus;

import javax.swing.*;

/**
 *
 * @author marvin_smith1
 */
public class NITF_Loader {

    static public ImagePlus read_NTIF(String filename, JComponent panel ) {

        /*   Load header information in from GDAL  */
        GDAL_Info info = new GDAL_Info(filename);
        
        //initialize the image depending on the channel count and pixel type
        if( info.rasterCount == 1 && info.ptype == PixelType.Byte ){
            System.out.println("Loading Byte C1");
            return info.loadImageByte(panel);
        }
        else if( info.rasterCount == 1 && info.ptype == PixelType.UInt12 ){
            System.out.println("Loading UInt12 C1");
            return info.loadImageShort( PixelType.UInt12, panel );
        }
        else if( info.rasterCount == 1 && info.ptype == PixelType.UInt14 ){
            System.out.println("Loading UInt14 C1");
            return info.loadImageShort( PixelType.UInt14, panel );
        }
        else if( info.rasterCount == 1 && info.ptype == PixelType.UInt16 ){
            System.out.println("Loading UInt16 C1");
           return info.loadImageShort( PixelType.UInt16, panel );
        }
        else if( info.rasterCount == 3 && info.ptype == PixelType.Byte){
            System.out.println("Loading Byte C3");
            return info.loadImageByteC3( PixelType.Byte, panel );
        }
        else
            throw new IllegalArgumentException("Error: type not implemented: " + info.rasterCount + " Channels, ptype: " + info.ptype.getName());
        
    }
}


/*


  public void run(String arg) {
    
    OpenDialog od = new OpenDialog("Open image ...", arg);
    String directory = od.getDirectory();
    String fileName = od.getFileName();
    if (fileName==null) return;
  
    IJ.showStatus("Opening: " + directory + fileName);
        
    read(directory,fileName);
          
    if (img==null) return;
      
    img.show();      
    
  }


  protected void read(String dir, String filename)
  {
    try {
          
      BufferedReader br = new BufferedReader(new FileReader(dir+filename));
      
      String line;
      Vector lines = new Vector();
      
      line = br.readLine();
      
      while (line!=null) {
        lines.add(line);
        line = br.readLine();
      }
      
      int height = lines.size();
      int width = getLineAsArray((String)lines.elementAt(0)).length;
      
      img = NewImage.createByteImage(filename, width, height, 1, NewImage.FILL_BLACK);
      
      byte[] pixels = (byte[]) img.getProcessor().getPixels();
      
      for (int i=0;i<height;i++) {
        short [] values = getLineAsArray((String)lines.elementAt(i));
        
        int offset = i*width;
        
        for (int j=0;j<width;j++) {
          pixels[offset+j] = (byte) values[j];
        }
      }
      
      
    } catch (Exception e) {
      IJ.error("Simple ASCII Reader", e.getMessage());
      return;
    }
    
  }
  
  protected short[] getLineAsArray(String line) {
    StringTokenizer tokenizer = new StringTokenizer(line," ");
    int size = tokenizer.countTokens();
    
    short[] values = new short[size];
    
    int i = 0;
    
    while (tokenizer.hasMoreTokens()) {
      String token = tokenizer.nextToken();
      
      values[i] = (new Short(token)).shortValue();
      
      i++;
      
    }
    
    return values;
    
  }
  
  

}*/