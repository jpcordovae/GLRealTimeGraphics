/* 
 * File:   main.cpp
 * Author: jpcordovae
 *
 * Created on May 5, 2012, 9:04 AM
 */

#include <cstdlib>
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
    
    myApplication* pApp = new myApplication;
    glCartesianGraphic* gyroWindow = new glCartesianGraphic(200,300,100,100);
    gyroWindow->SetTitle("datos acelerometro y giroscopio");
    gyroWindow->SetAxisPosition(AXIS_VERTICAL_CENTERED | AXIS_HORIZONTAL_CENTERED);
    gyroWindow->SetDrawBox(false);
    pApp->run();
    
    while(1){
        sleep(10);
    };
    
    delete pApp;
    return EXIT_SUCCESS;
}

