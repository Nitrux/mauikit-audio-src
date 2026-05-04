
// SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "audio_plugin.h"

#include <QQmlEngine>

#include "mediaplayer.h"

QUrl AudioPlugin::componentUrl(const QString &fileName) const
{
    return QUrl(resolveFileUrl(fileName));
}

void AudioPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.mauikit.audio"));

    qmlRegisterType<MediaPlayer>(uri, 1, 0, "MediaPlayer");
}

void AudioPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}
