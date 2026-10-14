project('mauikit-audio', ['cpp', 'c'], version: '4.0.5', meson_version: '>=1.0.0', default_options: ['cpp_std=c++17', 'buildtype=release'])

cpp_compiler = meson.get_compiler('cpp')
if cpp_compiler.get_id() == 'gcc' and cpp_compiler.has_argument('-Wno-sfinae-incomplete')
    add_project_arguments('-Wno-sfinae-incomplete', language: 'cpp')
endif
qt6 = import('qt6')
qt = dependency('qt6', version: '>=6.4', modules: ['Core', 'Quick', 'Qml', 'Sql', 'Multimedia', 'DBus', 'Widgets', 'Network'])
kf6_include_prefix = get_option('prefix') / 'include' / 'KF6'
kf6_core = declare_dependency(
    include_directories: include_directories(kf6_include_prefix / 'KCoreAddons'),
    dependencies: [cpp_compiler.find_library('KF6CoreAddons')],
)
kf6_i18n = declare_dependency(
    include_directories: include_directories(kf6_include_prefix / 'KI18n'),
    dependencies: [cpp_compiler.find_library('KF6I18n')],
)
mauikit = dependency('MauiKit4', version: '>=4.0.5')
taglib = dependency('taglib', required: true)
cpp_args = host_machine.cpu_family() == 'x86_64' ? ['-march=x86-64-v3'] : []
audio_version_header = configure_file(
    input: 'meson-content.in',
    output: 'audio_version.h',
    configuration: {'CONTENT': ('''#pragma once
#define Audio_VERSION_STRING "''' + meson.project_version() + '''"
#define GIT_BRANCH "Stable"
#define GIT_COMMIT_HASH "''' + meson.project_version() + '''"
''')}
)
audio_export_header = configure_file(
    input: 'meson-content.in',
    output: 'audio_export.h',
    configuration: {'CONTENT': ('''#pragma once
#define AUDIO_EXPORT __attribute__((visibility("default")))
''')}
)
qmmp_cpp_args = cpp_args + ['-DAPP_NAME_SUFFIX=""']


qmmp_sources = run_command('find', 'src/lib/qmmp', '-maxdepth', '2', '-type', 'f', check: true).stdout().strip().split('\n')
qmmp_cpp = []
foreach source : qmmp_sources
    if source.endswith('.cpp') or source.endswith('.c')
        qmmp_cpp += [source]
    endif
endforeach
qmmp_moc_headers = run_command('find', 'src/lib/qmmp', '-maxdepth', '1', '-type', 'f', '-name', '*.h', check: true).stdout().strip().split('\n')
moc_extra_arguments = ['-I' + meson.current_source_dir() + '/src/lib']
qmmp_moc = qt6.preprocess(moc_headers: qmmp_moc_headers, moc_extra_arguments: moc_extra_arguments)
plugin_include = include_directories('src/lib', 'src/lib/qmmp', 'src/lib/plugins')
plugin_ui = run_command('find', 'src/lib/plugins', '-type', 'f', '-name', '*.ui', check: true).stdout().strip().split('\n')
plugin_dir = get_option('libdir') / 'MauiKitAudio'
input_plugin_dir = plugin_dir / 'Input'
output_plugin_dir = plugin_dir / 'Output'


audio_core = static_library('MauiKitAudioPlugin', qmmp_cpp + qmmp_moc, dependencies: [qt], include_directories: plugin_include, cpp_args: qmmp_cpp_args, pic: true)
audio = shared_library('MauiKitAudio4', files('src/code/mediaplayer.cpp', 'src/code/moduleinfo.cpp') + qt6.preprocess(moc_headers: ['src/code/mediaplayer.h']) + [audio_export_header, audio_version_header], link_with: audio_core, dependencies: [qt, kf6_core, kf6_i18n, mauikit], include_directories: [include_directories('src/code'), plugin_include], cpp_args: cpp_args + ['-DMAUIKITAUDIO_LIBRARY'], version: '4.0.5', soversion: '4', install: true)


