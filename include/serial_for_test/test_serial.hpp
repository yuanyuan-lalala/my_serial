#pragma once

#include "iostream"
#include "sensor_data.hpp"
#include "serial/serial.h"
#include "glog/logging.h"

#define RSYS_RX_FREE             0
#define RSYS_RX_FIRST_GOOD       1
#define RSYS_RX_SECOND_GOOD      2
#define RSYS_RX_LENGTH           3
#define RSYS_RX_CHECK            4
#define RSYS_RX_PASS             5


class TestSerial{

public:

    TestSerial(int argc,char** argv);

    void build_serial();

    void my_sleep(unsigned long milliseconds) const;	

    int run	(int argc,char ** argv);

    void send(unsigned char* buffer, size_t& length);

    void receive();

    bool decode();
    
    unsigned char check_sum(std::vector<unsigned char>& Rsys_RX_Real_Data);

    char* m_port_name;

    int m_argc;
    
    char ** m_argv;

    unsigned long m_baudrate;

    std::string m_test_string;
    
    uint8_t m_buffer[10000];
        
    size_t m_length_to_read = 0;
    size_t m_length_real;

    

    serial::Serial m_my_serial;
   private:

    Sensor_Data sensor_data;

}; 

