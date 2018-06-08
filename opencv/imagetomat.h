#ifndef IMAGETOMAT_H
#define IMAGETOMAT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <QImage>
#include <iostream>
using namespace cv;


class ImageToMat
{
public:

    Mat imageToMat(const QImage &inImage, bool inCloneImageData = true);

};

#endif // IMAGETOMAT_H
