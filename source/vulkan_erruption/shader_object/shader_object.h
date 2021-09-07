//
// @file: ShaderObject.h
// @author: GrandChris
// @date: 26.06.2021
// @brief: An object containing what and how something is rendered
//

#pragma once

class RenderEngineInterface;

//
// @class: ShaderObject
// @brief: An object containing what and how something is rendered
// @author: GrandChris
// @date: 26.06.2021
//
class ShaderObject 
{
public:
    virtual void setup(RenderEngineInterface&) = 0;
    virtual void draw(RenderEngineInterface&, size_t const imageIndex) = 0;
    virtual void cleanup(RenderEngineInterface&) = 0;
};

