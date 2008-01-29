uniform int bVertical;
void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;	
	if ( bVertical == 0 ){
		gl_TexCoord[1] = gl_MultiTexCoord0+vec4(1,0,0,0);	
		gl_TexCoord[2] = gl_MultiTexCoord0+vec4(2,0,0,0);	
		gl_TexCoord[3] = gl_MultiTexCoord0+vec4(3,0,0,0);	
		gl_TexCoord[4] = gl_MultiTexCoord0-vec4(1,0,0,0);	
		gl_TexCoord[5] = gl_MultiTexCoord0-vec4(2,0,0,0);	
		gl_TexCoord[6] = gl_MultiTexCoord0-vec4(3,0,0,0);	
	} else {
 	        gl_TexCoord[1] = gl_MultiTexCoord0+vec4(0,1,0,0);	
		gl_TexCoord[2] = gl_MultiTexCoord0+vec4(0,2,0,0);	
		gl_TexCoord[3] = gl_MultiTexCoord0+vec4(0,3,0,0);	
		gl_TexCoord[4] = gl_MultiTexCoord0-vec4(0,1,0,0);	
		gl_TexCoord[5] = gl_MultiTexCoord0-vec4(0,2,0,0);	
		gl_TexCoord[6] = gl_MultiTexCoord0-vec4(0,3,0,0);	
	}
	gl_Position = ftransform();
}