/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import org.gdal.gdalconst.gdalconstConstants;

/**
 *
 * @author ms6401
 */
public enum PixelType {
    
    Byte("Byte"),
    UInt12("UInt12"),
    UInt14("UInt14"),
    UInt16("UInt16"),
    UInt32("UInt32"),
    Unknown("Unknown");
    
    private final String typeName;
    
    private PixelType( String tn ){
        typeName = tn;
    }
    
    static public PixelType getType( String tp ){
        
        if( tp.compareTo("UInt16") == 0 )
            return UInt16;
        
        if( tp.compareTo("UInt14") == 0 )
            return UInt14;
        
        if( tp.compareTo("UInt12") == 0 )
            return UInt12;
        
        if( tp.compareTo("UInt32") == 0 )
            return UInt32;
        
        return Byte;
        
    }
    
    static public PixelType getType( String tp, double minP, double maxP ){
 
        if( tp.compareTo("UInt16") == 0 && minP >= 0 && maxP < 4096 )
            return UInt12;
        
        if( tp.compareTo("UInt16") == 0 && minP >= 0 && maxP < 16384 )
            return UInt14;
        
        if( tp.compareTo("UInt16") == 0 )
            return UInt16;
        
        if( tp.compareTo("UInt14") == 0 )
            return UInt14;
        
        if( tp.compareTo("UInt12") == 0 )
            return UInt12;
        
        if( tp.compareTo("UInt32") == 0 )
            return UInt32;
        
        return Byte;
        
    }
    
    public int GDAL_Type(){
        
        if( this == Byte)
            return gdalconstConstants.GDT_Byte;
        
        if( this == UInt12)
            return gdalconstConstants.GDT_UInt16;
        
        if( this == UInt14)
            return gdalconstConstants.GDT_UInt16;
        
        if( this == UInt16)
            return gdalconstConstants.GDT_UInt16;
        
        if( this == UInt32 )
            return gdalconstConstants.GDT_UInt32;
        
        throw new IllegalArgumentException("TYPE NOT SUPPORTED");
        
    }
    
    public String getName(){ return typeName; }
    
    public static float setPixelGivenTypes( int value, PixelType inputType, PixelType outputType ){
        
        if( inputType == outputType )
            return value;
        
        else if( inputType == Byte && outputType == UInt32 ){
            return (value*16777215);
        }
        else if( inputType == Byte && outputType == UInt16 ){
            return (value*256);
        }
        else if( inputType == Byte && outputType == UInt14 ){
            return (value*64);
        }
        else if( inputType == Byte && outputType == UInt12 ){
            return (value*16);
        }
        else if( inputType == UInt16 && outputType == Byte ){
            return (value/(float)265);
        }
        else if( inputType == UInt16 && outputType == UInt12 ){
            return (value/(float)16);
        }
        else if( inputType == UInt16 && outputType == UInt14 ){
            return (value/(float)4);
        }
        else if( inputType == UInt16 && outputType == UInt32 ){
            return (value*2);
        }
        else if( inputType == UInt32 && outputType == UInt16 ){
            return (value/(float)65536);
        }
        else if( inputType == UInt32 && outputType == UInt14 ){
            return (value/(float)262144);
        }
        else if( inputType == UInt32 && outputType == UInt12 ){
            return (value/(float)1048576);
        }
        else if( inputType == UInt32 && outputType == Byte ){
            return ((float)value);///(float)16777215);
        }
        else 
            throw new IllegalArgumentException("Invalid type");
                  
    }
    
}