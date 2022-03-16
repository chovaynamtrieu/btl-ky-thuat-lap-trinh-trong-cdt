#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "QMessageBox"
#include "QFileDialog"
#include "QDebug"

//--------------ĐỊNH NGHĨA CÁC MENU---------------//

void MainWindow::on_actionAbout_triggered()
{
    QString info_text = "Xin chào.\nĐây là phần mềm phục vụ bài tập lớn\nhọc phần \"Kỹ thuật lập trình trong Cơ điện tử\"." ;
    info_text += "\n\nNhóm 8\nCác thành viên:\n\tPhạm Thanh Tùng \n\tĐoàn Ngọc Phú \n\tNguyễn Hữu Tú";
    info_text += "\n\nHà Nội, 2021\n\nSupported by Qt 5.15.2 (MSVC 2019, 64 bit)";
    QMessageBox::information(this, "Về phần mềm này", info_text);
}


void MainWindow::on_actionSave_as_triggered()
{
//    QString result_string = "Xin chào thế giới";    // sau này có hàm cập nhật kết quả rồi sửa sau
    QString result_string = "";
    QString filename = QFileDialog::getSaveFileName(this, tr("Lưu kết quả thành tập tin"), "",
    tr("Text Files (*.txt);;CSV File (*.csv);;JSON file (*.json)"));

    // Nếu chọn lưu thành file txt -> kết quả lưu dạng khác
    if(filename.contains(".txt")){
        for (int i=0;i<3;++i) result_string += color_num[i] + ": " + QString::number(current_num[i]) + "\n";
    }
    // Nếu chọn lưu thành file CSV -> kết quả lưu dạng khác
    else if (filename.contains(".csv")){
        for (int i=0;i<3;++i) result_string += color_num[i] + ',';
        result_string += "\n";
        for (int i=0;i<3;++i) result_string += QString::number(current_num[i]) + ',';
    }
    // Nếu chọn lưu thành file JSON -> kết quả lưu dạng khác
    else if (filename.contains(".json")){
        result_string = "\"Result\": {\n";
        for (int i=0;i<3;++i) result_string += "\t\"" + color_num[i] +  "\": " + QString::number(current_num[i]) + ";\n";
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


void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}


void MainWindow::on_actionSave_log_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Lưu nhật ký thành tập tin"), "",
    tr("Text Files (*.txt);;CSV File (*.csv);;JSON file (*.json)"));

    if (!filename.isEmpty()){
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this, "Warning", "Cannot save with selected name");
        } else {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << ui->textEditReceiveData->toPlainText();;
            stream.flush();
            file.close();
        }
    }
}
