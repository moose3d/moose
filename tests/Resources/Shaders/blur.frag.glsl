#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect tex0;
uniform int bVertical;

vec4 weights = vec4(0.0239689, 0.0493521, 0.0909301, 0.149918);
vec4 weights2 = vec4(0.221181, 0.292001, 0.344959, 0.364665);

//vec4 weights = vec4(0.144997, 0.27089, 0.394142, 0.446622);
//vec4 weights = vec4(0.00701665, 0.0854803, 0.383096, 0.631619);

void main()
{


	vec4 base  = texture2DRect(tex0, gl_TexCoord[0].xy) * weights2.w;
	vec2 dir = vec2(1,0);
	if ( bVertical != 0){
	  dir = vec2(0,1);
	}
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*1.0)) * weights2.z;
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*2.0)) * weights2.y;
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*3.0)) * weights2.x;
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*4.0)) * weights.w;
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*5.0)) * weights.z;
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*6.0)) * weights.y;
	base += texture2DRect(tex0, gl_TexCoord[0].xy+(dir*7.0)) * weights.x;

	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*1.0)) * weights2.z;
	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*2.0)) * weights2.y;
	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*3.0)) * weights2.x;
	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*4.0)) * weights.w;
	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*5.0)) * weights.z;
	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*6.0)) * weights.y;
	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*7.0)) * weights.x;

//	base += texture2DRect(tex0, gl_TexCoord[0].xy-dir) * weights.z;
//	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*2.0)) * weights.y;
//	base += texture2DRect(tex0, gl_TexCoord[0].xy-(dir*3.0)) * weights.x;




	gl_FragData[0] = base;
}