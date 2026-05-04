#pragma once

#include <QString>
#include <KAboutData>
#include "audio_export.h"

namespace MauiKitAudio
{
AUDIO_EXPORT QString versionString();
AUDIO_EXPORT QString buildVersion();
AUDIO_EXPORT KAboutComponent aboutData();
AUDIO_EXPORT KAboutPerson credit();
};
