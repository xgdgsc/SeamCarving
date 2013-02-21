#ifndef LIBS_H
#define LIBS_H
#include<opencv2/opencv.hpp>
#include<QImage>
#include<QDebug>
#include<QTemporaryFile>

using namespace cv;
using namespace std;

//Some libs for conversion between Qt image and OpenCV format
 QImage IplImage2QImage(const _IplImage *iplImage);
 IplImage* qImage2IplImage(const QImage& qImage);
 float minTri(float tri[]);
 int minTriIndex(float tri[]);
QImage Mat2QImage(Mat mat);

//conversion by writing to a temporary file and reading it again
QImage Mat2QImageBySL(Mat mtx);
#endif // LIBS_H
