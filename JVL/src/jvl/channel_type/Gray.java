/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvl.channel_type;

/**
 *
 * @author marvin_smith1
 */
public class Gray < PType extends Number> implements ChannelType {
    
    public PType pix;
    
    public Gray(){
        
    }
    
    public Gray( PType p ){
        pix = p;
    }
    
    @Override
    public int num_channels(){
        return 1;
    }
    
    @Override
    public Gray<PType> copy(){
        return new Gray(pix);
    }
    
    @Override
    public String isType(){
        StringBuilder str = new StringBuilder();
        str.append(this.getClass().getSimpleName())
           .append("<").append(pix.getClass().getSimpleName())
           .append(">");
        return str.toString();
    }
    
    
}
