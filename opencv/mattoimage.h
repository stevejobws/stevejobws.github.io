#ifndef MATTOIMAGE_H
#define MATTOIMAGE_H
#include <QImage>
#include "dealpicture.h"
using namespace cv;
class mattoimage
{
public:
  QImage cvMatTOImage(const Mat &inMat);

};

#endif // MATTOIMAGE_H
