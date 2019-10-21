
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Website used to help write code: https://learnopengl.com/code_viewer.php?code=lighting/basic_lighting-exercise2
    // Remember to set gl_Position (correctly) or you will get a black screen...
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
}
