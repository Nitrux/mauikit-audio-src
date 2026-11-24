project(libqtmultimedia)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install path prefix" FORCE)
endif()

SET(libqtmultimedia_SRCS
  outputqtmultimedia.cpp
  outputqtmultimediafactory.cpp
  qtmultimediasettingsdialog.cpp
  qtmultimediasettingsdialog.ui
  # translations/translations.qrc
)

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(qtmultimedia MODULE ${libqtmultimedia_SRCS} ${libqtmultimedia_UIS_H} ${libqtmultimedia_RCC_SRCS})
target_link_libraries(qtmultimedia PRIVATE Qt6::Widgets Qt6::Multimedia libMauiKitAudioPlugin)
install(TARGETS qtmultimedia DESTINATION ${PLUGIN_DIR}/Output)
