#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include <QCameraInfo>
#include <QGridLayout>
#include <QIcon>
#include <QStandardItem>
#include <QSize>
#include <QDateTime>
#include "string"

#include "connect_function.cpp"
#include "speech_function.cpp"
#include "menu_action.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_speech(nullptr)                       // KHỞI TẠO WINDOW
{
    ui->setupUi(this);

    //---------SETUP CHỨC NĂNG KẾT NỐI----------//

    //tạo (con trỏ tới) object mới của các lớp QSerialPort, ComSerial
    deviceSerial = new QSerialPort(this);

    proSerial = new ComSerial(deviceSerial);


    // tìm danh sách các cổng có kết nối thiết bị IO
    QStringList listSerial = proSerial->connectedDevices();

    // hiển thị danh sách trong  comboBoxPortName
    ui->comboBoxPortName->addItems(listSerial);

    // hiển thị kết quả trong textEdit
    if (listSerial.length()>0){
        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ### Serial port is ready for use");
    } else {
        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ### No serial port was detected");
    }

    // kết nối tín hiệu readyRead của QSerialPort và slot
    connect(deviceSerial, SIGNAL(readyRead()), this, SLOT(ReadData()));

    ui->labelConnectStatus->setStyleSheet("QLabel { background-color : red; color : white; font-weight: bold; font-size: 18px}");
    ui->pushButtonDisconnect->setEnabled(false);

    //---------SETUP CHỨC NĂNG GIỌNG NÓI----------//

    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));


    ui->engineComboBox->addItem("Default", QString("default"));
    const auto engines = QTextToSpeech::availableEngines();
    for (const QString &engine : engines)
        ui->engineComboBox->addItem(engine, engine);
    ui->engineComboBox->setCurrentIndex(0);
    engineSelected(0);

    connect(ui->speakButton, &QPushButton::clicked, this, &MainWindow::speak);
    connect(ui->pitchSlider, &QSlider::valueChanged, this, &MainWindow::setPitch);
    connect(ui->rateSlider, &QSlider::valueChanged, this, &MainWindow::setRate);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::setVolume);
    connect(ui->engineComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::engineSelected);

    //---------TRẠNG THÁI MẶC ĐỊNH CỦA CÁC NÚT START, PAUSE, RESET----------//

    ui->pushButtonPauseProc->setEnabled(false);
    ui->pushButtonStartProc->setEnabled(false);
    ui->pushButtonResetProc->setEnabled(false);

    //---------ĐỔI VỊ TRÍ TOOLBAR----------//
    removeToolBar(ui->toolBar);
    addToolBar(Qt::LeftToolBarArea, ui->toolBar);
    ui->toolBar->show();

    //---------KHỞI TẠO CÁC ICON----------//
    QPixmap prog_icon = QPixmap(":/new/prefix1/icon/colors.png").scaled(50,50, Qt::KeepAspectRatio);
    ui->labelProgramIcon->setPixmap(prog_icon);

    QPixmap ready_icon = QPixmap(":/new/prefix1/icon/small-not-ready-icon.png").scaled(100,100,Qt::KeepAspectRatio);
    ui->labelToReadyIcon->setPixmap(ready_icon);

}


void MainWindow::UpdateResult(const QString& raw_result){   // CẬP NHẬT KẾT QUẢ
    if (raw_result.isEmpty()) return;  // nếu không nhận được gì từ Arduino --> thoát luôn

    //  xâu này có dạng gì cũng được, miễn là chứa 3 số theo thứ tự r,g,y
    //  raw_result = "r100g23y3e";

    // phân tích xâu để ra được 3 biến kết quả
    int new_num[3];
    const QRegExp rx(QLatin1Literal("[^0-9]+"));
    const auto&& parts = raw_result.split(rx, QString::SkipEmptyParts);
    // đây là mảng hai chiều vì là mảng của các QString (1 QString cũng là 1 mảng)
    for (int i=0;i<3;++i) new_num[i] = parts[i].toInt();

    // sau đó check xem có giống với 3 biến kết quả đang lưu trong MainWindow không
    bool isChanged = false;
    int indexChanged = 4;
    for (int i=0;i<3;++i) if (new_num[i] != current_num[i]) {isChanged = true; indexChanged = i; break;}

    // nếu có sự thay đổi thì: cập nhật các label trên màn hình và đọc ra loa
    if (isChanged){
        ui->plainRedNum->setPlainText(parts[0]);
        ui->plainWhiteNum->setPlainText(parts[1]);
        ui->plainBlackNum->setPlainText(parts[2]);
        QString speech_string = "The number of " + color_num[indexChanged] + " is " + parts[indexChanged];
        m_speech->say(speech_string);

        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ### " + speech_string);
    }

    // cập nhật cho các biến lưu trữ kết quả trong MainWindow
    for (int i=0;i<3;++i) current_num[i]=new_num[i];

    // xét các chế độ dừng
    if(ui->radioButtonDefault->isChecked()){             // nếu ở chế độ 1: tiếp tục chạy
    proSerial->Write(may_chay.toUtf8());
    }
    else if (ui->radioButtonMode2->isChecked()) {         // nếu ở chế độ 3: kiểm tra tổng số viên đã thu thập
        int soVienMuonLay= ui->lineEditMode2->text().toInt();
        //cap nhat all
        TongLuongVien=0;
        for (int i=0;i<3;++i) TongLuongVien += current_num[i];
        if (TongLuongVien<soVienMuonLay){
        proSerial->Write(may_chay.toUtf8());}
        else {
            on_pushButtonPauseProc_clicked();
            ui->radioButtonDefault->setEnabled(true);
            ui->radioButtonMode1->setEnabled(true);
            ui->radioButtonMode2->setEnabled (true);
            ui->lineEditMode1->setEnabled (true);
            ui->lineEditMode2->setEnabled (true);
            ui->comboBoxMode1->setEnabled (true);
        }
    }
    else if (ui->radioButtonMode1->isChecked()) {          // nếu ở chế độ 2: kiểm tra số viên của màu mong muốn
        int soVienMuonLay= ui->lineEditMode1->text().toInt();
        QString mauMuonLay = ui->comboBoxMode1->currentText();
        int indexMau=0;
        for (int i=0;i<3;++i){
            if (color_num[i]==mauMuonLay){
                indexMau=i; break;
            }
        }
        if (current_num[indexMau]<soVienMuonLay){
        proSerial->Write(may_chay.toUtf8());}
        else {
            on_pushButtonPauseProc_clicked();
            ui->radioButtonDefault->setEnabled(true);
            ui->radioButtonMode1->setEnabled(true);
            ui->radioButtonMode2->setEnabled (true);
            ui->lineEditMode1->setEnabled (true);
            ui->lineEditMode2->setEnabled (true);
            ui->comboBoxMode1->setEnabled (true);
        }
    }

}

