// Documentation and Refactor TODO

#include "dicomloader.h"

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include "../utils/MathUtils.h"

#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <cctype>

namespace fs = std::filesystem;
using namespace math;

// ============================================================
// Helpers
// ============================================================

static void parseDoubleValues(const OFString& dicomString, OFVector<double>& outputValues)
{
    outputValues.clear();

    std::string valueString(dicomString.c_str());
    size_t startPos = 0;

    while (startPos < valueString.size())
    {
        size_t separatorPos = valueString.find('\\', startPos);

        std::string token = valueString.substr(
            startPos,
            (separatorPos == std::string::npos)
                ? std::string::npos
                : separatorPos - startPos
            );

        size_t begin = token.find_first_not_of(" \t\r\n");
        size_t end   = token.find_last_not_of(" \t\r\n");

        if (begin != std::string::npos)
            outputValues.push_back(std::stod(token.substr(begin, end - begin + 1)));

        if (separatorPos == std::string::npos)
            break;

        startPos = separatorPos + 1;
    }
}

// ============================================================
// Structure interne représentant un slice DICOM
// ============================================================

struct SliceMetadata
{
    std::string filePath;

    OFVector<double> imagePositionPatient;

    double slicePositionProjection = 0.0;
    int    instanceNumber = 0;

    double rescaleSlope     = 1.0;
    double rescaleIntercept = 0.0;

    uint16_t bitsAllocated       = 16;
    uint16_t pixelRepresentation = 0; // 0 = unsigned, 1 = signed
};

// ============================================================
// Chargement volume DICOM
// ============================================================

