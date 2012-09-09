/* 
 * File:   main.cpp
 * Author: jpcordovae
 *
 * Created on May 5, 2012, 9:04 AM
 */

#include <cstdlib>
#include "DataPair.hpp"
#include "glApplication.h"
#include "glCartesianGraphic.hpp"
#include <thread>
#include <boost/thread.hpp>
#include <memory>

bool THREAD_EXIT;
typedef boost::shared_ptr<DataPair<> > DataPairType;
DataPairType ptrDP1( new DataPair<>() );
DataPairType ptrDP2( new DataPair<>() );

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:

	virtual void OnInit( )
	{
		std::cout << "Hello World!\n";
	}
};

//-----------------------------------------------------------------------------

void thread_data_pair1( void )
{
	//ptrDP->UnlimitedSize();
	boost::posix_time::ptime time_tmp;
	double rnd;
	ptrDP1->SetBufferSize(5000);
	ptrDP1->SetColor(1.0f,0.0f,0.0f);
	ptrDP1->SetName(std::string("data 1"));
	while (!THREAD_EXIT)
	{
		time_tmp = boost::posix_time::microsec_clock::local_time();
		rnd = double(rand());
		
		try{
			ptrDP1->AddPair(time_tmp, rnd);
		}catch(std::exception &e){
			std::cerr << e.what() << std::endl;
		}
		
		usleep(1000);
	}
}

void thread_data_pair2( void )
{
	//ptrDP->UnlimitedSize();
	boost::posix_time::ptime time_tmp;
	double rnd;
	ptrDP2->SetBufferSize(5000);
	ptrDP2->SetColor(0.0f,0.0f,1.0f);
	ptrDP2->SetName(std::string("data 2"));
	while (!THREAD_EXIT)
	{
		time_tmp = boost::posix_time::microsec_clock::local_time();
		rnd = double(rand());
		
		try{
			ptrDP2->AddPair(time_tmp, rnd);
		}catch(std::exception &e){
			std::cerr << e.what() << std::endl;
		}
		
		usleep(1000);
	}
}

//-----------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	//IMU_DAC dac;
	//dac.SetSerialDeviceName("/dev/ttyACM0");
	//dac.Connect();
	boost::thread rand_thread1(&thread_data_pair1);
	boost::thread rand_thread2(&thread_data_pair2);
	
	std::cout << "lanzando hebra" << rand_thread1.get_id() << std::endl;
	std::cout << "lanzando hebra" << rand_thread2.get_id() << std::endl;
	
	myApplication* pApp = new myApplication;
	glCartesianGraphic* gyroWindow = new glCartesianGraphic(500, 500, 100, 100);
	gyroWindow->SetTitle("datos acelerometro y giroscopio");
	gyroWindow->SetAxisPosition(AXIS_VERTICAL_CENTERED | AXIS_HORIZONTAL_CENTERED | AXIS_VERTICAL_TOP );
	gyroWindow->SetDrawBox(false);
	gyroWindow->SetBufferSizeToShow(100);
	gyroWindow->Repaint();
	gyroWindow->AddDataPair(ptrDP1);
	gyroWindow->AddDataPair(ptrDP2);
	
	pApp->run(); // init windows and enter to opengl render loop
	
	rand_thread1.interrupt();
	rand_thread2.interrupt();
	
	delete pApp;
	delete gyroWindow;
	return EXIT_SUCCESS;
}
