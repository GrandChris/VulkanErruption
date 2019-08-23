///////////////////////////////////////////////////////////////////////////////
// File: File.h
// Date: 23.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Function to load a file
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <fstream>

inline std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t const fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}