#pragma once

#include <opencv2/core.hpp>
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/c/c_api.h"
#include "tensorflow/lite/delegates/xnnpack/xnnpack_delegate.h"
#include <vector>
#include <numeric>

using namespace cv;

struct DetectResult {
    int label = -1;
    float score = 0;
    float ymin = 0.0;
    float xmin = 0.0;
    float ymax = 0.0;
    float xmax = 0.0;
};

class ObjectDetector {
public:
    ObjectDetector(const char* tfliteModelPath, bool quantized = false, bool useXnn = false);
    ~ObjectDetector();
    DetectResult* detect(Mat src);
    const int DETECT_NUM = 20;
    void setBoxThreshold(float threshold);
private:
    // members
    int model_resize_width;
    int model_resize_height;
    int model_resize_channels;

    const float IMAGE_MEAN = 128.0;
    const float IMAGE_STD = 128.0;
    bool m_modelQuantized = false;
    TfLiteModel* m_model;
    TfLiteInterpreter* m_interpreter;
    TfLiteDelegate* m_xnnpack_delegate;
    TfLiteTensor* m_input_tensor = nullptr;
    const TfLiteTensor* m_output_locations = nullptr;
    const TfLiteTensor* m_output_classes = nullptr;
    const TfLiteTensor* m_output_scores = nullptr;
    const TfLiteTensor* m_num_detections = nullptr;

    // Methods
    void initDetectionModel(const char* tfliteModelPath, bool useXnn);

    // NMS method
    std::vector<int> applyNMS(const std::vector<DetectResult>& detections, float iouThreshold);
    float intersectionOverUnion(const DetectResult& a, const DetectResult& b);

    // Constants
    static volatile float NMS_IOU_THRESHOLD;
};