plugin = shared_module('MauiKitAudio4plugin', ['src/code/audio_plugin.cpp'] + qt6.preprocess(moc_headers: ['src/code/audio_plugin.h']), link_with: audio, dependencies: [qt, kf6_core, kf6_i18n, mauikit], include_directories: include_directories('src/code'), cpp_args: cpp_args, install: true, install_dir: get_option('libdir') / 'qt6/qml/org/mauikit/audio')
qml_dir = get_option('libdir') / 'qt6/qml/org/mauikit/audio'
install_data(configure_file(
    input: 'meson-content.in',
    output: 'qmldir',
    configuration: {'CONTENT': ('module org.mauikit.audio 1.3\nplugin MauiKitAudio4plugin\n')}
), install_dir: qml_dir)
install_subdir('src/controls', install_dir: qml_dir, strip_directory: true)
install_headers(files('src/code/mediaplayer.h', 'src/code/moduleinfo.h') + [audio_export_header, audio_version_header], subdir: 'MauiKit4/Audio')


input_plugins_with_taglib = []
input_plugins_without_taglib = []
output_plugins = []

mad = dependency('mad', required: false)
mpg123 = dependency('libmpg123', required: false)
enabled_decoders = []
mpeg_sources = ['src/lib/plugins/Input/mpeg/decodermpegfactory.cpp', 'src/lib/plugins/Input/mpeg/mpegsettingsdialog.cpp', 'src/lib/plugins/Input/mpeg/tagextractor.cpp', 'src/lib/plugins/Input/mpeg/mpegmetadatamodel.cpp']
mpeg_moc = ['src/lib/plugins/Input/mpeg/decodermpegfactory.h', 'src/lib/plugins/Input/mpeg/mpegsettingsdialog.h', 'src/lib/plugins/Input/mpeg/mpegmetadatamodel.h']
mpeg_ui = ['src/lib/plugins/Input/mpeg/mpegsettingsdialog.ui']
mpeg_deps = []
mpeg_args = []
if mad.found()
    mpeg_sources += ['src/lib/plugins/Input/mpeg/decoder_mad.cpp']
    mpeg_moc += ['src/lib/plugins/Input/mpeg/decoder_mad.h']
    mpeg_deps += [mad]
    mpeg_args += ['-DWITH_MAD']
endif
if mpg123.found()
    mpeg_sources += ['src/lib/plugins/Input/mpeg/decoder_mpg123.cpp']
    mpeg_moc += ['src/lib/plugins/Input/mpeg/decoder_mpg123.h']
    mpeg_deps += [mpg123]
    mpeg_args += ['-DWITH_MPG123']
endif
if mad.found() or mpg123.found()
    mpeg_moc_files = qt6.preprocess(moc_headers: mpeg_moc, ui_files: mpeg_ui, moc_extra_arguments: moc_extra_arguments)
    shared_module('mpeg', mpeg_sources + mpeg_moc_files, link_with: audio_core, dependencies: [qt, taglib] + mpeg_deps, include_directories: plugin_include, cpp_args: cpp_args + mpeg_args, install: true, install_dir: input_plugin_dir)
    enabled_decoders += ['mpeg']
endif


flac = dependency('flac', required: false)
if flac.found()
    input_plugins_with_taglib += [['flac', ['src/lib/plugins/Input/flac/decoder_flac.cpp', 'src/lib/plugins/Input/flac/decoderflacfactory.cpp', 'src/lib/plugins/Input/flac/flacmetadatamodel.cpp'], ['src/lib/plugins/Input/flac/decoderflacfactory.h'], [flac]]]
    enabled_decoders += ['flac']
endif


ogg = dependency('ogg', required: false)
vorbis = dependency('vorbis', required: false)
vorbisfile = dependency('vorbisfile', required: false)
if ogg.found() and vorbis.found() and vorbisfile.found()
    input_plugins_with_taglib += [['vorbis', ['src/lib/plugins/Input/vorbis/decoder_vorbis.cpp', 'src/lib/plugins/Input/vorbis/decodervorbisfactory.cpp', 'src/lib/plugins/Input/vorbis/vorbismetadatamodel.cpp'], ['src/lib/plugins/Input/vorbis/decodervorbisfactory.h'], [ogg, vorbis, vorbisfile]]]
    enabled_decoders += ['vorbis']
endif


