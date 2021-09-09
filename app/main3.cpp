

#include "vulkan_erruption/vulkan_erruption.h"
#include "vulkan_erruption/shader/advanced_shader.h"
#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object.h"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <array>


using namespace std;

int main() {

    cout << "Hello World!" << endl;

    AdvancedShader::VertexBufferElement vertexBufferData[] = {
        {{0.5f, 0.5f, 0.0f}, {0.8f, 0.0f, 0.0f}},
        {{0.5f, -0.5, 0.0f}, {0.0f, 0.8f, 0.0f}},
        {{-0.5f, -0.5, 0.0f}, {0.0f, 0.0f, 0.8f}}
    };

    ConcreteShaderObject<DynamicPointObject, AdvancedShader> obj(std::size(vertexBufferData));

    size_t count = 0;
    auto lbdVertex = [&](std::span<AdvancedShader::VertexBufferElement> data) {
        vertexBufferData[0].pos.x = sin(count++ * 0.001f);

        std::copy(std::cbegin(vertexBufferData), std::cend(vertexBufferData), std::begin(data));
    };

    obj.updateVertexBuffer.add(lbdVertex);


    auto lbdUniform = [&](AdvancedShader::UnformBuffer & data) {
        data.model = glm::mat4(1);
        data.view = glm::mat4(1);
        data.proj = glm::mat4(1);
    };

    obj.updateUniformBuffer.add(lbdUniform);


    RenderEngine renderEngine;
	renderEngine.add(obj);

    renderEngine.run();


    return 0;
}

