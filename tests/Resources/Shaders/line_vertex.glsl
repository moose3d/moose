attribute vec4 endposThickness;
varying vec4 color;
uniform float time;
void main()
{
	/* into EYE coordinates */	
	vec4 endPos = gl_ModelViewProjectionMatrix * vec4(endposThickness.xyz,gl_Vertex.w);
	vec4 startPos = gl_ModelViewProjectionMatrix * gl_Vertex;

	vec3 linedir = (endPos - startPos).xyz;
	vec3 offsetVec = normalize(cross(startPos.xyz,linedir));


	/*float texcoef = abs( dot( normalize(linedir), normalize(eyeToStart) ));*/

	startPos = vec4(startPos.xyz + (offsetVec * endposThickness.w),startPos.w);
	gl_Position = startPos;
	gl_TexCoord[0].x = (gl_MultiTexCoord0.x * length(endPos - startPos))+ time;
	gl_TexCoord[0].y = gl_MultiTexCoord0.y;
}
