varying vec4 color;

// per-fragment interpolated values from the vertex shader
varying  vec4 fN;
varying  vec4 fL;
varying  vec4 fE;

varying vec4 ambient;
varying vec4 diffuse;
varying vec4 specular;
varying float shiny;

// drawmode values
// GL_TRIANGLES : 0
// GL_LINES     : 1
uniform  int drawmode;

void main() 
{ 
   if ( drawmode != 1 )
   {
      // Normalize the input lighting vectors
      vec3 N = normalize(fN.xyz);
      vec3 E = normalize(fE.xyz);
      vec3 L = normalize(fL.xyz);

      vec3 H = normalize( L + E );

      float Kd = max(dot(L, N), 0.0);
      vec4 diff = Kd*diffuse;

      float Ks = pow(max(dot(N, H), 0.0), shiny);

      vec4 spec = Ks*specular;

      // discard the specular highlight if the light's behind the vertex
      if( dot(L, N) < 0.0 ) {
         spec = vec4(0.0, 0.0, 0.0, 1.0);
      }

      // limit values between 0.0 and 1.0 (prevents overbright)
      gl_FragColor = clamp(ambient + diff + spec,0.0,1.0);
   }
   else
   {
      gl_FragColor = diffuse;
   }
} 

