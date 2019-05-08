#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)//Ui::MainWindow是Forms/mainwindow.ui产生的类
{
    ui->setupUi(this);

    camera=new QCamera(this);
    viewfinder=new QCameraViewfinder(this);//实时读取摄像头
    imageCapture=new QCameraImageCapture(camera);

    ui->ImageView->addWidget(viewfinder);//ImageView是Horizental Layout的一个对象
    ui->ImageCapture->setScaledContents(true);//ImageCapture是Label的一个对象,设置Label自动缩放

    camera->setViewfinder(viewfinder);
    camera->start();

    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));

    connect(ui->buttonCapture, SIGNAL(clicked()), this, SLOT(captureImage()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveImage()));
    connect(ui->buttonQuit, SIGNAL(clicked()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::captureImage()
{
    ui->statusBar->showMessage(tr("capturing..."), 1000);
    imageCapture->capture();
}

void MainWindow::displayImage(int , QImage image)
{
    ui->ImageCapture->setPixmap(QPixmap::fromImage(image));

    ui->statusBar->showMessage(tr("capture OK!"), 5000);
}

void MainWindow::saveImage()
{
    QString fileName=QFileDialog::getSaveFileName(this, tr("save file"), QDir::homePath(), tr("jpegfile(*.jpg)"));
    if(fileName.isEmpty()) {
        ui->statusBar->showMessage(tr("save cancel"), 5000);
        return;
    }
    const QPixmap* pixmap=ui->ImageCapture->pixmap();
    if(pixmap) {
        pixmap->save(fileName);
        ui->statusBar->showMessage(tr("save OK"), 5000);
    }
}
