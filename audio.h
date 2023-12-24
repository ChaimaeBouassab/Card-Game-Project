#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
//#include <QMediaPlayer>
//#include <QAudioOutput>

class Audio
{
public:
    Audio(QString src, qreal volume);

//    QMediaPlayer *media;
//    QAudioOutput *audio;

    bool played{false};

    void play();
    void stop();


};

#endif // AUDIO_H
