#ifndef DICOMLOADER_H
#define DICOMLOADER_H

// Documentation TODO

#include <data/volume.h>
#include <string>

class DICOMLoader
{
public:
    // Charge une serie DICOM depuis un dossier (une serie unique ; les autres series sont ignor�es)
    // - Tri des slices par projection IPP sur la normale issue de IOP
    // - spacing.x,y depuis PixelSpacing ; spacing.z = mediane des gaps
    // - direction = [row; col; normal]
    // - voxels : valeurs brutes en uint16 (getOutputData(16) de DicomImage)
    static HUVolume loadFromDirectory(const std::string& directoryPath);
};
#endif // DICOMLOADER_H
