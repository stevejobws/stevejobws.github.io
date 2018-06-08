#include "dealpicture.h"
#include <QDebug>
Mat DealPicture::Detection(Mat mat , int &con)
{

    vector<Rect> found, foundRect;
    HOGDescriptor defaultHog;
    //1.设置SVM，默认的可能不一定适用于各种场所，可自行根据场景训练以提高准确率
    defaultHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

        //2.调整图像大小，越大检测越慢准确率越高，可自行调整
        cv::resize(mat, mat, Size(768, 768*mat.rows/mat.cols), INTER_LINEAR);
        double duration = static_cast<double>(getTickCount());
        defaultHog.detectMultiScale(mat, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
        duration = static_cast<double>(getTickCount()) - duration;
        duration /= getTickFrequency();
        for(int i = 0; i < found.size(); i++){
            Rect r = found[i];

            int j = 0;
            for(; j < found.size(); j++){
                //如果是嵌套的就退出循环
                if( j != i && (r & found[j]) == r)
                    break;
            }
            if(j == found.size()){
                foundRect.push_back(r);
            }
        }

        //画长方形，圈出行人
        for(int i = 0; i < foundRect.size(); i++){
            Rect r = foundRect[i];
            //检测出来的框框稍大，缩小一点
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.07);
            r.height = cvRound(r.height*0.8);
            rectangle(mat, r.tl(), r.br(), Scalar(0, 0, 255), 3);
            qDebug()<<"家里有人请注意";          
             con =1;         
        }
        foundRect.clear();
        //waitKey(50);

    return mat;


}

Mat DealPicture::DealHand(Mat frame, int &con)
{
    bool lastImgHasHand = false;

    int previousX = 0;
    int previousY = 0;
    Mat frameHSV;   // hsv空间
    Mat mask(frame.rows, frame.cols, CV_8UC1);  // 2值掩膜
    Mat dst(frame); // 输出图像

    vector< vector<Point> > contours;   // 轮廓
    vector< vector<Point> > filterContours; // 筛选后的轮廓
    vector< Vec4i > hierarchy;    // 轮廓的结构信息
    vector< Point > hull; // 凸包络的点集

    int count = 0;

    int presentX = 0;
    int presentY = 0;

    int minX = 320;//屏幕的一半
    int maxX = 240;
    int minY = 320;
    int maxY = 240;

    // 中值滤波，去除椒盐噪声
    medianBlur(frame, frame, 5);
    cvtColor(frame, frameHSV, CV_BGR2HSV);

    Mat dstTemp1(frame.rows, frame.cols, CV_8UC1);
    Mat dstTemp2(frame.rows, frame.cols, CV_8UC1);
    // 对HSV空间进行量化，得到2值图像，亮的部分为手的形状
    inRange(frameHSV, Scalar(0, 30, 30), Scalar(40, 170, 256), dstTemp1);
    inRange(frameHSV, Scalar(156, 30, 30), Scalar(180, 170, 256), dstTemp2);
    bitwise_or(dstTemp1, dstTemp2, mask);
    //  inRange(frameHSV, Scalar(0,30,30), Scalar(180,170,256), dst);

    // 形态学操作，去除噪声，并使手的边界更加清晰
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(mask, mask, element);
    morphologyEx(mask, mask, MORPH_OPEN, element);
    dilate(mask, mask, element);
    morphologyEx(mask, mask, MORPH_CLOSE, element);
    frame.copyTo(dst, mask);
    contours.clear();
    hierarchy.clear();
    filterContours.clear();
    // 得到手的轮廓
    findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    // 去除伪轮廓

    for (size_t i = 0; i < contours.size(); i++)
    {
        //  approxPolyDP(Mat(contours[i]), Mat(approxContours[i]), arcLength(Mat(contours[i]), true)*0.02, true);
        if (fabs(contourArea(Mat(contours[i]))) > 30000) //判断手进入区域的阈值
        {
            filterContours.push_back(contours[i]);
        }
    }
    // 画轮廓
    if (filterContours.size()>0)
    {

        count++;
        lastImgHasHand = true;
        drawContours(dst, filterContours, -1, Scalar(255, 0, 255), 3/*, 8, hierarchy*/);

        for (size_t j = 0; j<filterContours.size(); j++)
        {
            convexHull(Mat(filterContours[j]), hull, true);
            int hullcount = (int)hull.size();
            for (int i = 0; i<hullcount - 1; i++)
            {
                line(dst, hull[i + 1], hull[i], Scalar(255, 255, 255), 2, CV_AA);//白色
                printf("num%d:x=%d\ty=%d\t\n", i, hull[i].x, hull[i].y);
                if (hull[i].x>maxX)
                    maxX = hull[i].x;
                if (hull[i].x<minX)
                    minX = hull[i].x;
                if (hull[i].y>maxY)
                    maxY = hull[i].y;
                if (hull[i].y<minY)
                    minY = hull[i].y;
                printf("miniX=%d\tminiY=%d\tmaxX=%d\tmaxY=%d\t\n", minX, minY, maxX, maxY);

            }

            line(dst, hull[hullcount - 1], hull[0], Scalar(0, 255, 0), 2, CV_AA);//绿色，最后一条

            if (count == 1)//第一个轮廓的中心位置存在全局变量中，到最后一个再跟它比。
            {
                previousX = (minX + maxX) / 2;
                //printf(" minX=%d\n,maxX=%d\n,previousX=%d\n",minX,maxX, previousX);
                previousY = (minY + maxY) / 2;
               // printf("previousY=%d\n", previousY);
            }
            else
            {
                presentX = (minX + maxY) / 2;
                printf("presentX=%d",presentX);
                presentY = (minY + maxY) / 2;
                printf("presentY=%d",presentY);
            }
        }
    }
    else
    {
        if (lastImgHasHand == true)
        {
            count = 0;
            lastImgHasHand = false;
        }
    }

    printf("previousX=%d\tpresentX=%d\tpreviousY=%d\tpresentY=%d\n", previousX, presentX, previousY, presentY);
    printf("count=%d\n", count);
    if(previousY>220&&previousY<300)
    {
        con = 1;
        //qDebug()<<"11111111111111111111111111111111111111";
    }
    if(previousY>300&&previousY<350)
    {
        con = 0;
        //qDebug()<<"222222222222222222222222222222222222222";
    }

    return dst;

}

