varying vec4 projCoord;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;
varying vec4 diffuse,ambient;
varying float dist;

void main()
{
	vec4 realPos = gl_ModelViewMatrix * gl_Vertex;
	vec3 aux;

	normal = normalize(gl_NormalMatrix * gl_Normal);
	aux = vec3(gl_LightSource[0].position-realPos);
	lightDir = normalize(aux);
	dist = length(aux);
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
 
	projCoord     = gl_TextureMatrix[0] * realPos;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();

}
