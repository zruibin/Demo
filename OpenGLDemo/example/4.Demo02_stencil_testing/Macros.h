/*
 * Macros.h
 *
 * Created by zhouruibin734 on 2020/07/22.
 * Copyright (c) 2020年 zhouruibin734 All rights reserved.
 */

#ifndef MACROS_H
#define MACROS_H

#include <string>

const std::string vShaderSource = R"(

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

)";


const std::string fShaderSource = R"(

#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}

)";

const std::string stencilFShaderSource = R"(

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
}

)";


#endif


