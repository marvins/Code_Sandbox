
attribute  vec4 vPosition;
attribute  vec4 vColor;
attribute  vec4 vNormal;
attribute  vec4 vAmbient;
attribute  vec4 vDiffuse; 
attribute  vec4 vSpecular;
attribute  float vShiny;

varying vec4 color;

// from world coordinates to eye coords (this is what the camera uses)
uniform mat4 worldview;

uniform mat4 projection; 
uniform vec4 translation;
uniform vec4 light_position;

// output values that will be interpretated per-fragment
varying  vec4 fN;
varying  vec4 fE;
varying  vec4 fL;
varying  vec4 ambient;
varying  vec4 diffuse;
varying  vec4 specular;
varying  float shiny;

void main() 
{
   // build the "model to world" transformation matrix (no rotation or scale right now)
   mat4 modelworld = mat4( 1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           translation.x, translation.y, translation.z, 1.0);

   // put vertex into world coordinates
   gl_Position = modelworld * vPosition;

   fN = vNormal;
   fE = vPosition;
   fL = vec4(light_position.xyz - gl_Position.xyz,1.0);

   ambient = vAmbient;
   diffuse = vDiffuse;
   specular = vSpecular;
   shiny = max(1.0,vShiny);

   color = vColor;
   
   // put vertex into view(or eye) then projection(clipping) coordinates
   gl_Position = projection * worldview * gl_Position;
} 

