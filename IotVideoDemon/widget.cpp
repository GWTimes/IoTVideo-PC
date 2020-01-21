#include "widget.h"
#include "ui_widget.h"
#include <functional>
#include <iostream>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QSettings>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPalette pa;
    pa.setColor(QPalette::Background,QColor(135,206,235)); //!<淡蓝色
    ui->videoShowWind->setAutoFillBackground(true);
    ui->videoShowWind->setPalette(pa);
    initDialog();
    connectCtls();
    userRegister();
}

void Widget::connectCtls()
{
    connect(this,SIGNAL(showModelStatus_sig(QString)),this,SLOT(showModelStatus_slot(QString)));
    connect(this,SIGNAL(getModelDataTimeout_sig(QString)),this,SLOT(getModelDataTimeout_slot(QString)));
    connect(this,SIGNAL(getModelDataError_sig(QString,QString,int)),this,SLOT(getModelDataError_slot(QString,QString,int)));
    connect(this,SIGNAL(getModelDataSuccess_sig(QString,QString)),this,SLOT(getModelDataSuccess_slot(QString,QString)));
    connect(this,SIGNAL(showPlayerStatus_sig(QString)),this,SLOT(showPlayerStatus_slot(QString)));

    connect(this,SIGNAL(setModelDataTimeout_sig(QString)),this,SLOT(setModelDataTimeout_slot(QString)));
    connect(this,SIGNAL(setModelDataError_sig(QString,QString,int)),this,SLOT(setModelDataError_slot(QString,QString,int)));
    connect(this,SIGNAL(setModelDataSuccess_sig(QString,QString)),this,SLOT(setModelDataSuccess_slot(QString,QString)));
}

void Widget::showModelStatus_slot(QString text)
{
    ui->edtModelStatus->append(text);
}

void Widget::getModelDataTimeout_slot(QString devid)
{
    QString qsShowTimeout = "get model data time out devid=" + devid;
    QMessageBox::information(this,"info",qsShowTimeout);
}

void Widget::getModelDataError_slot(QString devid,QString msg, int err)
{
    QString qsShowErr = "get model data error,error code=" + QString("%1").arg(err);
    QMessageBox::information(this,"info",qsShowErr);
}

void Widget::getModelDataSuccess_slot(QString devid,QString msg)
{
    QString qsModel = devid + "\n" + msg;
    ui->edtModelResp->setText(qsModel);
}

void Widget::setModelDataTimeout_slot(QString devid)
{
    QString qsShowTimeout = "set model data time out devid=" + devid;
    QMessageBox::information(this,"info",qsShowTimeout);
}

void Widget::setModelDataError_slot(QString devid,QString msg, int err)
{
    QString qsShowErr = "set model data error,devid=" +devid +" error code=" + QString("%1").arg(err);
    QMessageBox::information(this,"info",qsShowErr);
}

void Widget::setModelDataSuccess_slot(QString devid,QString msg)
{
    QString qsShowSuccess = "set model data success! devid=" + devid;
    QMessageBox::information(this,"info",qsShowSuccess);
}

void Widget::showPlayerStatus_slot(QString text)
{
    ui->edtVideoStatus->append(text);
}

void Widget::initDialog()
{
    setWindowTitle("IoTVideoPlayer");
    ui->edtDevId->setText("42949672978");
    m_pIotVideoSdk = IoTVideoSdk::getInstance();
    m_pIotVideoPlayer = new IoTVideoPlayer;
    HWND hShowWind = (HWND)ui->videoShowWind->winId();
    m_pVideoRender = new VideoRenderCase(hShowWind);
    m_pIotVideoPlayer->setVideoRender(m_pVideoRender);
    m_pAudioRneder = new AudioRenderCase;
    m_pIotVideoPlayer->setAudioRender(m_pAudioRneder);
    m_pIotVideoPlayer->setStatusListener(std::bind(&Widget::playerStatusListener,this,std::placeholders::_1));
}

