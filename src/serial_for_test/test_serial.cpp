/*
by yuanyuan
*/
#pragma once

# include "test_serial.hpp"
#include "chrono"
#include "thread"


TestSerial::TestSerial(int argc,char** argv):m_argc(argc),m_argv(argv){
  
  if(m_argc < 3) {
    std::cerr << "Usage: test_serial <serial port address> ";
    std::cerr << "<baudrate> [test string]" << std::endl;
    return ;
  } 
  m_port_name = m_argv[1];
  m_baudrate = sscanf(m_argv[2], "%lu", &m_baudrate);
  m_test_string = m_argv[3];
  
  
  std::cout<<"init log!"<<std::endl;
  google::InitGoogleLogging("my_serial_log");
  google::SetLogDestination(google::GLOG_INFO, "/home/yuanyuan/catkin_LINS/src/my_serial/log/");
  
  
  LOG(INFO)<<"frameID "<<"frameCount "<<"workStatus "<<"comboStatus "<<"longitude "<<"latitude "
  <<"height "<<"northSpeed "<<"verticalSpeed "<<"eastSpeed "<<"heading "<<"pitch "<<"roll "<<"wx "
  <<"wy "<<"wz "<<"ax "<<"ay "<<"az "<<"gpsLongitude "<<"gpsLatitude "<<"gpsHeight "<<"gpsHorizontalSpeed "
  <<"gpsTrackAngle "<<"gpsVerticalSpeed "<<"gpsHeading "<<"year "<<"month "<<"day "<<"hour "<<"minute "
  <<"millisecond "<<"gpsPositionStatus "<<"gpsSpeedStatus "<<"gpsDirectionStatus "<<"gpsTimeStatus ";
   

}

void TestSerial::build_serial(){

  m_my_serial.setPort(m_port_name);
  m_my_serial.setBaudrate(m_baudrate);
  m_my_serial.setTimeout(0,1000,0,1000,0);
  
  m_my_serial.open();
  std::cout << "Is the serial port open?";
  if(m_my_serial.isOpen()){
    std::cout << " Yes." << std::endl;}
  else{
    std::cout << " No." << std::endl;}
    // m_my_serial.write("Hello, Serial!");
  std::cout << "the baudrate now : " << m_my_serial.getBaudrate() << std::endl;
  std::cout << "the baudrate now : " << m_my_serial.getBaudrate() << std::endl;
  std::cout << "get the byteSize now : " << m_my_serial.getBytesize() << std::endl;
  std::cout << "get the flow control : " << m_my_serial.getFlowcontrol() << std::endl;

}

void TestSerial::my_sleep(unsigned long milliseconds) const{

#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));     
    //   usleep(milliseconds*1000); // 100 ms
#endif

}

void TestSerial::receive(){

  m_length_real = m_my_serial.read(m_buffer, m_length_to_read);
  if (!decode())  ///解析数据
      {
          my_sleep(1000);
      }

  std::cout<<"decode finished!"<<std::endl;
  m_my_serial.flush();
}

void TestSerial::send(unsigned char* buff, size_t& length){


}

