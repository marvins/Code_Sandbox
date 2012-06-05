/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import ij.*;
import ij.IJ;
import ij.ImagePlus;
import ij.plugin.filter.PlugInFilter;
import ij.process.ImageProcessor;
import java.awt.event.KeyEvent;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

import java.io.*;

/**
 *
 * @author ms6401
 */
public class NITF_Writer extends JPanel implements ActionListener {
   
    protected JButton findHeaderFileButton;
    protected JButton saveImageButton;
    
    GridBagConstraints layoutConstraints;
    
    
    final ImageProcessor imageData;
    GDAL_Header header_info;
    
    //ui elements
    JLabel     headerFilenameLabel;    
    JTextField headerFilenameField;
    JButton    headerFilenameButton;
    
    JLabel     headerPixelTypeLabel;
    JComboBox  headerPixelTypeField;
    
    JLabel     headerChannelCntLabel;
    JComboBox  headerChannelCntField;
    
    
    
    NITF_Writer( ImageProcessor ip ){
        
        this.setLayout(new GridBagLayout());
        
        layoutConstraints = new GridBagConstraints();
       
        //create find header button
        imageData = ip;
        findHeaderFileButton = new JButton("Find Header Data File");
        findHeaderFileButton.setVerticalAlignment(AbstractButton.TOP);
        findHeaderFileButton.setMnemonic(KeyEvent.VK_F2);
        findHeaderFileButton.setActionCommand("Find");
        findHeaderFileButton.setEnabled(true);
        findHeaderFileButton.addActionListener(this);
        layoutConstraints.gridx = 0;
        layoutConstraints.gridy = 0;
        layoutConstraints.weightx = 0.5;
        layoutConstraints.fill = GridBagConstraints.HORIZONTAL;
        add(findHeaderFileButton, layoutConstraints);
        
        //create save image button
        saveImageButton = new JButton("Save the Selected Image");
        saveImageButton.setVerticalAlignment(AbstractButton.CENTER);
        saveImageButton.setMnemonic(KeyEvent.VK_F3);
        saveImageButton.setActionCommand("Save");
        saveImageButton.setEnabled(true);
        saveImageButton.addActionListener(this);
        layoutConstraints.gridx = 1;
        layoutConstraints.gridy = 0;
        layoutConstraints.weightx = 0.5;
        layoutConstraints.fill = GridBagConstraints.HORIZONTAL;
        add(saveImageButton, layoutConstraints );
        
        //create metadata form
        header_info = new GDAL_Header(ip);
       
        headerFilenameLabel = new JLabel("Image Filename");
        layoutConstraints.gridx = 0;
        layoutConstraints.gridy = 2;
        layoutConstraints.insets.top = 10;
        
        add(headerFilenameLabel, layoutConstraints);
        
        headerFilenameField = new JTextField(20);
        headerFilenameField.setActionCommand("Image Filename");
        headerFilenameField.setText(header_info.image_filename);
        headerFilenameField.addActionListener(this);
        layoutConstraints.gridx = 1;
        layoutConstraints.gridy = 2;
        layoutConstraints.insets.top = 10;
        layoutConstraints.anchor = GridBagConstraints.NORTH;
        layoutConstraints.fill = GridBagConstraints.HORIZONTAL;
        add(headerFilenameField, layoutConstraints);
        
        headerFilenameButton = new JButton("Choose File");
        headerFilenameButton.setActionCommand("Choose File");
        headerFilenameButton.addActionListener(this);
        layoutConstraints.gridx = 2;
        add(headerFilenameButton, layoutConstraints);
        
        
        /**  PixelType label */
        headerPixelTypeLabel = new JLabel("PixelType");
        layoutConstraints.gridx = 0;
        layoutConstraints.gridy = 3;
        layoutConstraints.fill = GridBagConstraints.HORIZONTAL;
        add(headerPixelTypeLabel, layoutConstraints);
        
        headerPixelTypeField = new JComboBox( GDAL_Header.pixelTypes );
        headerPixelTypeField.setSelectedIndex(header_info.getPixelTypeIndex());
        headerPixelTypeField.setActionCommand("PixelType");
        headerPixelTypeField.addActionListener(this);
        
        layoutConstraints.gridx = 1;
        add(headerPixelTypeField, layoutConstraints);
        
        /** Number of channels label */
        headerChannelCntLabel = new JLabel("Number Channels");
        layoutConstraints.gridx = 0;
        layoutConstraints.gridy = 4;
        layoutConstraints.fill = GridBagConstraints.HORIZONTAL;
        layoutConstraints.weighty = 1.0;
        add( headerChannelCntLabel, layoutConstraints);
        
        headerChannelCntField = new JComboBox( GDAL_Header.channelCnt );
        if( header_info.nChannels == 1 )
            headerChannelCntField.setSelectedIndex(0);
        else if( header_info.nChannels == 3 )
            headerChannelCntField.setSelectedIndex(1);
        else
            throw new IllegalArgumentException("I am confused");
        headerChannelCntField.addActionListener(this);
        
        layoutConstraints.gridx = 1;
        add( headerChannelCntField, layoutConstraints);
        
        //headerPixelTypeField = new JTextField(20);
        
    }
    
    @Override
    public void actionPerformed( ActionEvent e ){
        
        System.out.println("Print action: " + e.getActionCommand());
        if( "Find".equals(e.getActionCommand())){
            
            //create a filechooser
            Save_FileChooser.loadHeaderData( this, ".", header_info);
            headerFilenameField.setText( header_info.original_filename);
            header_info.image_filename = header_info.original_filename;
            System.out.println("PTYPE: " + header_info.pixel.getName());
            
        }
        if( "Save".equals(e.getActionCommand())){
            
            //
            GDAL_Info.write( header_info, imageData );
            
        }
        if( "Choose File".equals(e.getActionCommand())){
            System.out.println("PTYPE: " + header_info.pixel.getName());
            Save_FileChooser.selectImageName( this, header_info);
            headerFilenameField.setText( header_info.image_filename);
        }
        if( "Image Filename".equals(e.getActionCommand())){
            JTextField source = (JTextField)e.getSource();
            headerFilenameField.setText(source.getText());
            header_info.image_filename = source.getText();
            System.out.println("Image filename changed to : " + header_info.image_filename);
            
        }
        
        if( "PixelType".equals(e.getActionCommand())){
            
            //get the preferred pixel type
            JComboBox cb = (JComboBox)e.getSource();
            String pixType = (String)cb.getSelectedItem();
        
            //change the preferred pixeltype
            header_info.pixel = PixelType.getType(pixType);
        
        }
        if( "Number Channels".equals(e.getActionCommand())){
            //do nothing
            JComboBox cb = (JComboBox)e.getSource();
            String nChan = (String)cb.getSelectedItem();
            
            //chance the output channel type
            
        }
        
    }
    
}
