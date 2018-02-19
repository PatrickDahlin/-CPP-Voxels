layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec4 color;
layout(location = 3)in vec2 uv;

out vec2 fragUv;
out vec3 fragNorm;

void main()
{
	fragUv = uv;
	fragNorm = normal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}