void MainWindow::on_pushButtonStartProc_clicked()     // nhấn nút Start
{
    may_chay = "B";  // B viết tắt của 'bật'
    proSerial->Write(may_chay.toUtf8());

    ui->pushButtonStartProc->setEnabled(false);
    ui->pushButtonPauseProc->setEnabled(true);
    ui->pushButtonResetProc->setEnabled(false);
    ui->pushButtonMax->setEnabled(false);
    ui->pushButtonMin->setEnabled(false);
    ui->pushButtonNumAll->setEnabled(false);
    ui->pushButtonPhanTichAll->setEnabled(false);
    ui->pushButtonLuuPhaTich->setEnabled(false);
    ui->radioButtonDefault->setEnabled(false);
    ui->radioButtonMode1->setEnabled(false);
    ui->radioButtonMode2->setEnabled (false);
    ui->lineEditMode1->setEnabled (false);
    ui->lineEditMode2->setEnabled (false);
    ui->comboBoxMode1->setEnabled (false);

    QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    ui->textEditReceiveData->append(current_time + " ### The process was started");

}

void MainWindow::on_pushButtonPauseProc_clicked()       // nhấn nút Pause
{
    may_chay = "D"; // D viết tắt của 'dừng'
    proSerial->Write(may_chay.toUtf8());

    ui->pushButtonStartProc->setEnabled(true);
    ui->pushButtonPauseProc->setEnabled(false);
    ui->pushButtonResetProc->setEnabled(true);
    ui->pushButtonMax->setEnabled(true);
    ui->pushButtonMin->setEnabled(true);
    ui->pushButtonNumAll->setEnabled(true);
    ui->pushButtonPhanTichAll->setEnabled(true);
    ui->pushButtonLuuPhaTich->setEnabled(true);

    QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    ui->textEditReceiveData->append(current_time + " ### The process was paused");
}

void MainWindow::on_pushButtonResetProc_clicked()     // nhấn nút Reset
{
    // cập nhật lại số liệu lưu trữ + số liệu hiển thị
    soVienMauMax=0;
    soVienMauMin=0;
    TongLuongVien=0;
    /*for (int i=0;i<3;++i) current_num[i]=0;
    ui->plainRedNum->setPlainText("0");
    ui->plainWhiteNum->setPlainText("0");
    ui->plainBlackNum->setPlainText("0");*/

    // thông báo ra loa
    QString speech_string = " The results were reset";
    m_speech->say(speech_string);

    // nút Reset biến mất

    ui->pushButtonStartProc->setEnabled(true);
    ui->pushButtonPauseProc->setEnabled(false);
    ui->pushButtonResetProc->setEnabled(false);
    ui->pushButtonMax->setEnabled(false);
    ui->pushButtonMin->setEnabled(false);
    ui->pushButtonNumAll->setEnabled(false);
    ui->pushButtonPhanTichAll->setEnabled(false);
    ui->pushButtonLuuPhaTich->setEnabled(false);
    ui->lineEditSoLuongMauMin->setText("");
    ui->lineEditSoLuongMauMax->setText("");
    ui->lineEditNumAll->setText("");
    ui->radioButtonDefault->setEnabled(true);
    ui->radioButtonMode1->setEnabled(true);
    ui->radioButtonMode2->setEnabled (true);
    ui->lineEditMode1->setEnabled (true);
    ui->lineEditMode2->setEnabled (true);
    ui->comboBoxMode1->setEnabled (true);

    // khó khăn: kết quả vẫn được arduino lưu :))))
    may_chay = "L"; // L viết tắt của 'lại'
    proSerial->Write(may_chay.toUtf8());
    may_chay = "D";

    QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ###");
    ui->textEditReceiveData->append(current_time + speech_string);
}
