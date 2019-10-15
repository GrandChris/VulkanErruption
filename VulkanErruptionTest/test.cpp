///////////////////////////////////////////////////////////////////////////////
// File: test.cpp
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Tests VulkanErruption
///////////////////////////////////////////////////////////////////////////////


#include "gtest/gtest.h"

#include "ParticleRenderer.h"

#include <iostream>
#include <thread>

using namespace std;

TEST(TestCaseName, TestName) {

	bool succes = true;


	auto app = ParticleRenderer::createVulkan();

	std::vector<Vertex> vertices =
	{
		{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-2.0f, 2.0f}, {0.0f, 1.0f, 1.0f}}
	};

	app->draw(vertices);

	auto app2 = ParticleRenderer::createVulkan();
	app2->draw(vertices);
	
	app->join();
	app2->join();

  EXPECT_TRUE(succes);
}