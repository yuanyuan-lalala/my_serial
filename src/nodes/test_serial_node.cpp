# include "iostream"
# include "serial/serial.h"
# include "vector"
# include "test_serial.hpp"
# include "ros/ros.h"

int main(int argc,char** argv){

  ros::init(argc, argv, "serial_port");
  // 创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
  ros::NodeHandle n;

  TestSerial test_serial(argc, argv);

  test_serial.build_serial();

  ros::Rate loop_rate(200);

  while (ros::ok()) {

    try {
        //获取缓冲区内的字节数
        size_t n = test_serial.m_my_serial.available();
      if (n > 0) {
        test_serial.m_length_to_read = test_serial.m_my_serial.available();
        test_serial.receive();

      }
    } catch (const std::exception &e) {

      std::cerr << "Exception: " << e.what() << std::endl;
    }

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    loop_rate.sleep();
  }
  return 0;
}

