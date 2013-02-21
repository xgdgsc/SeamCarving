#ifndef IMAGERESIZER_H
#define IMAGERESIZER_H
#include<opencv2/opencv.hpp>
#include<vector>
#include<libs.h>
#include<fstream>

#include"show.h"

using namespace std;
using namespace cv;

//Type def for seams
typedef vector<int> Seam;

//class to handle image resizing by seam carving
class ImageResizer
{
public:
    ImageResizer();
    void calcEnergy();//calc Energy for pixels
    Seam chooseOptimalSeam();   //choose the best path for seam carving
    Mat carveCol(); //carve one column seam
    Mat carveRow(); //carve one row seam , implemented by simply transpose the Mat and apply carveCol()
    Mat seamResize(int row,int col);    //public Interface for resizing
    void setOrigin(Mat mtx);    //Set original mat
    Mat getOrigin();    //access original image
    Mat getSeamed();    //access final seamed image
    Mat getEnergy();    //DEBUG:access energy Mat
private:
    bool reset; //whether user started a new resizing job
    Mat mtx_orig;   //Mat for original image
    Mat mtx_processing; //Mat for middle manipulation
    Mat mtx_seamed; //final seamed Mat
    Mat energyMat;  //energy Matrix to store energy of pixels

};

#endif // IMAGERESIZER_H
