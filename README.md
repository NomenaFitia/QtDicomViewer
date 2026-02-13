DICOM Viewer - C++ / QT / OpenGL

Un visualiseur DICOM performant et multiplateforme développé en C++, utilisant Qt pour l’interface graphique et OpenGL pour le rendu accéléré GPU.
La documentation du projet est générée automatiquement avec Doxygen.

---

Fonctionnalités

- Chargement de fichiers DICOM (.dcm)
- Affichage 2D haute performance (OpenGL)
- Gestion des séries d'images
- Zoom / Pan
- Ajsutement Window / Level 
- Mesures simples (distance, ROI-régions d'intérêts)
- Documentation automatique via Doxygen
- Architecture modulaire et maintenable

---

Technologies utilisés

- C++17
- Qt 6 / Qt 5
- OpenGL
- CMake
- Doxygen

---

Dépendances

- Qt (Widget + OpenGL module)
- Opengl 3.3+
- Cmake > 3.16
- Doxygen (pour la génération de la documentation)
- (Optionnel) DCMTK pour le parsing DICOM

---

Structure du projet

DicomViewer/
│
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── MainWindow.h / .cpp
│   ├── DicomLoader.h / .cpp
│   ├── GLViewer.h / .cpp
│
├── resources/
├── docs/
│   └── Doxyfile
└── README.md

---

Installation et compilation

1. Cloner le projet

git clone git@github.com:NomenaFitia/QtDicomViewer.git
cd DicomViewer

2. Build avec CMake 

mkdir build
cd build
cmake ..
cmake --build .

3. Lancer l'application

./DicomViewer

---

Génération de la documentation (Doxygen)

Assurez-vous que Doxygen est installé:

doxygen --version


Générer la documentation:

cd docs
doxygen Doxyfile


La documentation HTML sera disponible dans:

docs/html/index.html

---

Architecture

- DicomLoader
    Lecture des fichiers DICOM
    Extraction des métadonnées
    Conversion vers buffer exploitable OpenGL
    
- GLViewer (QOpenGLWidget)
    Rendu GPU
    Gestion des shaders
    Interactin utilisateur (zoom, pan)
    
- MainWindow
    Interface utilisateur
    Gestion des menus
    Communication entre les modules
    
---

Bonnes pratiques
    Respect des conventions modernes C++
    Smart pointers ( std::unique_pt, std::shared_ptr )
    Séparation claire de UI / logique métier
    Commentaires compatibles Doxygen
    Architecture orientée composants
    
---

Capture d'écran

---

Licence 

Projet sous licence MIT (modifiable selon vos besoins)

---

A propos
Ce projet est destiné à:
    L'apprentissage du rendu médical
    Les projets académiques
    Les prototypes d'applications médicales