Mat DealPicture::DealFire(Mat &inImg)
{
    Mat fireImg,fireImg_src;
    fireImg_src=inImg;
    fireImg.create(inImg.size(),CV_8UC1);

    int redThre=115;
    int saturationTh=55;
    //Mat multiRGB[3];
    vector<Mat> mv;

    split(fireImg_src,mv);

    for(int i=0;i<fireImg_src.rows;i++)
    {
        for(int j=0;j < fireImg_src.cols; j ++)
        {
            float B,G,R;
            B = mv[0].at<uchar>(i,j); //每个像素的R,G,B值
            G = mv[1].at<uchar>(i,j);
            R = mv[2].at<uchar>(i,j);

            int minValue=min(min(B,G),R);

            double S = (1-3.0*minValue/(R+G+B));

            if(R>redThre&&R>=G&&G>=B&&S>0.20&&S>((255-R)*saturationTh/redThre))
            {
                fireImg.at<uchar>(i,j)=255;
            }
            else
            {
                fireImg.at<uchar>(i,j)=0;
            }
        }
    }


    dilate(fireImg,fireImg,Mat(5,5,CV_8UC1));


    return fireImg;
}

Mat DealPicture::DrawFire(Mat &inputImg, Mat foreImg,int &con)
{
    vector<vector<Point>> contours_set;//保存轮廓提取后的点集及拓扑关系

    findContours(foreImg,contours_set,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    /*
    Mat result0;
    Scalar holeColor;
    Scalar externalColor;
    */

    vector<vector<Point> >::iterator iter = contours_set.begin() ;
    for(; iter!= contours_set.end(); )
    {
        Rect rect = boundingRect(*iter );
        float radius;
        Point2f center;
        minEnclosingCircle(*iter,center,radius);

        if (rect.area()> 0)
        {

            rectangle(inputImg,rect,Scalar(0,255,0));
            ++ iter;
            con = 1;
        }
        else
        {
            iter = contours_set.erase(iter);
            con = 0;
        }
    }
    return inputImg;
}
