
in vec2 fragUv;
in vec3 fragNorm;

uniform sampler2D myTex;

out vec4 outColor;

void main()
{
	//outColor = vec4(0.6,0.6,0.6,1.0);
	float light = max(0.0,dot(fragNorm, normalize(vec3(1,1,1))));
	outColor = texture(myTex, fragUv);
}