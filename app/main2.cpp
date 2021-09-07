

#include "vulkan_erruption/vulkan_erruption.h"
#include "vulkan_erruption/shader/simple_shader.h"
#include "vulkan_erruption/object/simple_object/hello_triangle.h"

#include <iostream>
#include <cassert>
#include <algorithm>


using namespace std;

int main() {

    cout << "Hello World!" << endl;


    SimpleShader shader;
    HelloTriangle obj(shader);

    RenderEngine renderEngine;
	renderEngine.add(obj);

    renderEngine.run();


    return 0;
}

