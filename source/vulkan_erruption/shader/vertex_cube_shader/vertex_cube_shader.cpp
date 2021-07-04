

#include "vertex_cube_shader.h"


#include "shaders/cube.vert.h"
#include "shaders/cube_specular.geom.h"
#include "shaders/specular.frag.h"

#include "shaders/cube.geom.h"
#include "shaders/shader.frag.h"



std::vector<vk::VertexInputAttributeDescription> VertexCubeShader::getVertexAttributeDescriptions() const
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(2);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(VertexBufferElement, pos));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(VertexBufferElement, color));

	return attributeDescriptions;
}


DynamicPointShader::SpecializationData VertexCubeShader::getSpecializationInfoVertexShader() const
{
	SpecializationData res = {};

	return res;
}


DynamicPointShader::SpecializationData VertexCubeShader::getSpecializationInfoGeometryShader() const
{
	SpecializationData res = {};
	res.entries.resize(1);

	std::vector<vk::SpecializationMapEntry> specializationMap(1);

	res.entries[0].setConstantID(0);
	res.entries[0].setSize(sizeof(SpecializationInfoGeometryShader().cubeSize));
	res.entries[0].setOffset(offsetof(SpecializationInfoGeometryShader, cubeSize));

	res.data = std::vector<uint8_t>(sizeof(SpecializationInfoGeometryShader));
	SpecializationInfoGeometryShader specializationInfo = {};
	memcpy(res.data.data(), &specializationInfo, sizeof(SpecializationInfoGeometryShader));

	return res;
}


DynamicPointShader::SpecializationData VertexCubeShader::getSpecializationInfoFragmentShader() const
{
	SpecializationData res = {};

	return res;
}



std::vector<char> VertexCubeShader::getVertexShaderCode() const
{
	switch (mShaderType) {
	default:
	case VertexCubeShaderType::Pong:
		return cube_vert_spv;
		break;
	case VertexCubeShaderType::NoLighting:
		return cube_vert_spv;
		break;
	};
}


std::vector<char> VertexCubeShader::getGeometryShaderCode() const
{
	switch (mShaderType) {
	default:
	case VertexCubeShaderType::Pong:
		return cube_specular_geom_spv;
		break;
	case VertexCubeShaderType::NoLighting:
		return cube_geom_spv;
		break;
	};
}


std::vector<char> VertexCubeShader::getFragmentShaderCode() const
{
	switch (mShaderType) {
	default:
	case VertexCubeShaderType::Pong:
		return specular_frag_spv;
		break;
	case VertexCubeShaderType::NoLighting:
		return shader_frag_spv;
		break;
	};
}
