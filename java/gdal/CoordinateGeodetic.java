/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package navigationsimulator.geographic;

/**
 *
 * @author Marvin Smith
 */
public class CoordinateGeodetic extends CoordinateBase {
    
    // latitude in decimal degrees
    private double m_latitude;
    
    // longitude in decimal degrees
    private double m_longitude;
    
    /**
     * Default Constructor
     * Latitude=0, Longitude=0, Altitude=0, Datum=WGS84
     */
    public CoordinateGeodetic(){
        
        // create parent
        super(0, Datum.WGS84);
        
        // create latitude
        this.m_latitude = 0;
        
        // create longitude
        this.m_longitude = 0;
        
    }
    
    /**
     * Parameterized Constructor
     * 
     * @param latitude Latitude in degrees
     * @param longitude Longitude in degrees
     */
    public CoordinateGeodetic( double latitude, double longitude ){
        
        // create parent
        super(0, Datum.WGS84);
        
        // create latitude
        this.m_latitude = latitude;
        
        // set longitude
        this.m_longitude = longitude;
        
    }
    
    /**
     * Parameterized Constructor
     * 
     * @param latitude Latitude in degrees
     * @param longitude Longitude in degrees
     * @param altitude Altitude in meters
     */
    public CoordinateGeodetic( double latitude, double longitude, double altitude ){
        
        // set parent
        super( altitude, Datum.WGS84 );
        
        // set latitude
        this.m_latitude = latitude;
        
        // set longitude
        this.m_longitude = longitude;
        
    }
    
    /**
     * Set latitude
     * @param latitude Latitude in degrees
     */
    public void setLatitude( double latitude ){
        this.m_latitude = latitude;
    }
    
    /**
     * Get latitude
     * @return latitude in degrees
     */
    public double getLatitude( ){
        return m_latitude;
    }
    
    /**
     * Set Longitude
     * @param longitude Longitude in decimal degrees
     */
    public void setLongitude( double longitude ){
        this.m_longitude = longitude;
    }

    /**
     * Get Longitude
     * @return Longitude in decimal degrees
     */
    public double getLongitude(){
        return m_longitude;
    }
    
    /**
     * Return the geographic coordinate type
     * @return GeographicCoordinateType for Geodetic
     */
    @Override
    public GeographicCoordinateType type(){
        return GeographicCoordinateType.Geodetic;
    }
    
    @Override
    public CoordinateGeodetic clone(){
        return new CoordinateGeodetic(this.m_latitude, this.m_longitude, this.getAltitude());
    }
}
