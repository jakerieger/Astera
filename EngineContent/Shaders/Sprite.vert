#version 460 core
layout (location = 0) in vec4 aVertex;

uniform mat4 uMVP;

out vec2 TexCoord;

void main() {
    vec2 position = aVertex.xy;
    vec2 texCoord = aVertex.zw;
    gl_Position = uMVP * vec4(position, 0.0, 1.0);
    TexCoord = texCoord;
}
