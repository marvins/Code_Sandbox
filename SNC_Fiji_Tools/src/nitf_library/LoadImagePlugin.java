/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package nitf_library;

import ij.*;
import ij.plugin.*;
import ij.plugin.frame.PlugInFrame;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.io.*;
import java.util.List;


/**
 *
 * @author ms6401
 */
public class LoadImagePlugin extends PlugInFrame implements ActionListener {
    
    JMenuBar mainMenuBar;
    
    JMenu fileMenu;
    
    JMenuItem menuItem_loadImage;
    JMenuItem menuItem_loadImageStack;
    JMenuItem menuItem_quit;
    
    public final static Dimension menuBarSize = new Dimension(300,50);
    private final static boolean RIGHT_TO_LEFT = false;
    
    JPanel contentPane;
    
    String currentDir = ".";
    
    GridBagLayout gridBagLayout = new GridBagLayout();
    
    public LoadImagePlugin(){
        super("NITF Loader");
    }
    
    
    public JMenuBar createMenuBar() {

        //create fileMenu bar
        mainMenuBar = new JMenuBar();

        mainMenuBar.setOpaque(true);
        mainMenuBar.setPreferredSize(menuBarSize);

        //create fileMenu object
        fileMenu = new JMenu("File");
        fileMenu.getAccessibleContext().setAccessibleDescription("File menu for main functional components");
        fileMenu.setFont( new Font("Serif",Font.BOLD,16));
        mainMenuBar.add(fileMenu);


        //load image item
        menuItem_loadImage = new JMenuItem("Load Images Individually");
        menuItem_loadImage.addActionListener(this);
        menuItem_loadImage.setFont(new Font("Serif", Font.PLAIN, 14));
        fileMenu.add(menuItem_loadImage);

        //load image item
        menuItem_loadImageStack = new JMenuItem("Load Images into Stack");
        menuItem_loadImageStack.addActionListener(this);
        menuItem_loadImageStack.setFont(new Font("Serif", Font.PLAIN, 14));
        fileMenu.add(menuItem_loadImageStack);

        return mainMenuBar;

    }

    
    public Container createContentPane() {

        //create pane
        contentPane = new JPanel(new BorderLayout());
        contentPane.setOpaque(true);
        contentPane.setLayout(gridBagLayout);

        if (RIGHT_TO_LEFT) {
            contentPane.setComponentOrientation(ComponentOrientation.RIGHT_TO_LEFT);
        }
        return contentPane;

    }
    
    @Override
    public void run( String arg ){
        
        JPanel framePanel = new JPanel();
       
        
        LoadImagePlugin plug = new LoadImagePlugin();
        
        framePanel.add(plug.createMenuBar());
        
        this.add(framePanel);
        
        int maxWidth  = Math.max(LoadImagePlugin.menuBarSize.width, 200);
        int maxHeight = LoadImagePlugin.menuBarSize.height+30;
        
        this.setSize(maxWidth,maxHeight);
        this.show();
        
    }
    
    
    //action listener
    @Override
    public void actionPerformed(ActionEvent e) {

        JMenuItem source = (JMenuItem) (e.getSource());
        
        //check if the quit button was selected
        if (source == menuItem_quit) {
            
            
            
        }

        //check if the load image button was selected
        if (source == menuItem_loadImage) {
            
            //load a single image
            List<String> filenames = GDAL_FileChooser.getImageFilenames( contentPane, true, currentDir );
            
            if( filenames.size() > 0 ){
                File f = new File(filenames.get(0));
                currentDir = f.getParent();
            }
            for(int i=0; i<filenames.size(); i++){
                ImagePlus img = NITF_Loader.read_NTIF(filenames.get(i), contentPane );
                
                
                img.show();
                
            }
            
        }
        
        //check if the load image button was selected
        if (source == menuItem_loadImageStack) {
            
            //load a single image
            List<String> filenames = GDAL_FileChooser.getImageFilenames( contentPane, true, currentDir );
        }
    }
    
}
