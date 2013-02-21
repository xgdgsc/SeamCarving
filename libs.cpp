
#include"libs.h"

 IplImage* qImage2IplImage(const QImage& qImage)
{
  int width = qImage.width();
  int height = qImage.height();

  // Creates a iplImage with 3 channels
  IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
  char * imgBuffer = img->imageData;

  //Remove alpha channel
  int jump = (qImage.hasAlphaChannel()) ? 4 : 3;

  for (int y=0;y<img->height;y++){
      QByteArray a((const char*)qImage.scanLine(y), qImage.bytesPerLine());
      for (int i=0; i<a.size(); i+=jump){
          //Swap from RGB to BGR
          imgBuffer[2] = a[i];
          imgBuffer[1] = a[i+1];
          imgBuffer[0] = a[i+2];
          imgBuffer+=3;
        }
    }

  return img;
}


 QImage IplImage2QImage(const _IplImage *iplImage)
{
  int height = iplImage->height;
  int width = iplImage->width;

  if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
      const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
      QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
      return img.rgbSwapped();
    }
  else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
      const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
      QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);

      QVector<QRgb> colorTable;
      for (int i = 0; i < 256; i++){
          colorTable.push_back(qRgb(i, i, i));
        }
      img.setColorTable(colorTable);
      return img;
    }
  else{
      qWarning() << "Image cannot be converted.";
      return QImage();
    }
}

 void deleteBGchannel(Mat *mtx)
 {
   Vec3b tmp;
   Mat mtx_copy=mtx->clone();
   for(int i=0;i<mtx->rows;i++)
     for(int j=0;j<mtx->cols-5;j++)
       {
         tmp=mtx_copy.at<Vec3b>(i,j+5);
         tmp[0]=0;
         tmp[1]=0;
         mtx->at<Vec3b>(i,j)=tmp;
       }
 }

 void deleteRchannel(Mat *mtx)
 {
   Vec3b tmp;
   Mat mtx_copy=mtx->clone();
   for(int i=0;i<mtx->rows;i++)
     for(int j=5;j<mtx->cols;j++)
       {
         tmp=mtx_copy.at<Vec3b>(i,j-5);
         tmp[2]=0;
         mtx->at<Vec3b>(i,j)=tmp;
       }
 }

 float minTri(float tri[]){
     float min1=min(tri[0],tri[1]);
     float min2=min(min1,tri[2]);
     return min2;
 }

 int minTriIndex(float tri[]){
     float minValue=tri[0];
     int index=0;
     for(int i=1;i<3;i++){
         if(minValue>tri[i]){
             minValue=tri[i];
             index=i;
         }
     }
     //cout<<"minIndex:"<<index<<endl;
     return index;
 }

QImage Mat2QImage(Mat mat)
 {
     Mat rgb;
     QImage img;
     if(mat.channels() == 3)    // RGB image
     {
         cvtColor(mat,rgb,CV_BGR2RGB);
         img = QImage((const uchar*)(rgb.data),  //(const unsigned char*)
                      rgb.cols,rgb.rows,
                      rgb.cols*rgb.channels(),   //new add
                      QImage::Format_RGB888);
     }else                     // gray image
     {
         img = QImage((const uchar*)(mat.data),
                      mat.cols,mat.rows,
                      mat.cols*mat.channels(),    //new add
                      QImage::Format_Indexed8);
     }
   return img;
 }

QImage Mat2QImageBySL(Mat mtx){
    QTemporaryFile tmpImg;
    tmpImg.setFileTemplate(QString("Seam_XXXXXX.png"));
    tmpImg.open();
    imwrite(tmpImg.fileName().toStdString(),mtx);
    QImage qimg;
    qimg.load(tmpImg.fileName());
    return qimg;
}
