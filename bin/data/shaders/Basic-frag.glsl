in vec4 fragColor;
in vec2 fragUv;

uniform sampler2D myTex;

out vec4 outColor;

void main()
{
	outColor = texture(myTex, fragUv) * fragColor;
}