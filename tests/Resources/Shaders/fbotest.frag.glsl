uniform sampler2D tex0;
void main()
{
	vec4 base = texture2D(tex0, gl_TexCoord[0].xy);
	vec4 test = base + vec4(0,0,1,0);

	gl_FragData[0] = base;
	gl_FragData[1] = test;
	
}