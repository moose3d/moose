uniform sampler2DShadow		shadowMap;
uniform sampler2D		base;
varying vec4			projCoord;
varying vec4			diffuse,ambient;
varying vec3			normal,lightDir,halfVector;
varying float			dist;	


void main()
{
  vec3 n,halfV;
  float NdotL,NdotHV;
  /* The ambient term will always be present */
  vec4 color = ambient;

  /* a fragment shader can't write a varying variable, hence we need
     a new variable to store the normalized interpolated normal */
  n = normalize(normal);
  float spotEffect,att;	
  /* compute the dot product between normal and ldir */
  NdotL = max(dot(n,normalize(lightDir)),0.0);
  vec4 base = texture2D( base, gl_TexCoord[0].st);	
  color = color * base;
  if (NdotL > 0.0) 
  {
    spotEffect = dot(normalize(gl_LightSource[0].spotDirection), normalize(-lightDir));

    if ( spotEffect > gl_LightSource[0].spotCosCutoff)
    {
      spotEffect = pow(spotEffect, gl_LightSource[0].spotExponent);
      att = spotEffect / (gl_LightSource[0].constantAttenuation +
			  gl_LightSource[0].linearAttenuation * dist +
			  gl_LightSource[0].quadraticAttenuation * dist * dist);
      color += att * (diffuse * NdotL + ambient);
		
      halfV = normalize(halfVector);
      NdotHV = max(dot(n,halfV),0.0);
      color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
    }
  } 
  vec3 shadowUV = projCoord.xyz / projCoord.q;
  float mapScale = 1.0 / 512.0;		

  // sample PCF shadow
  float shadowColor = shadow2D(shadowMap, shadowUV).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale,  mapScale, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale, -mapScale, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3( mapScale,  	      0, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale,  mapScale, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale, -mapScale, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(-mapScale,  	      0, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(        0,  mapScale, 0)).r;
  shadowColor += shadow2D(shadowMap, shadowUV.xyz + vec3(        0, -mapScale, 0)).r;

  shadowColor = shadowColor / 9.0;
  shadowColor = clamp(shadowColor, 0.0, 1.0);

  if(shadowUV.x >= 0.0 && shadowUV.y >= 0.0 && shadowUV.x <= 1.0 && shadowUV.y <= 1.0 )
  {
    if ( shadowColor < 0.7){
      color = base * ambient;
    }
    else 
    {
      color = color * shadowColor;
    }
  }

  gl_FragColor = color;
}

