#include "Options.h"


/**
  * Options Constructor
*/
Options::Options( ){

    //Glut Parameters
    window_width  = 300;
    window_height = 300;

    start_x = 10;
    start_y = 10;

    window_name = "GeoViewer";
    
    //general parameters
    debug_mode = 0;
    mouse_down = false;

    
    cam_lookStep = 1.0;
    cam_moveStep = 0.05;
    cam_timerStep = 10;

}

/**
 * Options Destructor
 */
Options::~Options( ){


}


/*

   Parameters::~Parameters(){

//load xml file
TiXmlDocument doc( "data/options.xml");

//load xml file
if( doc.LoadFile() == true ){

//create handle
TiXmlHandle hDoc(&doc);
TiXmlElement* pElem;

//write camera positions
if( camera.write_new_camera_pos == true ){

//access camera position
pElem = hDoc.FirstChildElement().FirstChildElement("Camera").Element();

//set center, eye, and up variables
pElem = pElem->FirstChildElement(); //skip state, leave alone
//set eye
pElem = pElem->NextSiblingElement(); 
pElem->SetDoubleAttribute("x",camera.eye.x());
pElem->SetDoubleAttribute("y",camera.eye.y());
pElem->SetDoubleAttribute("z",camera.eye.z());
pElem = pElem->NextSiblingElement();
pElem->SetDoubleAttribute("x",camera.center.x());
pElem->SetDoubleAttribute("y",camera.center.y());
pElem->SetDoubleAttribute("z",camera.center.z());
pElem = pElem->NextSiblingElement();
pElem->SetDoubleAttribute("x",camera.center.x());
pElem->SetDoubleAttribute("y",camera.center.y());
pElem->SetDoubleAttribute("z",camera.center.z());
} 
doc.SaveFile("data/options.xml");
}

}

void Parameters::Print(){
cout << "DEBUGGING DATA" << endl;
cout << "--------------" << endl;
cout << "1. shape: ";
if( shape_type == 0 ) cout << "cube" << endl;
else if( shape_type == 1 ) cout << "pyramid" << endl;
else if( shape_type == 2 ) cout << "sphere" << endl;
else{
cout << endl;
General_Error("Shape not defined", __FILE__, __LINE__);
}
cout << "2. Window Width: " << window_width << endl;
cout << "3. Window Height: " << window_height << endl;
cout << "4. Start X pos: " << start_x << endl;
cout << "5. Start Y pos: " << start_y << endl;
cout << "6. Window Name: " << window_name << endl;
cout << endl;

cout << "Shape Parameters" << endl;
cout << "7. Number of vertices: " << flush << shape->num_vertices() << endl;
shape->print_shape(cout); 
}

 */
