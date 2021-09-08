

#include "vulkan_erruption/vulkan_erruption.h"
#include "vulkan_erruption/shader/advanced_shader.h"
#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object.h"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <ranges>


using namespace std;

int main() {

    cout << "Hello World!" << endl;

    std::vector<glm::vec3> vertexBufferData = {
        {0.5f, 0.5f, 0.0f},
        {0.5f, -0.5, 0.0f},
        {-0.5f, -0.5, 0.0f}
    };

    ConcreteShaderObject<DynamicPointObject, AdvancedShader> obj(vertexBufferData.size());

    size_t count = 0;
    auto lbd = [&](std::span<AdvancedShader::VertexBufferElement> data) {
        vertexBufferData[0].x = sin(count++ * 0.001f);

        std::copy(vertexBufferData.cbegin(), vertexBufferData.cend(), data.begin());
    };

    obj.updateVertexBuffer.add(lbd);

    RenderEngine renderEngine;
	renderEngine.add(obj);

    renderEngine.run();


    return 0;
}

