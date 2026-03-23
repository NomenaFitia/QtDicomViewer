#ifndef DICOMLOADER_H
#define DICOMLOADER_H

/**
 *
 * @brief Charge des fichiers DICOM depuis un répertoire
 *
 * @param directoryPath Chemin des fichiers DICOM
 * @return un HUVolume
 *
 * @note Charge une série unique, les autres series sont ignorées, spacing.x,y depuis PixelSpacing; spacing.z = mediane des gaps; direction = [row; col; normal]; calcul des HU directement dans la fonction
 * @todo décomposer pour appliquer le single responsability principle (SRP)
 *
 * */

#include "../data/volume.h"
#include <string>

class DICOMLoader
{
public:
    static HUVolume loadFromDirectory(const std::string& directoryPath);
};
#endif // DICOMLOADER_H
