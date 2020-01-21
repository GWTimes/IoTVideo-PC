//
// Created by yepan on 2019/12/17.
//

#ifndef IOTVIDEO_IOTVIDEOPLAYER_H
#define IOTVIDEO_IOTVIDEOPLAYER_H

#include <string>
#include <functional>
#include <memory>
#include "IoTVideoPlayerCommon.h"

#ifdef IOTVIDEOSDKDLL_EXPORTS
#define IOTVIDEOSDKDLL_API __declspec(dllexport)
#else
#define IOTVIDEOSDKDLL_API __declspec(dllimport)
#endif

class IoTVideoSdkPrivate;
class IoTRequest;
class IoTRequestPrivate;
typedef std::shared_ptr<IoTRequestPrivate> IoTRequestPrivatePtr;

class IOTVIDEOSDKDLL_API IoTVideoSdk{
 public:
    /**
        @\brief 用户接入
        @\param accessId 用户id
        @\param accessToken 用户token
    */
    void Register(int64_t accessId, const std::string& accessToken);
    /**
        @\brief 用户退出
    */
    void unRegister();
    /**
        @\brief 添加状态消息回调
    */
    void addModelDataListener(std::function<void(std::string, std::string)> fn);
    /**
        @\brief 添加事件消息回调
    */
    void addEventListener(std::function<void(std::string, std::string)> fn);
    static IoTVideoSdk* getInstance();
 private:
    IoTVideoSdk();
    virtual ~IoTVideoSdk() = delete;
    IoTVideoSdk(const IoTVideoSdk&) = delete;
    IoTVideoSdk& operator=(const IoTVideoSdk&) = delete;

private:
    static IoTVideoSdk* singletonSdk;
    IoTVideoSdkPrivate* d;
};

class IOTVIDEOSDKDLL_API IoTRequest
{
public:
    /**
        @\brief 消息请求构造
        @\param devid 设备id
    */
    IoTRequest(const std::string& devid);

    /**
        @\brief 设置物模型
        @\param objLeaf 物模型类型
        @\param jData   物模型设置参数
    */
    IoTRequest& IoTSendModelData(const std::string& objLeaf, const std::string& jData);

    /**
        @\brief 获取物模型
        @\param type 物模型类型
    */
    IoTRequest& IoTGetModelData(const std::string& type);

    /**
        @\brief 设置超时回调函数
    */
    IoTRequest& IoTOnTimeout(std::function<void(std::string)> fn);

    /**
        @\brief 设置错误回调函数
    */
    IoTRequest& IoTError(std::function<void(std::string,int err)> fn);
    /**
        @\brief 设置成功回调函数
    */
    IoTRequest& IoTSuccess(std::function<void(std::string)> fn);
private:
    IoTRequestPrivate* privReq;
};

class IoTVideoPlayerImpl;

class IOTVIDEOSDKDLL_API IoTVideoPlayer {
public:
    IoTVideoPlayer();

    virtual ~IoTVideoPlayer();

public:
    void setDataResource(uint64_t deviceId, int callType);

    void prepare();

    void play();

    void pause();

    void resume();

    void stop();

    void release();

    int getPlayState();

    AVHeader getAVHeader();

    bool isMute();

    void mute(bool on);

    void startTalk();

    void stopTalk();

    bool startRecord(const std::string&, std::function<void(int, std::string)> func);

    bool isRecording();

    void stopRecord();

    void snapShot(const std::string& path, std::function<void(int, std::string)> func);

    int setUserData(uint8_t *data, int size);

    void setPreparedListener(std::function<void(void)> func);

    void setStatusListener(std::function<void(int)> func);

    void setTimeListener(std::function<void(long)> func);

    void setErrorListener(std::function<void(int)> func);

    void setUserDataListener(std::function<void(uint8_t *, int size)> func);

    void setAudioEncoder(AudioEncoder *encoder);

    void setAudioDecoder(AudioDecoder *decoder);

    void setVideoEncoder(VideoEncoder *encoder);

    void setVideoDecoder(VideoDecoder *decoder);

    void setVideoRender(VideoRender *render);

    void setAudioRender(AudioRender *render);

    void setCaptureHeader(AVHeader *header);

    void sendAudioData(AVData *data);

    void sendVideoData(AVData *data);

public:
    IoTVideoPlayerImpl *impl = nullptr;
};


#endif //IOTVIDEO_IOTVIDEOPLAYER_H