avcodec = dependency('libavcodec', required: false)
avformat = dependency('libavformat', required: false)
avutil = dependency('libavutil', required: false)
if avcodec.found() and avformat.found() and avutil.found()
    input_plugins_with_taglib += [['ffmpeg', ['src/lib/plugins/Input/ffmpeg/decoder_ffmpeg.cpp', 'src/lib/plugins/Input/ffmpeg/decoder_ffmpegcue.cpp', 'src/lib/plugins/Input/ffmpeg/decoder_ffmpegm4b.cpp', 'src/lib/plugins/Input/ffmpeg/decoderffmpegfactory.cpp', 'src/lib/plugins/Input/ffmpeg/ffmpegmetadatamodel.cpp', 'src/lib/plugins/Input/ffmpeg/ffmpegsettingsdialog.cpp', 'src/lib/plugins/Input/ffmpeg/replaygainreader.cpp'], ['src/lib/plugins/Input/ffmpeg/decoderffmpegfactory.h', 'src/lib/plugins/Input/ffmpeg/ffmpegmetadatamodel.h', 'src/lib/plugins/Input/ffmpeg/ffmpegsettingsdialog.h'], [avcodec, avformat, avutil]]]
    enabled_decoders += ['ffmpeg']
endif


mpcdec = dependency('mpcdec', required: false)
if mpcdec.found()
    input_plugins_with_taglib += [['mpc', ['src/lib/plugins/Input/mpc/decoder_mpc.cpp', 'src/lib/plugins/Input/mpc/decodermpcfactory.cpp', 'src/lib/plugins/Input/mpc/mpcmetadatamodel.cpp'], ['src/lib/plugins/Input/mpc/decodermpcfactory.h'], [mpcdec]]]
    enabled_decoders += ['mpc']
endif


sndfile = dependency('sndfile', required: false)
if sndfile.found()
    input_plugins_without_taglib += [['sndfile', ['src/lib/plugins/Input/sndfile/decoder_sndfile.cpp', 'src/lib/plugins/Input/sndfile/decodersndfilefactory.cpp'], ['src/lib/plugins/Input/sndfile/decodersndfilefactory.h'], [sndfile]]]
    enabled_decoders += ['sndfile']
endif


wavpack = dependency('wavpack', required: false)
if wavpack.found()
    input_plugins_without_taglib += [['wavpack', ['src/lib/plugins/Input/wavpack/decoder_wavpack.cpp', 'src/lib/plugins/Input/wavpack/decoderwavpackfactory.cpp', 'src/lib/plugins/Input/wavpack/wavpackmetadatamodel.cpp'], ['src/lib/plugins/Input/wavpack/decoderwavpackfactory.h'], [wavpack]]]
    enabled_decoders += ['wavpack']
endif


xmp = dependency('libxmp', required: false)
if xmp.found()
    input_plugins_without_taglib += [['xmp', ['src/lib/plugins/Input/xmp/decoder_xmp.cpp', 'src/lib/plugins/Input/xmp/decoderxmpfactory.cpp', 'src/lib/plugins/Input/xmp/xmpmetadatamodel.cpp', 'src/lib/plugins/Input/xmp/xmpsettingsdialog.cpp'], ['src/lib/plugins/Input/xmp/decoderxmpfactory.h', 'src/lib/plugins/Input/xmp/xmpsettingsdialog.h'], [xmp]]]
    enabled_decoders += ['xmp']
endif


faad = dependency('faad2', required: false)
if faad.found()
    input_plugins_with_taglib += [['aac', ['src/lib/plugins/Input/aac/decoder_aac.cpp', 'src/lib/plugins/Input/aac/decoderaacfactory.cpp', 'src/lib/plugins/Input/aac/aacfile.cpp'], ['src/lib/plugins/Input/aac/decoderaacfactory.h'], [faad]]]
    enabled_decoders += ['aac']
endif


input_plugins_without_taglib += [['cue', ['src/lib/plugins/Input/cue/decoder_cue.cpp', 'src/lib/plugins/Input/cue/decodercuefactory.cpp', 'src/lib/plugins/Input/cue/cuesettingsdialog.cpp', 'src/lib/plugins/Input/cue/cuefile.cpp', 'src/lib/plugins/Input/cue/cuemetadatamodel.cpp'], ['src/lib/plugins/Input/cue/decodercuefactory.h', 'src/lib/plugins/Input/cue/cuesettingsdialog.h', 'src/lib/plugins/Input/cue/cuemetadatamodel.h'], []]]

