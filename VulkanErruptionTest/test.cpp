///////////////////////////////////////////////////////////////////////////////
// File: test.cpp
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Tests VulkanErruption
///////////////////////////////////////////////////////////////////////////////


#include <gtest/gtest.h>


#include "ParticleRenderer.h"
#include "StaticPointRenderObject.h"
#include "TwoPointMovingRenderObject.h"
#include "DynamicPointRenderObject.h"
#include "Array3DShader.h"

#include <iostream>
#include <thread>

using namespace std;

TEST(TestRenderObject, DISABLED_simple) {

	bool succes = true;


	std::vector<StaticPointRenderObject::Vertex> vertices =
	{
		{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-2.0f, 2.0f}, {0.0f, 1.0f, 1.0f}}
	};

	auto app = ParticleRenderer::createVulkan();

	auto obj = StaticPointRenderObject::createVulkan();
	obj->setVertices(vertices);
	app->add(std::move(obj));

	app->run();


	EXPECT_TRUE(succes);
}


TEST(TestRenderObject, DISABLED_twoPoint) {

	bool succes = true;


	std::vector<TwoPointMovingRenderObject::Vertex> vertices =
	{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} ,{1.0f, 1.0f, 1.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
		{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}},
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.0f, 0.0f, 0.5f}, {0.0f, 0.0f, 0.5f} ,{1.0f, 1.0f, 1.0f}}
	};

	auto app = ParticleRenderer::createVulkan();

	auto obj = TwoPointMovingRenderObject::createVulkan();
	obj->setVertices(vertices);
	obj->setTime(4.0);
	obj->setPosition({ -4.0f, 0.0f, 0.0f });
	app->add(std::move(obj));

	app->run();


	EXPECT_TRUE(succes);
}