HUVolume DICOMLoader::loadFromDirectory(const std::string& directoryPath)
{
    // --------------------------------------------------------
    // 1) Recherche des fichiers DICOM
    // --------------------------------------------------------

    std::vector<std::string> dicomFilePaths;

    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (!entry.is_regular_file())
            continue;

        std::string extension = entry.path().extension().string();

        std::transform(extension.begin(), extension.end(), extension.begin(),
                       [](unsigned char c)
                       {
                           return static_cast<char>(std::tolower(c));
                       });

        if (extension == ".dcm")
            dicomFilePaths.push_back(entry.path().string());
    }

    if (dicomFilePaths.empty())
        throw std::runtime_error("Aucun fichier DICOM dans : " + directoryPath);

    // --------------------------------------------------------
    // 2) Lecture des métadonnées
    // --------------------------------------------------------

    OFVector<double> referenceImageOrientation;
    OFVector<double> referenceImagePosition;

    uint16_t referenceRows    = 0;
    uint16_t referenceColumns = 0;

    double pixelSpacingRow    = 1.0;
    double pixelSpacingColumn = 1.0;

    std::string referenceSeriesUID;

    std::vector<SliceMetadata> slices;
    slices.reserve(dicomFilePaths.size());

    for (const std::string& filePath : dicomFilePaths)
    {
        DcmFileFormat fileFormat;

        if (fileFormat.loadFile(filePath.c_str()).bad())
            continue;

        DcmDataset* dataset = fileFormat.getDataset();
        if (!dataset)
            continue;

        Uint16 rows = 0;
        Uint16 columns = 0;

        if (dataset->findAndGetUint16(DCM_Rows, rows).bad() ||
            dataset->findAndGetUint16(DCM_Columns, columns).bad())
            continue;

        // Vérification de la série
        OFString seriesUID;
        if (dataset->findAndGetOFString(DCM_SeriesInstanceUID, seriesUID).good())
        {
            if (referenceSeriesUID.empty())
                referenceSeriesUID = seriesUID.c_str();
            else if (referenceSeriesUID != seriesUID.c_str())
                continue;
        }

        // Orientation et position
        OFString orientationString;
        OFString positionString;

        if (dataset->findAndGetOFStringArray(DCM_ImageOrientationPatient, orientationString).bad())
            continue;

        if (dataset->findAndGetOFStringArray(DCM_ImagePositionPatient, positionString).bad())
            continue;

        OFVector<double> imageOrientation;
        OFVector<double> imagePosition;

        parseDoubleValues(orientationString, imageOrientation);
        parseDoubleValues(positionString, imagePosition);

        if (imageOrientation.size() != 6 || imagePosition.size() != 3)
            continue;

        // Pixel Spacing
        OFString pixelSpacingString;
        if (dataset->findAndGetOFStringArray(DCM_PixelSpacing, pixelSpacingString).good())
        {
            OFVector<double> spacingValues;
            parseDoubleValues(pixelSpacingString, spacingValues);

            if (spacingValues.size() >= 2)
            {
                pixelSpacingRow    = spacingValues[0];
                pixelSpacingColumn = spacingValues[1];
            }
        }

        double rescaleSlope = 1.0;
        double rescaleIntercept = 0.0;

        dataset->findAndGetFloat64(DCM_RescaleSlope, rescaleSlope);
        dataset->findAndGetFloat64(DCM_RescaleIntercept, rescaleIntercept);

        Uint16 pixelRepresentation = 0;
        Uint16 bitsAllocated = 16;

        dataset->findAndGetUint16(DCM_PixelRepresentation, pixelRepresentation);
        dataset->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);

        if (referenceImageOrientation.empty())
        {
            referenceImageOrientation = imageOrientation;
            referenceImagePosition    = imagePosition;
            referenceRows             = rows;
            referenceColumns          = columns;
        }

        Sint32 instanceNumber = 0;
        dataset->findAndGetSint32(DCM_InstanceNumber, instanceNumber);

        // Calcul projection sur la normale du slice
        glm::vec3 rowDirection{
            (float)imageOrientation[0],
            (float)imageOrientation[1],
            (float)imageOrientation[2]
        };

        glm::vec3 columnDirection{
            (float)imageOrientation[3],
            (float)imageOrientation[4],
            (float)imageOrientation[5]
        };

        glm::vec3 sliceNormal = normalize(cross(rowDirection, columnDirection));

        glm::vec3 deltaPosition{
            (float)(imagePosition[0] - referenceImagePosition[0]),
            (float)(imagePosition[1] - referenceImagePosition[1]),
            (float)(imagePosition[2] - referenceImagePosition[2])
        };

        double sliceProjection =
            deltaPosition.x * sliceNormal.x +
            deltaPosition.y * sliceNormal.y +
            deltaPosition.z * sliceNormal.z;

        slices.push_back({
            filePath,
            imagePosition,
            sliceProjection,
            (int)instanceNumber,
            rescaleSlope,
            rescaleIntercept,
            bitsAllocated,
            pixelRepresentation
        });
    }

    if (slices.empty())
        throw std::runtime_error("Aucun slice DICOM exploitable.");

    // --------------------------------------------------------
    // 3) Tri des slices
    // --------------------------------------------------------

    std::stable_sort(slices.begin(), slices.end(),
                     [](const SliceMetadata& a, const SliceMetadata& b)
                     {
                         if (a.slicePositionProjection == b.slicePositionProjection)
                             return a.instanceNumber < b.instanceNumber;

                         return a.slicePositionProjection < b.slicePositionProjection;
                     });

    // --------------------------------------------------------
    // 4) Construction du volume
    // --------------------------------------------------------

    DicomImage firstImage(slices.front().filePath.c_str());

    if (firstImage.getStatus() != EIS_Normal || !firstImage.isMonochrome())
        throw std::runtime_error("Première image illisible ou non monochrome.");

    const uint32_t volumeWidth  = firstImage.getWidth();
    const uint32_t volumeHeight = firstImage.getHeight();
    const uint32_t volumeDepth  = (uint32_t)slices.size();

    if (!volumeWidth || !volumeHeight || !volumeDepth)
        throw std::runtime_error("Dimensions invalides.");

    HUVolume volume;

    volume.width  = volumeWidth;
    volume.height = volumeHeight;
    volume.depth  = volumeDepth;

    volume.spacing = {
        (float)pixelSpacingColumn,
        (float)pixelSpacingRow,
        1.0f
    };

    volume.origin = {
        (float)referenceImagePosition[0],
        (float)referenceImagePosition[1],
        (float)referenceImagePosition[2]
    };

    volume.voxels.resize((size_t)volumeWidth * volumeHeight * volumeDepth);

    // --------------------------------------------------------
    // 5) Lecture PixelData
    // --------------------------------------------------------

    const size_t pixelsPerSlice = (size_t)volumeWidth * volumeHeight;

    for (size_t sliceIndex = 0; sliceIndex < slices.size(); ++sliceIndex)
    {
        DcmFileFormat fileFormat;

        if (fileFormat.loadFile(slices[sliceIndex].filePath.c_str()).bad())
            throw std::runtime_error("Échec lecture : " + slices[sliceIndex].filePath);

        DcmDataset* dataset = fileFormat.getDataset();
        if (!dataset)
            throw std::runtime_error("Dataset nul : " + slices[sliceIndex].filePath);

        DcmElement* pixelDataElement = nullptr;

        if (dataset->findAndGetElement(DCM_PixelData, pixelDataElement).bad() ||
            !pixelDataElement)
            throw std::runtime_error("PixelData manquant : " + slices[sliceIndex].filePath);

        float* destination =
            volume.voxels.data() + sliceIndex * pixelsPerSlice;

        const double slope     = slices[sliceIndex].rescaleSlope;
        const double intercept = slices[sliceIndex].rescaleIntercept;

        const bool isSigned =
            (slices[sliceIndex].pixelRepresentation != 0);

        if (slices[sliceIndex].bitsAllocated <= 8)
        {
            Uint8* pixelData8 = nullptr;

            if (pixelDataElement->getUint8Array(pixelData8).bad() || !pixelData8)
                throw std::runtime_error("Pixels 8 bits indisponibles.");

            if (isSigned)
            {
                const int8_t* signedData =
                    reinterpret_cast<const int8_t*>(pixelData8);

                for (size_t i = 0; i < pixelsPerSlice; ++i)
                    destination[i] =
                        float(double(signedData[i]) * slope + intercept);
            }
            else
            {
                for (size_t i = 0; i < pixelsPerSlice; ++i)
                    destination[i] =
                        float(double(pixelData8[i]) * slope + intercept);
            }
        }
        else
        {
            Uint16* pixelData16 = nullptr;

            if (pixelDataElement->getUint16Array(pixelData16).bad() || !pixelData16)
                throw std::runtime_error("Pixels 16 bits indisponibles.");

            if (isSigned)
            {
                const int16_t* signedData =
                    reinterpret_cast<const int16_t*>(pixelData16);

                for (size_t i = 0; i < pixelsPerSlice; ++i)
                    destination[i] =
                        float(double(signedData[i]) * slope + intercept);
            }
            else
            {
                for (size_t i = 0; i < pixelsPerSlice; ++i)
                    destination[i] =
                        float(double(pixelData16[i]) * slope + intercept);
            }
        }
    }

    return volume;
}
