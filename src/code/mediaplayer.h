#pragma once

#include <QObject>
#include <QUrl>
#include <QQmlEngine>
#include <QStringList>

#include "audio_export.h"

class SoundCore;
class Output;
class OutputFactory;

/**
 *  Plays a local audio source and exposes playback state to QML.
 *
 * MediaPlayer controls playback, seeking, volume, and the selected Qmmp output
 * backend. duration and elapsed are measured in milliseconds, while position is
 * a normalized value from 0.0 to 1.0.
 */
class AUDIO_EXPORT MediaPlayer : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    /** The local audio URL to play. */
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

    /** The current playback or error state. */
    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)

    /** The playback volume as reported by the audio backend. */
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)

    /** The short name of the selected output backend. */
    Q_PROPERTY(QString preferredOutput READ preferredOutput WRITE setPreferredOutput NOTIFY preferredOutputChanged FINAL)

    /** The source duration in milliseconds. */
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged FINAL)

    /** The elapsed playback time in milliseconds. */
    Q_PROPERTY(qint64 elapsed READ elapsed NOTIFY elapsedChanged)

    /** The normalized playback position from 0.0 to 1.0. */
    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged)

    /** The short names of the available output backends. */
    Q_PROPERTY(QStringList outputs READ outputs NOTIFY outputsChanged FINAL)


public:
    enum State
    {
        Playing = 0, /*!< The source is playing. */
        Paused,      /*!< Playback is paused and can be resumed. */
        Stopped,     /*!< Playback is stopped. */
        Buffering,   /*!< The player is waiting for enough data to continue. */
        NormalError, /*!< The source is invalid or unsupported; callers may skip it. */
        FatalError   /*!< Playback cannot continue because of an output failure. */
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
    /** Starts the source or resumes paused playback. */
    void play();

    /** Toggles the backend into its paused state. */
    void pause();

    /** Stops playback and resets the exposed position. */
    void stop();

    /** Seeks to  pos milliseconds from the beginning of the source. */
    void seek(qint64 pos);

    /** Increases the volume by the backend-defined step. */
    void volumeUp();

    /** Decreases the volume by the backend-defined step. */
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

