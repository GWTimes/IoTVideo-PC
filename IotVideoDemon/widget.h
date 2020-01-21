#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "IoTVideoPlayer.h"
#include "videorendercase.h"
#include "audiorendercase.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void connectCtls();
    void initDialog();
    void userRegister();


    // 回调函数
    void ModelDataResp(std::string devid, std::string msg);
    void playerStatusListener(int status);  //播放器状态回调
signals:
    void showModelStatus_sig(QString text);
    void getModelDataTimeout_sig(QString devid);
    void getModelDataError_sig(QString devid,QString msg, int err);
    void getModelDataSuccess_sig(QString devid,QString msg);

    void setModelDataTimeout_sig(QString devid);
    void setModelDataError_sig(QString devid,QString msg, int err);
    void setModelDataSuccess_sig(QString devid,QString msg);

    void showPlayerStatus_sig(QString text);

private slots:

    void showModelStatus_slot(QString text);
    void getModelDataTimeout_slot(QString devid);
    void getModelDataError_slot(QString devid,QString msg, int err);
    void getModelDataSuccess_slot(QString devid,QString msg);

    void setModelDataTimeout_slot(QString devid);
    void setModelDataError_slot(QString devid,QString msg, int err);
    void setModelDataSuccess_slot(QString devid,QString msg);

    void showPlayerStatus_slot(QString text);

    void on_btnVideoPlay_clicked();

    void on_btnVideoStop_clicked();

    void on_btnGetModel_clicked();

    void on_btnSetModel_clicked();

    void on_btnRecord_clicked();

    void on_btnScreenShot_clicked();

    void on_btnTalk_clicked();

private:
    Ui::Widget *ui;
    IoTVideoSdk*    m_pIotVideoSdk = nullptr;       //!< sdk实例
    IoTVideoPlayer* m_pIotVideoPlayer = nullptr;    //!< 播放器对象指针
    VideoRenderCase* m_pVideoRender = nullptr;      //!< 渲染实现实例
    AudioRenderCase* m_pAudioRneder = nullptr;      //!< 声音播放实例
};

#endif // WIDGET_H
