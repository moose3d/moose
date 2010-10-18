#version 150
in vec3 v_texcoord;
uniform samplerCube diffuse;
out vec4 gl_FragColor;

void main(){
     gl_FragColor = texture(diffuse,v_texcoord);
}
