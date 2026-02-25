#include <cmath>
#include <gtest/gtest.h>
#include <data/volume.h>
#include <io/dicomloader.h>

class DicomLoaderTest : public ::testing::Test
{
protected:
    std::string validDirectory = "test_data/valid_series";
    std::string invalidDirectory = "test_data/invalid_series";
};

TEST_F(DicomLoaderTest, LoadValidDirectory_ShouldReturnValidVolume)
{
    HUVolume volume = DICOMLoader::loadFromDirectory(validDirectory);

    // Dimensions non nulles
    EXPECT_GT(volume.width, 0u);
    EXPECT_GT(volume.height, 0u);
    EXPECT_GT(volume.depth, 0u);

    // Vérifie la taille du buffer
    EXPECT_EQ(volume.voxels.size(),
              volume.width * volume.height * volume.depth);

    // Spacing positif
    EXPECT_GT(volume.spacing.x, 0.f);
    EXPECT_GT(volume.spacing.y, 0.f);
    EXPECT_GT(volume.spacing.z, 0.f);
}

TEST_F(DicomLoaderTest, LoadInvalidDirectory_ShouldThrow)
{
    EXPECT_THROW({
        DICOMLoader::loadFromDirectory(invalidDirectory);
    }, std::exception);
}


TEST_F(DicomLoaderTest, LoadEmptyDirectory_ShouldThrow)
{
    std::string emptyDir = "test_data/empty";
    EXPECT_THROW({
        DICOMLoader::loadFromDirectory(emptyDir);
    }, std::exception);
}

TEST_F(DicomLoaderTest, DirectionMatrix_ShouldBeNormalized)
{
    HUVolume volume = DICOMLoader::loadFromDirectory(validDirectory);

    // Vérifie que les vecteurs ligne/colonne sont normalisés
    float rowNorm =
        std::sqrt(volume.direction[0]*volume.direction[0] +
                  volume.direction[1]*volume.direction[1] +
                  volume.direction[2]*volume.direction[2]);

    float colNorm =
        std::sqrt(volume.direction[3]*volume.direction[3] +
                  volume.direction[4]*volume.direction[4] +
                  volume.direction[5]*volume.direction[5]);

    EXPECT_NEAR(rowNorm, 1.0f, 1e-3f);
    EXPECT_NEAR(colNorm, 1.0f, 1e-3f);
}

