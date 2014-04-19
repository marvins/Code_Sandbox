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
public enum Datum {
    
    WGS84;
    
    
    public String getWellKnownCS(){
        if( this == WGS84 ){
            return "WGS84";
        } else {
            return "UNKNOWN";
        }
    }
}
