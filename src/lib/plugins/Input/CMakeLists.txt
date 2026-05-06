set(USE_MAD            TRUE CACHE BOOL "enable/disable mad decoder (mpeg plugin)")
set(USE_MPG123         TRUE CACHE BOOL "enable/disable mpg123 decoder (mpeg plugin)")
set(USE_FLAC           TRUE CACHE BOOL "enable/disable flac plugin")
set(USE_VORBIS         TRUE CACHE BOOL "enable/disable ogg vorbis plugin")
set(USE_FFMPEG         TRUE CACHE BOOL "enable/disable ffmpeg plugin")
set(USE_MPC            TRUE CACHE BOOL "enable/disable mpc plugin")
set(USE_SNDFILE        TRUE CACHE BOOL "enable/disable sndfile plugin")
set(USE_WAVPACK        TRUE CACHE BOOL "enable/disable wavpack plugin")
set(USE_XMP            TRUE CACHE BOOL "enable/disable xmp plugin")
set(USE_AAC            TRUE CACHE BOOL "enable/disable aac plugin")
set(USE_CUE            TRUE CACHE BOOL "enable/disable cue plugin")
set(USE_CDA            TRUE CACHE BOOL "enable/disable cd audio support")
set(USE_MIDI           TRUE CACHE BOOL "enable/disable midi support")
set(USE_GME            TRUE CACHE BOOL "enable/disable game music plugin")
set(USE_OPUS           TRUE CACHE BOOL "enable/disable opus plugin")
set(USE_SID            TRUE CACHE BOOL "enable/disable sid plugin")
set(USE_ARCHIVE        TRUE CACHE BOOL "enable/disable archive plugin")
set(REQUIRE_WORKING_AUDIO_DECODER TRUE CACHE BOOL "Fail configuration when no practical local-audio decoder plugin can be built")


if((USE_MAD OR USE_MPG123) AND TAGLIB_FOUND)
    add_subdirectory(mpeg)
endif((USE_MAD OR USE_MPG123) AND TAGLIB_FOUND)

if(USE_FLAC AND TAGLIB_FOUND)
    add_subdirectory(flac)
endif(USE_FLAC AND TAGLIB_FOUND)

if(USE_VORBIS AND TAGLIB_FOUND)
    add_subdirectory(vorbis)
endif(USE_VORBIS AND TAGLIB_FOUND)

if(USE_FFMPEG AND TAGLIB_FOUND)
    add_subdirectory(ffmpeg)
endif(USE_FFMPEG AND TAGLIB_FOUND)

if(USE_MPC AND TAGLIB_FOUND)
    add_subdirectory(mpc)
endif(USE_MPC AND TAGLIB_FOUND)

if(USE_SNDFILE)
   add_subdirectory(sndfile)
endif(USE_SNDFILE)

if(USE_WAVPACK)
    add_subdirectory(wavpack)
endif(USE_WAVPACK)

if(USE_XMP)
   add_subdirectory(xmp)
endif(USE_XMP)

if(USE_AAC AND TAGLIB_FOUND)
   add_subdirectory(aac)
endif(USE_AAC AND TAGLIB_FOUND)

if(USE_CUE)
add_subdirectory(cue)
endif(USE_CUE)

if(USE_CDA)
add_subdirectory(cdaudio)
endif(USE_CDA)

if(USE_MIDI)
add_subdirectory(wildmidi)
endif(USE_MIDI)

if(USE_GME)
    add_subdirectory(gme)
endif(USE_GME)

if(USE_OPUS AND TAGLIB_FOUND)
    add_subdirectory(opus)
endif(USE_OPUS AND TAGLIB_FOUND)

if(USE_SID)
    add_subdirectory(sid)
endif(USE_SID)

if(USE_ARCHIVE AND TAGLIB_FOUND)
    add_subdirectory(archive)
endif(USE_ARCHIVE AND TAGLIB_FOUND)

# Guard against silently producing an unusable build (for local audio playback)
# where only utility/metadata plugins (e.g. cue, cdaudio) are present.
if(REQUIRE_WORKING_AUDIO_DECODER)
    set(MAUIKIT_AUDIO_WORKING_DECODER_TARGETS
        mpeg
        ffmpeg
        flac
        vorbis
        opus
        aac
        sndfile
        wavpack
        mpc
        xmp
        sid
        wildmidi
        gme
        archive)

    set(MAUIKIT_AUDIO_ENABLED_DECODERS "")
    foreach(decoderTarget IN LISTS MAUIKIT_AUDIO_WORKING_DECODER_TARGETS)
        if(TARGET ${decoderTarget})
            list(APPEND MAUIKIT_AUDIO_ENABLED_DECODERS ${decoderTarget})
        endif()
    endforeach()

    if(MAUIKIT_AUDIO_ENABLED_DECODERS STREQUAL "")
        message(FATAL_ERROR
            "No practical audio decoder plugins were built. "
            "At least one of [mpeg, ffmpeg, flac, vorbis, opus, aac, sndfile, wavpack, mpc, xmp, sid, wildmidi, gme, archive] is required. "
            "Install the missing -dev dependencies (for example: taglib, ffmpeg/libav*, mpg123/mad, flac, vorbis, opus, sndfile) and reconfigure.")
    endif()

    message(STATUS "Enabled input decoder plugins: ${MAUIKIT_AUDIO_ENABLED_DECODERS}")
endif()
