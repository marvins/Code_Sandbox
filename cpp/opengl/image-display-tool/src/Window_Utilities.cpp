/**
 * @file    Window_Utilities.cpp
 * @author  Marvin Smith
 * @date    9/3/2015
*/
#include "Window_Utilities.hpp"

// C++ Libraries
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>


// Pugixml
#include <pugixml.hpp>

// Project Libraries
#include "OpenGL_Utilities.hpp"

/**********************************/
/*          Constructor           */
/**********************************/
Render_Config::Render_Config( const std::string& config_pathname )
 : m_rows(1000),
   m_cols(1000),
   m_window_name("Image-Render-Utility")
{
    // Create XML Document
    pugi::xml_document xmldoc;
    pugi::xml_parse_result result = xmldoc.load_file( config_pathname.c_str() );

    // Make sure it opened properly
    if( result == false ){
        std::stringstream sin;
        sin << "File: " << __FILE__ << ", Line: " << __LINE__ << ". Configuration file parsed with errors. Details: " << result.description();
        std::cerr << sin.str() << std::endl;
        return;
    }

    
    // Get the root node
    pugi::xml_node root_node = xmldoc.child("image-render-utility");

    // Check the node
    if( root_node == pugi::xml_node() ){ 
        throw std::runtime_error("No <image-render-utility> node found.");
    }
    
    
    // Get the Window Node
    pugi::xml_node window_node = root_node.child("window");

    // Make sure it is valid
    if( window_node == pugi::xml_node() ){
        throw std::runtime_error("no <window> node found.");
    }

    // Get the size info
    m_rows = window_node.child("size").attribute("rows").as_int();
    m_cols = window_node.child("size").attribute("cols").as_int();
    m_window_name = window_node.child("name").attribute("name").as_string();

    // Background color
    m_background_color[0] = window_node.child("background-color").attribute("red").as_float();
    m_background_color[1] = window_node.child("background-color").attribute("green").as_float();
    m_background_color[2] = window_node.child("background-color").attribute("blue").as_float();
    m_background_color[3] = window_node.child("background-color").attribute("alpha").as_float();

}

/**********************************/
/*          Constructor           */
/**********************************/
Render_Config::Render_Config( const int& window_rows,
                              const int& window_cols,
                              const std::string& window_name )
  : m_rows(window_rows),
    m_cols(window_cols)
{

}


/***********************************/
/*          Constructor            */
/***********************************/
Render_Window::Render_Window( Render_Config const& render_config )
 :  m_class_name("Render_Window"),
    m_config(render_config),
    m_window(nullptr)
{
}


/*************************************/
/*        Initialize Window          */
/*************************************/
void Render_Window::Initialize()
{
    // Initialize GLFW
    if( !glfwInit() )
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(-1);
    }

    // Set windows
    glfwWindowHint( GLFW_SAMPLES, 4);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1);
    
    
    // Open a window and create its OpenGL context
	m_window = glfwCreateWindow( m_config.Get_Cols(),
                                 m_config.Get_Rows(),
                                 m_config.Get_Window_Name().c_str(),
                                 NULL,
                                 NULL);
   
    // Check the window
	if( m_window == NULL ){
	    throw std::runtime_error("Failed to open GLFW Window.");
    }
	
    // Take the window
    glfwMakeContextCurrent(m_window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}
    
    
    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set Background Color
	glClearColor( m_config.Get_Background_Color()[0],
                  m_config.Get_Background_Color()[1],
                  m_config.Get_Background_Color()[2],
                  m_config.Get_Background_Color()[3]);


    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	
    // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

    // Create and compile our GLSL program from the shaders
	m_program_id = Load_Shaders( "TransformVertexShader.vertexshader", 
                                 "TextureFragmentShader.fragmentshader" );



}


/*************************************/
/*          Finalize Window          */
/*************************************/
void Render_Window::Finalize()
{
    
    // Close OpenGL window and terminate GLFW
	glfwTerminate();

}


