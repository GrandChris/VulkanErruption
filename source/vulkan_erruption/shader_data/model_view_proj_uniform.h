

#pragma once

#include "include_glm.h"

struct ModelViewProjectionUniform
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};