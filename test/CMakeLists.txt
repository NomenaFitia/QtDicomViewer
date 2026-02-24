include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(DicomViewerTests
    test.cpp
    DicomLoaderTest.cpp
)

target_link_libraries(DicomViewerTests
    PRIVATE
    gtest_main
    DicomCore   # ✅ on teste la vraie librairie
)

include(GoogleTest)
gtest_discover_tests(DicomViewerTests)

# Copier test_data dans le build
file(COPY ${CMAKE_SOURCE_DIR}/test/test_data
    DESTINATION ${CMAKE_BINARY_DIR})
