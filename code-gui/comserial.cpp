#include "comserial.h"

ComSerial::ComSerial(QSerialPort *myDevice)
{
    deviceSerial = myDevice;
}

QStringList ComSerial::connectedDevices()
{
    QStringList devices;
    foreach (const QSerialPortInfo infor, QSerialPortInfo::availablePorts()) {
        deviceSerial->setPort(infor);
        //kiểm tra xem cổng có cho phép truyền/nhận dữ liệu không
        if(deviceSerial->open(QIODevice::ReadWrite))
        {
            deviceSerial->close();//đóng và lưu cổng vào danh sách
            devices << infor.portName();
        }
    }

    return devices;
}

bool ComSerial::Connect(QString Port, uint64_t baud_rate)
{
    //thiết lập cổng
    deviceSerial->setPortName(Port);
    //thiết lập tốc độ truyền
    switch (baud_rate) {
        case 2400:
            deviceSerial->setBaudRate(QSerialPort::Baud2400);
        break;
        case 4800:
            deviceSerial->setBaudRate(QSerialPort::Baud4800);
        break;
        case 9600:
            deviceSerial->setBaudRate(QSerialPort::Baud9600);
        break;
        case 19200:
            deviceSerial->setBaudRate(QSerialPort::Baud19200);
        break;
        case 115200:
            deviceSerial->setBaudRate(QSerialPort::Baud115200);
        break;
    }
    //thiết lập Flow Control
    deviceSerial->setFlowControl(QSerialPort::NoFlowControl);
    //thiết lập số lượng bit dữ liệu
    deviceSerial->setDataBits(QSerialPort::Data8);
    //thiết lập bit chẵn lẻ
    deviceSerial->setParity(QSerialPort::NoParity);
    //thiết lập bit dừng
    deviceSerial->setStopBits(QSerialPort::OneStop);
    //kiểm tra xem có mở được cổng không
    if(deviceSerial->open(QIODevice::ReadWrite))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ComSerial::Disconnect(void)
{
    deviceSerial->clear();//xóa toàn bộ bộ nhớ đệm vào/ra
    deviceSerial->close();//đóng cổng
    //kiểm tra
    if(deviceSerial->error() == 0 || !deviceSerial->isOpen())
    {
        return true;
    }
    else
        return false;
}

qint64 ComSerial::Write(const char* message)
{
    qint64 sizeWritten = deviceSerial->write(message, qstrlen(message));
    return sizeWritten;
}

QString ComSerial::Read()
{
    QByteArray data=NULL;
    if (deviceSerial->canReadLine()){
        data = deviceSerial->readAll();
    }
    QString bufferInSerial=QString(data);

    return bufferInSerial;
}

QString ComSerial::Read(int bufferSize)
{
    char* buf = new char[bufferSize];
    if(deviceSerial->canReadLine())
    {
        deviceSerial->read(buf, bufferSize);
    }

    //chuyển dữ liệu đọc được về dạng string
    QString str = QString::fromUtf8(buf, bufferSize);
    delete [] buf;

    return str;
}
