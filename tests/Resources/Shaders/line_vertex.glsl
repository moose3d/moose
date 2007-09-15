/*
vector3 a,b,dir  <- given. Dir = a to b.
vector3 v1,v2,v3,v4 <- We're drawing a quad, so they are calculated, but must be sent. Initially v1,v2 = a; v3,v4 =b;
float thickness  <- given for each vertex, either -v or +v.
*/

attribute vec4 linedirThickness;
uniform vec3 cameraPos;

void main()
{
	vec3 linedir = linedirThickness.xyz;
	vec3 eyeDir = cameraPos - gl_Vertex.xyz;
	vec3 offsetVec = normalize(cross(cameraPos,linedir));
	vec3 middlePoint = gl_Vertex.xyz + (linedir * 0.5);
	float texcoef = dot( normalize(linedir),middlePoint );
	vec3 posOffset = gl_Vertex.xyz + (offsetVec * linedirThickness.w);
	vec3 perpVector = normalize(cross(offsetVec,linedir));
	posOffset = (perpVector * (texcoef * abs(linedirThickness.w))) + posOffset;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(posOffset.xyz,gl_Vertex.w);	



}




