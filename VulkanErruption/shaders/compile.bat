C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe shader.vert -O -o vert.spv
FileToHeader.exe vert.spv

C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe vec3.vert -O -o vec3_vert.spv
FileToHeader.exe vec3_vert.spv

C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe index_logo.vert -O -o index_logo_vert.spv
FileToHeader.exe index_logo_vert.spv

C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe dynamic.vert -O -o dynamic_vert.spv
FileToHeader.exe dynamic_vert.spv


C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe shader.frag -O -o frag.spv
FileToHeader.exe frag.spv


