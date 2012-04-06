/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvl.channel_type;

import java.lang.*;

/**
 *
 * @author marvin_smith1
 */
public class RGB < PType extends Number> implements ChannelType {
    
    
    public PType R; /*< Red   Value */
    public PType G; /*< Green Value */
    public PType B; /*< Blue  Value */
    
    /**
     * Default Constructor
     */
    public RGB(){}
    
    
    /**
     * Parameterized constructor
     * @param r red pixel
     * @param g green pixel
     * @param b blue pixel
     */
    public RGB( PType r, PType g, PType b ){
        R = r;
        G = g;
        B = b;
    }
    
    /**
     * Query the number of channels in the image.
     * @return number of channels
     */
    @Override
    public int num_channels(){
        return 3;
    }
    
    @Override
    public RGB<PType> copy(){
        return new RGB(R,G,B);
    }
    
    @Override
    public String isType(){
        StringBuilder str = new StringBuilder();
        str.append(this.getClass().getSimpleName())
           .append("<").append(R.getClass().getSimpleName())
           .append(">");
        return str.toString();
    }
    
    @Override
    public String toString(){
        StringBuilder str = new StringBuilder();
        str.append(this.getClass().getSimpleName()).append("(");
        str.append(R.toString()).append(",").append(G.toString());
        str.append(",").append(B.toString()).append(")");
        return str.toString();
    }
    
    
}
