uniform sampler2D tex0;
uniform sampler2D tex1;
void main()
{
	vec4 base  = texture2D(tex0, gl_TexCoord[0].xy);
	vec4 base2 = texture2D(tex1, gl_TexCoord[1].xy);
	gl_FragData[0] = base;

	if ( base2.g > 0.0 )
	{
		gl_FragData[1] = base;
	}
	else {
	     gl_FragData[1] = vec4(0,0,0,1);
	}
/*		gl_FragData[1] = vec4(0,1,0,1); */

	
}