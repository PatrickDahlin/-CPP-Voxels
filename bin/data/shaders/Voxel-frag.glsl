
in vec2 fragUv;
in vec3 fragNorm;

uniform sampler2D myTex;

out vec4 outColor;

void main()
{
	vec3 norm = normalize(fragNorm);
	norm = mat3(transpose(inverse(modelMatrix))) * norm;
	vec3 lightDir = vec3(1,1,1);

	float diff = dot(norm, lightDir);

	if(diff < 0)
	{
		diff = -diff;
		diff *= 0.1;
	}

	float light = max(diff, 0);
	outColor = texture(myTex, fragUv) * light;
	outColor.a = 1.0;
}