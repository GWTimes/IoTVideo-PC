#include "videorendercase.h"
#include <stdio.h>
#include "DirectDraw.h"

VideoRenderCase::VideoRenderCase(HWND hShowWind)
{
    m_hShowWind = hShowWind;
    DD_Initialize(&m_hDDrawWind);
}

VideoRenderCase::~VideoRenderCase()
{
    if(m_hDDrawWind)
    {
         DD_Uninitialize(m_hDDrawWind);
         m_hDDrawWind = nullptr;
    }
}

void VideoRenderCase::init(AVHeader *header)
{
    printf("%s videoType=%d\n",__FUNCTION__,header->videoType);
    m_iVideoType = header->videoType;
}

void VideoRenderCase::frameUpdate(AVData *data)
{
    //printf("%s frameUpdate render is running\n", __FUNCTION__);
    ImageProperties imageProperties;
    imageProperties.dwHeight = data->height;
    imageProperties.dwWidth = data->width;
    imageProperties.dwVideoType = m_iVideoType;
    imageProperties.lpY = data->data;
    imageProperties.lpU = data->data1;
    imageProperties.lpV = data->data2;
    imageProperties.dwImageFormat = IMAGE_FORMAT_YV12;
    if(DD_Draw(m_hDDrawWind,m_hShowWind,&imageProperties) != 0)
    {
        printf("DD_Draw error\n");
    }
}

void VideoRenderCase::release()
{
    m_iVideoType = 0;
}
