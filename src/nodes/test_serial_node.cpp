#include "iostream"
#include "serial/serial.h"
#include "vector"
# include "test_serial.hpp"


int main(int argc,char** argv){                                
  TestSerial test_serial(argc,argv);
  test_serial.build_serial();
  test_serial.receive();
  // test_serial.run(argc, argv);
  // test_serial.decode();
  // try {
  //   return test_serial.run(argc, argv);
  // } catch (std::exception &e) {
  //   std::cerr << "Unhandled Exception: " << e.what() << std::endl;
  // }

    return 0;
}