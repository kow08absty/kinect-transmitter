// KinectLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "KinectLib.h"
#include <Windows.h>

void KinectLib::Kinect::Loop() {
    while (isRunning) {
        k4a_capture_t sensorCapture = nullptr;
        k4a_wait_result_t getCaptureResult = k4a_device_get_capture(device, &sensorCapture, 0); // timeout_in_ms is set to 0

        if (getCaptureResult == K4A_WAIT_RESULT_SUCCEEDED) {
            // timeout_in_ms is set to 0. Return immediately no matter whether the sensorCapture is successfully added
            // to the queue or not.
            k4a_wait_result_t queueCaptureResult = k4abt_tracker_enqueue_capture(tracker, sensorCapture, 0);

            // Release the sensor capture once it is no longer needed.
            k4a_capture_release(sensorCapture);

            if (queueCaptureResult == K4A_WAIT_RESULT_FAILED) {
                std::cout << "Error! Add capture to tracker process queue failed!" << std::endl;
                break;
            }
        } else if (getCaptureResult != K4A_WAIT_RESULT_TIMEOUT) {
            std::cout << "Get depth capture returned error: " << getCaptureResult << std::endl;
            break;
        }

        // Pop Result from Body Tracker
        k4abt_frame_t bodyFrame = nullptr;
        k4a_wait_result_t popFrameResult = k4abt_tracker_pop_result(tracker, &bodyFrame, 0); // timeout_in_ms is set to 0
        if (popFrameResult == K4A_WAIT_RESULT_SUCCEEDED) {
            /************* Successfully get a body tracking result, process the result here ***************/
            k4a_capture_t originalCapture = k4abt_frame_get_capture(bodyFrame);
            k4a_image_t depthImage = k4a_capture_get_depth_image(originalCapture);
            uint32_t numBodies = k4abt_frame_get_num_bodies(bodyFrame);
            for (uint32_t i = 0; i < numBodies; i++) {
                k4abt_body_t body;
                VERIFY(k4abt_frame_get_body_skeleton(bodyFrame, i, &body.skeleton), "Get skeleton from body frame failed!");
                body.id = k4abt_frame_get_body_id(bodyFrame, i);

                auto right = GetJointPosition(body, K4ABT_JOINT_HAND_RIGHT);
                auto left = GetJointPosition(body, K4ABT_JOINT_HAND_LEFT);

                char str_right[64] = {0};
                char str_left[64] = {0};
                Float3ToStr(right, str_right);
                Float3ToStr(left, str_left);

                std::string msg = "id:" + std::to_string(body.id) + ";RH:" + str_right + ";LH:" + str_left + ";";
                captureCallback(msg);
            }
            k4a_capture_release(originalCapture);
            k4a_image_release(depthImage);

            //Release the bodyFrame
            k4abt_frame_release(bodyFrame);
        }
    }
}

void KinectLib::Kinect::Init() {
    std::cout << "Waiting for Kinect is ready";
    while (k4a_device_open(0, &device) != K4A_RESULT_SUCCEEDED && isRunning) {
        std::cout << ".";
        Sleep(1500);
    }
    std::cout << " Connected." << std::endl;

    // Start camera. Make sure depth camera is enabled.
    k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
    VERIFY(k4a_device_start_cameras(device, &deviceConfig), "Start K4A cameras failed!");

    // Get calibration information
    k4a_calibration_t sensorCalibration;
    VERIFY(k4a_device_get_calibration(device, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration),
        "Get depth camera calibration failed!");
    int depthWidth = sensorCalibration.depth_camera_calibration.resolution_width;
    int depthHeight = sensorCalibration.depth_camera_calibration.resolution_height;

    // Create Body Tracker
    k4abt_tracker_configuration_t trackerConfig = K4ABT_TRACKER_CONFIG_DEFAULT;
    trackerConfig.processing_mode = K4ABT_TRACKER_PROCESSING_MODE_GPU_DIRECTML;
    VERIFY(k4abt_tracker_create(&sensorCalibration, trackerConfig, &tracker), "Body tracker initialization failed!");
}

void KinectLib::Kinect::StartCapture(BodyFrameCallback callback) {
    captureCallback = callback;
    looper = std::thread(&KinectLib::Kinect::Loop, this);
}

void KinectLib::Kinect::Stop() {
    isRunning = false;

    k4abt_tracker_shutdown(tracker);
    k4abt_tracker_destroy(tracker);

    k4a_device_stop_cameras(device);
    k4a_device_close(device);
}
