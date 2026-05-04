#include "mediaplayer.h"
#include <QDebug>
#include <QTime>

#include <qmmp/soundcore.h>
#include <qmmp/abstractengine.h>
#include <qmmp/output.h>

MediaPlayer::MediaPlayer(QObject *parent)
    : QObject{parent}
    , m_core(new SoundCore(this))
{
    m_preferredOutput = Output::currentFactory()->properties().shortName;

    connect(m_core, &SoundCore::finished, this, &MediaPlayer::finished);
    connect(m_core, &SoundCore::stateChanged, this, &MediaPlayer::stateChanged);
    connect(m_core, &SoundCore::volumeChanged, this, &MediaPlayer::volumeChanged);
    connect(m_core, &SoundCore::elapsedChanged, this, [this](qint64) {
        Q_EMIT positionChanged();
        Q_EMIT elapsedChanged();
    });
        connect(m_core, &SoundCore::stateChanged, this, &MediaPlayer::durationChanged);

}

QUrl MediaPlayer::source() const
{
    return m_source;
}

void MediaPlayer::setSource(const QUrl &newSource)
{
    if (m_source == newSource)
        return;
    m_source = newSource;
    Q_EMIT sourceChanged();
}

void MediaPlayer::play()
{
    if (!m_source.isValid() || m_source.isEmpty()) {
        Q_EMIT error("The source URL is not valid or is not set");
        return;
    }

    if (m_core->state() == Qmmp::State::Paused) {
        m_core->pause();
        return;
    }

    const auto ok = m_core->play(m_source.toLocalFile());
    if (!ok) {
        Q_EMIT error("File could not be played.");
    } else {
        // Q_EMIT trackChanged();
    }
}

qint64 MediaPlayer::duration() const
{
    return m_core->duration();
}

void MediaPlayer::pause()
{
    m_core->pause();
}

void MediaPlayer::stop()
{
    m_core->stop();
    Q_EMIT trackChanged();
    Q_EMIT positionChanged();
}

qint64 MediaPlayer::elapsed() const
{
    if (m_core->elapsed() < 0)
        return 0;
    return m_core->elapsed();
}

double MediaPlayer::position() const
{
    if (m_core->elapsed() <= 0 || m_core->duration() <= 0)
        return 0;
    double value = (double)m_core->elapsed() / m_core->duration();
    return std::ceil(value * 100.0) / 100.0;
}

void MediaPlayer::setPosition(double pos)
{
    if (pos > 1.0 || pos < 0.0)
        return;

    m_core->seek(m_core->duration() * pos);
    Q_EMIT positionChanged();
}

void MediaPlayer::seek(qint64 pos)
{
    m_core->seek(pos);
}

void MediaPlayer::volumeUp()
{
    m_core->volumeUp();
}

void MediaPlayer::volumeDown()
{
    m_core->volumeDown();
}

OutputFactory *MediaPlayer::findOutput(const QString &shortName)
{
    auto factories = Output::factories();
    auto it = std::find_if(factories.cbegin(), factories.cend(), [&](OutputFactory *fac) -> bool {
        return fac->properties().shortName == shortName;
    });

    if (it != factories.cend()) { // Compare with the const end iterator
        return *it;
    } else {
        qWarning() << "Could not find OutputFactory for : " << shortName;
        return nullptr;
    }
}

MediaPlayer::State MediaPlayer::state() const
{
    return static_cast<MediaPlayer::State>(m_core->state());
}

QStringList MediaPlayer::outputs() const
{
    QStringList outputs;

    for (auto out : Output::factories())
        outputs << out->properties().shortName;

    return outputs;
}

QString MediaPlayer::preferredOutput() const
{
    return m_preferredOutput;
}

void MediaPlayer::setPreferredOutput(const QString &newPreferredOutput)
{
    if (m_preferredOutput == newPreferredOutput)
        return;

    auto factory = findOutput(newPreferredOutput);
    if (!factory) {
        qWarning() << "Could not set the preferred output : " << newPreferredOutput;
        return;
    }

    Output::setCurrentFactory(factory);

    m_preferredOutput = newPreferredOutput;

    Q_EMIT preferredOutputChanged();
}

int MediaPlayer::volume() const
{
    return m_core->volume();
}

void MediaPlayer::setVolume(int newVolume)
{
    m_core->setVolume(newVolume);
}

QString MediaPlayer::formatTime_ms(qint64 ms)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(ms);

    // Format the QTime object into a string
    // "HH" for hours, "mm" for minutes, "ss" for seconds, "zzz" for milliseconds
    return time.toString("mm:ss");
}
