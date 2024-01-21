#include "iostream"
#include "serial/serial.h"
#include "vector"
# include "test_serial.hpp"


int main(int argc,char** argv){                                
  TestSerial test_serial(argc,argv);
  test_serial.build_serial();


  while(true){
  try {
            if (test_serial.m_my_serial.available() > 0) {
                test_serial.receive();
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
  
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    
  }

    return 0;
}