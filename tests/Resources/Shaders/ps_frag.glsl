uniform sampler2D texture;
varying float alpha;
void main()
{
	vec4 color = texture2D(texture,gl_TexCoord[0].st); 

	if ( color.a > alpha){
		color.a = alpha;
	}

	gl_FragColor = color;
}
