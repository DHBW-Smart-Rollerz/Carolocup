#pragma once
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



class StartboxDetector
{
public:
    bool checkQRCode(cv::Mat inputImage);
    StartboxDetector();
    
private:
    cv::Mat inputImage;
    cv::QRCodeDetector qrDecoder;
    cv::Mat bbox, rectifiedImage;
    std::string extractedData;
    
    const std::string STOP_TEXT = "STOP";
};

