cmake_minimum_required(VERSION 3.5)

project(com_chibifire_ass_imp_importer VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(DEBUG_ENABLED DEBUG_ENABLED DEBUG_MEMORY_ENABLED)
set(TOOLS_ENABLED TOOLS_ENABLED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(assimp_CMAKE_ARGS
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_DEBUG_POSTFIX=
    -DENABLE_BOOST_WORKAROUND=ON 
	-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
    -DASSIMP_BUILD_TESTS=OFF
	-DDOUBLE_PRECISION=ON
	-DBUILD_SHARED_LIBS=OFF
)

# See https://github.com/ivansafrin/Polycode/blob/master/CMake/ExternalAssimp.cmake
include(ExternalProject)
ExternalProject_Add(assimp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/assimp

    DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/downloads

    GIT_REPOSITORY "https://github.com/assimp/assimp.git"

    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/assimp
    CMAKE_ARGS ${assimp_CMAKE_ARGS}
)

add_library(com_chibifire_ass_imp_importer
            SHARED
        "sample/addons/com_chibifire_ass_imp_importer/src/Godot.cpp"
        "sample/addons/com_chibifire_ass_imp_importer/src/ComChibifireAssImpImporter.cpp"
        "sample/addons/com_chibifire_ass_imp_importer/src/ComChibifireAssImpImporter.h"
        ${fbx_osx}
        ${fbx_linux}
        ${fbx_mingw}
        ${fbx_msvc}
        "thirdparty/godot-cpp/godot_headers/gdnative/gdnative.h"
        "thirdparty/godot-cpp/godot_headers/nativescript/godot_nativescript.h")

target_include_directories(com_chibifire_ass_imp_importer
                            PUBLIC
							${CMAKE_CURRENT_BINARY_DIR}/assimp/include
                            "thirdparty/godot/modules/gdnative/include"
                            "thirdparty/godot-cpp/include"
                            "thirdparty/godot-cpp/godot_headers"
                            "thirdparty/godot-cpp/include/core")

if(MSVC)
target_link_libraries(com_chibifire_ass_imp_importer
${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/godot-cpp/bin/libgodot-cpp.windows.release.64.lib
${CMAKE_CURRENT_BINARY_DIR}/assimp/lib/assimp-vc140-mt.lib
${CMAKE_CURRENT_BINARY_DIR}/assimp/lib/IrrXML.lib
${CMAKE_CURRENT_BINARY_DIR}/assimp/lib/zlibstatic.lib)
endif(MSVC)
