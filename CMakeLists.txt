# SPDX-FileCopyrightText: 2020 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)

set(QT_MAJOR_VERSION 6)
set(REQUIRED_QT_VERSION 6.4)

set(KF_MAJOR_VERSION 6)
set(REQUIRED_KF_VERSION 6.13.0)

set(MAUI_MAJOR_VERSION 4)
set(MAUIKIT_VERSION 4.0.4)
set(REQUIRED_MAUIKIT_VERSION 4.0.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(AUTOMOC_MOC_OPTIONS -Muri=org.mauikit.audio)

project(Audio LANGUAGES CXX VERSION ${MAUIKIT_VERSION})

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "This project supports Linux only.")
endif()


if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

if(POLICY CMP0175)
    cmake_policy(SET CMP0175 NEW)
endif()

find_package(ECM ${REQUIRED_KF_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

#installation paths
include(FindPkgConfig)
include(UsePkgConfig)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMFindQmlModule)
include(ECMQmlModule)
include(ECMSourceVersionControl)

include(KDEClangFormat)
# Preserve externally supplied CMAKE_INSTALL_* paths (for distro builds)
# while avoiding KDEInstallDirs "got its value from CMAKE_INSTALL_*" warnings.
set(_KDE_INSTALL_DIR_SUFFIXES
    BINDIR
    SBINDIR
    LIBDIR
    LIBEXECDIR
    INCLUDEDIR
    LOCALSTATEDIR
    SHAREDSTATEDIR
    DATAROOTDIR
    DATADIR
    LOCALEDIR
    MANDIR
    INFODIR
    SYSCONFDIR
)
foreach(_suffix IN LISTS _KDE_INSTALL_DIR_SUFFIXES)
    if(DEFINED CMAKE_INSTALL_${_suffix} AND NOT DEFINED KDE_INSTALL_${_suffix})
        set(KDE_INSTALL_${_suffix} "${CMAKE_INSTALL_${_suffix}}" CACHE PATH "KDE install dir override from CMAKE_INSTALL_${_suffix}")
    endif()
endforeach()
unset(_suffix)
unset(_KDE_INSTALL_DIR_SUFFIXES)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(CheckCXXCompilerFlag)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060200 -DQT_DEPRECATED_WARNINGS -DQT_MESSAGELOGCONTEXT)
add_compile_options(-Wall -Wextra)

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64)$")
    check_cxx_compiler_flag("-march=x86-64-v3" CXX_SUPPORTS_X86_64_V3)
    if(CXX_SUPPORTS_X86_64_V3)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-march=x86-64-v3>)
    endif()
endif()

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src) #libqmmp & libqmmpui -> move to /src

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Core
    Quick
    Sql
    Multimedia
    DBus
    Widgets
    Network)

find_package(KF${KF_MAJOR_VERSION} ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS
    I18n
    CoreAddons)

find_package(MauiKit${MAUI_MAJOR_VERSION} ${REQUIRED_MAUIKIT_VERSION} REQUIRED)

if(${ECM_SOURCE_UNDER_VERSION_CONTROL})
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Get the latest abbreviated commit hash of the working branch
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
    add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

else()
    add_definitions(-DGIT_COMMIT_HASH="${PROJECT_VERSION}")
    add_definitions(-DGIT_BRANCH="Stable")
endif()

if (QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

##CMAKE PARTS
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/MauiKitAudio${MAUI_MAJOR_VERSION}")

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX Audio
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/audio_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/MauiKitAudio${MAUI_MAJOR_VERSION}ConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR})

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MauiKitAudioConfig.cmake.in.${QT_MAJOR_VERSION}"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitAudio${MAUI_MAJOR_VERSION}Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitAudio${MAUI_MAJOR_VERSION}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitAudio${MAUI_MAJOR_VERSION}Config.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel)

install(EXPORT MauiKitAudio${MAUI_MAJOR_VERSION}Targets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE MauiKitAudio${MAUI_MAJOR_VERSION}Targets.cmake
    NAMESPACE MauiKit${MAUI_MAJOR_VERSION}::)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/audio_version.h"
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Audio
    COMPONENT Devel)

add_definitions(-DTRANSLATION_DOMAIN="mauikitaudio")
ki18n_install(po)

add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

add_subdirectory(src)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE
        @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
