#extension GL_ARB_texture_rectangle : enabled

uniform sampler2DRect tex0;
void main()
{
	vec4 base  = texture2DRect(tex0, gl_TexCoord[0].xy);
	gl_FragData[0] = base;
}