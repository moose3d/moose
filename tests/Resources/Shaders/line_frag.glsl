uniform sampler2D beamTexture;
varying float alpha;
uniform vec3 beam_color;
void main()
{
  gl_FragColor = vec4(beam_color*texture2D(beamTexture,gl_TexCoord[0].st).x, alpha);
}
