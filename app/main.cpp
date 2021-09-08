

// #include "vulkan_erruption/vulkan_erruption.h"
// #include "vulkan_erruption/shader/vertex_cube_shader/vertex_cube_shader.h"
// #include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object.h"

// #include <iostream>
// #include <cassert>
// #include <algorithm>


// using namespace std;

// int main2() {

//     cout << "Hello World!" << endl;


//     // using ShaderType = VertexCubeShader;
// 	// using ShaderObject = DynamicPointObject<ShaderType>;
// 	using Vertex = VertexCubeShader::VertexBufferElement;
// 	using Vertices = std::vector<Vertex>;

// 	Vertices vertices =
// 	{
// 		{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
// 		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
// 		{{0.0f, 0.0f, 0.5f}, {0.5f, 0.5f, 0.5f}},
// 		{{0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
// 		{{0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
// 		{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
// 	};

//     VertexCubeShader shader;
//     DynamicPointObject obj = DynamicPointObject(shader, vertices.size());

//     auto lbaUpdateVertices = [&](std::span<Vertex> & data, size_t const fullUpdate)
//     {
//         assert(data.size() == vertices.size());
// 		std::copy(vertices.cbegin(), vertices.cend(), data.begin());
// 		// std::ranges::copy(vertices, data);
//     };

//     obj.updateVertexBuffer.add(lbaUpdateVertices);
	
// 	// auto obj = RenderObj::createVulkan();

// 	// obj->setVertices([&]() -> Vertices
// 	// 	{
// 	// 		return vertices;
// 	// 	}, vertices.size());

// 	// obj->setPosition({ 0.0f, 0.0f, 0.0f });


// 	// auto obj2 = RenderObj::createVulkan();

// 	// obj2->setVertices([&]() -> Vertices
// 	// 	{
// 	// 		return vertices;
// 	// 	}, vertices.size());

// 	// obj2->setPosition({ 1.0f, 0.5f, -1.0f });

	
// 	// auto app = ParticleRenderer::createVulkan();
// 	// app->add(std::move(obj));
// 	// app->add(std::move(obj2));
// 	// app->setView({ 6.0, 5.0f, 1.0f });

// 	// app->run();


//     RenderEngine renderEngine;
// 	renderEngine.add(obj);

//     renderEngine.run();


//     return 0;
// }

