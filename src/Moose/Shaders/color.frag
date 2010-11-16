#ifdef GL_ES
#define IN varying
#define OUT 
precision mediump float;
#else
#define IN in
#define OUT out
#endif

uniform vec4 color;

#ifndef GL_ES
OUT vec4 gl_FragColor;
#endif

void main(){
     gl_FragColor = color;
}
