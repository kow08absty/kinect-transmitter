#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <iostream>
#include <thread>
#include <variant>
#include "k4a/k4a.hpp"
#include "k4abt.hpp"
#include "k4abttypes.h"

#define VERIFY(function, message) if (function != K4A_RESULT_SUCCEEDED) { std::cout << message << std::endl; exit(-1); }

typedef void(*BodyFrameCallback)(std::string);

namespace KinectLib {
	class Kinect {
	private:
		k4a_device_t device = nullptr;
		k4abt_tracker_t tracker = nullptr;
		
		bool isRunning = true;
		BodyFrameCallback captureCallback = nullptr;
		std::thread looper;

		void Loop();

	public:
		void Init();
		void StartCapture(BodyFrameCallback callback);
		void Stop();

		k4a_float3_t GetJointPosition(k4abt_body_t body, k4abt_joint_id_t joint) {
			auto target = body.skeleton.joints[joint];
			if (target.confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW) {
				return target.position;
			}
			return k4a_float3_t();
		}
		template<size_t _Size> void Float3ToStr(k4a_float3_t floats, char(&dst)[_Size]) {
			sprintf_s(dst, _Size, "%.2f,%.2f,%.2f", floats.xyz.x, floats.xyz.y, floats.xyz.z);
		}
	};
}
