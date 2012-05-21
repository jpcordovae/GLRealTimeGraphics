//******************************************************************************/
// There are many implementations of glWindow, using glut, SDL, wxWidgets, QT.
//
// This is the FreeGLUT Version.
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/


#include "glutwindow.h"

using namespace cwc;

//-----------------------------------------------------------------------------

std::list<glutWindow*> glutWindow::_gWinInstances;
bool glutWindow::_bLMouseDown;

//-----------------------------------------------------------------------------

glutWindow::glutWindow():
        m_width(640),
        m_height(480),
        m_windows_position_x(100),
        m_windows_position_y(100)
{
    _bLMouseDown = false;
    _gWinInstances.push_back(this);
    //-------------------------------------------
    // #TODO: Use Parameters to setup this stuff: 
    
    glutInitWindowSize(m_width,m_height);
    glutInitWindowPosition(m_windows_position_x,m_windows_position_y);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //--------------------------------------------
    _gWindow = glutCreateWindow("OpenGL Output");
    _bInit = false;
    glutDisplayFunc(sDisplay);
    glutReshapeFunc(sReshape);
    glutMouseFunc(sMouse);
    glutMotionFunc(sMotion);
    #ifdef FREEGLUT
    glutMouseWheelFunc(sMouseWheel);
    glutCloseFunc(sClose);
    #endif   
    glutKeyboardFunc(sKeyDown);
    glutSpecialFunc(sSpecialDown);
    glutKeyboardUpFunc(sKeyUp);
    glutSpecialUpFunc(sSpecialUp);
    glutIdleFunc(sIdle);
    
}

//-----------------------------------------------------------------------------

glutWindow::~glutWindow(void)
{
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if (*i == this)
        {
            _gWinInstances.erase(i);
            return;
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sClose(void)
{
    int CurrentWindow = glutGetWindow();

    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnClose();
            delete (*i);
            return;
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sReshape(int w, int h)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnResize(w, h);
        }

        i++;
    }

}

//-----------------------------------------------------------------------------

void glutWindow::sDisplay(void)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if (!(*i)->_bInit)
        {
            (*i)->OnInit();
            (*i)->_bInit = true;
        }

        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnRender();
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sMouse(int button, int updown, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            if (updown)
            {
                if (button == 0)
                    _bLMouseDown = false;
                (*i)->OnMouseUp(button, x, y);
            }
            else
            {
                if (button == 0)
                    _bLMouseDown = true;
                (*i)->OnMouseDown(button, x, y);
            }
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sMotion(int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            if (_bLMouseDown)
                (*i)->OnLeftMouseDrag(x, y);

            (*i)->OnMouseMove(x, y);
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sMouseWheel(int wheel_number, int direction, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnMouseWheel(wheel_number, direction, x, y);
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sJoystick(unsigned int a, int b, int c, int d)
{
}

//-----------------------------------------------------------------------------

void glutWindow::sSpecialUp(int key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnKeyUp(key, 0);
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sKeyUp(unsigned char key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnKeyUp(0, key);
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sSpecialDown(int key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnKeyDown(key, 0);
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sKeyDown(unsigned char key, int x, int y)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnKeyDown(0, key);
        }

        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::sIdle(void)
{
    int CurrentWindow = glutGetWindow();
    std::list<glutWindow*>::iterator i = _gWinInstances.begin();

    while (i != _gWinInstances.end())
    {
        if ((*i)->_gWindow == CurrentWindow)
        {
            (*i)->OnIdle();
        }
        i++;
    }
}

//-----------------------------------------------------------------------------

void glutWindow::Repaint()
{
    glutPostWindowRedisplay(_gWindow);
}

//-----------------------------------------------------------------------------

void glutWindow::SetFullscreen(bool _bFullscreen)
{
    if(m_bFullscreen==_bFullscreen){
        return;
    }
    
    m_bFullscreen = _bFullscreen;
    
    if (_bFullscreen)
    {
        // save last position and size
        m_width = glutGet(GLUT_WINDOW_WIDTH);
        m_height = glutGet(GLUT_WINDOW_HEIGHT);
        m_windows_position_x = glutGet(GLUT_WINDOW_X);
        m_windows_position_y = glutGet(GLUT_WINDOW_Y);
        // go to full screen
        glutFullScreen();
    }
    else
    {
        glutPositionWindow(m_windows_position_x,m_windows_position_y);
        glutReshapeWindow(m_width,m_height);
    }
}

//-----------------------------------------------------------------------------

void glutWindow::Hide()
{
    glutHideWindow();
}

//-----------------------------------------------------------------------------

void glutWindow::Show()
{
    glutShowWindow();
}

//-----------------------------------------------------------------------------   

void glutWindow::Close()
{
    glutDestroyWindow(_gWindow);
}

//-----------------------------------------------------------------------------