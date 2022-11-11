#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;
uniform mat4 transform;
out vec3 fragmentColor;

void main(){
    // mat4 transform2 = mat2 * transform;
    gl_Position =  MVP * transform * vec4(vertexPosition_modelspace,1);

    fragmentColor = vertexColor;
}