#ifndef AUDIORENDERCASE_H
#define AUDIORENDERCASE_H
#include "IoTVideoPlayerCommon.h"

class QAudioFormat;
class QAudioOutput;
class QIODevice;
class AudioRenderCase:public AudioRender
{
public:
    AudioRenderCase();
    ~AudioRenderCase();
    /**
     * 初始化
     */
    void init(AVHeader *header);

    /**
     * 渲染一帧音频帧
     * @param data
     * @return
     */
    void frameUpdate(AVData *data);

    /**
     * 释放
     */
    void release();
private:
    QAudioFormat *_fmt;
    QAudioOutput *_out;
    QIODevice *_io;
};

#endif // AUDIORENDERCASE_H
