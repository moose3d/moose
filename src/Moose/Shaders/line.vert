#ifdef GL_ES
#define OUT varying
#define IN attribute
#else
#version 150
#define OUT out
#define IN in
#endif
/* Does not use model transform*/
attribute vec3 a_vertex;
attribute vec4 a_endpos_thickness;
uniform mat4 m_viewMatrix;
uniform mat4 m_projMatrix;

void main()
{
  vec4 endPos   = m_viewMatrix * vec4(a_endpos_thickness.xyz,1.0);
  vec4 startPos = m_viewMatrix * vec4(a_vertex,1.0);
  vec3 linedir = (endPos.xyz - startPos.xyz);
  vec3 offsetVec = normalize(cross(startPos.xyz,linedir));
  startPos = vec4(startPos.xyz + (offsetVec * a_endpos_thickness.w),startPos.w);
  gl_Position = m_projMatrix * startPos;
}
