#include "audiorendercase.h"
#include <QAudioFormat>
#include <QAudioOutput>

AudioRenderCase::AudioRenderCase()
{

}

AudioRenderCase::~AudioRenderCase()
{
    if(_out)
    {
        _out->stop();
        delete _out;
        _out = nullptr;
    }
    if(_fmt)
    {
        delete _fmt;
        _fmt = nullptr;
    }
}

void AudioRenderCase::init(AVHeader *header)
{
    _fmt = new QAudioFormat;
    if(_fmt)
    {
        _fmt->setSampleRate(header->audioSampleRate);
        _fmt->setSampleSize(16);
        _fmt->setChannelCount(1);
        _fmt->setCodec("audio/pcm");
        _fmt->setByteOrder(QAudioFormat::LittleEndian);
        _fmt->setSampleType(QAudioFormat::SignedInt);
    }
    _out = new QAudioOutput(*_fmt);
    if(_out)
    {
        _io = _out->start();
        if(nullptr ==_io)
        {
            printf("%s audio start fail,error=%d!\n", __FUNCTION__,_out->error());
        }
    }
}

void AudioRenderCase::frameUpdate(AVData *data)
{
    if(_io)
    {
        _io->write((const char*)data->data, data->size);
    }
}

void AudioRenderCase::release()
{
}
