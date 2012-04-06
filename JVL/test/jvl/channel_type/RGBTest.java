/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvl.channel_type;

import org.junit.*;
import static org.junit.Assert.*;

/**
 *
 * @author marvin_smith1
 */
public class RGBTest {
    
    public RGBTest() {
    }

    @BeforeClass
    public static void setUpClass() throws Exception {
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
    }
    
    @Before
    public void setUp() {
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of num_channels method, of class RGB.
     */
    @Test
    public void RGB_tests(){
        
        RGB<Double> val = new RGB<Double>(100d,100d,100d);
    
        System.out.println("VAL: " + val);
        
    }
    
    
}
