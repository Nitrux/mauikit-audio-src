project(libMauiKitAudioPlugin)

add_definitions(-DMAUIKITAUDIO_LIBRARY)
set(libMauiKitAudioPlugin_DEFINES
    LIB_DIR="/${LIB_DIR}"
    QMMP_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}"
    APP_NAME_SUFFIX="${APP_NAME_SUFFIX}"
)

set(libMauiKitAudioPlugin_DEFINES ${libMauiKitAudioPlugin_DEFINES} MAUIKITAUDIO_PLUGIN_DIR="${CMAKE_INSTALL_PREFIX}/${PLUGIN_DIR}")

set(MAUIKITAUDIO_DEFAULT_OUTPUT "" CACHE STRING "default output plugin")
if(MAUIKITAUDIO_DEFAULT_OUTPUT)
    SET(libMauiKitAudioPlugin_DEFINES ${libMauiKitAudioPlugin_DEFINES} MAUIKITAUDIO_DEFAULT_OUTPUT="${MAUIKITAUDIO_DEFAULT_OUTPUT}")
    MESSAGE(STATUS "Default output: ${MAUIKITAUDIO_DEFAULT_OUTPUT}")
endif(MAUIKITAUDIO_DEFAULT_OUTPUT)

set(libMauiKitAudioPlugin_SRCS
    visual.cpp
    recycler.cpp
    decoder.cpp
    output.cpp
    outputwriter.cpp
    trackinfo.cpp
    equ/iir.c
    equ/iir_cfs.c
    equ/iir_fpu.c
    soundcore.cpp
    effect.cpp
    qmmp.cpp
    statehandler.cpp
    volumehandler.cpp
    metadatamodel.cpp
    tagmodel.cpp
    qmmpaudioengine.cpp
    abstractengine.cpp
    audioparameters.cpp
    inputsource.cpp
    fileinputsource.cpp
    emptyinputsource.cpp
    metadatamanager.cpp
    replaygain.cpp
    qmmpsettings.cpp
    audioconverter.cpp
    eqsettings.cpp
    qmmpevents.cpp
    qmmpplugincache.cpp
    channelmap.cpp
    channelconverter.cpp
    volume.cpp
    dithering.cpp
    visualbuffer.cpp
    cueparser.cpp
    qmmptextcodec.cpp
)

SET(libMauiKitAudioPlugin_DEVEL_HDRS
    abstractengine.h
    audioconverter.h
    audioparameters.h
    buffer.h
    channelmap.h
    cueparser.h
    decoderfactory.h
    decoder.h
    effectfactory.h
    effect.h
    enginefactory.h
    eqsettings.h
    inputsourcefactory.h
    inputsource.h
    metadatamanager.h
    metadatamodel.h
    outputfactory.h
    output.h
    qmmp_export.h
    qmmp.h
    qmmpsettings.h
    qmmptextcodec.h
    soundcore.h
    statehandler.h
    tagmodel.h
    trackinfo.h
    visualfactory.h
    visual.h
    volume.h
    volumehandler.h
)

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libMauiKitAudioPlugin STATIC ${libMauiKitAudioPlugin_SRCS})
target_link_libraries(libMauiKitAudioPlugin PRIVATE Qt6::Widgets)
set_target_properties(libMauiKitAudioPlugin PROPERTIES VERSION ${QMMP_VERSION} SOVERSION ${QMMP_SOVERSION} OUTPUT_NAME MauiKitAudio4)
target_compile_definitions(libMauiKitAudioPlugin PRIVATE ${libMauiKitAudioPlugin_DEFINES})
