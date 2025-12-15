// Created with GLSLtoC
// Shader program: SpriteInstanced

#pragma once

inline static const char* kSpriteInstancedVertex = R""(#version 460 core

// Per-vertex attributes (shared quad)
layout (location = 0) in vec4 aVertex;// position.xy, texcoord.xy

// Per-instance attributes
layout (location = 1) in mat4 aInstanceTransform;
layout (location = 5) in vec4 aInstanceTint;

out vec2 TexCoord;
out vec4 TintColor;

void main() {
    // Transform vertex position by instance transform
    gl_Position = aInstanceTransform * vec4(aVertex.xy, 0.0, 1.0);
    TexCoord = aVertex.zw;
    TintColor = aInstanceTint;
})"";

inline static const char* kSpriteInstancedFragment = R""(#version 460 core

in vec2 TexCoord;
in vec4 TintColor;

out vec4 FragColor;

uniform sampler2D uSprite;

void main() {
    vec4 texColor = texture(uSprite, TexCoord);
    FragColor = texColor * TintColor;
})"";

