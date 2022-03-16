#include "mainwindow.h"
#include "opencv2/videoio.hpp"
#include "utilities.h"
#include <QCameraInfo>

void MainWindow::Camera_Information()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    QString info = QString("Available Cameras: \n");

    foreach (const QCameraInfo &cameraInfo, cameras) {
        info += "  - " + cameraInfo.deviceName() +  ": ";
        info += cameraInfo.description() + "\n";
        ui->comboBoxCamera->addItem(cameraInfo.description());
    }
    //QMessageBox::information(this, "Cameras", info);
}

void MainWindow::on_actionE_xit_triggered()
{
    QApplication::quit();
}

void MainWindow::updateFrame(cv::Mat *mat)
{
    int width = 0, height = 0;
    width = ui->imageView->width();
    height = ui->imageView->height();
    data_lock->lock();
    currentFrame = *mat;
    showFrame = currentFrame.clone();
    cv::resize(showFrame, showFrame, cv::Size(width, height));
    data_lock->unlock();

    QImage frame(
        showFrame.data,
        showFrame.cols,
        showFrame.rows,
        showFrame.step,
        QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);

    imageScene->clear();
    ui->imageView->resetMatrix();
    imageScene->addPixmap(image);
    imageScene->update();
    ui->imageView->setSceneRect(image.rect());
}

void MainWindow::updateFPS(float fps)
{
    ui->statusbar->showMessage(QString("FPS of current camera is %1").arg(fps));
}

void MainWindow::on_action_Open_Camera_triggered()
{
    if(capturer != nullptr) {
        // if a thread is already running, stop it
        capturer->setRunning(false);
        disconnect(capturer, &CaptureThread::frameCaptured, this, &MainWindow::updateFrame);
        disconnect(capturer, &CaptureThread::fpsChanged, this, &MainWindow::updateFPS);
        connect(capturer, &CaptureThread::finished, capturer, &CaptureThread::deleteLater);
    }

    // Usually, the Index of the first camera is 0.
    int camID = ui->comboBoxCamera->currentIndex();
    capturer = new CaptureThread(camID, data_lock);
    connect(capturer, &CaptureThread::frameCaptured, this, &MainWindow::updateFrame);
    connect(capturer, &CaptureThread::fpsChanged, this, &MainWindow::updateFPS);
    capturer->start();
    ui->statusbar->showMessage(QString("Capturing Camera %1").arg(camID));
}

void MainWindow::on_action_Calculate_FPS_triggered()
{
    if(capturer != nullptr) {
            capturer->startCalcFPS();
        }
}

void MainWindow::on_pushButtonStart_clicked()
{
    capturer->setImageProcessingStatus(true);
}

void MainWindow::on_pushButtonOpenCamera_clicked()
{
    on_action_Open_Camera_triggered();
}

