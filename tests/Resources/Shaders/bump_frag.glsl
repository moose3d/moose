varying vec3 lightVec;
varying vec3 eyeVec;
varying vec2 texCoord;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

void main (void)
{
	float att = 1.0; 
	vec3 lVec = lightVec;
	vec3 vVec = normalize(eyeVec);
	vec4 base = vec4(texture2D(colorMap, texCoord).xyz,1);
	vec3 bump = normalize( texture2D(normalMap, 
				         texCoord).xyz * 2.0 - 1.0);
	vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient + 
	     	      	gl_LightModel.ambient * gl_FrontMaterial.ambient;

	float diffuse = max( dot(lVec, bump), 0.0 );

	vec4 vDiffuse = gl_LightSource[0].diffuse * 
	     	      	gl_FrontMaterial.diffuse * 
			diffuse;	

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 
	                 gl_FrontMaterial.shininess );
	
	vec4 vSpecular;

	if ( texture2D(colorMap, texCoord).a == 0.0 ){
		vSpecular = vec4(0,0,0,0); 
	} else {
		vSpecular = gl_LightSource[0].specular * 
			    gl_FrontMaterial.specular * 
			    specular;	
	}
	gl_FragColor = ( vAmbient*base + vDiffuse*base + vSpecular) * att;

}
