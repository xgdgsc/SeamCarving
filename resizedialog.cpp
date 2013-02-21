#include "resizedialog.h"
#include "ui_resizedialog.h"

ResizeDialog::ResizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeDialog)
{
    ui->setupUi(this);
    connect(ui->OpenButton,SIGNAL(clicked()),this,SLOT(openImage()));
    connect(ui->ScaleButton,SIGNAL(clicked()),this,SLOT(scaleImage()));
    connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(close()));
}

ResizeDialog::~ResizeDialog()
{
    delete ui;
}

//load image
void ResizeDialog::openImage(){
    fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image Files (*.png *.jpg *.bmp *.jpeg *.tiff *.tif *.p?m *.sr *.jp2);;All Files (*.*);;BMP(*.bmp);;JPEG(*.jpg *.jpeg *.jp2);;PNG(*.png);;TIFF(*.tif *.tiff)"));

    if (!fileName.isEmpty())
      {
        origImg.load(fileName);

        ui->spinBoxHeight->setRange(1,origImg.height());
        ui->spinBoxWidth->setRange(1,origImg.width());

       ui->spinBoxHeight->setValue(origImg.height());
       ui->spinBoxWidth->setValue(origImg.width());

        imgResizer.setOrigin(imread(fileName.toStdString(),1));
       // IplImage* img=new IplImage(imgResizer.getOrigin());
        //QImage qimg=IplImage2QImage(img);
        ui->OriginalImageLabel->setPixmap(QPixmap::fromImage(origImg));
//        ui->OriginalImageLabel2->setPixmap(QPixmap::fromImage(qimg));
      }
}

//transfer the target size to the ImageResizer class and compare with normal method
void ResizeDialog::scaleImage(){
    imgResizer.seamResize(ui->spinBoxHeight->value(),ui->spinBoxWidth->value());
    showDialog.setSeamed(imgResizer.getSeamed());
    QImage normalScale=origImg.scaled(ui->spinBoxWidth->value(),ui->spinBoxHeight->value());
    normalMethodDialog.setScaled(normalScale);
}
