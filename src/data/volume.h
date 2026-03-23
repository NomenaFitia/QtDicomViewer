#pragma once
#include <vector>
#include <cstdint>
#include <glm/vec3.hpp>

// Documentation TODO

enum class TissueLabel : uint8_t { Background = 0, Bone = 1, Soft = 2, Fat = 3 };

struct RawVolume
{
    uint32_t width = 0, height = 0, depth = 0;
    glm::vec3 spacing{ 1,1,1 };
    glm::vec3 origin{ 0,0,0 };
    float direction[9] = { 1,0,0, 0,1,0, 0,0,1 };
    std::vector<uint16_t> rawValue;
};

// initialement valeur de retour de dicomloader

struct HUVolume
{
    uint32_t width = 0, height = 0, depth = 0;     // Columns, Rows, #slices
    glm::vec3 spacing{ 1,1,1 };                        // mm : x=col, y=row, z=slice gap
    glm::vec3 origin{ 0,0,0 };                         // IPP du 1er slice trié
    float direction[9] = { 1,0,0, 0,1,0, 0,0,1 };   // (row, col, normal)

    // Voxels : Hounsfield Units directement
    std::vector<float> voxels;                     // size = W*H*D, en HU
};

struct LabeledVolume
{
    uint32_t width = 0, height = 0, depth = 0;
    glm::vec3 spacing{ 1,1,1 };
    glm::vec3 origin{ 0,0,0 };
    float direction[9] = { 1,0,0, 0,1,0, 0,0,1 };
    std::vector<uint8_t> labels;                  // TissueLabel par voxel (0/1/2/3)
};