void Widget::userRegister()
{
    //int64_t accessId = -9223371598768111602;
    //std::string accessToken("010FF9C4EE068F1554F5E074660000005D49A8701558D8CA269EC8A6478B8A1FC9E85A813D7B94D682CBC5D44E7F78017C00201A541A09EBAF865E0EF95D876A");
    QString iniFilePath = QDir::currentPath() + "/userInfo.ini";
    QSettings* usrInfo = new QSettings(iniFilePath,QSettings::IniFormat);
    if(nullptr == usrInfo)
    {
        printf("%s user info file is not exist!\n", __FUNCTION__);
        delete usrInfo;
        usrInfo = nullptr;
    }

    int64_t accessId = usrInfo->value("user/accessId","").toLongLong();
    std::string accessToken = usrInfo->value("user/accessToken","").toString().toStdString();
    if(nullptr == m_pIotVideoSdk)
    {
        printf("%s IotVideoSdk is null!\n", __FUNCTION__);
        return;
    }
    m_pIotVideoSdk->addModelDataListener(std::bind(&Widget::ModelDataResp,
                                                   this,std::placeholders::_1,std::placeholders::_2));
    m_pIotVideoSdk->Register(accessId,accessToken);
}

void Widget::ModelDataResp(std::string devid, std::string msg)
{
    printf("%s devid=%s, msg:%s\n", __FUNCTION__, devid.c_str(),msg.c_str());
    QString qDevId = QString::fromStdString(devid);
    QString qMsg = QString::fromStdString(msg);
    QString qShowText = qDevId + "\n" + qMsg;
    emit showModelStatus_sig(qShowText);
}

void Widget::playerStatusListener(int status)
{
    if(STATE_PREPARING == status)
    {
        QString text = QStringLiteral("准备中");
        emit showPlayerStatus_sig(text);
    }
    else if(STATE_PLAY == status)
    {
        QString text = QStringLiteral("播放");
        emit showPlayerStatus_sig(text);
    }
    else if(STATE_STOP == status)
    {
        QString text = QStringLiteral("停止播放");
        emit showPlayerStatus_sig(text);
    }
    else
    {
        printf("%s onter status=%d\n", __FUNCTION__, status);
    }
}

Widget::~Widget()
{
    if(nullptr != m_pIotVideoPlayer)
    {
        delete m_pIotVideoPlayer;
        m_pIotVideoPlayer = nullptr;
    }
    if(nullptr != m_pIotVideoSdk)
    {
        m_pIotVideoSdk->unRegister();
    }
    delete ui;
}

void Widget::on_btnVideoPlay_clicked()
{
    printf("%s enter\n", __FUNCTION__);
    if(nullptr == m_pIotVideoPlayer)
    {
        printf("%s IotVideoPlayer is nullptr!\n", __FUNCTION__);
        return;
    }
    QString sDevID = ui->edtDevId->text();
    uint64_t u64DevID = sDevID.toULongLong();
    if(0 == u64DevID)
    {
        printf("%s device id is 0\n", __FUNCTION__);
        return;
    }
    m_pIotVideoPlayer->setDataResource(u64DevID, 1);
    m_pIotVideoPlayer->play();
}

void Widget::on_btnVideoStop_clicked()
{
    printf("%s enter\n", __FUNCTION__);
    if(nullptr == m_pIotVideoPlayer)
    {
        printf("%s IotVideoPlayer is nullptr!\n", __FUNCTION__);
        return ;
    }
    m_pIotVideoPlayer->stop();
}

void Widget::on_btnGetModel_clicked()
{
    printf("%s enter\n", __FUNCTION__);
    if(nullptr == m_pIotVideoPlayer)
    {
        printf("%s IotVideoPlayer is nullptr!\n", __FUNCTION__);
        return ;
    }

    QString qDevID = ui->edtDevId->text();
    std::string sDevID = qDevID.toStdString();
    QString qsModelType = ui->edtGetModelType->text();
    std::string sGetModeType = qsModelType.toStdString();
    IoTRequest getModelReq(sDevID);
    getModelReq.IoTGetModelData(sGetModeType)
            .IoTOnTimeout([qDevID,this](std::string msg){

                emit this->getModelDataTimeout_sig(qDevID);
             })
            .IoTError([qDevID,this](std::string msg,int err){
                QString qsMsg = QString::fromStdString(msg);
                emit this->getModelDataError_sig(qDevID,qsMsg,err);
            })
            .IoTSuccess([qDevID,this](std::string msg){
                printf("sDevID=%s msg=%s\n",qDevID.toStdString().c_str(),msg.c_str());
                QString qsMsg = QString::fromStdString(msg);
                emit this->getModelDataSuccess_sig(qDevID,qsMsg);
            });

}

