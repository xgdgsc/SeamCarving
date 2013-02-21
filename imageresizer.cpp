#include "imageresizer.h"

ImageResizer::ImageResizer()
{
    reset=true;
}

//initilize Origin matrix
void ImageResizer::setOrigin(Mat mtx){
    mtx_orig=mtx;
    mtx_processing=mtx_orig;
    reset=true;
}

Mat ImageResizer::getOrigin(){
    return mtx_orig;
}

Mat ImageResizer::getSeamed(){
    return mtx_seamed;
}

Mat ImageResizer::getEnergy(){
    return energyMat;
}

void calcGradX( const IplImage *img, IplImage *gx)
{
    int w = img->width;
    int h = img->height;
    int channel = img->nChannels;

    cvZero( gx );
    for(int i=0;i<h;i++)
        for(int j=0;j<w;j++)
            for(int c=0;c<channel;++c)
            {
                CV_IMAGE_ELEM(gx,float,i,j*channel+c) =
                    (float)CV_IMAGE_ELEM(img,uchar,i,(j+1)*channel+c) - (float)CV_IMAGE_ELEM(img,uchar,i,j*channel+c);
            }
}

void calcGradY( const IplImage *img, IplImage *gy)
{
    int w = img->width;
    int h = img->height;
    int channel = img->nChannels;

    cvZero( gy );
    for(int i=0;i<h;i++)
        for(int j=0;j<w;j++)
            for(int c=0;c<channel;++c)
            {
                CV_IMAGE_ELEM(gy,float,i,j*channel+c) =
                    (float)CV_IMAGE_ELEM(img,uchar,(i+1),j*channel+c) - (float)CV_IMAGE_ELEM(img,uchar,i,j*channel+c);

            }
}

//calc Energy for pixels
void ImageResizer::calcEnergy(){
    //TODO:Optimize effect
    Mat mtx_gray,xGradient,yGradient,xGradientAbs,yGradientAbs;
    energyMat=Mat::zeros(mtx_processing.rows,mtx_processing.cols,CV_32F);
    //GaussianBlur(mtx_processing,mtx_gray,Size(3,3),0,0);
   // cvtColor(mtx_gray,mtx_gray,CV_RGB2GRAY);
    cvtColor(mtx_processing,mtx_gray,CV_RGB2GRAY);  //convert to gray scale
    xGradient=Mat::zeros(mtx_gray.rows,mtx_gray.cols,CV_32FC1);
    yGradient=Mat::zeros(mtx_gray.rows,mtx_gray.cols,CV_32FC1);
    xGradientAbs=Mat::zeros(mtx_gray.rows,mtx_gray.cols,CV_32FC1);
    yGradientAbs=Mat::zeros(mtx_gray.rows,mtx_gray.cols,CV_32FC1);

    //calculate gradients
    //int ddepth = CV_32F;
    int ddepth = CV_16S;
    IplImage *igray=new IplImage(mtx_gray);
    IplImage *gradX = cvCreateImage(cvGetSize(igray),32,1);
    IplImage *gradY = cvCreateImage(cvGetSize(igray),32,1);
    calcGradX(igray,gradX);
    calcGradY(igray,gradY);
    xGradient=Mat(gradX);
    yGradient=Mat(gradY);
    //Sobel(mtx_gray,xGradient,ddepth, 1, 0, 3 );
    //Sobel(mtx_gray, yGradient, ddepth, 0, 1, 3);
    convertScaleAbs(xGradient,xGradientAbs);
    convertScaleAbs(yGradient,yGradientAbs);
    addWeighted(xGradientAbs,0.5,yGradientAbs,0.5,0,energyMat);

    //DEBUG
//    ofstream outfile;
//    outfile.open("energyMat.txt",ios::out);
//    outfile<<energyMat.type()<<endl;

    //    for(int i=0;i<energyMat.rows;i++){
//        for(int j=0;j<energyMat.cols;j++){
//            outfile<<energyMat.at<float>(i,j)<<' ';
//        }
//        outfile<<endl;
//    }

}

