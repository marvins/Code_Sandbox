#include "Object.h"

// Draw the normals for the object
//#define DEBUG_DRAW_NORMALS

Object::Object( const string& fname, const double& sf, const GLuint prog ){

   program  = prog;
   scale_factor = sf;
   model_name = fname;
   vertices = NULL;
   normals  = NULL;
   ambients = NULL;
   diffuses = NULL;
   speculars= NULL;
   shininess= NULL;
}

Object::~Object(){
   
   make_empty();

}

void Object::make_empty(){
   if( vertices != NULL )
      delete [] vertices;
   if( normals != NULL )
      delete [] normals;
   if( ambients != NULL )
      delete [] ambients;
   if( diffuses != NULL )
      delete [] diffuses;
   if( speculars != NULL )
      delete [] speculars;
   if( shininess != NULL )
      delete [] shininess;
}


void Object::init_buffers( const vec4& lpos, const vec4& l_amb, const vec4& l_diff, const vec4& l_spec){


   //load data from model
   //model_data.load(model_name.c_str());
  
   //load model sizes
   //vertex_count = model_data.faceCount*3;
   vertex_count = 6;

   //compute shininess and vertex array size
   vertices_size = sizeof(vec4)*vertex_count;
   shininess_size = sizeof(GLfloat)*vertex_count;
   
   //assign memory to arrays
   vertices  = new vec4[vertex_count];
   normals   = new vec4[vertex_count];
   ambients  = new vec4[vertex_count];
   diffuses  = new vec4[vertex_count];
   speculars = new vec4[vertex_count];
   shininess = new GLfloat[vertex_count];
   
   //Copy model data into arrays
   build_arrays( lpos, l_amb, l_diff, l_spec);

   ///Get width of object
   double x_min = vertices[0].x;
   double x_max = vertices[0].x;
   double y_min = vertices[0].y;
   double y_max = vertices[0].y;
   double z_min = vertices[0].z;
   double z_max = vertices[0].z;
   for(size_t i=1; i<vertex_count; i++){
      if( vertices[i].x < x_min )
         x_min = vertices[i].x;
      if( vertices[i].x > x_max )
         x_max = vertices[i].x;
      if( vertices[i].y < y_min )
         y_min = vertices[i].y;
      if( vertices[i].y > y_max )
         y_max = vertices[i].y;
      if( vertices[i].z < z_min )
         z_min = vertices[i].z;
      if( vertices[i].z > z_max )
         z_max = vertices[i].z;
   }
   x_width = x_max - x_min;
   y_width = y_max - y_min;
   z_width = z_max - z_min;
   centroid = vec3(  (x_max-x_min)/2.0+x_min,
                     (y_max-y_min)/2.0+y_min,
                     (z_max-z_min)/2.0+z_min);


   /// Initialize Shader
   // Create a vertex array object
   #ifdef __APPLE__
   glGenVertexArraysAPPLE( 1, &vao );
   glBindVertexArrayAPPLE( vao );
   #else
   glGenVertexArrays( 1, &vao );
   glBindVertexArray( vao ); 
   #endif


   // Create and initialize a buffer object
   glGenBuffers( 1, &vbo );
   glBindBuffer( GL_ARRAY_BUFFER, vbo );
   glBufferData( GL_ARRAY_BUFFER, 5*vertices_size, NULL, GL_STATIC_DRAW );
   glBufferSubData( GL_ARRAY_BUFFER, 0*vertices_size, vertices_size, vertices );
   glBufferSubData( GL_ARRAY_BUFFER, 1*vertices_size, vertices_size, normals );
   glBufferSubData( GL_ARRAY_BUFFER, 2*vertices_size, vertices_size, ambients );
   glBufferSubData( GL_ARRAY_BUFFER, 3*vertices_size, vertices_size, diffuses );
   glBufferSubData( GL_ARRAY_BUFFER, 4*vertices_size, vertices_size, speculars );
   glBufferSubData( GL_ARRAY_BUFFER, 5*vertices_size, shininess_size, shininess );

   //load program
   glUseProgram( program );

   // get ids of and set up attribute variables
   GLuint vPosition = glGetAttribLocation( program, "vPosition" );
   glEnableVertexAttribArray( vPosition );
   glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
         BUFFER_OFFSET(0) );

   GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
   glEnableVertexAttribArray( vNormal );
   glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((1*vertices_size)));

   GLuint vAmbient = glGetAttribLocation( program, "vAmbient" ); 
   glEnableVertexAttribArray( vAmbient );
   glVertexAttribPointer( vAmbient, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((2*vertices_size)));

   GLuint vDiffuse = glGetAttribLocation( program, "vDiffuse" ); 
   glEnableVertexAttribArray( vDiffuse );
   glVertexAttribPointer( vDiffuse, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((3*vertices_size)));

   GLuint vSpecular = glGetAttribLocation( program, "vSpecular" ); 
   glEnableVertexAttribArray( vSpecular );
   glVertexAttribPointer( vSpecular, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((4*vertices_size)));

   GLuint vShiny    = glGetAttribLocation( program, "vShiny" ); 
   glEnableVertexAttribArray( vShiny );
   glVertexAttribPointer( vShiny, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((5*vertices_size)));

   // get ids of uniform variables
   worldview = glGetUniformLocation( program, "worldview" );
   projection = glGetUniformLocation( program, "projection" );
   light_position = glGetUniformLocation( program, "light_position");
   drawmode = glGetUniformLocation( program, "drawmode");
   translation_id = glGetUniformLocation( program, "translation");

   translation = vec4(0,0,0,1);
}

