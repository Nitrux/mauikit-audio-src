set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Extract version from qmmp.h
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/qmmp/qmmp.h"
     QMMP_VERSION_DATA REGEX "^#define[ \t]+QMMP_VERSION_[A-Z]+[ \t]+[0-9]+.*$")

if(QMMP_VERSION_DATA)
    foreach(item IN ITEMS MAJOR MINOR PATCH STABLE)
        string(REGEX REPLACE ".*#define[ \t]+QMMP_VERSION_${item}[ \t]+([0-9]+).*"
        "\\1" QMMP_VERSION_${item} ${QMMP_VERSION_DATA})
    endforeach(item IN ITEMS MAJOR MINOR PATCH)
   
    set(QMMP_VERSION "${QMMP_VERSION_MAJOR}.${QMMP_VERSION_MINOR}.${QMMP_VERSION_PATCH}")
    set(QMMP_SOVERSION "${QMMP_VERSION_MAJOR}")
   
    if(QMMP_VERSION_STABLE)
       message(STATUS "Qmmp API version: ${QMMP_VERSION} (stable)")
    else(QMMP_VERSION_STABLE)
       message(STATUS "Qmmp API version: ${QMMP_VERSION} (development)")
    endif(QMMP_VERSION_STABLE)
else(QMMP_VERSION_DATA)
    message(FATAL_ERROR "invalid header")
endif(QMMP_VERSION_DATA)

set(PLUGIN_DIR "${KDE_INSTALL_LIBDIR}/MauiKitAudio" CACHE STRING "plugin path")
set(USE_DIR_ASSOC TRUE CACHE BOOL "enable/disable inode/directory mime type association")

add_subdirectory(qmmp)
add_subdirectory(plugins)

if(MAD_FOUND)
    set(MPEG_DECODERS "MAD")
endif(MAD_FOUND)

if(MPG123_FOUND)
    set(MPEG_DECODERS ${MPEG_DECODERS} "mpg123")
endif(MPG123_FOUND)

message(STATUS "Enabled Linux output plugins are configured via src/lib/plugins/Output")
if(MPEG_DECODERS)
    message(STATUS "MPEG decoders: ${MPEG_DECODERS}")
endif()
