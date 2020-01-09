#pragma once
#include "LaneDetector.h"
#include <opencv2/core/cuda.hpp>

#include <opencv2/cudaimgproc.hpp>
class PointLaneDetector :
	public LaneDetector
{
public:
	PointLaneDetector();
	virtual void calculateFrame(cv::Mat) override;
	virtual VisionResult getResult() override;

private:
	bool calculateAlgorithm(cv::Mat&, int startLine);
	void laneMiddlePoints(std::vector<cv::Point>&, cv::Mat, int);
	void calculateSolveMatrix(cv::Point, cv::Mat& , cv::Mat& , int i);
	void doGPUTransform(cv::Mat frame, cv::Mat& edgeImage, cv::Mat& binaryImage);
	void classifyPoints(int line);
	void prepareInterpolation(int);
	void debugDraw(cv::Mat&);
	bool solveClothoide();
	void clear();

	//MODELLPARAMETER
	//Grad des Modells (Klothoide)
	const static int grade = 3;
	//Offset vom oberen und unteren Bildrand
	const static int edgeOffset = 1;
	//Anzahl der Detektionslinien
	const static int numberOfLines = 60;

	int stepSize = 0;

	cv::Ptr<cv::cuda::CannyEdgeDetector> canny;

	cv::Mat transformationMat;

	//Aufteilen der Spurklothoide
	cv::Mat leftLane1;
	cv::Mat middleLane1;
	cv::Mat rightLane1;

	cv::Mat leftLane2;
	cv::Mat middleLane2;
	cv::Mat rightLane2;

	//Ergebnis
	std::array<std::vector<cv::Point>, 3> result;
	std::vector<std::vector<cv::Point>> detectedPoints;

	//Temporaere Variablen
	cv::Mat linePoints;
	std::vector<cv::Point> laneMiddles;

	cv::Mat lA;
	cv::Mat lB;

	cv::Mat rA;
	cv::Mat rB;

	cv::Mat mA;
	cv::Mat mB;

	bool foundLL = false;
	bool foundML = false;
	bool foundRL = false;

	int numberOfLeftPoints = 0;
	int numberOfMiddlePoints = 0;
	int numberOfRightPoints = 0;

	cv::Point leftLaneStartPoint, middleLaneStartPoint, rightLaneStartPoint;

	std::array<double, numberOfLines> leftDistances;
	std::array<double, numberOfLines> middleDistances;
	std::array<double, numberOfLines> rightDistances;

	std::array<int, numberOfLines> leftLineIndices;
	std::array<int, numberOfLines> middleLineIndices;
	std::array<int, numberOfLines> rightLineIndices;

};