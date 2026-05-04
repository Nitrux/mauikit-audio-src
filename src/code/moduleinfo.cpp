#include "moduleinfo.h"
#include "../audio_version.h"

QString MauiKitAudio::versionString()
{
    return QStringLiteral(Audio_VERSION_STRING);
}

QString MauiKitAudio::buildVersion()
{
    return QStringLiteral(GIT_BRANCH)+QStringLiteral("/")+QStringLiteral(GIT_COMMIT_HASH);
}

KAboutComponent MauiKitAudio::aboutData()
{
    return KAboutComponent(QStringLiteral("MauiKit Audio"),
                         QStringLiteral("Audio playback plugins."),
                         QStringLiteral(Audio_VERSION_STRING),
                         QStringLiteral("http://mauikit.org"),
                         KAboutLicense::LicenseKey::LGPL_V3);
}

KAboutPerson MauiKitAudio::credit()
{
    return KAboutPerson(QStringLiteral("Qmmp Devs"),
                         QStringLiteral("Forked from libqmmp."),
                         QStringLiteral(),
                         QStringLiteral("https://qmmp.ylsoftware.com/"));
}
