﻿cmake_minimum_required(VERSION 3.21)

# Detect vcpkg ports
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" ENV_VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${ENV_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
endif()

# Import Qt6 dirctory if available
if(DEFINED ENV{Qt6_HOME})
    set(CMAKE_PREFIX_PATH $ENV{Qt6_HOME})
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(ShiftIntrinsicGuide
    LANGUAGES CXX
    VERSION 1.0.5
    DESCRIPTION "A GUI for viewing x86 intrinsics and associated performance data"
)

# Default to a release build if desired configuration is not specified
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

# Set the install location to the source location if not currently specified
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
    message("Installing into source folder")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "..." FORCE)
endif()

# Use folder structure for arranging files within IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt6 COMPONENTS Quick QML Network XML Concurrent REQUIRED)

qt_add_executable(ShiftIntrinsicGuide MANUAL_FINALIZATION)

if(WIN32)
set_target_properties(ShiftIntrinsicGuide PROPERTIES
    WIN32_EXECUTABLE TRUE
)
target_sources(ShiftIntrinsicGuide PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/source/Resources.rc"
)
elseif(APPLE)
set_target_properties(ShiftIntrinsicGuide PROPERTIES
    MACOSX_BUNDLE TRUE
)
endif()

configure_file(include/Version.h.in Version.h)

# Add in the executable code
target_sources(ShiftIntrinsicGuide PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/InternalData.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/IntrinsicModel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/IntrinsicProxyModel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/TechnologyModel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/TypeModel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/CategoryModel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/MeasurementModel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Downloader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/DataProvider.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/IntrinsicModel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/IntrinsicProxyModel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/TechnologyModel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/TypeModel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/CategoryModel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/MeasurementModel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/Downloader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/DataProvider.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/Resources.qrc"
)

target_compile_features(ShiftIntrinsicGuide
    INTERFACE cxx_std_17
)

target_include_directories(ShiftIntrinsicGuide
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${PROJECT_BINARY_DIR}"
)

target_link_libraries(ShiftIntrinsicGuide PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Network
    Qt6::Xml
    Qt6::Concurrent
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" REGULAR_EXPRESSION "*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" REGULAR_EXPRESSION "*.cpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" REGULAR_EXPRESSION "*.qml")

qt_add_qml_module(ShiftIntrinsicGuide
    URI qml
    RESOURCE_PREFIX /
    VERSION 1.0
    QML_FILES    
    "source/main.qml"
    "source/Page1.qml"
    "source/MenuView.qml"
    "source/IntrinsicView.qml"
)

include(InstallRequiredSystemLibraries)
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
include(GNUInstallDirs)
install(TARGETS ShiftIntrinsicGuide
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

qt_import_qml_plugins(ShiftIntrinsicGuide)
qt_finalize_executable(ShiftIntrinsicGuide)

# Include Qt deployment helper function
include(DeployQt)
deployqt(ShiftIntrinsicGuide "${CMAKE_CURRENT_SOURCE_DIR}/source/")

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
include(CPack)
