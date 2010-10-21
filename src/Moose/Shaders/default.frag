#version 150
// Moose material struct
////////////////////////////////////////////////////////////////////////////////
struct MMaterial {
  vec4 diffuse;
  vec4 ambient;
  vec4 specular;
  vec4 emission;
  float shininess;
};
////////////////////////////////////////////////////////////////////////////////
// Moose light struct, apparently only Struct of Arrays is supported.
struct MLights 
{
  vec4 ambient[3];
  vec4 diffuse[3];
  vec4 specular[3];
  vec3 position[3]; 
  vec3 halfVector[3];
  vec3 direction[3];
  float spotCutoff[3]; 
  float spotExponent[3];
  float constantAttenuation[3];
  float linearAttenuation[3];
  float quadraticAttenuation[3];
};
////////////////////////////////////////////////////////////////////////////////
in vec2 v_texcoord;
in vec3 v_normal;
uniform MMaterial m_Material;
uniform MLights   m_Lights;
uniform sampler2D diffuse;
out vec4 gl_FragColor;
uniform vec4 m_globalAmbient;
in vec4 v_color;

////////////////////////////////////////////////////////////////////////////////
void main()
{
  gl_FragColor = v_color;
}
////////////////////////////////////////////////////////////////////////////////
