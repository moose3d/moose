#extension GL_ARB_texture_rectangle : enable
#extension GL_ARB_draw_buffers : enable
uniform sampler2DRect tex0;
const vec4 weights = vec4(0.0239689, 0.0493521, 0.0909301, 0.149918);
const vec4 weights2 = vec4(0.221181, 0.292001, 0.344959, 0.364665);

//vec4 weights = vec4(0.144997, 0.27089, 0.394142, 0.446622);
//vec4 weights = vec4(0.00701665, 0.0854803, 0.383096, 0.631619);

void main()
{


	vec4 base  = texture2DRect(tex0, gl_TexCoord[0].xy)  * weights2.w ;
	base += texture2DRect(tex0, gl_TexCoord[1].xy) * weights2.z;
	base += texture2DRect(tex0, gl_TexCoord[2].xy) * weights2.y;
	base += texture2DRect(tex0, gl_TexCoord[3].xy) * weights2.x;
	base += texture2DRect(tex0, gl_TexCoord[4].xy) * weights2.z;
	base += texture2DRect(tex0, gl_TexCoord[5].xy) * weights2.y;
	base += texture2DRect(tex0, gl_TexCoord[6].xy) * weights2.x;
	gl_FragData[0] = base;

}