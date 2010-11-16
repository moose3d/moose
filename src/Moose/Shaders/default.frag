#ifdef GL_ES
#define IN varying
#define OUT 
precision mediump float;
#else
#define IN in
#define OUT out
#endif
// Moose material struct
////////////////////////////////////////////////////////////////////////////////
struct MMaterial { 
  vec4 diffuse;
  vec4 ambient;
  vec4 specular;
  vec4 emission;
  float shininess;
};

struct MLights 
{
  vec4 ambient[3];
  vec4 diffuse[3];
  vec4 specular[3];
  vec3 position[3]; 
  vec3 halfVector[3];
  vec3 direction[3];
  float spotAngle[3]; 
  float spotExponent[3];
  float constantAttenuation[3];
  float linearAttenuation[3];
  float quadraticAttenuation[3];
  bool   enabled[3];
};
////////////////////////////////////////////////////////////////////////////////


uniform MMaterial m_Material;
uniform MLights   m_Lights;
uniform sampler2D diffusetex;
uniform vec4 m_globalAmbient;

IN vec2  texcoord;
IN vec3  lightDir[3];
IN vec3  normal;
IN vec3  halfVector[3];
IN float ldist[3];

#ifndef GL_ES
OUT vec4 gl_FragColor;
#endif

////////////////////////////////////////////////////////////////////////////////
void main()
{
  
  vec4 color = m_globalAmbient * m_Material.ambient;
  vec3 n = normalize(normal);
  
  for( int i=0;i<3;i++)
  {
    vec4 diffuse = m_Material.diffuse * m_Lights.diffuse[i];
    vec4 ambient = m_Material.ambient * m_Lights.ambient[i];
    if ( m_Lights.enabled[i] )
    {
      if ( m_Lights.spotAngle[i] < 0.0 )
      {
        float NdotL = max(dot(n, lightDir[i]),0.0);
        color += ambient;
        if ( NdotL > 0.0 )
        {
          color += diffuse * NdotL;
          vec3  halfV = normalize(halfVector[i]);
          float NdotHV = max(dot(n,halfV),0.0);
          color += m_Material.specular *
                   m_Lights.specular[i] *
                   pow(NdotHV, m_Material.shininess);
        }
      }
      else if ( m_Lights.spotAngle[i] > 90.0 )/* point light */
      {
        float NdotL = max(dot(n, normalize(lightDir[i])),0.0);
        if ( NdotL > 0.0 )
        {
          float att = 1.0 / ldist[i];
          color += att * (diffuse * NdotL + ambient );
          vec3  halfV = normalize(halfVector[i]);
          float NdotHV = max(dot(n,halfV),0.0);
          
          color += att * m_Material.specular *
          m_Lights.specular[i] *
          pow(NdotHV, m_Material.shininess);

        }
      }
    }
  }
  gl_FragColor = color;
}
////////////////////////////////////////////////////////////////////////////////
