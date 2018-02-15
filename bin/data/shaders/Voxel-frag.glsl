in vec4 fragColor;
in vec2 fragUv;

uniform sampler2D myTex;

out vec4 outColor;

void main()
{
	outColor = vec4(0.6,0.6,0.6,1.0);
	//outColor = texture(myTex, fragUv) * fragColor;
}