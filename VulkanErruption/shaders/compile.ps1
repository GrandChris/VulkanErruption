
$glslc = "C:/VulkanSDK/1.1.126.0/Bin32/glslc.exe"

function Compile-File($Path) {
    Write-Output $Path
    $filename = Split-Path -Path $Path -Leaf

    $spv_filename = "generated\" + $filename + ".spv"

    & $glslc $Path -O -o $spv_filename
    .\FileToHeader.exe $spv_filename

    if(Test-Path $spv_filename) {
        Remove-Item $spv_filename
    }
}

Compile-File "src\shader.vert"
Compile-File "src\vec3.vert"
Compile-File "src\index_logo.vert"
Compile-File "src\dynamic.vert"
Compile-File "src\cube.vert"
Compile-File "src\array3D_cube.vert"
Compile-File "src\array3D.vert"
Compile-File "src\cube.geom"
Compile-File "src\cube_specular.geom"
Compile-File "src\array3D_cube.geom"
Compile-File "src\array3D_cube_specular.geom"
Compile-File "src\shader.frag"
Compile-File "src\specular.frag"
Compile-File "src\array3D_cube_specular.geom"

Compile-File "src\two_vertex_cube.vert"
Compile-File "src\two_vertex_cube_specular.geom"

