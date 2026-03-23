find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(DCMTK REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# =========================
# LIBRAIRIE METIER
# =========================

add_library(DicomCore
    io/dicomloader.cpp
    data/slice.cpp
    core/segmentation.cpp
)

target_include_directories(DicomCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${DCMTK_INCLUDE_DIRS}

    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(DicomCore PUBLIC
    # ne jamais link globalement DCMTK
    ${DCMTK_LIBRARIES}

    # sinon on a un problème avec ceux là par contre - avec un erreur "ne peut pas trouver -lnsl : Aucun fichier ou dossier de ce nom
    #DCMTK::dcmdata
    #DCMTK::dcmimage
    #DCMTK::dcmimgle

    glm::glm

    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets

)

# =========================
# APPLICATION QT
# =========================

qt_add_executable(DicomViewer
    main.cpp
    platform/mainwindow.cpp
    platform/mainwindow.h
    ui/mainwindow.ui
)

target_link_libraries(DicomViewer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    DicomCore
)
