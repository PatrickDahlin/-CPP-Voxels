layout(location = 0)in vec3 position;
layout(location = 3)in vec2 uv;

out vec2 fragUv;
out vec3 fragNorm;

void main()
{
	fragUv = uv;
	mat4 rotView = mat4(mat3(viewMatrix));
	gl_Position = projectionMatrix * rotView * modelMatrix * vec4(position, 1.0);
}