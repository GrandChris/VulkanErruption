///////////////////////////////////////////////////////////////////////////////
// File:		  main
// Revision:	  1
// Date Creation: 06.11.2019
// Last Change:	  06.11.2019
// Author:		  Christian Steinbrecher
// Descrition:	  Test if CudaPointRenderObject.h
///////////////////////////////////////////////////////////////////////////////


#include <ParticleRenderer.h>
#include <CudaPointRenderObject.h>

#include <CudaVertexBuffer.h>

int main()
{

	// draw
	std::vector<CudaPointRenderObject::Vertex> vertices =
	{
		{{0.0f, -0.0f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-2.0f, 2.0f}, {0.0f, 1.0f, 1.0f}}
	};

	auto app = ParticleRenderer::createVulkan();

	auto obj = CudaPointRenderObject::createVulkan();


	size_t vertexBufferSize = vertices.size();
	CudaExternalVertexBuffer<CudaPointRenderObject::Vertex> dp_VertexBuffer;

	auto lbd = [&](bool init) {
		// manipulate dp_VertexBuffer
		assert(dp_VertexBuffer.size() == vertices.size());

		static float count = 0;
		vertices[0].pos.x = sin(count += 0.001f);


		cudaMemcpy(dp_VertexBuffer.get(), vertices.data(), dp_VertexBuffer.size() * sizeof(CudaPointRenderObject::Vertex), cudaMemcpyHostToDevice);
	};


	obj->setVertices(dp_VertexBuffer, vertexBufferSize, lbd);
	app->add(std::move(obj));

	app->run();


	return 0;
}