#ifndef MESHDATA_H
#define MESHDATA_H

#include <cstdint>
#include <vector>
#include <glm/vec3.hpp>

struct MeshData{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices; // triangles i0,i1,i2
};

#endif // MESHDATA_H