//choose the path with least energy
Seam ImageResizer::chooseOptimalSeam(){
    calcEnergy();
    Mat M=energyMat.clone();
    Mat pathMat=energyMat.clone();  //store path
   // M.zeros(energyMat.rows,energyMat.cols,CV_32FC1);
    float initialEnergy=1e6;
    float minArg[3];
    float move[]={-1,0,1};
    float minLastRowEnergy=initialEnergy;
    int minLastRowIndex=0;
    //traverse  from second row to last
    for(int i=1;i<energyMat.rows;i++){
        for(int j=0;j<energyMat.cols;j++){
            minArg[0]=(j==0?initialEnergy:M.at<uchar>(i-1,j-1));
            minArg[1]=M.at<uchar>(i-1,j);
            minArg[2]=(j==energyMat.cols-1?initialEnergy:M.at<uchar>(i-1,j+1));
            float e=energyMat.at<uchar>(i,j);
            //cout<<"type:"<<M.type();
            M.at<uchar>(i,j)=e+minTri(minArg);
            int minIndex=minTriIndex(minArg);
            pathMat.at<uchar>(i,j)=move[minIndex];
            if(i==energyMat.rows-1){
//                if(M.at<float>(i,j)<minLastRowEnergy){
//                    minLastRowIndex=j;
//                    minLastRowEnergy=M.at<float>(i,j);
//                }
                if(minArg[minIndex] < minLastRowEnergy){
                    minLastRowIndex=j;
                    minLastRowEnergy=minArg[minIndex];
                }
            }
        }
    }

    //get seam path
    Seam minSeam;
    minSeam.push_back(minLastRowIndex);
    int currentIndex=minLastRowIndex;
    for(int i=energyMat.rows-1;i>0;i--){
        int movement=(int)pathMat.at<char>(i,currentIndex);
        //cout<<i<<' '<<movement<<endl;
        currentIndex+=movement;
       // cout<<"currentIndex:"<<currentIndex<<endl;
        minSeam.push_back(currentIndex);
    }
    reverse(minSeam.begin(),minSeam.end());
    return minSeam;
}

//carve one column
Mat ImageResizer::carveCol(){
   // cout<<"carving\n";
    Seam carvedCol=chooseOptimalSeam();

//    for(unsigned int  i=0;i<carvedCol.size();i++){
//        cout<<carvedCol[i]<<endl;
//    }

    if(reset==true){
        reset=false;
        mtx_processing=mtx_orig;
    }

    //carve one column
    Mat mtx_carved=mtx_processing.colRange(0,mtx_processing.cols-1).clone();
    for(int i=0;i<mtx_carved.rows;i++){
        int seamIndex=carvedCol[i];
        bool carved=false;
        for(int j=0;j<mtx_carved.cols+1;j++){
            if(j==seamIndex){
                j++;
                carved=true;
            }
           if(carved==false){
                mtx_carved.at<Vec3b>(i,j)=mtx_processing.at<Vec3b>(i,j);
            }
           else if(carved==true){
                mtx_carved.at<Vec3b>(i,j-1)=mtx_processing.at<Vec3b>(i,j);
            }
        }
    }
    mtx_processing=mtx_carved;
    //imwrite("1.png",mtx_processing);
    return mtx_carved;
}

//carve by transpose
Mat ImageResizer::carveRow(){
    Mat tmp;
    tmp= mtx_processing.t();
    mtx_processing=tmp.clone();
    carveCol();
    tmp= mtx_processing.t();
    mtx_processing=tmp.clone();
    return mtx_processing;
}

Mat ImageResizer::seamResize(int row, int col){
    //cout<<"col"<<col<<endl;
    //cout<<"cols"<<mtx_processing.cols<<endl;
    reset=true;
    for(int i=0;i<mtx_orig.cols-col;i++){
        carveCol();
    }

    for(int i=0;i<mtx_orig.rows-row;i++){
        carveRow();
    }
    mtx_seamed=mtx_processing;
    return mtx_seamed;
}
