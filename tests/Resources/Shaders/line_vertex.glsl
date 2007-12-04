attribute vec4 endposThickness;
varying float alpha;
uniform vec2 time;
void main()
{
	/* into EYE coordinates */	
	vec4 endPos = gl_ModelViewMatrix * vec4(endposThickness.xyz,gl_Vertex.w);
	vec4 startPos = gl_ModelViewMatrix * gl_Vertex;

	vec3 linedir = (endPos - startPos).xyz;
	vec3 offsetVec = normalize(cross(startPos.xyz,linedir));


	/*float texcoef = abs( dot( normalize(linedir), normalize(eyeToStart) ));*/
	alpha = time.y;
	startPos = vec4(startPos.xyz + (offsetVec * endposThickness.w),startPos.w);
	/* into window coordinates */
	gl_Position = gl_ProjectionMatrix * startPos;
	gl_TexCoord[0].x = (gl_MultiTexCoord0.x * length(endPos - startPos))+ time.x;
	gl_TexCoord[0].y = gl_MultiTexCoord0.y;
}
