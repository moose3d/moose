uniform sampler2D beamTexture;
void main()
{
	gl_FragColor = texture2D(beamTexture,gl_TexCoord[0].st);
}