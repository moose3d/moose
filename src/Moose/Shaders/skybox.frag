#ifdef GL_ES
#define IN varying
#define OUT 
precision mediump float;
#else
#define IN in
#define OUT out
#endif
IN vec3 v_texcoord;
uniform samplerCube diffuse;
#ifndef GL_ES
OUT vec4 gl_FragColor;
#endif
void main(){
#ifdef GL_ES
     gl_FragColor = textureCube(diffuse,v_texcoord);
#else
     gl_FragColor = texture(diffuse,v_texcoord);
#endif
}
