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
)

target_include_directories(DicomCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${DCMTK_INCLUDE_DIRS}

    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(DicomCore PUBLIC
    ${DCMTK_LIBRARIES}
    glm::glm

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

qt_finalize_executable(DicomViewer)
