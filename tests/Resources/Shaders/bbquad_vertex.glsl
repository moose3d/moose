
attribute vec4 dirWidth;
varying float alpha;
void main()
{

	vec4 start = gl_ModelViewMatrix * gl_Vertex;
	vec4 end   = gl_ModelViewMatrix * vec4(gl_Vertex.xyz-dirWidth.xyz,gl_Vertex.w);
	vec4 linedir = vec4(end.xyz-start.xyz,0);
	vec3 off = normalize(cross(-start.xyz,linedir.xyz));
	float coef = abs( dot( normalize(linedir.xyz), normalize(start.xyz) ));
	alpha = smoothstep(0.15, 0.35, coef);

	gl_Position = gl_ProjectionMatrix * vec4(start.xyz+(off*dirWidth.w*0.79),start.w);
	gl_TexCoord[0].x = gl_MultiTexCoord0.x;
	gl_TexCoord[0].y = gl_MultiTexCoord0.y;
}

