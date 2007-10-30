uniform sampler2D beamTexture;
varying float alpha;
void main()
{
	vec4 color = texture2D(beamTexture,gl_TexCoord[0].st);
	color.a = clamp(color.a - alpha, 0.0, 1.0);
	gl_FragColor = color;
}
