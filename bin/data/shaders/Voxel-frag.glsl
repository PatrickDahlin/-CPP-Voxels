
in vec2 fragUv;
in vec3 fragNorm;

uniform sampler2D myTex;

out vec4 outColor;

void main()
{
	vec3 norm = normalize(fragNorm);
	norm = mat3(transpose(inverse(modelMatrix))) * norm;
	vec3 lightDir = vec3(1,1,1);
	float diff = max(dot(norm, lightDir),0.4);
	outColor = texture(myTex, fragUv) * diff;
	outColor.a = 1.0;
}