void Widget::on_btnSetModel_clicked()
{
    QString qsSetModelText = ui->edtSetModelText->toPlainText();
    if(0 == qsSetModelText.length())
    {
        QMessageBox::information(this,"info","please input setting info");
        return;
    }
    QString qDevID = ui->edtDevId->text();
    std::string sDevID = qDevID.toStdString();
    QString qsModelType = ui->setModelType->text();
    std::string sSetModeType = qsModelType.toStdString();
    IoTRequest setModelReq(sDevID);
    std::string sJdata = qsSetModelText.toStdString();
    std::string sLeaf(sSetModeType);
    setModelReq.IoTSendModelData(sLeaf,sJdata)
            .IoTOnTimeout([qDevID,this](std::string msg){
                emit this->setModelDataTimeout_sig(qDevID);
            })
            .IoTError([qDevID,this](std::string msg, int err){
                QString qsMsg = QString::fromStdString(msg);
                emit this->setModelDataError_sig(qDevID,qsMsg,err);
            })
            .IoTSuccess([qDevID,this](std::string msg){
                QString qsMsg = QString::fromStdString(msg);
                emit this->setModelDataSuccess_sig(qDevID,qsMsg);
            })
            ;

}

void Widget::on_btnRecord_clicked()
{
    if(nullptr == m_pIotVideoPlayer)
    {
        return;
    }
    if(STATE_PLAY != m_pIotVideoPlayer->getPlayState())
    {
        QMessageBox::information(this,"info","player did not playing!");
        return;
    }
    if(m_pIotVideoPlayer->isRecording())
    {
        m_pIotVideoPlayer->stopRecord();
        ui->btnRecord->setText(QStringLiteral("开始录像"));
    }
    else
    {
        std::string path;
        QString qsCurrentTime = QDateTime::currentDateTime().toString("yyyyMMddhhmm");
        QString qsCurrentPath = QDir::currentPath() +"/record";
        QDir pictureDir(qsCurrentPath);
        if(!pictureDir.exists())
        {
            pictureDir.mkdir(qsCurrentPath);
        }
        path = qsCurrentPath.toStdString() +"/" + qsCurrentTime.toStdString() + ".mp4";
        m_pIotVideoPlayer->startRecord(path,[this](int err,std::string path){
            printf("record path=%s\n",path.c_str());
            QString text;
            if(0 != err)
            {
                text = QStringLiteral("录像失败，error=") + QString("%1").arg(err);
            }
            else
            {
                text = QStringLiteral("录像文件:") + QString::fromStdString(path);
            }
            emit this->showPlayerStatus_sig(text);
        });
        ui->btnRecord->setText(QStringLiteral("停止录像"));
    }
}

void Widget::on_btnScreenShot_clicked()
{
    if(nullptr == m_pIotVideoPlayer)
    {
        return;
    }
    if(STATE_PLAY != m_pIotVideoPlayer->getPlayState())
    {
        QMessageBox::information(this,"info","player did not playing!");
        return;
    }
    std::string path;
    QString qsCurrentTime = QDateTime::currentDateTime().toString("yyyyMMddhhmm");
    QString qsCurrentPath = QDir::currentPath() +"/picture";
    QDir pictureDir(qsCurrentPath);
    if(!pictureDir.exists())
    {
        pictureDir.mkdir(qsCurrentPath);
    }
    path = qsCurrentPath.toStdString() + "/"+ qsCurrentTime.toStdString() + ".jpeg";
    m_pIotVideoPlayer->snapShot(path,[this](int err, std::string path){
        QString text;
        if(0 != err)
        {
            text = QStringLiteral("截图失败，error=") + QString("%1").arg(err);
        }
        else
        {
            text = QStringLiteral("截图文件:") + QString::fromStdString(path);
        }
        emit this->showPlayerStatus_sig(text);
    });

}

void Widget::on_btnTalk_clicked()
{
    if(nullptr == m_pIotVideoPlayer)
    {
        return;
    }
    if(STATE_PLAY != m_pIotVideoPlayer->getPlayState())
    {
        QMessageBox::information(this,"info","player did not playing!");
        return;
    }

    if(m_pIotVideoPlayer->isMute())
    {
        m_pIotVideoPlayer->mute(false);
        ui->btnTalk->setText(QStringLiteral("静音"));
    }
    else
    {
        m_pIotVideoPlayer->mute(true);
        ui->btnTalk->setText(QStringLiteral("开启声音"));
    }
}
