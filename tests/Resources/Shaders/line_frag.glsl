uniform sampler2D beamTexture;
varying float alpha;
void main()
{
	gl_FragColor = vec4(texture2D(beamTexture,gl_TexCoord[0].st).xyz,alpha);

}
