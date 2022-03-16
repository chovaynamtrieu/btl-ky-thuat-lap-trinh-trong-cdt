#ifndef COMSERIAL_H
#define COMSERIAL_H

#include <QSerialPort>
#include <QSerialPortInfo>

class ComSerial
{
protected:
    QSerialPort *deviceSerial;//thuộc tính

public:
    ComSerial(QSerialPort *myDevice);//hàm tạo
    ~ComSerial();//hàm hủy
    QStringList connectedDevices();//tìm các thiết bị kết nối với máy tính qua cổng nối tiếp
    bool Connect(QString Port, uint64_t baud_rate);//kết nối với thiết bị qua cổng Port với tốc độ truyền = baud_rate
    bool Disconnect(void);//ngắt kết nối thiết bị

    qint64 Write(const char* message);//gửi dữ liệu "message" tới phần cứng
    QString Read();//đọc dữ liệu gửi lên từ phần cứng
    QString Read(int bufferSize);//đọc dữ liệu gửi lên từ phần cứng với kích thước = bufferSize
};

#endif // COMSERIAL_H
