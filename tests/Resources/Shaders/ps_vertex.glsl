attribute vec4 stuff;
varying float alpha;
void main()
{
	vec4 center = gl_ModelViewMatrix * gl_Vertex;
	center.x += stuff.w * 1.0 * stuff.x;
	center.y += stuff.w * 1.0 * stuff.y;
	alpha = stuff.z;
	gl_Position = gl_ProjectionMatrix * center;
	gl_TexCoord[0].x = clamp(stuff.x + 1, 0.0, 1.0);
	gl_TexCoord[0].y = clamp(stuff.y + 1, 0.0, 1.0);
}

