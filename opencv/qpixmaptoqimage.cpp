#include "qpixmaptoqimage.h"


QImage QPixmapToQImage::qpixmaptoqImage(QPixmap map)
{
   QImage image = map.toImage();
   return image;
}
