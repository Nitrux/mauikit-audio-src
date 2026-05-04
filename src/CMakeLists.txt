include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_library(MauiKitAudio${MAUI_MAJOR_VERSION})

ecm_add_qml_module(MauiKitAudio${MAUI_MAJOR_VERSION}
    VERSION 1.3
    URI "org.mauikit.audio"
    CLASS_NAME AudioPlugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    DEPENDENCIES
    "org.mauikit.controls")

set_target_properties(MauiKitAudio${MAUI_MAJOR_VERSION} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME "Audio")

add_library(MauiKit${MAUI_MAJOR_VERSION}::Audio ALIAS MauiKitAudio${MAUI_MAJOR_VERSION})

set(headers
    code/mediaplayer.h
    code/moduleinfo.h)

target_sources(MauiKitAudio${MAUI_MAJOR_VERSION}
    PRIVATE
    code/mediaplayer.h
    code/mediaplayer.cpp

    code/moduleinfo.cpp
    code/moduleinfo.h)

target_sources(MauiKitAudio${MAUI_MAJOR_VERSION}plugin
    PRIVATE
    code/mediaplayer.h
    code/mediaplayer.cpp

    code/audio_plugin.cpp
    code/audio_plugin.h)

ecm_target_qml_sources(MauiKitAudio${MAUI_MAJOR_VERSION} SOURCES

    controls/PlayerControls.qml)

add_subdirectory(lib)
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# add_executable(qmmpTest ${app_SRCS})
# set_target_properties(qmmpTest PROPERTIES OUTPUT_NAME qmmp${APP_NAME_SUFFIX})
target_link_libraries(MauiKitAudio${MAUI_MAJOR_VERSION}
    PRIVATE
    KF${KF_MAJOR_VERSION}::I18n
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Sql
    Qt${QT_MAJOR_VERSION}::Widgets
    Qt${QT_MAJOR_VERSION}::Network
    libMauiKitAudioPlugin
    MauiKit${MAUI_MAJOR_VERSION})

target_link_libraries(MauiKitAudio${MAUI_MAJOR_VERSION}plugin
    PRIVATE
    KF${KF_MAJOR_VERSION}::I18n
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Sql
    Qt${QT_MAJOR_VERSION}::Widgets
    Qt${QT_MAJOR_VERSION}::Network
    libMauiKitAudioPlugin
    MauiKit${MAUI_MAJOR_VERSION})

ecm_finalize_qml_module(MauiKitAudio${MAUI_MAJOR_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR})
generate_export_header(MauiKitAudio${MAUI_MAJOR_VERSION} BASE_NAME Audio)
install(TARGETS MauiKitAudio${MAUI_MAJOR_VERSION} EXPORT MauiKitAudio${MAUI_MAJOR_VERSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


target_include_directories(MauiKitAudio${MAUI_MAJOR_VERSION}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Audio>")

target_include_directories(MauiKitAudio${MAUI_MAJOR_VERSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

install(FILES
    ${headers}
    ${CMAKE_CURRENT_BINARY_DIR}/audio_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Audio
    COMPONENT Devel)