cdio = dependency('libcdio', required: false)
cdio_cdda = dependency('libcdio_cdda', required: false)
cdio_paranoia = dependency('libcdio_paranoia', required: false)
cddb = dependency('libcddb', required: false)
if cdio.found() and cdio_cdda.found() and cdio_paranoia.found()
    cdaudio_args = cddb.found() ? ['-DWITH_LIBCDDB'] : []
    cdaudio_deps = [cdio, cdio_cdda, cdio_paranoia]
    if cddb.found()
        cdaudio_deps += [cddb]
    endif
    cdaudio_moc = qt6.preprocess(moc_headers: ['src/lib/plugins/Input/cdaudio/decodercdaudiofactory.h', 'src/lib/plugins/Input/cdaudio/cdaudiosettingsdialog.h'], ui_files: ['src/lib/plugins/Input/cdaudio/cdaudiosettingsdialog.ui'], moc_extra_arguments: moc_extra_arguments)
    shared_module('cdaudio', ['src/lib/plugins/Input/cdaudio/decoder_cdaudio.cpp', 'src/lib/plugins/Input/cdaudio/decodercdaudiofactory.cpp', 'src/lib/plugins/Input/cdaudio/cdaudiosettingsdialog.cpp'] + cdaudio_moc, link_with: audio_core, dependencies: [qt] + cdaudio_deps, include_directories: plugin_include, cpp_args: cpp_args + cdaudio_args, install: true, install_dir: input_plugin_dir)
endif


wildmidi = dependency('wildmidi', required: false)
if wildmidi.found()
    input_plugins_without_taglib += [['wildmidi', ['src/lib/plugins/Input/wildmidi/decoder_wildmidi.cpp', 'src/lib/plugins/Input/wildmidi/decoderwildmidifactory.cpp', 'src/lib/plugins/Input/wildmidi/wildmidisettingsdialog.cpp', 'src/lib/plugins/Input/wildmidi/wildmidihelper.cpp'], ['src/lib/plugins/Input/wildmidi/decoderwildmidifactory.h', 'src/lib/plugins/Input/wildmidi/wildmidisettingsdialog.h', 'src/lib/plugins/Input/wildmidi/wildmidihelper.h'], [wildmidi]]]
    enabled_decoders += ['wildmidi']
endif


gme = dependency('gme', required: false)
if not gme.found()
    gme = dependency('libgme', required: false)
endif
if gme.found()
    input_plugins_without_taglib += [['gme', ['src/lib/plugins/Input/gme/decoder_gme.cpp', 'src/lib/plugins/Input/gme/decodergmefactory.cpp', 'src/lib/plugins/Input/gme/gmehelper.cpp', 'src/lib/plugins/Input/gme/gmesettingsdialog.cpp'], ['src/lib/plugins/Input/gme/decodergmefactory.h', 'src/lib/plugins/Input/gme/gmehelper.h', 'src/lib/plugins/Input/gme/gmesettingsdialog.h'], [gme]]]
    enabled_decoders += ['gme']
endif


opus = dependency('opus', required: false)
opusfile = dependency('opusfile', required: false)
if opus.found() and opusfile.found()
    input_plugins_with_taglib += [['opus', ['src/lib/plugins/Input/opus/decoder_opus.cpp', 'src/lib/plugins/Input/opus/decoderopusfactory.cpp', 'src/lib/plugins/Input/opus/opusmetadatamodel.cpp'], ['src/lib/plugins/Input/opus/decoderopusfactory.h'], [opus, opusfile]]]
    enabled_decoders += ['opus']
endif


sidplayfp = dependency('libsidplayfp', required: false)
if sidplayfp.found()
    input_plugins_without_taglib += [['sid', ['src/lib/plugins/Input/sid/decoder_sid.cpp', 'src/lib/plugins/Input/sid/decodersidfactory.cpp', 'src/lib/plugins/Input/sid/sidsettingsdialog.cpp', 'src/lib/plugins/Input/sid/sidhelper.cpp'], ['src/lib/plugins/Input/sid/decodersidfactory.h', 'src/lib/plugins/Input/sid/sidsettingsdialog.h', 'src/lib/plugins/Input/sid/sidhelper.h'], [sidplayfp]]]
    enabled_decoders += ['sid']
endif


