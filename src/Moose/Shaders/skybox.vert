#ifdef GL_ES
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif
IN vec3 position;
IN vec3 a_texcoord;
OUT vec3 v_texcoord;
uniform mat4 m_viewMatrix;
uniform mat4 m_projMatrix;

void main(){
     mat4 mv = m_viewMatrix;
     mv[3][0] = 0.0; mv[3][1] = 0.0; mv[3][2] = 0.0;
     gl_Position = m_projMatrix * mv * vec4(position,1.0);
     v_texcoord = a_texcoord;
}

