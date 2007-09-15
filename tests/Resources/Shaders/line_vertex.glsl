/*
vector3 a,b,dir  <- given. Dir = a to b.
vector3 v1,v2,v3,v4 <- We're drawing a quad, so they are calculated, but must be sent. Initially v1,v2 = a; v3,v4 =b;
float thickness  <- given for each vertex, either -v or +v.
*/

attribute vec4 endposThickness;
uniform vec3 cameraPos;
varying vec4 color;

void main()
{
	vec3 endPos = endposThickness.xyz;
	vec3 linedir = endPos - gl_Vertex.xyz;				

	vec3 offsetVec = normalize(cross(cameraPos,linedir));
 	vec3 normal = normalize(cross(linedir, offsetVec));
	vec3 eyeToStart = gl_Vertex.xyz - cameraPos;

	/*float texcoef = abs( dot( normalize(linedir), normalize(eyeToStart) ));*/
	vec3 posOffset = gl_Vertex.xyz + (offsetVec * endposThickness.w);
	gl_Position = gl_ModelViewProjectionMatrix * vec4(posOffset.xyz,gl_Vertex.w);	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}




