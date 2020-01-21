//
// Created by yepan on 2019/12/17.
//

#ifndef IOTVIDEO_IOTVIDEOPLAYERCOMMON_H
#define IOTVIDEO_IOTVIDEOPLAYERCOMMON_H

#include <string>
#include <functional>

typedef enum {
    STATE_IDLE = 0, //播放器初始状态
    STATE_INITIALIZED = 1, //已经初始化
    STATE_PREPARING = 2,   //准备中
    STATE_READY = 3,       //准备完毕
    STATE_LOADING = 4,     //加载
    STATE_PLAY = 5,        //播放状态
    STATE_PAUSE = 6,       //暂停
    STATE_STOP = 7,        //停止
} PlayerState;

/**
 * 音视频帧数据结构
 */
typedef struct AVData
{
    uint8_t  *data = nullptr;
    uint32_t size = 0;
    uint8_t  *data1 = nullptr;
    uint32_t size1 = 0;
    uint8_t  *data2 = nullptr;
    uint32_t size2 = 0;

    uint64_t pts = 0;
    uint64_t dts = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint8_t  keyFrame = 0;
} AVData;

/**
 * 音视头信息
 */
typedef struct AVHeader
{
    int audioType = 0;          // 音频编码格式
    int audioCodecOption = 0;   // 音频编码的参数
    int audioMode = 0;          // 音频模式： 单声道/双声道
    int audioBitWidth = 0;      // 音频位宽，目前只支持16bit
    int audioSampleRate = 0;    // 音频采样率
    int sampleNumPerFrame = 0;  // 每帧数据里的采样数

    int videoType = 0;          // 视频类型
    int videoWidth = 0;         // 视频像素宽度
    int videoHeight = 0;        // 视频像素高度
    int videoFrameRate = 0;     // 视频帧率
}AVHeader;

/**
 * 音频编码器
 */
class AudioEncoder {
public:
    virtual ~AudioEncoder(){};
    /**
    * 初始化
    */
    virtual void init(AVHeader* header) = 0;

    /**
     * 解码一帧
     * @param inPacket
     * @param outFrame
     * @return
     */
    virtual int encode(AVData *inFrame, AVData *outPacket) = 0;

    /**
     * 释放
     */
    virtual void release() = 0;
};

/**
 * 音频解码器
 */
class AudioDecoder {
public:
    virtual ~AudioDecoder(){};
    /**
     * 初始化
     */
    virtual void init(AVHeader* header) = 0;


    /**
     * 解码一帧
     * @param inPacket
     * @param outFrame
     * @return
     */
    virtual int decode(AVData *inPacket, AVData *outFrame) = 0;

    /**
     * 释放
     */
    virtual void release() = 0;
};

/**
 * 视频编码器
 */
class VideoEncoder {
public:
    virtual ~VideoEncoder(){};
    /**
     * 初始化
     */
    virtual void init(AVHeader *header) = 0;

    /**
     * 解码一帧
     * @param inFrame
     * @param outPacket
     * @return
     */
    virtual int encode(AVData *inFrame, AVData *outPacket) = 0;

    /**
     * 释放
     */
    virtual void release() = 0;
};

/**
 * 视频解码器
 */
class VideoDecoder {
public:
    virtual ~VideoDecoder(){};
    /**
     * 初始化
     */
    virtual void init(AVHeader* header) = 0;


    /**
     * 解码一帧
     * @param inPacket
     * @param outFrame
     * @return
     */
    virtual int decode(AVData *inPacket, AVData *outFrame) = 0;

    /**
     * 释放
     */
    virtual void release() = 0;
};

/**
 * 视频渲染
 */
class VideoRender {
public:
    virtual ~VideoRender(){};
    /**
     * 初始化
     */
    virtual void init(AVHeader *header) = 0;

    /**
     * 渲染一帧视频帧
     * @param data
     * @return
     */
    virtual void frameUpdate(AVData *data) = 0;

    /**
     * 释放
     */
    virtual void release() = 0;
};

/**
 * 音频渲染
 */
class AudioRender {
public:
    virtual ~AudioRender(){};
    /**
     * 初始化
     */
    virtual void init(AVHeader *header) = 0;

    /**
     * 渲染一帧音频帧
     * @param data
     * @return
     */
    virtual void frameUpdate(AVData *data) = 0;

    /**
     * 释放
     */
    virtual void release() = 0;
};

#endif //IOTVIDEO_IOTVIDEOPLAYERCOMMON_H
