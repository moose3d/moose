#version 150
struct MMaterial {
  vec4 diffuse;
  vec4 ambient;
  vec4 specular;
  vec4 emission;
  float shininess;
};
////////////////////////////////////////////////////////////////////////////////
// Moose light struct
// , apparently only Struct of Arrays is supported.
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

in  vec3 a_vertex;
in  vec2 a_texcoord;
in  vec3 a_normal;
out vec2 v_texcoord;
out vec3 v_normal;

uniform mat4 m_viewMatrix;
uniform mat4 m_projMatrix;
uniform mat4 m_modelMatrix;

uniform MLights m_Lights;
uniform vec4    m_globalAmbient;
out vec4 v_color;

void main()
{
   vec4 vert    = m_viewMatrix * m_modelMatrix * vec4(a_vertex,1.0);
   vec4 lDir    = vec4(-m_Lights.direction[0],0.0);
   float NdotL = max(dot((m_modelMatrix * vec4(a_normal,0.0)).xyz, lDir.xyz),0.0);
   vec4 diffuse = vec4(0.7,0.7,0.7,1.0) * m_Lights.diffuse[0];

   v_color = NdotL * diffuse + m_globalAmbient;
   gl_Position  = m_projMatrix * vert;
   v_normal = a_normal;
}                 
