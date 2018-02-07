layout(location = 0)in vec2 position;
layout(location = 2)in vec4 color;
layout(location = 3)in vec2 uv;

out vec4 fragColor;
out vec2 fragUv;

void main()
{
	fragColor = color;
	fragUv = uv;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 0.0, 1.0);
}