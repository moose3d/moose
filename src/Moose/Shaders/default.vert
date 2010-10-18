#version 150
in  vec3 a_vertex;
in  vec2 a_texcoord;
in  vec3 a_normal;
out vec2 v_texcoord;
out vec3 v_normal;
 
uniform mat4 m_viewMatrix;
uniform mat4 m_projMatrix;
uniform mat4 m_modelMatrix;

void main()
{
   gl_Position = m_projMatrix * m_viewMatrix * m_modelMatrix * vec4(a_vertex,1.0);
   v_texcoord = a_texcoord;
   v_normal = a_normal;
}
