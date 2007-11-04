varying vec4 diffuse,ambientGlobal,ambient,colorv;
	varying vec3 normal,lightDir,halfVector;
	varying float dist;

	void main()
	{	
		vec4 ecPos;
		vec3 aux;
		
		normal = normalize(gl_NormalMatrix * gl_Normal);
		
		/* these are the new lines of code to compute the light's direction */
		ecPos = gl_ModelViewMatrix * gl_Vertex;
		aux = vec3(gl_LightSource[0].position-ecPos);
		lightDir = normalize(aux);
		dist = length(aux);
	
		halfVector = normalize(gl_LightSource[0].halfVector.xyz);
		
		/* Compute the diffuse, ambient and globalAmbient terms */
		diffuse = gl_Color * gl_LightSource[0].diffuse;
		

		ambient = gl_Color * gl_LightSource[0].ambient;
		
		
		colorv = gl_Color;
		gl_Position = ftransform();
	} 