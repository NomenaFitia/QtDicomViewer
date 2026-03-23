#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "../data/volume.h"

/**
 * @brief The SegmentationRules
 * @todo adapt the rules from a file, or maybe find a way to automatically set these from other data. Implement the interface segregation principle
 */

struct SegmentationRules {
    int bone_min = 300;
    int soft_min = -29;
    int soft_max = 150;
    int fat_min = -190;
    int fat_max = -30;
};

/**
 * @brief The Segmentation class
 */

class Segmentation
{
public:
    Segmentation();

    /**
     * @brief SegmentVolumeHU
     * @param huVolume Volume with HU values
     * @param segmentationRules
     * @return LabeledVolume ( segmented volume )
     *
     * @todo optimize the space complexity because the function basically create a new volume without deleting the raw one
     */

    static LabeledVolume SegmentVolumeHU(const HUVolume& huVolume, const SegmentationRules& segmentationRules);
};

#endif // SEGMENTATION_H
