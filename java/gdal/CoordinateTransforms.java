/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package navigationsimulator.geographic;

import org.gdal.osr.CoordinateTransformation;
import org.gdal.osr.SpatialReference;

/**
 *
 * @author Marvin Smith
 */
public class CoordinateTransforms {
    
    /**
     * Compute the required UTM Zone given a Geodetic Coordinate
     */
    static int getUTMZone( CoordinateGeodetic coordinate ){
        
        return (int) (Math.floor((coordinate.getLongitude() + 180)/6.0)+1);
    }
    
    /**
     * Convert a coordinate with a fixed UTM Zone
     */
    static public CoordinateBase convert( CoordinateBase inputCoordinate, GeographicCoordinateType outputType ){
        
        return convert(inputCoordinate, outputType, inputCoordinate.getDatum(), -1);
    }
    
    /**
     * Convert a coordinate with a fixed UTM Zone
     */
    static public CoordinateBase convert( CoordinateBase inputCoordinate, GeographicCoordinateType outputType, Datum outputDatum ){
        
        return convert(inputCoordinate, outputType, outputDatum, -1);
    }
    
    /**
     * Convert a coordinate from one Coordinate Type to another.
     * @param inputCoordinate Coordinate to convert
     * @param outputType Output Coordinate Type to convert to
     * @param outputDatum Datum to convert to
     * @return Converted Coordinate
     */
    static public CoordinateBase convert( CoordinateBase inputCoordinate, GeographicCoordinateType outputType, Datum outputDatum, int fixedUTMZone ){
        
        /**
         * If the input coordinate type is the same type as the output type and has the same datum, then just clone itself.
         */
        if( inputCoordinate.type() == outputType && inputCoordinate.getDatum() == outputDatum ){
            return inputCoordinate;
        }
        
        // Create the output coordinate
        SpatialReference src = new SpatialReference("");
        SpatialReference dst = new SpatialReference("");
        
        // if we have a UTM To Lat/Lon Conversion
        if( inputCoordinate.type() == GeographicCoordinateType.UTM && outputType == GeographicCoordinateType.Geodetic ){
            src.SetWellKnownGeogCS(inputCoordinate.getDatum().getWellKnownCS());
            src.SetUTM(((CoordinateUTM)inputCoordinate).getZone(), ((CoordinateUTM)inputCoordinate).isNorth());
            
            dst.SetWellKnownGeogCS(outputDatum.getWellKnownCS());
            
            CoordinateTransformation ct = CoordinateTransformation.CreateCoordinateTransformation(src, dst);
            
            // create the output point
            CoordinateUTM coord = (CoordinateUTM)inputCoordinate;
            CoordinateGeodetic output = new CoordinateGeodetic();
            double[] points = ct.TransformPoint(coord.getEasting(), coord.getNorthing(), coord.getAltitude());
            
            output.setLatitude(points[1]);
            output.setLongitude(points[0]);
            output.setAltitude(points[2]);
            output.setDatum(outputDatum);
            
            return output;
        }
        // if we have a Lat/Lon to UTM Conversion
        else if( inputCoordinate.type() == GeographicCoordinateType.Geodetic && outputType == GeographicCoordinateType.UTM ){
            
            // set the conversion coordinate system
            src.SetWellKnownGeogCS(inputCoordinate.getDatum().getWellKnownCS());
            
            dst.SetWellKnownGeogCS(outputDatum.getWellKnownCS());
            
            // compute the required UTM Zone
            int utmZone = getUTMZone((CoordinateGeodetic)inputCoordinate);
            if( fixedUTMZone != -1 ){
                utmZone = fixedUTMZone;
            }
            int isNorth = 1;
            if(((CoordinateGeodetic)inputCoordinate).getLatitude() < 0){
                isNorth = 0;
            }
            dst.SetUTM(utmZone, isNorth);
            
            CoordinateTransformation ct = CoordinateTransformation.CreateCoordinateTransformation(src, dst);
            
            // create the output point
            CoordinateGeodetic coord = (CoordinateGeodetic)inputCoordinate;
            CoordinateUTM output = new CoordinateUTM();
            double[] points = ct.TransformPoint(coord.getLongitude(), coord.getLatitude(), coord.getAltitude());
            
            output.setEasting(points[0]);
            output.setNorthing(points[1]);
            output.setAltitude(points[2]);
            output.setZone(utmZone);
            output.setDatum(outputDatum);
            
            return output;
        }
        else{
            System.out.println("error: unknown Coordinate Conversion Type");
            System.exit(1);
        }
        
        
        return new CoordinateBase();
    
    }
    
    
}
