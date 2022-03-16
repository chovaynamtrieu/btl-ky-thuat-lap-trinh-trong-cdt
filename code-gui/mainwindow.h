#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "comserial.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMutex>
#include <QStandardItemModel>
#include <QTimer>

//#include "opencv2/opencv.hpp"
//#include "capture_thread.h"
//#include "ui_mainwindow.h"

#include <QTextToSpeech>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonConnect_clicked();

    // hàm đọc dữ liệu từ thiết bị
    void ReadData();

    void on_pushButtonDisconnect_clicked();

    void on_pushButtonRefresh_clicked();

private:
    Ui::MainWindow *ui;

    // biến để thực hiện việc giao tiếp với thiết bị
    QSerialPort *deviceSerial;
    ComSerial *proSerial;

private slots:
//    void on_actionE_xit_triggered();

//    void on_action_Open_Camera_triggered();
//    void updateFrame(cv::Mat*);
//    void updateFPS(float);
//    void on_action_Calculate_FPS_triggered();
//    void on_pushButtonStart_clicked();
//    void on_pushButtonOpenCamera_clicked();

    void on_actionAbout_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();
    void on_pushButtonStartProc_clicked();
    void on_pushButtonPauseProc_clicked();


    void on_pushButtonResetProc_clicked();

    void on_actionSave_log_as_triggered();

//private:
//    void Camera_Information();

//private:
//    QGraphicsScene *imageScene;
//    QStandardItemModel *list_model;
//    cv::Mat currentFrame, showFrame;

//    // for capture thread
//    QMutex *data_lock;
//    CaptureThread *capturer;

    // for speech
    void on_pushButtonMax_clicked();

    void on_pushButtonMin_clicked();

    void on_pushButtonNumAll_clicked();

    void on_pushButtonPhanTichAll_clicked();

    void on_pushButtonLuuPhaTich_clicked();

public slots:
        void speak();
        void stop_speak();

        void setRate(int);
        void setPitch(int);
        void setVolume(int volume);

        void stateChanged(QTextToSpeech::State state);
        void engineSelected(int index);
        void languageSelected(int language);
        void voiceSelected(int index);

        void localeChanged(const QLocale &locale);

private:
        QTextToSpeech *m_speech;
        QVector<QVoice> m_voices;
public:
        void UpdateResult(const QString&);
private:
        int current_num[3] = {0,0,0}, soVienMauMin=0, soVienMauMax=0, TongLuongVien=0;
        int indexMauMin=0, indexMauMax=0;
        QString color_num[3] = {"Red", "White", "Black"};
        QString may_chay = "B";//B: bắt đầu, D là dừng lại, L là khởi động lại.
        QString mode;//mode == 0 chế độ mặc định, 1 là chế độ 1, 2 là chế độ 2.
};
#endif // MAINWINDOW_H
