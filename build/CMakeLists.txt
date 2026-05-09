#############################################################################
# Name:        CMakeLists.txt
# Purpose:     Build script for the wxPDFDocument minimal sample
# Author:      Blake Madden
# Created:     2026-05-08
# Copyright:   (c) 2026 Blake Madden
# Licence:     wxWindows licence
#############################################################################

# Standalone CMake build for the wxPDFDocument samples.
#
# This file downloads and builds (statically) wxWidgets, then builds
# wxPDFDocument and the following sample executables against that:
#
#   - minimal  — console program bundling the tutorial/demo programs
#                (tutorial1-7, bookmark, drawing, rotation, gradients,
#                cjktest, indicfonts, charting, barcodes, etc.).
#   - pdfdc    — GUI printing demo. Shows the wxDC-compatible
#                drawing-context layer and integration with the printing
#                framework. Requires wx::richtext and wx::html.
#
# Each executable is emitted into its own "samples/<name>/" folder so the
# relative paths (data files, ../../lib/fonts, etc.) resolve correctly.
#
# Usage (from the repo root):
#     cmake -S build -B build/_cmake
#     cmake --build build/_cmake --config Release
#
# Or in Visual Studio: File > Open > Folder..., select the "build/" folder,
# and wait for CMake to configure (will take a moment the first time while
# wxWidgets is cloned). Finally, select "minimal.exe" or "pdfdc.exe" from
# the startup combobox and build.

cmake_minimum_required(VERSION 3.16)
project(wxpdfdoc_minimal LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WXPDFDOC_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

# ---------------------------------------------------------------------------
# wxWidgets — fetched dynamically
# ---------------------------------------------------------------------------
include(FetchContent)

set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)
set(wxBUILD_MONOLITHIC OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE STRING "" FORCE)
set(wxBUILD_SAMPLES OFF CACHE STRING "" FORCE)
set(wxBUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(wxBUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(wxUSE_REGEX builtin CACHE STRING "" FORCE)

FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    # Latest snapshot
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(wxWidgets)

# ---------------------------------------------------------------------------
# Bundled brotli + woff2
# ---------------------------------------------------------------------------
set(WOFF2_DIR ${WXPDFDOC_ROOT}/thirdparty/woff2)

add_library(wxpdfdoc_woff2 STATIC
    ${WOFF2_DIR}/brotli/common/constants.c
    ${WOFF2_DIR}/brotli/common/context.c
    ${WOFF2_DIR}/brotli/common/dictionary.c
    ${WOFF2_DIR}/brotli/common/platform.c
    ${WOFF2_DIR}/brotli/common/shared_dictionary.c
    ${WOFF2_DIR}/brotli/common/transform.c
    ${WOFF2_DIR}/brotli/dec/bit_reader.c
    ${WOFF2_DIR}/brotli/dec/decode.c
    ${WOFF2_DIR}/brotli/dec/huffman.c
    ${WOFF2_DIR}/brotli/dec/state.c
    ${WOFF2_DIR}/src/table_tags.cc
    ${WOFF2_DIR}/src/variable_length.cc
    ${WOFF2_DIR}/src/woff2_common.cc
    ${WOFF2_DIR}/src/woff2_dec.cc
    ${WOFF2_DIR}/src/woff2_out.cc
)
target_include_directories(wxpdfdoc_woff2 PUBLIC
    ${WOFF2_DIR}/include
    ${WOFF2_DIR}/src
    ${WOFF2_DIR}/brotli/include
)

# ---------------------------------------------------------------------------
# Bundled zint
# ---------------------------------------------------------------------------
set(ZINT_DIR ${WXPDFDOC_ROOT}/thirdparty/zint/backend)
file(GLOB ZINT_SRCS CONFIGURE_DEPENDS ${ZINT_DIR}/*.c)

add_library(wxpdfdoc_zint STATIC ${ZINT_SRCS})
target_compile_definitions(wxpdfdoc_zint PRIVATE ZINT_NO_PNG _LIB)
target_include_directories(wxpdfdoc_zint PUBLIC ${ZINT_DIR} ${ZINT_DIR}/fonts)

# ---------------------------------------------------------------------------
# wxPDFDocument library
# ---------------------------------------------------------------------------
# Gather all *.cpp files from "src/" and its "crypto/" and "woff/" subdirs.
file(GLOB WXPDFDOC_SRCS CONFIGURE_DEPENDS
    ${WXPDFDOC_ROOT}/src/*.cpp
    ${WXPDFDOC_ROOT}/src/crypto/*.cpp
    ${WXPDFDOC_ROOT}/src/woff/*.cpp
)

add_library(wxpdfdoc STATIC ${WXPDFDOC_SRCS})
target_include_directories(wxpdfdoc PUBLIC ${WXPDFDOC_ROOT}/include)
target_compile_definitions(wxpdfdoc PUBLIC WXUSINGLIB_PDFDOC)
target_link_libraries(wxpdfdoc
    PUBLIC  wx::core wx::base wx::xml
    PRIVATE wxpdfdoc_woff2 wxpdfdoc_zint
)

if(MSVC)
    target_compile_definitions(wxpdfdoc PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
        _CRT_NONSTDC_NO_DEPRECATE
    )
endif()

# ---------------------------------------------------------------------------
# minimal sample executable
# ---------------------------------------------------------------------------
file(GLOB MINIMAL_SRCS CONFIGURE_DEPENDS ${WXPDFDOC_ROOT}/samples/minimal/*.cpp)

add_executable(minimal ${MINIMAL_SRCS})
if(WIN32)
    target_sources(minimal PRIVATE ${WXPDFDOC_ROOT}/samples/minimal/minimal.rc)
endif()
target_link_libraries(minimal PRIVATE wxpdfdoc)

# Place the executable into samples/minimal so its relative data paths
# (wxpdfdoc.png, default font path ../../lib/fonts, etc.) resolve when
# the program is run.
# Override per-config so multi-config generators don't append Debug/Release.
set_target_properties(minimal PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${WXPDFDOC_ROOT}/samples/minimal"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${WXPDFDOC_ROOT}/samples/minimal"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${WXPDFDOC_ROOT}/samples/minimal"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${WXPDFDOC_ROOT}/samples/minimal"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${WXPDFDOC_ROOT}/samples/minimal"
    VS_DEBUGGER_WORKING_DIRECTORY           "${WXPDFDOC_ROOT}/samples/minimal"
)

# ---------------------------------------------------------------------------
# pdfdc sample executable (GUI printing/wxPdfDC demo)
# ---------------------------------------------------------------------------
file(GLOB PDFDC_SRCS CONFIGURE_DEPENDS ${WXPDFDOC_ROOT}/samples/pdfdc/*.cpp)

add_executable(pdfdc WIN32 ${PDFDC_SRCS})
if(WIN32)
    target_sources(pdfdc PRIVATE ${WXPDFDOC_ROOT}/samples/pdfdc/printing.rc)
endif()
target_link_libraries(pdfdc PRIVATE wxpdfdoc wx::richtext wx::html)

# Emit alongside the sample's data files (test.html, smile.jpg, xpms, .ico)
# so the program's relative paths resolve when it runs.
set_target_properties(pdfdc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${WXPDFDOC_ROOT}/samples/pdfdc"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${WXPDFDOC_ROOT}/samples/pdfdc"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${WXPDFDOC_ROOT}/samples/pdfdc"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${WXPDFDOC_ROOT}/samples/pdfdc"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${WXPDFDOC_ROOT}/samples/pdfdc"
    VS_DEBUGGER_WORKING_DIRECTORY           "${WXPDFDOC_ROOT}/samples/pdfdc"
)
