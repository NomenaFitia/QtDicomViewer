#include "segmentation.h"

#include <stdexcept>
#include <algorithm>

static inline TissueLabel ClassifyHU(float hu, const SegmentationRules& r) {
    if ((int)hu >= r.bone_min) return TissueLabel::Bone;
    if ((int)hu >= r.fat_min && (int)hu <= r.fat_max) return TissueLabel::Fat;
    if ((int)hu >= r.soft_min && (int)hu <= r.soft_max) return TissueLabel::Soft;
    return TissueLabel::Background;
}

LabeledVolume Segmentation::SegmentVolumeHU(const HUVolume& huVolume, const SegmentationRules& segmentationRules)
{
    const size_t N = (size_t)huVolume.width * huVolume.height * huVolume.depth;
    if (huVolume.voxels.size() != N) throw std::runtime_error("SegmentVolumeHU: taille voxels != W*H*D");

    LabeledVolume out;
    out.width = huVolume.width; out.height = huVolume.height; out.depth = huVolume.depth;
    out.spacing = huVolume.spacing; out.origin = huVolume.origin;
    std::copy(std::begin(huVolume.direction), std::end(huVolume.direction), std::begin(out.direction));
    out.labels.resize(N, (uint8_t)TissueLabel::Background);

    for (size_t i = 0; i < N; ++i) {
        out.labels[i] = (uint8_t)ClassifyHU(huVolume.voxels[i], segmentationRules);
    }
    return out;
}
