#pragma once

#include"gameObjects/gameobject.h"
#include"gameObjects/ball.h"
#include<memory>

namespace factory
{
    // Mesh
    static float vertices[] = {
         0.5f,  0.5f, 0.f,      //top right
         0.5f, -0.5f, 0.f,      //bottom right
        -0.5f, -0.5f, 0.f,       //bottom left
        -0.5f,  0.5f, 0.f      //top left
    };
    static uint32_t elements[] = {
        0, 1, 3,
        3, 1, 2
    };

    // Shader
    static const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;
                    uniform mat4 model = mat4(1.0);
                    out vec3 pos;
                    void main()
                    {
                        pos = position;
                        gl_Position = model * vec4(position,1.0);
                    }
                      )";

    static const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;

                    void main()
                    {
                        outColor = vec4(vec3(0.0),1.0);
                    }
                      )";

    static const char* ballFragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    in vec3 pos;
                    void main()
                    {
                        if(length(pos)<0.5 && length(pos)>0.4) 
                        {
                            outColor = vec4(0.f,0.f,0.f,1.0f);
                        }
                        else if(length(pos)>0.5) discard;
                        else outColor = vec4(vec3(1.00,1.00,0.17),1.0);
                    }
                      )";
    static const char* BGfragShader = R"(
                #version 410 core
                    out vec4 fragColor;
                    in vec3 pos;
                    uniform float iTime=0.0f;;
                    void main()
                    {
                        vec2 uv = pos.xy;
                        vec2 uv2 = uv;
                        vec2 uv3;
                        float stalk;
                        float petal;
                        float center;
                        float clouds;
	                    uv *= 5.;
                        uv3 = uv;
    
                        uv3 -= vec2(0.2, 0.0);
                        if(uv2.y < 0.15)
   		                    stalk = 1.- smoothstep(0.80, 1.0, abs((uv3.x + sin(uv3.y * 0.5)) * 20.));
    
                        uv -= vec2(-0.2, 0.8);
                        float r = cos(atan(uv.y, uv.x) * 7.) * .6 + 0.1;
                        petal = smoothstep(r-0.05, r, 1.-length(uv));
    
                        center = smoothstep(0.8 - 0.01, 0.8, 1.-length(uv));
    
                        vec2 c0 = uv3;
                        c0.x += iTime;
                        c0 = mod(c0, vec2(8., 4.)) - vec2(1.8, 1.4);
    
                        if(uv2.y > 0.05)
                        {
    	                    r = (sin(c0.x * 8.) * 0.8 + 9.);
    	                    c0 *= 5.;
    	                    clouds += 1.-smoothstep(r-0.1, r, length(vec2(c0.x, c0.y * 3.)));
                        }

    
                        // mix layer together
                        vec3 col = vec3(0.0, 0.6, 1.0);
                        col = mix(col, vec3(1.0, 1.0, 1.0), clouds);
                        col = mix(col, vec3(0.2, 0.6, 0.2), stalk);
                        col = mix(col, vec3(1.0, 0.4, 0.4), petal);
                        col = mix(col, vec3(1.0, 1.0, 0.4), center);
    
                        fragColor = vec4(col, 1.0);
                    }
                        )";
    std::shared_ptr<GameObject> createPaddle(const glm::vec2& Pos)
    {
        std::shared_ptr<banana::graphics::Mesh> mesh=std::make_shared<banana::graphics::Mesh>(vertices,4,3,elements,6);
        std::shared_ptr<banana::graphics::Shader> shader=std::make_shared<banana::graphics::Shader>(vertexShader,fragShader);
        glm::vec2 size{ 0.1f,0.3f };
        auto pad = std::make_shared<GameObject>(mesh, shader, Pos,size);
        return pad;
    }

    std::shared_ptr<Ball> createBall(const glm::vec2& Pos)
    {
        std::shared_ptr<banana::graphics::Mesh> mesh = std::make_shared<banana::graphics::Mesh>(vertices, 4, 3, elements, 6);
        std::shared_ptr<banana::graphics::Shader> shader = std::make_shared<banana::graphics::Shader>(vertexShader, ballFragShader);
        glm::vec2 size{ 0.075f,0.1f };
        auto pad = std::make_shared<Ball>(mesh, shader, Pos, size);
        return pad;
    }

    std::shared_ptr<GameObject> createBG()
    {
        std::shared_ptr<banana::graphics::Mesh> mesh = std::make_shared<banana::graphics::Mesh>(vertices, 4, 3, elements, 6);
        std::shared_ptr<banana::graphics::Shader> shader = std::make_shared<banana::graphics::Shader>(vertexShader, BGfragShader);
       
        glm::vec2 pos{ 0.f,0.f };
        glm::vec2 size{ 2.f,3.f };
        auto pad = std::make_shared<GameObject>(mesh, shader, pos, size);
        return pad;
    }
}