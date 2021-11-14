

#include "vulkan_erruption/vulkan_erruption.h"
#include "vulkan_erruption/shader/advanced_shader.h"
#include "vulkan_erruption/shader/cube_shader.h"
#include "vulkan_erruption/shader/cube_array_shader.h"
#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object.h"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <array>


// Vulkan Coordinate System
//  z
//	 /|    
//   |  __ y
//   |   /| 
//   |  /
//   | /
//   |_____________> x
//
//


using namespace std;

auto createCube(float const a = 0.5f)
{
    // Vertices
    const glm::vec3 frontTopLeft =      glm::vec3( a, -a,  a);    
    const glm::vec3 frontTopRight =     glm::vec3( a,  a,  a);   
    const glm::vec3 frontBottomLeft =   glm::vec3( a, -a, -a); 
    const glm::vec3 frontBottomRight =  glm::vec3( a,  a, -a);     

    const  glm::vec3 BackTopLeft =       glm::vec3(-a, -a,  a);  
    const  glm::vec3 BackTopRight =      glm::vec3(-a,  a,  a);    
    const  glm::vec3 BackBottomLeft =    glm::vec3(-a, -a, -a);   
    const  glm::vec3 BackBottomRight =   glm::vec3(-a,  a, -a);   

    const int facesSize = 6;
    const int vertPerFace = 4;
    const int cubeSize = facesSize * vertPerFace;
    const std::array<glm::vec3, cubeSize> cube = 
    {
        frontTopLeft, 
        frontBottomLeft, 
        frontTopRight, 
        frontBottomRight, 

        BackBottomLeft, 
        BackTopLeft, 
        BackBottomRight, 
        BackTopRight, 

        BackTopLeft, 
        BackBottomLeft, 
        frontTopLeft, 
        frontBottomLeft, 

        frontTopRight,
        frontBottomRight,
        BackTopRight,
        BackBottomRight,

        BackTopLeft, 
        frontTopLeft,
        BackTopRight,
        frontTopRight,

        frontBottomLeft, 
        BackBottomLeft,
        frontBottomRight,
        BackBottomRight
    };

    return cube;
}

template<typename T>
auto addColor(T vec, glm::vec3 const & color = {0.5f, 0.4f, 0.3f})
{
    std::vector<AdvancedShader::VertexBufferElement> res;

    for(auto const & elem: vec)
    {
        res.push_back({elem, color});
    }

    return res;
}

class Cube
{
public:
    Cube(glm::vec3 const & pos, glm::vec3 const & color,
        glm::mat4 const & view, glm::mat4 const & proj, AdvancedShader::LightingType light)
    : vertexData(addColor(createCube(), color)),
      mShader(light),
      mObj(vertexData.size(), mShader),
      mPos(pos),
      mView(view),
      mProj(proj)
    {
        mObj.updateVertexBuffer.add<Cube::updateVertexData>(*this);
        mObj.updateUniformBuffer.add<Cube::updateUniformData>(*this);
    }

    ConcreteShaderObject<DynamicPointObject, AdvancedShader> & get() {
        return mObj;
    }

    void updateVertexData(std::span<AdvancedShader::VertexBufferElement> data, bool updateRequired)
    {
        std::copy(std::cbegin(vertexData), std::cend(vertexData), std::begin(data));
    }

    void updateUniformData(AdvancedShader::UnformBuffer & data) 
    {
        data.model =  glm::translate(glm::mat4(1), mPos);
        data.model = glm::rotate(data.model, count++ * 0.00003f, {0.5f, 0.5f, 0.0f});
        data.view = mView;
        data.proj = mProj;
		data.lightPosition = glm::vec3(10.0f, 10.0f, 10.0f);
		data.ambient = 0.2f;
    };

private:
    std::vector<AdvancedShader::VertexBufferElement> const vertexData;

    AdvancedShader mShader;
    ConcreteShaderObject<DynamicPointObject, AdvancedShader> mObj;

    glm::vec3 mPos;

    glm::mat4 const & mView;
    glm::mat4 const & mProj;

    uint64_t count = 0;
};

// void find_neighbour(uint32_t array3D[], size_t const length = 16)
// {
//     for(size_t z = 0; z < 16; ++z) {
//         for(size_t y = 0; y < 16; ++y) {
//             for(size_t x = 0; x < 16; ++x) {
//                 int const left
//             }
//         }
//     }
// }


