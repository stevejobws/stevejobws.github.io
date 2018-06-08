#include "imagetomat.h"
#include <QDebug>

Mat ImageToMat::imageToMat(const QImage &inImage, bool inCloneImageData)
{
	
    switch ( inImage.format() )
          {
             // 8-bit, 4 channel
             case QImage::Format_ARGB32:
             case QImage::Format_ARGB32_Premultiplied:
             {
               Mat  mat( inImage.height(), inImage.width(),
                              CV_8UC4,
                              const_cast<uchar*>(inImage.bits()),
                              static_cast<size_t>(inImage.bytesPerLine())
                              );

                return (inCloneImageData ? mat.clone() : mat);
             }

            
             case QImage::Format_RGB32:
             case QImage::Format_RGB888:
             {
                if ( !inCloneImageData )
                {
                   qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
                }

                QImage   swapped = inImage;

                if ( inImage.format() == QImage::Format_RGB32 )
                {
                   swapped = swapped.convertToFormat( QImage::Format_RGB888 );
                }

                swapped = swapped.rgbSwapped();

                return Mat( swapped.height(), swapped.width(),
                                CV_8UC3,
                                const_cast<uchar*>(swapped.bits()),
                                static_cast<size_t>(swapped.bytesPerLine())
                                ).clone();
             }

             // 8-bit, 1 channel
             case QImage::Format_Indexed8:
             {
                Mat  mat( inImage.height(), inImage.width(),
                              CV_8UC1,
                              const_cast<uchar*>(inImage.bits()),
                              static_cast<size_t>(inImage.bytesPerLine())
                              );

                return (inCloneImageData ? mat.clone() : mat);
             }

             default:
                qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
                break;
          }

          return Mat();
}
