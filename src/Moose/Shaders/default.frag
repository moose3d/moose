#version 150
in vec2 v_texcoord;
in vec3 v_normal;
uniform sampler2D diffuse;
out vec4 gl_FragColor;
void main(){
     gl_FragColor = texture(diffuse,v_texcoord);
}