void Object::draw_shape( mat4 const& worldviewMat, mat4 const& proj, vec4 const& l_pos ){

#ifdef __APPLE__
   glBindVertexArrayAPPLE( vao );
#else
   glBindVertexArray( vao ); 
#endif
   
   glUniform4fv( translation_id, 1, translation );
   glUniform4fv( light_position,   1, l_pos );
   glUniformMatrix4fv( projection, 1, GL_TRUE, proj );
   glUniformMatrix4fv( worldview, 1, GL_TRUE, worldviewMat);

   glUniform1i( drawmode, (GLint)TRIANGLES );
   glDrawArrays( GL_TRIANGLES, 0, vertex_count );

}
      
double Object::get_x_width()const{
   return x_width;
}
double Object::get_y_width()const{
   return y_width;
}
double Object::get_z_width()const{
   return z_width;
}

vec3 Object::get_dimensions()const{
   return vec3(get_x_width(), get_y_width(), get_z_width());
}

vec3 Object::get_centroid()const{
   return centroid+get_translation();  
}

void Object::build_arrays( const vec4& lpos, const vec4& l_amb, const vec4& l_dif, const vec4& l_spec ){
    
    size_t vsize = 6;
    vertices[0] = vec4( 0, 0, 0, 1); normals[0] = vec4( 0, 0, -1, 1);
    vertices[1] = vec4( 1, 0, 0, 1); normals[1] = vec4( 0, 0, -1, 1);
    vertices[2] = vec4( 1, 1, 0, 1); normals[1] = vec4( 0, 0, -1, 1);
    vertices[3] = vec4( 0, 0, 0, 1); normals[1] = vec4( 0, 0, -1, 1);
    vertices[4] = vec4( 1, 1, 0, 1); normals[1] = vec4( 0, 0, -1, 1);
    vertices[5] = vec4( 0, 1, 0, 1); normals[1] = vec4( 0, 0, -1, 1);

    for( size_t i=0; i<vsize; i++){
        diffuses[i]  = vec4(1,1,1,1);
        ambients[i]  = vec4(1,1,1,1);
        speculars[i] = vec4(1,1,1,1);
        shininess[i] = 1;
    }


}

void Object::adjust_translation( const vec3& motion ){

    translation.x += motion.x;
    translation.y += motion.y;
    translation.z += motion.z;

}

vec3 Object::get_translation()const{
    return vec3( translation.x, translation.y, translation.z);
}

void Object::set_translation( const vec3& trans ){
    translation.x = trans.x;
    translation.y = trans.y;
    translation.z = trans.z;
}

vec4* Object::get_vertices(){
    return vertices;
}

size_t Object::num_verts(){
    return vertex_count;
}
