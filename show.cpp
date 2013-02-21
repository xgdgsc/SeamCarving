#include "show.h"
#include "ui_show.h"

Show::Show(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Show)
{
    ui->setupUi(this);
    connect(ui->ButtonSave,SIGNAL(clicked()),this,SLOT(saveImage()));
    setWindowTitle("Seamed");
    ui->label->setBackgroundRole(QPalette::Base);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label->setScaledContents(true);
    //ui->scrollArea->setWidget(ui->label);
}

Show::~Show()
{
    delete ui;
}

void Show::setSeamed(Mat mtx)
{
    show();
    mtx_seamed=mtx;
    scaleMethod=seamed;
    QImage qimg=Mat2QImageBySL(mtx_seamed);
    ui->label->setPixmap(QPixmap::fromImage(qimg));
    ui->label->resize(ui->label->pixmap()->size());
}

void Show::setScaled(QImage qimg){
    show();
    setWindowTitle("Scaled");

    ui->label->setPixmap(QPixmap::fromImage(qimg));
    ui->label->resize(ui->label->pixmap()->size());
    scaledImage=qimg;
    scaleMethod=scaled;
}

void Show::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Open Image"),".",tr("Image Files (*.png *.jpg *.bmp *.jpeg *.tiff *.tif *.p?m *.sr *.jp2);;All Files (*.*);;BMP(*.bmp);;JPEG(*.jpg *.jpeg *.jp2);;PNG(*.png);;TIFF(*.tif *.tiff)"));
    if (!fileName.isEmpty())
    {
        if(scaleMethod==seamed){
            imwrite(fileName.toStdString(),mtx_seamed);
        }
        else if(scaleMethod==scaled){
            scaledImage.save(fileName);
        }

        QMessageBox qmb;qmb.setText("Saved successfully!");qmb.exec();
    }
}
