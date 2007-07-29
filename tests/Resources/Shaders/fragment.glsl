uniform sampler2D tex0,tex1;
void main()
{
	vec4 color = texture2D(tex0,gl_TexCoord[0].st);
	vec4 color2 = texture2D(tex1, gl_TexCoord[1].st);
	gl_FragColor = color * color2;
}
