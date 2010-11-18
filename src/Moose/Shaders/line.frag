#ifdef GL_ES
precision mediump float;
#define IN varying
#define OUT 
#else
#version 150
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
