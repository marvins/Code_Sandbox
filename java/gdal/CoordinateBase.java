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
public class CoordinateBase {
    
    private double m_altitude;
    private Datum  m_datum;
    
    
    /**
     * Default Constructor
     * 
     * Note: Altitude defaults to 0 and Datum to WGS84
     */
    public CoordinateBase(){
        this.m_altitude = 0;
        this.m_datum = Datum.WGS84;
    }
    
    /**
     * Parameterized Constructor given Altitude
     * 
     * @param altitude Altitude in meters
     */
    public CoordinateBase( double altitude ){
        m_altitude = altitude;
        m_datum    = Datum.WGS84;
    }
    
    /**
     * Parameterized Constructor given Altitude and Datum.
     * 
     * @param altitude Altitude in meters.
     * @param datum    Datum for ellipsoid.
     */
    public CoordinateBase( double altitude, Datum datum ){
        this.m_altitude = 0;
        // set the altitude
        m_altitude = altitude;
        m_datum = Datum.WGS84;
    }
    
    /**
     * Set the altitude
     * @param altitude  Altitude in meters
     */
    public void setAltitude( double altitude ){
        m_altitude = altitude;
    }
    
    /**
     * Get the altitude
     * @return Altitude in meters
     */
    public double getAltitude(){
        return m_altitude;
    }
    
    /**
     * Set the datum
     * @param datum Datum
     */
    public void setDatum( Datum datum ){
        m_datum = datum;
    }
    
    /**
     * Get the datum
     * @return Datum
     */
    public Datum getDatum(){
        return this.m_datum;
    }
    
    /**
     * Get the type
     * @return Type of coordinate
     */
    public GeographicCoordinateType type(){
        return GeographicCoordinateType.Base;
    }
    
    
    
}
