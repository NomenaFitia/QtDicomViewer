#include <cmath>
#include <gtest/gtest.h>
#include <data/volume.h>
#include <io/dicomloader.h>
#include <core/segmentation.h>

class SegmentationTest : public ::testing::Test
{
protected:
    std::string validDirectory = "test_data/valid_series";
};

TEST_F(SegmentationTest, Segmentation_ShouldReturnValidVolume)
{
    HUVolume volume = DICOMLoader::loadFromDirectory(validDirectory);

    SegmentationRules rules;
    LabeledVolume segmentedVolume = Segmentation::SegmentVolumeHU(volume, rules);

    using LabelType = typename decltype(segmentedVolume.labels)::value_type;
    static_assert(std::is_same_v<LabelType, uint8_t>);
}
