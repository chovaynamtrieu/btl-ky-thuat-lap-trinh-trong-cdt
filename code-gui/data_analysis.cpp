#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QDebug"

//--------------ĐỊNH NGHĨA CÁC CHỨC NĂNG PHÂN TÍCH KẾT QUẢ---------------//

void MainWindow::on_pushButtonMax_clicked()
{
    indexMauMax=0;
    for (int i=0;i<3;++i){
        if (current_num[i]>=soVienMauMax){
            soVienMauMax=current_num[i];
            indexMauMax=i;
        }
    }
    QPixmap ready_icon = QPixmap(":/new/prefix1/icon/"+color_num[indexMauMax]+".png").scaled(100,100,Qt::KeepAspectRatio);
    ui->mauMax->setPixmap(ready_icon);
    ui->lineEditSoLuongMauMax->setText(QString::number(soVienMauMax)+" viên.");
}

void MainWindow::on_pushButtonMin_clicked()
{
    indexMauMin=0;
    soVienMauMin=current_num[0];
    for (int i=0;i<3;++i){
        if (current_num[i]<=soVienMauMin){
            soVienMauMin=current_num[i];
            indexMauMin=i;
        }
    }
    QPixmap ready_icon = QPixmap(":/new/prefix1/icon/"+color_num[indexMauMin]+".png").scaled(100,100,Qt::KeepAspectRatio);
    ui->mauMin->setPixmap(ready_icon);
    ui->lineEditSoLuongMauMin->setText(QString::number(soVienMauMin)+" viên.");
}

void MainWindow::on_pushButtonNumAll_clicked()
{
    TongLuongVien=0;
    for (int i=0;i<3;++i) TongLuongVien += current_num[i];
    ui->lineEditNumAll->setText(QString::number(TongLuongVien)+" viên.");
}

void MainWindow::on_pushButtonPhanTichAll_clicked()
{
    on_pushButtonMax_clicked();
    on_pushButtonMin_clicked();
    on_pushButtonNumAll_clicked();
}

void MainWindow::on_pushButtonLuuPhaTich_clicked()
{
    QString result_string = "";
    QString maxChange=ui->lineEditSoLuongMauMax->text();
    QString minChange=ui->lineEditSoLuongMauMin->text();
    QString allChange=ui->lineEditNumAll->text();
    QString filename = QFileDialog::getSaveFileName(this, tr("Lưu kết quả thành tập tin"), "",
    tr("Text Files (*.txt);;CSV File (*.csv);;JSON file (*.json)"));

    // Nếu chọn lưu thành file txt -> kết quả lưu dạng khác
    if(filename.contains(".txt")){
        for (int i=0;i<3;++i) result_string += color_num[i] + ": " + QString::number(current_num[i]) + "\n";
        if (maxChange!=nullptr) result_string += "Màu max : " + QString::number(soVienMauMax) + '\n';
        if (minChange!=nullptr) result_string += "Màu min : " + QString::number(soVienMauMin) + '\n';
        if (allChange!=nullptr) result_string += "Tổng cộng : "+ QString::number(TongLuongVien) + '\n';

    }
    // Nếu chọn lưu thành file CSV -> kết quả lưu dạng khác
    else if (filename.contains(".csv")){
        for (int i=0;i<3;++i) result_string += color_num[i] + ',';
        if (maxChange!=nullptr) result_string += "max,";
        if (minChange!=nullptr) result_string += "min,";
        if (allChange!=nullptr) result_string += "all";
        result_string += "\n";
        for (int i=0;i<3;++i) result_string += QString::number(current_num[i]) + ',';
        if (maxChange!=nullptr) result_string += QString::number(soVienMauMax)+",";
        if (minChange!=nullptr) result_string += QString::number(soVienMauMin)+",";
        if (allChange!=nullptr) result_string += QString::number(TongLuongVien);
    }
    // Nếu chọn lưu thành file JSON -> kết quả lưu dạng khác
    else if (filename.contains(".json")){
        result_string = "\"Result\": {\n";
        for (int i=0;i<3;++i) result_string += "\t\"" + color_num[i] +  "\": " + QString::number(current_num[i]) + ";\n";
        result_string += "},\n";

        result_string += "\"phan_tich\": {\n";
        result_string += "\t\"max\": " + QString::number(soVienMauMax) + ";\n";
        result_string += "\t\"min\": " + QString::number(soVienMauMin) + ";\n";
        result_string += "\t\"all\": " + QString::number(TongLuongVien) + ";\n";
        result_string += "},";
    }

    if (!filename.isEmpty()){
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this, "Warning", "Cannot save with selected name");
        } else {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << result_string;
            stream.flush();
            file.close();
        }
    }
}
