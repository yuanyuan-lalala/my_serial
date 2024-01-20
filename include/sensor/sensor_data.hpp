#pragma once
#include "iostream"
#include <vector>
#include <serial/serial.h>

class Sensor_Data {
public:
  struct Frame {

    uint8_t frameID;
    uint32_t frameCount;
    uint16_t workStatus;
    uint16_t comboStatus;
    int32_t longitude;
    int32_t latitude;
    int32_t height;
    int16_t northSpeed;
    int16_t verticalSpeed;
    int16_t eastSpeed;
    uint16_t heading;
    int16_t pitch;
    int16_t roll;
    int32_t wx;
    int32_t wy;
    int32_t wz;
    int32_t ax;
    int32_t ay;
    int32_t az;
    int32_t gpsLongitude;
    int32_t gpsLatitude;
    int32_t gpsHeight;
    uint16_t gpsHorizontalSpeed;
    uint16_t gpsTrackAngle;
    int16_t gpsVerticalSpeed;
    uint16_t gpsHeading;
    uint32_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint32_t millisecond;
    uint8_t gpsPositionStatus;
    uint8_t gpsSpeedStatus;
    uint8_t gpsDirectionStatus;
    uint8_t gpsTimeStatus;
  };
  // 在 parseFrame 函数中扩展对新字段的解析逻辑
  void parseFrame(const std::vector<uint8_t> &data) {
    if (data.size() < sizeof(Frame)) {
      std::cerr << "Error: Insufficient data for parsing." << std::endl;
      return;
    }

    const Frame *framePtr = reinterpret_cast<const Frame *>(data.data());

    // 在这里，你可以访问 framePtr 中的新增字段，例如：
    std::cout << "GPS Horizontal Speed: " << framePtr->gpsHorizontalSpeed
              << std::endl;
    std::cout << "GPS Track Angle: " << framePtr->gpsTrackAngle << std::endl;
    std::cout << "GPS Vertical Speed: " << framePtr->gpsVerticalSpeed
              << std::endl;
    // ... 其他新增字段的访问

    // 如果需要，你还可以将新增字段的数据存储到类的成员变量中
    // ...
  }
};