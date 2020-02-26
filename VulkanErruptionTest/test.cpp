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



TEST(TestRenderObject, dynamic) {

	bool succes = true;


	//std::vector<DynamicPointRenderObject::Vertex> vertices =
	//{
	//	{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
	//	{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 1.0f}},
	//	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	//	{{-2.0f, 2.0f, 0.0f}, {0.0f, 1.0f, 1.0f}}
	//};


	std::vector<DynamicPointRenderObject::Vertex> vertices =
	{
		{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},
		{{0.0f, 0.0f, 0.5f}, {0.5f, 0.5f, 0.5f}},
		{{0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
	};

	//std::vector<DynamicPointRenderObject::Vertex> vertices =
	//{
	//	{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
	//	{{0.0f, 0.2f, 0.0f}, {0.0f, 1.0f, 1.0f}},
	//	{{0.0f, -0.2f, 0.0f}, {0.5f, 0.5f, 0.5f}},
	//	{{0.2f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	//	{{-0.2f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},

	//	{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
	//};

	auto app = ParticleRenderer::createVulkan();

	auto obj = DynamicPointRenderObject::createVulkan();

	obj->setVertices([&]() -> std::vector<DynamicPointRenderObject::Vertex>
		{
			static float count = 0;
			count += 0.001f;
			vertices[0].pos.x = sin(count += 0.001f);
			//vertices[1].pos.y = 0.5f* sin(count += 0.001f);
			app->setView({ -4.0f * sin(count * 0.1f), -4.0f * cos(count * 0.1f), 2.0f });// *sin(count * 0.13f) });

			//app->setView({ -1.0f * sin(count * 0.10f), -1.0f * cos(count * 0.10f), 0.5f });
			//app->setView({ -1.0f, -1.0f, 1.0f + 1.0f * sin(count * 0.13f) });
			//app->setView({ -1.0f + sin(count * 0.1f), -0.5f + cos(count * 0.1f), 0.7f + 0.5f * sin(count * 0.13f) });

			return vertices;
		}, vertices.size());

	obj->setPosition({ 0.0f, 0.0f, 0.0f });
	obj->setUseCubes(true);

	app->add(std::move(obj));
	app->setView({ 4.0, 2.0f, 1.0f });

	app->run();


	EXPECT_TRUE(succes);
}