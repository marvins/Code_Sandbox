/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvl.channel_type;

/**
 *
 * @author marvin_smith1
 */
public interface ChannelType <PixelType extends Number> {
    
    public int num_channels();
    
    public ChannelType copy();
    
    public String isType();
    
}