int main() {

    cout << "Hello World!" << endl;

    glm::mat4 view = glm::mat4(1);
    glm::mat4 proj = glm::mat4(1);
    
    // ########################################################################
    // Advanced Shader

    Cube cub1 = Cube({-1.5f, 0.0f, -1.2f}, {0.1f, 0.4f, 0.7f}, view, proj, AdvancedShader::LightingType::None);
    Cube cub2 = Cube({1.5f, 0.0f, -1.2f}, {0.7f, 0.4f, 0.1f}, view, proj, AdvancedShader::LightingType::Diffuse);
    Cube cub3 = Cube({0.0f, 0.0f, 1.2f}, {0.4f, 0.7f, 0.1f}, view, proj, AdvancedShader::LightingType::Pong);

    // ########################################################################
    // Cube Shader

    CubeShader::VertexBufferElement cubeShaderVertexData[] = {
       {{0.0f, 2.0f, 0.0f}, {0.1f, 0.4f, 0.7f}},
       {{0.0f, 3.0f, 0.0f}, {0.7f, 0.4f, 0.1f}},
       {{0.0f, 4.0f, 0.0f}, {0.4f, 0.7f, 0.1f}},
       {{0.0f, 5.0f, 0.0f}, {0.6f, 0.7f, 0.3f}}
    };

    auto lbdVertexUpdate = [&](std::span<CubeShader::VertexBufferElement> data, bool updateRequired){
        assert(std::size(cubeShaderVertexData) == std::size(data));
        std::copy(std::cbegin(cubeShaderVertexData), std::cend(cubeShaderVertexData), std::begin(data));
    };

    auto lbdUniformUpdate = [&](CubeShader::UnformBuffer & data){
        data.model = glm::mat4(1);
        data.view = view;
        data.proj = proj;
        data.lightPosition = glm::vec3(10.0f, 10.0f, 10.0f);
		data.ambient = 0.2f;
    };

    CubeShader cubeShader;
    ConcreteShaderObject<DynamicPointObject, CubeShader> cubeObject(std::size(cubeShaderVertexData), cubeShader);
    cubeObject.updateVertexBuffer.add(lbdVertexUpdate);
    cubeObject.updateUniformBuffer.add(lbdUniformUpdate);

    // ########################################################################
    // Cube Array Shader

    CubeArrayShader::VertexBufferElement cubeArrayShaderVertexData[16*16*16] = {};
    uint8_t i = 0;
    for(auto & elem: cubeArrayShaderVertexData) {
        uint8_t const r = i++;
        uint8_t const g = 32;
        uint8_t const b = 128;
        uint8_t skip = 1;

        if(3 <= i%16 && i%16 <= 14) {
            skip = 0;
        }

        glm::uint res = (r << 24) | (g << 16) | (b << 8) | skip;

        elem.color = res;
    }

    auto lbdVertexUpdate2 = [&](std::span<CubeArrayShader::VertexBufferElement> data, bool updateRequired){
        assert(std::size(cubeArrayShaderVertexData) == std::size(data));
        std::copy(std::cbegin(cubeArrayShaderVertexData), std::cend(cubeArrayShaderVertexData), std::begin(data));
    };

    auto lbdUniformUpdate2 = [&](CubeArrayShader::UnformBuffer & data){
        data.model = glm::mat4(1);
        data.view = view;
        data.proj = proj;
        data.lightPosition = glm::vec3(10.0f, 10.0f, 10.0f);
		data.ambient = 0.2f;
    };

    CubeArrayShader cubeArrayShader;
    ConcreteShaderObject<DynamicPointObject, CubeArrayShader> cubeArrayObject(std::size(cubeArrayShaderVertexData), cubeArrayShader);
    cubeArrayObject.updateVertexBuffer.add(lbdVertexUpdate2);
    cubeArrayObject.updateUniformBuffer.add(lbdUniformUpdate2);



    // ########################################################################
    // Render Engine

    RenderEngine renderEngine;
	renderEngine.add(cub1.get());
	renderEngine.add(cub2.get());
	renderEngine.add(cub3.get());
	renderEngine.add(cubeObject);
    renderEngine.add(cubeArrayObject);

    uint64_t count = 0;
    auto lbdStartOfNextFrame = [&](){
        glm::vec3 posEye =  {0.0f, -8.0f, 0.0f};
        glm::vec3 posView = {0.0f, 0.0f, 0.0f};

        view = glm::lookAt(posEye, posView, glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, count++ * 0.0003f, {0.0f, 0.0f, 1.0f});
        proj = glm::perspective(glm::radians(45.0f), 
            static_cast<float>(renderEngine.getSwapChainExtent().width) / static_cast<float>(renderEngine.getSwapChainExtent().height), 
            0.1f, 100'000.0f);
        proj[1][1] *= -1; // invert Y for Vulkan
    };

    renderEngine.startOfNextFrame.add(lbdStartOfNextFrame);
    renderEngine.run();


    return 0;
}