TEST(TestRenderObject, DISABLED_dynamic) {

	bool succes = true;


	std::vector<DynamicPointRenderObject<>::Vertex> vertices =
	{
		{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
		{{0.0f, 0.0f, 0.5f}, {0.5f, 0.5f, 0.5f}},
		{{0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
	};

	auto app = ParticleRenderer::createVulkan();

	auto obj = DynamicPointRenderObject<>::createVulkan();

	obj->setVertices([&]() -> std::vector<DynamicPointRenderObject<>::Vertex>
		{
			static float count = 0;
			count += 0.001f;
			vertices[0].pos.x = sin(count += 0.001f);
			app->setView({ -4.0f * sin(count * 0.1f), -4.0f * cos(count * 0.1f), 2.0f + 0.5 *sin(count * 0.13f) });


			return vertices;
		}, vertices.size());

	obj->setPosition({ 0.0f, 0.0f, 0.0f });

	app->add(std::move(obj));
	app->setView({ 4.0, 2.0f, 1.0f });

	app->run();


	EXPECT_TRUE(succes);
}


#include <random>

void fillNormalDistributed(std::vector<DynamicPointRenderObject<>::Vertex> & vec, size_t const n, size_t const N)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-static_cast<int>(N), static_cast<int>(N));

	for(size_t i = 0; i < n; ++i)
	{
		float const x = static_cast<float>(distribution(generator));
		float const y = static_cast<float>(distribution(generator));
		float const z = static_cast<float>(distribution(generator));

		float const r = 0.5f / N * x + 0.5f;
		float const g = 0.5f / N * y + 0.5f;
		float const b = 0.5f / N * z + 0.5f;

		vec.push_back({ glm::vec3(x, y, z), glm::vec3(r, g, b) });
	}

}

void fill(std::vector<DynamicPointRenderObject<>::Vertex>& vec, int const N)
{

	for (int x = -N/2; x < N/2; ++x)
	{

		for (int y = -N / 2; y < N / 2; ++y)
		{

			for (int z = -N / 2; z < N / 2; ++z)
			{
				float const r = 0.5f / N * x + 0.5f;
				float const g = 0.5f / N * y + 0.5f;
				float const b = 0.5f / N * z + 0.5f;

				vec.push_back({ glm::vec3(x, y, z), glm::vec3(r, g, b) });
			}

		}
	}

}





TEST(TestRenderObject, DISABLED_dynamic_performance) {

	bool succes = true;

	//size_t const n = 2'000'000;
	//size_t const N = static_cast<size_t>(sqrt(n)) / 8;

	size_t const n = 2'000'000;
	size_t const N = 126;
	
	std::vector<DynamicPointRenderObject<>::Vertex> vertices;
	//fill(vertices, N);
	fillNormalDistributed(vertices, n, N);


	auto app = ParticleRenderer::createVulkan();

	auto obj = DynamicPointRenderObject<>::createVulkan();


	float const Nf = N * 3.0f;
	obj->setVertices([&]() -> std::vector<DynamicPointRenderObject<>::Vertex>
		{
			static float count = 0;
			count += 0.1f;
			app->setView({ -Nf * sin(count * 0.1f), -Nf * cos(count * 0.1f), Nf/2 + Nf/10 * sin(count * 0.13f) });


			return vertices;
		}, vertices.size());

	obj->setPosition({ 0.0f, 0.0f, 0.0f });

	app->add(std::move(obj));
	app->setView({ 4.0, 2.0f, 1.0f });

	app->run();


	EXPECT_TRUE(succes);
}



TEST(TestRenderObject, DISABLED_array3D_simple)
{
	using RenderObj = DynamicPointRenderObject<Array3DShader<>>;
	using Vertices = std::vector<RenderObj::Vertex>;

	Vertices vertices =
	{
		{{20}, {40}, {80}, {120}, {160}, {200}, {255}, {255}, {255}, {255}}
	};

	auto app = ParticleRenderer::createVulkan();

	auto obj = RenderObj::createVulkan();

	obj->setVertices([&]() -> Vertices
		{
			static float count = 0;
			count += 0.001f;
			app->setView({ -4.0f * sin(count * 0.1f), -4.0f * cos(count * 0.1f), 2.0f + 0.5 * sin(count * 0.13f) });


			return vertices;
		}, vertices.size());

	obj->setPosition({ 0.0f, 0.0f, 0.0f });
	Array3DShader<>::UniformBufferObject ubo;
	ubo.maxIndex = glm::uvec3(2, 2, 4);
	obj->setUbo(ubo);

	app->add(std::move(obj));
	app->setView({ 4.0, 2.0f, 1.0f });


	app->run();


	EXPECT_TRUE(true);
}

template<typename T>
void fill(std::vector<T>& vec, size_t const n, size_t const N)
{
	for (size_t i = 0; i < n; ++i)
	{
		vec.push_back({ i % 255 });
	}
}

TEST(TestRenderObject, DISABLED_array3D_performance)
{

	//size_t const n = 64'000'000;
	//size_t const N = 400;

	//size_t const n = 2'000'000;
	//size_t const N = 126;

	size_t const n = 16'000'000;
	size_t const N = 252;

	using ShaderType = Array3DShader<eShader::Diffuse>;
	using RenderObj = DynamicPointRenderObject<ShaderType>;
	using Vertices = std::vector<RenderObj::Vertex>;

	Vertices vertices;
	fill(vertices, n, N);

	auto app = ParticleRenderer::createVulkan();

	auto obj = RenderObj::createVulkan();

	float const Nf = N /2;
	obj->setVertices([&]() -> Vertices
		{
			static float count = 0;
			count += 0.01f;

			float const x = N / 2 + (3 * Nf + 10) * sin(count);
			float const y = N / 2 + (3 * Nf + 10) * cos(count);
			float const z = N / 2.0f + (3.0f * Nf + 10.0f) / 2.0f + (Nf / 2.0f) * sin(count * 0.7f);;

			app->setView({ x, y, z }, { N / 2, N / 2, N / 2 });


			return vertices;
		}, vertices.size());

	obj->setPosition({ 0.0f, 0.0f, 0.0f });
	ShaderType::UniformBufferObject ubo;
	ubo.maxIndex = glm::uvec3(N, N, N);
	obj->setUbo(ubo);

	app->add(std::move(obj));
	app->setView({ 4.0, 2.0f, 1.0f });


	app->run();


	EXPECT_TRUE(true);
}




TEST(TestMultiObject, TwoCubes)
{
	using ShaderType = VertexCubeShader;
	using RenderObj = DynamicPointRenderObject<ShaderType>;
	using Vertices = std::vector<RenderObj::Vertex>;

	Vertices vertices =
	{
		{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
		{{0.0f, 0.0f, 0.5f}, {0.5f, 0.5f, 0.5f}},
		{{0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
	};

	
	auto obj = RenderObj::createVulkan();

	obj->setVertices([&]() -> Vertices
		{
			return vertices;
		}, vertices.size());

	obj->setPosition({ 0.0f, 0.0f, 0.0f });


	auto obj2 = RenderObj::createVulkan();

	obj2->setVertices([&]() -> Vertices
		{
			return vertices;
		}, vertices.size());

	obj2->setPosition({ 1.0f, 0.5f, -1.0f });

	
	auto app = ParticleRenderer::createVulkan();
	app->add(std::move(obj));
	app->add(std::move(obj2));
	app->setView({ 6.0, 5.0f, 1.0f });

	app->run();


	EXPECT_TRUE(true);
}