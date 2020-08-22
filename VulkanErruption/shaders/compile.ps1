##############################################################################
# File: compile.ps1.h
# Date: 22.08.2020
# Author: FileToHeader
# Description: Compiles shaders
##############################################################################


$glslc = "C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe"

function Compile-File($Path) {
    Write-Output $Path
    $filename = Split-Path -Path $Path -Leaf
    $filename = $filename.Replace(".", "_")

    $spv_filename = "generated\" + $filename + ".spv"
    $h_filename = "generated\" + $filename + ".h"

    # check if file is newer
    $date1 = (Get-Item -Path $Path).LastWriteTime

    if(-Not (Test-Path $h_filename -NewerThan $date1))
    {
        & $glslc $Path -O -o $spv_filename
        .\FileToHeader.exe $spv_filename  $h_filename
        if(Test-Path $spv_filename) {
            Remove-Item $spv_filename
        } 
    }
}

Compile-File ".\src\shader.vert"
Compile-File ".\src\shader.frag"

Compile-File ".\src\vec3.vert"
Compile-File ".\src\specular.frag"

Compile-File ".\src\index_logo.vert"

# Compile-File "src\dynamic.vert"
# Compile-File "src\cube.vert"
# Compile-File "src\cube.geom"
# Compile-File "src\cube_specular.geom"

Compile-File ".\src\two_vertex_cube.vert"
Compile-File ".\src\two_vertex_cube_specular.geom"

Compile-File ".\src\array3D.vert"
Compile-File ".\src\array3D_cube.vert"
Compile-File ".\src\array3D_cube.geom"
Compile-File ".\src\array3D_cube_specular.geom"

Compile-File ".\src\VertexCube\cube.vert"
Compile-File ".\src\VertexCube\cube.geom"
Compile-File ".\src\VertexCube\cube_diffuse.geom"
Compile-File ".\src\VertexCube\cube_specular.geom"

Compile-File ".\src\Grid2D\Grid2D.vert"
Compile-File ".\src\Grid2D\Grid2D.geom"
Compile-File ".\src\Grid2D\Grid2D_gouraud.geom"
Compile-File ".\src\Grid2D\Grid2D.frag"

