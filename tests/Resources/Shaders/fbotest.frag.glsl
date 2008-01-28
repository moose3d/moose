uniform sampler2D tex0;
uniform sampler2D tex1;
void main()
{
	vec4 base  = texture2D(tex0, gl_TexCoord[0].xy) * texture2D(tex1, gl_TexCoord[1].xy).r;
	gl_FragData[0] = base;
}