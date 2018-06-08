#ifndef DEALPICTURE_H
#define DEALPICTURE_H
#include "imagetomat.h"

class DealPicture
{

public:
    Mat Detection(Mat mat, int &con);
    Mat DealHand(Mat frame,int &con);
    Mat DealFire(Mat &inImg);//火焰检测
    Mat DrawFire(Mat &inputImg,Mat foreImg,int &con);
};

#endif // DEALPICTURE_H
