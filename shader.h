#define R(...) #__VA_ARGS__
/*the light version*/

char* vertl = R(
#version 440\n
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) uniform mat4 model; 
layout(location = 2) uniform mat4 view;
layout(location = 3) uniform mat4 projection;
out vec3 color;
void main()
{
	gl_Position = projection*view*model*vec4(vertexPosition, 1.0f);
	color = vertexPosition;
}
);

char* fragl = R(
#version 440\n
out vec4 fragColor;
in  vec3 color;
void main()
{
	fragColor = vec4(color,1.0f);
}
);