libarchive = dependency('libarchive', required: false)
if libarchive.found()
    input_plugins_with_taglib += [['archive', ['src/lib/plugins/Input/archive/archiveinputdevice.cpp', 'src/lib/plugins/Input/archive/decoder_archive.cpp', 'src/lib/plugins/Input/archive/decoderarchivefactory.cpp', 'src/lib/plugins/Input/archive/archivetagreader.cpp'], ['src/lib/plugins/Input/archive/archiveinputdevice.h', 'src/lib/plugins/Input/archive/decoderarchivefactory.h'], [libarchive]]]
    enabled_decoders += ['archive']
endif


foreach plugin_spec : input_plugins_with_taglib
    plugin_moc = qt6.preprocess(moc_headers: plugin_spec[2], ui_files: plugin_ui, moc_extra_arguments: moc_extra_arguments)
    shared_module(plugin_spec[0], plugin_spec[1] + plugin_moc, link_with: audio_core, dependencies: [qt, taglib] + plugin_spec[3], include_directories: plugin_include, cpp_args: cpp_args, install: true, install_dir: input_plugin_dir)
endforeach

foreach plugin_spec : input_plugins_without_taglib
    plugin_moc = qt6.preprocess(moc_headers: plugin_spec[2], ui_files: plugin_ui, moc_extra_arguments: moc_extra_arguments)
    shared_module(plugin_spec[0], plugin_spec[1] + plugin_moc, link_with: audio_core, dependencies: [qt] + plugin_spec[3], include_directories: plugin_include, cpp_args: cpp_args, install: true, install_dir: input_plugin_dir)
endforeach

if enabled_decoders.length() == 0
    error('No practical audio decoder plugins were found. Install at least one codec development package.')
endif


alsa = dependency('alsa', required: false)
if alsa.found()
    output_plugins += [['alsa', ['src/lib/plugins/Output/alsa/outputalsa.cpp', 'src/lib/plugins/Output/alsa/outputalsafactory.cpp', 'src/lib/plugins/Output/alsa/alsasettingsdialog.cpp'], ['src/lib/plugins/Output/alsa/outputalsafactory.h', 'src/lib/plugins/Output/alsa/alsasettingsdialog.h'], [alsa]]]
endif


jack = dependency('jack', required: false)
soxr = dependency('soxr', required: false)
if jack.found() and soxr.found()
    output_plugins += [['jack', ['src/lib/plugins/Output/jack/outputjackfactory.cpp', 'src/lib/plugins/Output/jack/outputjack.cpp', 'src/lib/plugins/Output/jack/bio2jack.c'], ['src/lib/plugins/Output/jack/outputjackfactory.h'], [jack, soxr]]]
endif


output_plugins += [['null', ['src/lib/plugins/Output/null/outputnull.cpp', 'src/lib/plugins/Output/null/outputnullfactory.cpp'], ['src/lib/plugins/Output/null/outputnullfactory.h'], []]]

cc = meson.get_compiler('cpp')
if cc.compiles('#include <sys/soundcard.h>\n#ifndef SNDCTL_DSP_COOKEDMODE\n#error OSS4 support is unavailable\n#endif\nint main() { return 0; }', name: 'OSS4 support')
    output_plugins += [['oss4', ['src/lib/plugins/Output/oss4/outputoss4factory.cpp', 'src/lib/plugins/Output/oss4/outputoss4.cpp', 'src/lib/plugins/Output/oss4/oss4settingsdialog.cpp'], ['src/lib/plugins/Output/oss4/outputoss4factory.h', 'src/lib/plugins/Output/oss4/oss4settingsdialog.h'], []]]
endif


pipewire = dependency('libpipewire-0.3', required: false)
spa = dependency('libspa-0.2', required: false)
if pipewire.found() and spa.found()
    output_plugins += [['pipewire', ['src/lib/plugins/Output/pipewire/outputpipewire.cpp', 'src/lib/plugins/Output/pipewire/outputpipewirefactory.cpp'], ['src/lib/plugins/Output/pipewire/outputpipewirefactory.h'], [pipewire, spa]]]
endif


pulse = dependency('libpulse', required: false)
if pulse.found()
    output_plugins += [['pulseaudio', ['src/lib/plugins/Output/pulseaudio/outputpulseaudio.cpp', 'src/lib/plugins/Output/pulseaudio/outputpulseaudiofactory.cpp'], ['src/lib/plugins/Output/pulseaudio/outputpulseaudiofactory.h'], [pulse]]]
