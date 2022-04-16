#ifndef __FAKESENSOR_H
#define __FAKESENSOR_H

/**
 * Copyright (c) 2021  Bernd Porr <mail@berndporr.me.uk>
 **/

#include <math.h>
#include "CppTimer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "base64.h"


/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/
class SensorCallback {
public:
	/**
	 * Called after a sample has arrived.
	 **/
    virtual void hasSample(int sample) = 0;

    //
    virtual void hasSampleCV(cv::Mat sample) = 0;
};


/**
 * This class reads data from a fake sensor in the background
 * and calls a callback function whenever data is available.
 **/
class FakeSensor : public CppTimer {

public:
	FakeSensor() {
	}

	~FakeSensor() {
		stop();
	}

	/**
	 * Sets the callback which is called whenever there is a sample
	 **/
	void setCallback(SensorCallback* cb) {
		sensorCallback = cb;
	}

	/**
	 * Starts the data acquisition in the background and the
	 * callback is called with new samples
	 **/
	void startSensor() {
		start(250000000);
	}

	/**
	 * Stops the data acquistion
	 **/
	void stopSensor() {
		stop();
	}

	/**
	 * Fake the arrival of data
	 **/
    void timerEvent() {
        float value = sin(t) * 5 + 20;
        t += 0.1;
        if (nullptr != sensorCallback) {
            sensorCallback->hasSample(value);
        }
    }

    void timerEventCV() {
        cv::Mat image = cv::imread("/Users/littlesheep/Downloads/test1_result.jpg");  //存放自己图像的路径
        //imshow("显示图像", image);
        std::vector<unsigned char> data_encode;
        int res = imencode(".jpg", image, data_encode);
        std::string str_encode(data_encode.begin(), data_encode.end());
        const char* c = str_encode.c_str();
        JSONCGIHandler::JSONGenerator jsonGenerator;
        jsonGenerator.add("mat",base64_encode(c, str_encode.size()));

        return 0;


    }

private:
	SensorCallback* sensorCallback = nullptr;
	float t = 0;
    //
    cv::VideoCapture cap;
    uint8_t id = 0;
    cv::Mat current_frame;
    cv::Mat frame;
    cv::Mat gray;
    int i=0;
    //
};


#endif
