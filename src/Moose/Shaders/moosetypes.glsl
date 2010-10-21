// Moose material struct
struct MMaterial {
  vec4 diffuse;
  vec4 ambient;
  vec4 specular;
  vec4 emission;
  float shininess;
};
// Moose light struct
struct MLight {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position; 
  vec3 halfVector;
  vec3 spotDirection;
  float spotCutoff; 
  float spotExponent;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;
};
