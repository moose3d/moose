#ifdef GL_ES
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif
IN  vec3 a_vertex;

uniform mat4 m_viewMatrix;
uniform mat4 m_projMatrix;
uniform mat4 m_modelMatrix;

void main()
{
   gl_Position = m_projMatrix * m_viewMatrix * m_modelMatrix * vec4(a_vertex,1.0);
}
