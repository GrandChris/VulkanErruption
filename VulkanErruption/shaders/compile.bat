C:/VulkanSDK/1.1.114.0/Bin32/glslc.exe shader.vert -o vert.spv
FileToHeader.exe vert.spv

C:/VulkanSDK/1.1.114.0/Bin32/glslc.exe index_logo.vert -o index_logo_vert.spv
FileToHeader.exe index_logo_vert.spv

C:/VulkanSDK/1.1.114.0/Bin32/glslc.exe dynamic.vert -o dynamic_vert.spv
FileToHeader.exe dynamic_vert.spv


C:/VulkanSDK/1.1.114.0/Bin32/glslc.exe shader.frag -o frag.spv
FileToHeader.exe frag.spv


