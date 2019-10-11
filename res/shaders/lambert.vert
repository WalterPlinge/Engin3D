#version 410

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;

void main() {
    gl_Position = projection * view * translate * rotate * scale * vec4(vPosition, 1.0f);
    normal = vec3(rotate * vec4(vNormal, 1.0f));
}