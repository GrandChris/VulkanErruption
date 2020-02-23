C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe shader.vert -O -o generated/shader_vert.spv
FileToHeader.exe generated/shader_vert.spv

C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe vec3.vert -O -o generated/vec3_vert.spv
FileToHeader.exe generated/vec3_vert.spv

C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe index_logo.vert -O -o generated/index_logo_vert.spv
FileToHeader.exe generated/index_logo_vert.spv

C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe dynamic.vert -O -o generated/dynamic_vert.spv
FileToHeader.exe generated/dynamic_vert.spv


C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe shader.frag -O -o generated/shader_frag.spv
FileToHeader.exe generated/shader_frag.spv


