/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package navigationsimulator.geographic;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;


/**
 *
 * @author Marvin Smith
 */
public class KMLWriter {
    
    private LinkedList<CoordinateGeodetic> pointList;
    
    /**
     * Default Constructor
     */
    public KMLWriter(){
        
        // create the list
        pointList = new LinkedList<CoordinateGeodetic>();
        
    }
    
    /**
     * Add a point
     */
    public void addPoint( CoordinateGeodetic point ){
        pointList.add(point.clone());
    }
    
    /**
     * Write file
     * @param filename  Name of file to write to
     */
    public void writeFile( String filename ){
        
        // create the file object
        try{
            File file = new File(filename);
            BufferedWriter output = new BufferedWriter( new FileWriter(file));
            
            output.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            output.write("<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
            output.write("<Document>\n");
            output.write("  <name>Telemetry Simulator Output</name>\n");
            output.write("  <description>See Nav Sim</description>\n");
            output.write("  <Style id=\"yellowLineGreenPoly\">\n");
            output.write("  <LineStyle>\n");
            output.write("    <color>7f00ffff</color>\n");
            output.write("    <width>4</width>\n");
            output.write("  </LineStyle>\n");
            output.write("  <PolyStyle>\n");
            output.write("    <color>7f00ff00</color>\n");
            output.write("  </PolyStyle>\n");
            output.write("  </Style>\n");
            
            output.write("  <Placemark>\n");
            output.write("     <name>Aircraft Flight Path</name>\n");
            output.write("     <description></description>\n");
            output.write("     <styleUrl>#yellowLineGreenPoly</styleUrl>\n");
            output.write("     <LineString>\n");
            output.write("        <extrude>1</extrude>\n");
            output.write("        <tessellate>1</tessellate>\n");
            output.write("        <altitudeMode>absolute</altitudeMode>\n");
            output.write("        <coordinates>\n");
            
            // iterate over the coordinates
            Iterator iterator = pointList.iterator();
            while(iterator.hasNext()){
                
                CoordinateGeodetic point = (CoordinateGeodetic)iterator.next();
                output.write("     " + point.getLongitude() + "," + point.getLatitude() + "," + point.getAltitude() + "\n");
                
            }
            
            output.write("        </coordinates>\n");
            output.write("     </LineString>\n");
            output.write("  </Placemark>\n");
            output.write("  </Document>\n");
            output.write("</kml>");
            
            output.close();
        } catch (IOException e ){
            e.printStackTrace();
        }
    }
}
