/***************************************************************************
 *   Copyright (C) 2008-2025 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QLocale>
#include <QFile>
#include <QByteArray>
#include <QStandardPaths>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define DEV_SUFFIX "dev"

#include "qmmp.h"

QString Qmmp::m_configDir;
QString Qmmp::m_langID;

#ifdef QT_DEBUG
Q_LOGGING_CATEGORY(core, "qmmp.core", QtDebugMsg)
Q_LOGGING_CATEGORY(plugin, "qmmp.plugin", QtDebugMsg)
#else
Q_LOGGING_CATEGORY(core, "qmmp.core", QtWarningMsg)
Q_LOGGING_CATEGORY(plugin, "qmmp.plugin", QtWarningMsg)
#endif


QString Qmmp::configDir()
{
    if(m_configDir.isEmpty())
        return QStringLiteral("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation),
                                           QCoreApplication::organizationName());
    return m_configDir;
}

void Qmmp::setConfigDir(const QString &path)
{
    m_configDir = path;
}

QString Qmmp::cacheDir()
{
    if(m_configDir.isEmpty())
        return QStringLiteral("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation),
                                           QCoreApplication::organizationName());
    return m_configDir;
}

QString Qmmp::strVersion()
{
    QString ver = QStringLiteral("%1.%2.%3")
            .arg(QMMP_VERSION_MAJOR)
            .arg(QMMP_VERSION_MINOR)
            .arg(QMMP_VERSION_PATCH);
#if !QMMP_VERSION_STABLE
#ifdef SVN_REVISION
    ver += QStringLiteral("-svn-" SVN_REVISION);
#else
    ver += QStringLiteral("-" DEV_SUFFIX);
#endif
#endif
    return ver;
}

QString Qmmp::pluginPath()
{
    QByteArray path = qgetenv("MAUIKITAUDIO_PLUGIN_DIR");
    if (!path.isEmpty())
        return QString::fromLocal8Bit(path);
    QString fallbackPath = QStringLiteral("%1/../lib/qmmp-" STR(QMMP_VERSION_MAJOR) "." STR(QMMP_VERSION_MINOR)).arg(qApp->applicationDirPath());
#ifdef MAUIKITAUDIO_PLUGIN_DIR
    QDir dir(QStringLiteral(MAUIKITAUDIO_PLUGIN_DIR));
#else
    QDir dir(fallbackPath);
#endif
    if(!dir.exists())
        dir = QDir(fallbackPath);
    return dir.canonicalPath();
}

QStringList Qmmp::findPlugins(const QString &prefix)
{
    QDir pluginDir(pluginPath() + u"/"_s + prefix);
    QStringList paths;
    for(const QFileInfo &info : pluginDir.entryInfoList({ u"*.dll"_s, u"*.so"_s }, QDir::Files))
        paths << info.canonicalFilePath();
    qDebug() << "Looking for plugins at << " <<  pluginPath() << paths << pluginDir.path();

    return paths;
}

QString Qmmp::systemLanguageID()
{
    if(m_langID.isEmpty())
    {
        m_langID = uiLanguageID();
    }

    if(m_langID != "auto"_L1)
        return m_langID;

#ifdef Q_OS_UNIX
    QByteArray v = qgetenv ("LC_ALL");
    if (v.isEmpty())
        v = qgetenv ("LC_MESSAGES");
    if (v.isEmpty())
        v = qgetenv ("LANG");
    if (!v.isEmpty())
        return QLocale(QString::fromLatin1(v)).name();
#endif
    return  QLocale::system().name();
}

QString Qmmp::uiLanguageID()
{
    QSettings settings;
    QString langID = settings.value(u"General/locale"_s, u"auto"_s).toString();
    langID = langID.isEmpty() ? u"auto"_s : langID;
    return langID;
}

void Qmmp::setUiLanguageID(const QString &code)
{
    QSettings settings;
    settings.setValue(u"General/locale"_s, code);
    m_langID.clear();
}

QString Qmmp::dataPath()
{
    return QDir(QStringLiteral("%1/../share/qmmp" APP_NAME_SUFFIX).arg(qApp->applicationDirPath())).absolutePath();
}

QString Qmmp::userDataPath()
{
    if(m_configDir.isEmpty())
    {
        return QStringLiteral("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation),
                                           QCoreApplication::organizationName());
    }
    return m_configDir;
}
