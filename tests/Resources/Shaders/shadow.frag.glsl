uniform sampler2DShadow shadowMap;
uniform sampler2D base;
varying vec4 projCoord;
varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;
	
const float kTransparency = 0.3;	

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

	vec3 shadowUV = projCoord.xyz / projCoord.q;
	float mapScale = 1.0 / 1024.0;

	vec4 shadowColor = shadow2D(shadowMap, shadowUV);
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale,  mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale, -mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale,  	      0, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale,  mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale, -mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale,  	      0, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(        0,  mapScale, 0));
	shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(        0, -mapScale, 0));

	shadowColor = shadowColor / 9.0;

	shadowColor = clamp(shadowColor, 0.0, 1.0);
	
	vec3 coordPos  = projCoord.xyz / projCoord.w;
	vec4 base = texture2D( base, gl_TexCoord[0].st);

	if(shadowUV.x >= 0.0 && shadowUV.y >= 0.0 && shadowUV.x <= 1.0 && shadowUV.y <= 1.0 )
   	{
	   gl_FragColor = color * base * shadowColor;
 	}
	else
	{
	   gl_FragColor = base * ambient;
	}
	//gl_FragColor = vec4( coordPos.z, coordPos.z,coordPos.z,1.0);
}

