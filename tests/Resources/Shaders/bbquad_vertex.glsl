uniform vec3 camerapos;
attribute vec4 dirWidth;
varying float alpha;
void main()
{

	vec3 start = gl_Vertex.xyz;
	vec3 end   = gl_Vertex.xyz-dirWidth.xyz;
	vec3 linedir = end-start;
	vec3 eyeDir = start-camerapos;
	vec3 off = normalize(cross(eyeDir,linedir));
		
	gl_TexCoord[0].x = gl_MultiTexCoord0.x;
	gl_TexCoord[0].y = gl_MultiTexCoord0.y;
	float coef = abs( dot( normalize(linedir), normalize(eyeDir) ));

	alpha = smoothstep(0.15, 0.35, coef);

	gl_Position = gl_ModelViewProjectionMatrix * vec4(start.xyz+(off*dirWidth.w*0.79),gl_Vertex.w);
}

