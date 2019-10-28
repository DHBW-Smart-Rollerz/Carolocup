#pragma once
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class StartDetection
{
public:
    bool checkQRCode();
    StartDetection();
    
private:
    Mat inputImage;
    QRCodeDetector qrDecoder;
    Mat bbox, rectifiedImage;
    std::string extractedData;
    
    const std::string STOP_TEXT = "Stop";
};