bool TestSerial::decode() {

  static int state = RSYS_RX_FREE; /// 接收状态机，刚开始设为FREE

  static std::vector<unsigned char> Rsys_RX_Buffer;
  static std::vector<unsigned char> Rsys_RX_Real_Data;

  static std::vector<unsigned char> num;

  unsigned short data_length = 0;
  unsigned char buffer_data = 0;
  unsigned char check_flag = 0;

  // 遍历所有的buffer
  for (size_t i = 0; i < m_length_real; i++) {
    buffer_data = m_buffer[i];
    switch (state) {
    case RSYS_RX_FREE:
      Rsys_RX_Buffer.clear();
      Rsys_RX_Real_Data.clear();
      if (buffer_data ==
          0x55) /// 首位校验 数据帧起始字节，固定值为 0x55 1个字节
      {
        state = RSYS_RX_FIRST_GOOD; /// 状态进入第二个帧头校验环节
        Rsys_RX_Buffer.push_back(buffer_data);
      } else {
        state = RSYS_RX_FREE;
      }
      break;
    case RSYS_RX_FIRST_GOOD:
      if (buffer_data == 0xAA) { // 第二个帧头校验完成后进入长度环节
        state = RSYS_RX_LENGTH;
        Rsys_RX_Buffer.push_back(buffer_data);
      } else {
        state = RSYS_RX_FREE;
      }
    case RSYS_RX_LENGTH: ///   后面具体内容的长度 1个字节
      if (Rsys_RX_Buffer.size() <= 3) {
        Rsys_RX_Buffer.push_back(buffer_data); /// 收集前3字节


        Rsys_RX_Real_Data.push_back(buffer_data);
      } else {
        data_length = Rsys_RX_Buffer[2];
        Rsys_RX_Buffer.push_back(buffer_data);
        Rsys_RX_Real_Data.push_back(buffer_data);

        // 帧长+帧数据内容字节累加和的低8位
        if (Rsys_RX_Buffer.size() == 3 + data_length) {
          state = RSYS_RX_CHECK;
        }
      }
    case RSYS_RX_CHECK:
      check_flag = check_sum(Rsys_RX_Real_Data);
      if (check_flag != buffer_data) {
        // 校验码不对
        state = RSYS_RX_FREE;
      } else {
        // 继续解包
        sensor_data.parseFrame(Rsys_RX_Real_Data);
        if(sensor_data.framePtr->frameID != 0x41){
            state = RSYS_RX_FREE;
        }else{
          LOG(INFO)<<sensor_data.framePtr->frameID;
          //解包完毕
          return true;
        }
        
      }
    }
  }
}

unsigned char TestSerial::check_sum(std::vector<unsigned char>& Rsys_RX_Real_Data){

  unsigned int sum = 0;

  for(auto per_data : Rsys_RX_Real_Data){
    sum += per_data;
  }
  
  return static_cast<unsigned char>(sum % 256);
}













int TestSerial::run(int argc,char ** argv){

if(argc < 3) {
    std::cerr << "Usage: test_serial <serial port address> ";
    std::cerr << "<baudrate> [test string]" << std::endl;
    return 0;
  }
  // Argument 1 is the serial port 串口号
  std::string port(argv[1]);

  // Argument 2 is the baudrate 波特率
  unsigned long baud = 0;
  sscanf(argv[2], "%lu", &baud);

  // port, baudrate, timeout in milliseconds 迟后多长时间算失败
  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

  std::cout << "Is the serial port open?";
  if(my_serial.isOpen())
    std::cout << " Yes." << std::endl;
  else
    std::cout << " No." << std::endl;

  // Get the Test string
  int count = 0;
  std::string test_string;
  if (argc == 4) {
    test_string = argv[3];
  } else {
    test_string = "Testing.";
  }

  // Test the timeout, there should be 1 second between prints// 测试超时，打印之间应该有1秒的时间间隔
  std::cout << "Timeout == 1000ms, asking for 1 more byte than written." << std::endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    std::string result = my_serial.read(test_string.length()+1);

    std::cout << "Iteration: " << count << ", Bytes written: ";
    std::cout << bytes_wrote << ", Bytes read: ";
    std::cout << result.length() << ", String read: " << result << std::endl;

    count += 1;
  }

  // Test the timeout at 250ms 测试超时时间
  my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
  count = 0;
  std::cout << "Timeout == 250ms, asking for 1 more byte than written." << std::endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    std::string result = my_serial.read(test_string.length()+1);

    std::cout << "Iteration: " << count << ", Bytes written: ";
    std::cout << bytes_wrote << ", Bytes read: ";
    std::cout << result.length() << ", String read: " << result << std::endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking exactly for what was written
  count = 0;
  std::cout << "Timeout == 250ms, asking for exactly what was written." << std::endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    std::string result = my_serial.read(test_string.length());

    std::cout << "Iteration: " << count << ", Bytes written: ";
    std::cout << bytes_wrote << ", Bytes read: ";
    std::cout << result.length() << ", String read: " << result << std::endl;

    count += 1;
  }

  // Test the timeout at 250ms, but asking for 1 less than what was written
  count = 0;
  std::cout << "Timeout == 250ms, asking for 1 less than was written." << std::endl;
  while (count < 10) {
    size_t bytes_wrote = my_serial.write(test_string);

    std::string result = my_serial.read(test_string.length()-1);

    std::cout << "Iteration: " << count << ", Bytes written: ";
    std::cout << bytes_wrote << ", Bytes read: ";
    std::cout << result.length() << ", String read: " << result << std::endl;

    count += 1;
  }

  return 0;

}