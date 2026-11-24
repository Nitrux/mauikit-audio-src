project(liboss)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install path prefix" FORCE)
endif()

include(CheckIncludeFile)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#OSS
check_include_file(sys/soundcard.h SYS_SOUNDCARD_H_FOUND)
if(SYS_SOUNDCARD_H_FOUND)
    add_definitions(-DHAVE_SYS_SOUNDCARD_H)
endif(SYS_SOUNDCARD_H_FOUND)

set(liboss_SRCS
  outputossfactory.cpp
  outputoss.cpp
  osssettingsdialog.cpp
  osssettingsdialog.ui
  # translations/translations.qrc
)

add_library(oss MODULE ${liboss_SRCS})
target_link_libraries(oss PRIVATE Qt6::Widgets libMauiKitAudioPlugin)
install(TARGETS oss DESTINATION ${PLUGIN_DIR}/Output)
