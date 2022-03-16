#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDateTime"
#include <QPixmap>

//--------------ĐỊNH NGHĨA CÁC CHỨC NĂNG KẾT NỐI---------------//

void MainWindow::ReadData(){
    // đọc dữ liệu gửi lên máy tính từ thiết bị
    QString message = proSerial -> Read();
    // thực hiện cập nhật kết quả
    UpdateResult(message);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonConnect_clicked()
{
    // chọn cổng
    QString port = ui->comboBoxPortName->currentText();
    int baud_rate = ui->comboBoxBaudRate->currentText().toInt();
//    std::cout << port.toStdString() << " " << baud_rate << std::endl;
    //kết nối
    bool status = proSerial->Connect(port, baud_rate);
    if (status) {
        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonDisconnect->setEnabled(true);
        ui->pushButtonRefresh->setEnabled(false);
        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ### Successfully connected");
        ui->comboBoxBaudRate->setEnabled(false);
        ui->comboBoxPortName->setEnabled(false);

        ui->labelConnectStatus->setStyleSheet("QLabel { background-color : green; color : white; font-weight: bold; font-size: 18px}");
        ui->labelConnectStatus->setText("ĐÃ KẾT NỐI");

        ui->pushButtonStartProc->setEnabled(true);
        ui->pushButtonPauseProc->setEnabled(false);
        ui->pushButtonResetProc->setEnabled(false);

        QPixmap ready_icon = QPixmap(":/new/prefix1/icon/small-ready-icon.jpg").scaled(100,100,Qt::KeepAspectRatio);
        ui->labelToReadyIcon->setPixmap(ready_icon);

    } else {
        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ###Failed to connect");
    }
}

void MainWindow::on_pushButtonDisconnect_clicked()
{
    bool status = false;
    if (! ui->pushButtonConnect->isEnabled())
        status = proSerial->Disconnect();
    //status == true if connected and then successfully disconnected
    if (status) {
        ui->pushButtonConnect->setEnabled(true);
        ui->pushButtonDisconnect->setEnabled(false);
        ui->pushButtonRefresh->setEnabled(true);
        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ### Successfully disconnected");
        ui->comboBoxBaudRate->setEnabled(true);
        ui->comboBoxPortName->setEnabled(true);

        ui->labelConnectStatus->setStyleSheet("QLabel { background-color : red; color : white; font-weight: bold; font-size: 18px}");
        ui->labelConnectStatus->setText("CHƯA KẾT NỐI");

        ui->pushButtonPauseProc->setEnabled(false);
        ui->pushButtonStartProc->setEnabled(false);
        ui->pushButtonResetProc->setEnabled(false);

        QPixmap ready_icon = QPixmap(":/new/prefix1/icon/small-not-ready-icon.png").scaled(100,100,Qt::KeepAspectRatio);
        ui->labelToReadyIcon->setPixmap(ready_icon);
    } else {
        QString current_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        ui->textEditReceiveData->append(current_time + " ### Failed to disconnect");
    }
}

void MainWindow::on_pushButtonRefresh_clicked()
{
    ui->comboBoxPortName->clear();
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
}

