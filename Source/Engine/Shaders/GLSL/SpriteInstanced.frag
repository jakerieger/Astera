#version 460 core

in vec2 TexCoord;
in vec4 TintColor;

out vec4 FragColor;

uniform sampler2D uSprite;

void main() {
    vec4 texColor = texture(uSprite, TexCoord);
    FragColor = texColor * TintColor;
}