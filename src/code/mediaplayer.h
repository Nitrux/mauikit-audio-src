#pragma once

#include <QObject>
#include <QUrl>
#include <QQmlEngine>
#include <QStringList>

#include "audio_export.h"

class SoundCore;
class Output;
class OutputFactory;

class AUDIO_EXPORT MediaPlayer : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)
    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(QString preferredOutput READ preferredOutput WRITE setPreferredOutput NOTIFY preferredOutputChanged FINAL)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged FINAL)
    Q_PROPERTY(qint64 elapsed READ elapsed NOTIFY elapsedChanged)
    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged)
    //Models
    Q_PROPERTY(QStringList outputs READ outputs NOTIFY outputsChanged FINAL)


public:
    enum State
    {
        Playing = 0, /*!< The player is playing source */
        Paused,      /*!< The player has currently paused its playback */
        Stopped,     /*!< The player is ready to play source */
        Buffering,   /*!< The Player is waiting for data to be able to start playing.   */
        NormalError, /*!< Input source is invalid or unsupported. Player should skip this file */
        FatalError   /*!< This means unrecorvable error die audio output problems. Player should abort playback. */
    };Q_ENUM(State)
    explicit MediaPlayer(QObject *parent = nullptr);

    QUrl source() const;
    void setSource(const QUrl &newSource);

    State state() const;

    QStringList outputs() const;

    QString preferredOutput() const;
    void setPreferredOutput(const QString &newPreferredOutput);

    int volume() const;
    void setVolume(int newVolume);

    double position() const;
    void setPosition(double pos);

public Q_SLOTS:
    void play();
    void pause();
    void stop();
    void seek(qint64 pos);
    void volumeUp();
    void volumeDown();
    qint64 duration() const;
    qint64 elapsed() const;
    static QString formatTime_ms(qint64 ms);

Q_SIGNALS:
    void sourceChanged();
    void error(QString);
    void finished();
    void stateChanged();

    void outputsChanged();

    void preferredOutputChanged();

    void volumeChanged();
    void trackChanged();
    void elapsedChanged();
    void positionChanged();
    void durationChanged();

private:
    QUrl m_source;
    SoundCore *m_core;
    QString m_preferredOutput;

    OutputFactory *findOutput(const QString &shortName);
};

