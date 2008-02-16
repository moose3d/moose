uniform sampler2DShadow shadowMap;
varying vec4 projCoord;
varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;
	
	
void main()
{
	vec3 n,halfV;
	float NdotL,NdotHV;
	/* The ambient term will always be present */
	vec4 color = ambient;
	/* a fragment shader can't write a varying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,lightDir),0.0);
	if (NdotL > 0.0) {
		color += diffuse * NdotL;
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += gl_FrontMaterial.specular * 
				gl_LightSource[0].specular * 
				pow(NdotHV, gl_FrontMaterial.shininess);
	}

	const float kTransparency = 0.3;


	float rValue = shadow2DProj(shadowMap, projCoord).r;
	rValue = clamp(rValue, 0.0, 1.0);
	
	vec3 coordPos  = projCoord.xyz / projCoord.w;

	if(coordPos.x >= 0.0 && coordPos.y >= 0.0 && coordPos.x <= 1.0 && coordPos.y <= 1.0 )
   	{
		gl_FragColor = color * rValue;
 
	}
	else
	{
		gl_FragColor = color;
	}
	//gl_FragColor = vec4( coordPos.z, coordPos.z,coordPos.z,1.0);
}

