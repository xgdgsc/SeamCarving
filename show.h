#ifndef SHOW_H
#define SHOW_H

#include <QDialog>
#include<QFileDialog>
#include<opencv2/opencv.hpp>
#include<QImage>
#include<QDebug>
#include<QMessageBox>
#include"libs.h"
#include<QTemporaryFile>

using namespace cv;

namespace Ui {
  class Show;
}

//class for showing the result
class Show : public QDialog
{
  Q_OBJECT

public:
  explicit Show(QWidget *parent = 0);
  ~Show();
  void setSeamed(Mat mtx);  //seam carving method
  void setScaled(QImage qimg);  //normal method
public slots:
  void saveImage();
private:
  enum methodType{scaled,seamed};
  methodType scaleMethod;
  QImage scaledImage;
  Mat mtx_seamed;
  Ui::Show *ui;
};

#endif // SHOW_H