endif


output_plugins += [['qtmultimedia', ['src/lib/plugins/Output/qtmultimedia/outputqtmultimedia.cpp', 'src/lib/plugins/Output/qtmultimedia/outputqtmultimediafactory.cpp', 'src/lib/plugins/Output/qtmultimedia/qtmultimediasettingsdialog.cpp'], ['src/lib/plugins/Output/qtmultimedia/outputqtmultimediafactory.h', 'src/lib/plugins/Output/qtmultimedia/qtmultimediasettingsdialog.h'], []]]

shout = dependency('shout', required: false)
vorbisenc = dependency('vorbisenc', required: false)
if shout.found() and ogg.found() and vorbis.found() and vorbisenc.found() and soxr.found()
    output_plugins += [['shout', ['src/lib/plugins/Output/shout/shoutoutput.cpp', 'src/lib/plugins/Output/shout/outputshoutfactory.cpp', 'src/lib/plugins/Output/shout/shoutclient.cpp', 'src/lib/plugins/Output/shout/shoutsettingsdialog.cpp'], ['src/lib/plugins/Output/shout/outputshoutfactory.h', 'src/lib/plugins/Output/shout/shoutclient.h', 'src/lib/plugins/Output/shout/shoutsettingsdialog.h'], [shout, ogg, vorbis, vorbisenc, soxr]]]
endif


foreach plugin_spec : output_plugins
    plugin_moc = qt6.preprocess(moc_headers: plugin_spec[2], ui_files: plugin_ui, moc_extra_arguments: moc_extra_arguments)
    shared_module(plugin_spec[0], plugin_spec[1] + plugin_moc, link_with: audio_core, dependencies: [qt] + plugin_spec[3], include_directories: plugin_include, cpp_args: cpp_args, install: true, install_dir: output_plugin_dir)
endforeach

pkg = import('pkgconfig')
pkg.generate(audio, name: 'MauiKit Audio 4', filebase: 'MauiKitAudio4', version: meson.project_version(), description: 'MauiKit audio library', requires: 'MauiKit4')


cmake_targets = configure_file(
    input: 'meson-content.in',
    output: 'MauiKitAudio4Targets.cmake',
    configuration: {'CONTENT': (('''get_filename_component(_MAUIKIT_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)
if(NOT TARGET MauiKitAudio4)
  add_library(MauiKitAudio4 SHARED IMPORTED)
  set_target_properties(MauiKitAudio4 PROPERTIES
    IMPORTED_LOCATION "${_MAUIKIT_PREFIX}/@LIBDIR@/libMauiKitAudio4.so.4.0.5"
    INTERFACE_INCLUDE_DIRECTORIES "${_MAUIKIT_PREFIX}/include/MauiKit4/Audio")
endif()
if(NOT TARGET MauiKit4::Audio)
  add_library(MauiKit4::Audio ALIAS MauiKitAudio4)
endif()
if(NOT TARGET MauiKit::Audio)
  add_library(MauiKit::Audio ALIAS MauiKitAudio4)
endif()
''')).replace('@LIBDIR@', (get_option('libdir')))}
)
cmake_config = configure_file(
    input: 'meson-content.in',
    output: 'MauiKitAudio4Config.cmake',
    configuration: {'CONTENT': ('''include(CMakeFindDependencyMacro)
find_dependency(Qt6 6.4 COMPONENTS Core)
include("${CMAKE_CURRENT_LIST_DIR}/MauiKitAudio4Targets.cmake")
set(MauiKitAudio4_FOUND TRUE)
''')}
)
cmake_version = configure_file(
    input: 'meson-content.in',
    output: 'MauiKitAudio4ConfigVersion.cmake',
    configuration: {'CONTENT': (('''set(PACKAGE_VERSION "@VERSION@")
if(PACKAGE_FIND_VERSION VERSION_LESS_EQUAL PACKAGE_VERSION)
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
endif()
if(PACKAGE_FIND_VERSION VERSION_EQUAL PACKAGE_VERSION)
  set(PACKAGE_VERSION_EXACT TRUE)
endif()
''')).replace('@VERSION@', (meson.project_version()))}
)
install_data([cmake_targets, cmake_config, cmake_version], install_dir: get_option('libdir') / 'cmake' / 'MauiKitAudio4')
