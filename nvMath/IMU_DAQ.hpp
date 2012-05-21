/* 
 * File:   IMU_DAQ.hpp
 * Author: jpcordovae
 *
 * Created on May 21, 2012, 8:26 AM
 */

#ifndef IMU_DAQ_HPP
#define	IMU_DAQ_HPP

#include "serial_manager.hpp"
#include <string>

class IMU_DAQ {
public:
	IMU_DAQ();
	IMU_DAQ(const IMU_DAQ& orig);
	virtual ~IMU_DAQ();
	
	void SetSerialDeviceName(const std::string &_dev_name){
		m_dev_name = _dev_name;
	}
	
	std::string GetSerialDeviceName(void){
		return m_dev_name;
	}
	
private:
	std::string m_dev_name;
};

#endif	/* IMU_DAQ_HPP */

