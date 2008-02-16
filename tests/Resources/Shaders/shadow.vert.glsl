varying vec4 projCoord;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;
varying vec4 diffuse,ambient;
void main()
{
	vec4 realPos = gl_ModelViewMatrix * gl_Vertex;


	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

	projCoord     = gl_TextureMatrix[0] * realPos;
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}
