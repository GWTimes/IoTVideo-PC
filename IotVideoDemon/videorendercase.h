#ifndef VIDEORENDERCASE_H
#define VIDEORENDERCASE_H
#include "IoTVideoPlayerCommon.h"
#include <Windows.h>
class VideoRenderCase: public VideoRender
{
public:
    VideoRenderCase(HWND hShowWind);
    ~VideoRenderCase();
    /**
     * 初始化
     */
    void init(AVHeader *header);

    /**
     * 渲染一帧视频帧
     * @param data
     * @return
     */
    void frameUpdate(AVData *data);

    /**
     * 释放
     */
    void release();

    HANDLE m_hDDrawWind = nullptr;   //!< 绘画窗口
    HWND   m_hShowWind  = nullptr;   //!< 显示窗口
    int    m_iVideoType = 0;         //!< 视频类型
};

#endif // VIDEORENDERCASE_H
