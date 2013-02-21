#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>
#include<QFileDialog>
#include"imageresizer.h"
#include"libs.h"

namespace Ui {
class ResizeDialog;
}


//UI dialog class
class ResizeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResizeDialog(QWidget *parent = 0);
    ~ResizeDialog();
    
    ImageResizer imgResizer;
public slots:
    void openImage();
    void scaleImage();
private:
    QString fileName;
    QImage origImg;
    Ui::ResizeDialog *ui;
    Show showDialog;
    Show normalMethodDialog;
};

#endif // RESIZEDIALOG_H
