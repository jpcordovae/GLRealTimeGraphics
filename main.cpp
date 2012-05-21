/* 
 * File:   main.cpp
 * Author: jpcordovae
 *
 * Created on May 5, 2012, 9:04 AM
 */

#include <cstdlib>
#include "serial_manager.hpp"
#include "glApplication.h"
#include "glCartesianGraphic.hpp"

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
    virtual void OnInit()
    {
        std::cout << "Hello World!\n";
    }
};

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    
    Serial S("/dev/ttyACM0");
    myApplication* pApp = new myApplication;
    glCartesianGraphic* gyroWindow = new glCartesianGraphic(200,300,100,100);
    gyroWindow->SetTitle("datos acelerometro y giroscopio");
    gyroWindow->SetAxisPosition(AXIS_VERTICAL_CENTERED | AXIS_HORIZONTAL_CENTERED);
    gyroWindow->SetDrawBox(false);
    //S.ConnectCallBack();
    pApp->run();
    
    while(1){
        sleep(1);
    };
    
    delete pApp;
    return EXIT_SUCCESS;
}

/*
int main(int argc, char** argv)
{
    Serial S("/dev/ttyACM0");
    cwc::glutWindow *win = new cwc::glutWindow();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
 */
