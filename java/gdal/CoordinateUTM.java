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
public class CoordinateUTM extends CoordinateBase {
    
    // Easting component (X)
    private double m_easting;
    
    // Northing component (Y)
    private double m_northing;
        
    // UTM Zone
    private int m_zone;
    
    /**
     * Default Constructor
     *
     * Note:  Easting=0, Northing=0, Altitude=0, Datum=WGS84
     */
    public CoordinateUTM(){
        
        // set parent
        super(0, Datum.WGS84);
        
        // set easting
        this.m_easting = 0;
        
        // set northing
        this.m_northing = 0;
        
        // set zone
        this.m_zone = 31;
        
        
    }
    
    
    /**
     * Parameterized Constructor
     * @param easting Easting in meters
     * @param northing Northing in meters
     */
    public CoordinateUTM( int zone, double easting, double northing ){
        
        // set parent
        super( 0, Datum.WGS84 );
        
        // set zone
        m_zone = zone;
        
        // set easting
        this.m_easting = easting;
        
        // set northing
        this.m_northing = northing;
    }
    
    
    /**
     * Parameterized Constructor
     * 
     * @param easting Easting in meters
     * @param northing Northing in meters
     * @param altitude Altitude in meters.
     */
    public CoordinateUTM( int zone, double easting, double northing, double altitude ){
        
        // set parent
        super(altitude, Datum.WGS84);
        
        // set zone
        this.m_zone = zone;
        
        // set easting
        this.m_easting = easting;
        
        // set northing
        this.m_northing = northing;
        
    }
    
    /**
     * Parameterized Constructor
     * @param easting Easting in meters
     * @param northing Northing in meters
     * @param altitude Altitude in meters
     * @param datum Datum
     */
    public CoordinateUTM( int zone, double easting, double northing, double altitude, Datum datum ){
        
        // set parent
        super(altitude, datum);
        
        // set zone
        this.m_zone = zone;
        
        // set easting
        this.m_easting = easting;
        
        // set northing
        this.m_northing = northing;
    }
    
    /**
     * Get easting
     * @return Easting in meters
     */
    public double getEasting(){
        return this.m_easting;
    }
    
    /**
     * Set easting
     * @param easting Easting in meters
     */
    public void setEasting( double easting ){
        this.m_easting = easting;
    }
    
    public void addEasting( double easting ){
        this.m_easting += easting;
    }
    
    /**
     * Get northing
     * @return Northing in meters
     */
    public double getNorthing(){
        return this.m_northing;
    }
    
    /**
     * Set northing
     * @param northing Northing in meters
     */
    public void setNorthing( double northing ){
        this.m_northing = northing;
    }
    
    public void addNorthing( double northing ){
        this.m_northing += northing;
    }
    
    
    /**
     * Get zone
     */
    public int getZone(){
        return m_zone;
    }
    
    /**
     * Set zone
     */
    public void setZone( int zone ){
        this.m_zone = zone;
    }
    
    /**
     * Test if coordinate is in northern hemisphere
     */
    public int isNorth(){
        if( this.m_northing >= 0){
            return 1;
        } else {
            return 0;
        }
    }
    
    /**
     * Return the geographic coordinate type
     * @return GeographicCoordinateType for UTM
     */
    @Override
    public GeographicCoordinateType type(){
        return GeographicCoordinateType.UTM;
    }
    
    public static double Distance( CoordinateUTM coord1, CoordinateUTM coord2 ){
        return Math.sqrt( Math.pow(coord1.getEasting() - coord2.getEasting(),2) + Math.pow(coord1.getNorthing() - coord2.getNorthing(),2));       
    }
    
    public CoordinateUTM Addition( CoordinateUTM coord ){
        return new CoordinateUTM(this.getZone(), this.getEasting()+coord.getEasting(), this.getNorthing() + coord.getNorthing(), this.getAltitude() + coord.getAltitude(), this.getDatum());
    }
    
    public static CoordinateUTM Subtract( CoordinateUTM coord1, CoordinateUTM coord2 ){
        
        return new CoordinateUTM(coord1.getZone(), coord1.getEasting()  - coord2.getEasting(), coord1.getNorthing() - coord2.getNorthing(), coord1.getAltitude() - coord2.getAltitude(), coord1.getDatum());
    }
    
    public CoordinateUTM Multiply( double scale ){
        
        return new CoordinateUTM( this.m_zone, this.m_easting * scale, this.m_northing * scale, this.getAltitude() * scale, this.getDatum());
    }
    
    @Override
    public CoordinateUTM clone(){
        return new CoordinateUTM(this.m_zone, this.m_easting, this.m_northing, getAltitude(), getDatum());
    }
    
    public String toString(){
        String output = "CoordinateUTM(z=" + Integer.toString(this.m_zone) + ", e=" + Double.toString(this.m_easting) + ", n=" + Double.toString(this.m_northing) + ")";
        return output;
    }
}
