#version 330 core

layout(location = 0) in vec2 v;

uniform mat4 m;
uniform vec4 c;
uniform float z;

out vec4 vertcolor;

void main() {
    vertcolor = c;
    gl_Position =  m * vec4(v, z/10, 1);
}
