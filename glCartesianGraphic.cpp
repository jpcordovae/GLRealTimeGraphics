/* 
 * File:   glCartesianGraphic.cpp
 * Author: jpcordovae
 * 
 * Created on May 12, 2012, 2:17 AM
 */

#include <boost/thread/pthread/mutex.hpp>

#include "glCartesianGraphic.hpp"

glCartesianGraphic::~glCartesianGraphic()
{

}

void glCartesianGraphic::OnInit()
{
    m_border = 0.1; 
    markers_x = 10; 
    markers_y = 10; 
    m_axis_x_min = 0.0f; 
    m_axis_x_max = 10000.0f; 
    m_axis_y_min = -1.0f, 
    m_axis_y_max = -1.0f; 
    SetTitlePosition(TITLE_UP); // default option
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // ********************************************
    // Do your GLEW experiments here:
    // if (GLEW_VERSION_1_2){}
    // if (GLEW_ARB_shading_language_100) {}
    // *******************************************

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void drawString (const char *s)
{
  for (size_t i = 0; i < strlen (s); i++)
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
}

void glCartesianGraphic::OnRender()
{
    static char cTmp[100]; // char buffer
    glColor3f(1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    // foot note
    sprintf(cTmp,"(C)Juan Pablo Cordova Echeverria");
    glRasterPos2f(-glutGet(GLUT_WINDOW_WIDTH),-glutGet(GLUT_WINDOW_HEIGHT));
    drawString(cTmp);
    DrawTitle();
    DrawBox();
    // axis subdivisions
    DrawAxisSubdivision();
    // central axes
    DrawAxis();
    // draw lines
    float size_x_axis = glutGet(GLUT_WINDOW_WIDTH)*(1-m_border*2);
    std::map<std::string,std::list<nv::vec2f> >::iterator map_it = m_point_list_map.begin();
    for(map_it;map_it!=m_point_list_map.end();++map_it){
        size_t count = map_it->second.size();
        
    }
    
    glutSwapBuffers();
    Repaint();
}

void glCartesianGraphic::OnResize(int w, int h) 
{ 
    //
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-glutGet(GLUT_WINDOW_WIDTH),
             glutGet(GLUT_WINDOW_WIDTH),
            -glutGet(GLUT_WINDOW_HEIGHT),
             glutGet(GLUT_WINDOW_HEIGHT),
            -1,1);
    glViewport(0,
               1,
               glutGet(GLUT_WINDOW_WIDTH), 
               glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
}

bool glCartesianGraphic::AddLine(const std::string &_data_name)
{
    //m_point_list_map.insert(std::pair<std::string,std::list<nv::vec2f> >(_data_name, std::list<point_ptr>()));
    return false;
}

void glCartesianGraphic::DrawTitle(enum TITLE_POSITION _title_position, const std::string &_title)
{
    // titles
    glRasterPos2f(0.0f-strlen( _title.c_str())*10/2,glutGet(GLUT_WINDOW_HEIGHT)-10*3);
    drawString(_title.c_str());
}

void glCartesianGraphic::DrawTitle()
{
    DrawTitle(m_title_position,m_title);
}

void glCartesianGraphic::SetTitle(const std::string &_title)
{
    m_title = _title;
}

void glCartesianGraphic::SetTitlePosition(enum TITLE_POSITION _title_position)
{
    m_title_position = _title_position;
}

void glCartesianGraphic::DrawAxis(void)
{
    glBegin(GL_LINES);
    
    if(m_axis_position & AXIS_HORIZONTAL_CENTERED)
    {
        glVertex2f(0.0f, glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border));
        glVertex2f(0.0f,-glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border));
    }
    
    if(m_axis_position & AXIS_VERTICAL_CENTERED)
    {
        glVertex2f( glutGet(GLUT_WINDOW_WIDTH)*(1-m_border),0.0f);
        glVertex2f(-glutGet(GLUT_WINDOW_WIDTH)*(1-m_border),0.0f);
    }
    
    glEnd();
}

void glCartesianGraphic::SetAxisPosition(enum AXIS_POSITION _axis_position)
{
    m_axis_position = _axis_position;
}

void glCartesianGraphic::SetAxisDataDeployment(enum AXIS_DATA_DEPLOYMENT _data_deployment)
{
    m_axis_data_deployment = _data_deployment;
}

void glCartesianGraphic::SetDrawBox(bool _set_draw_box)
{
    m_bDrawBox = _set_draw_box;
}

void glCartesianGraphic::DrawBox(void)
{
    // axes world
    if(!m_bDrawBox)
    {
        return;
    }
    
    glColor3f(0.25f,0.0f,0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-glutGet(GLUT_WINDOW_WIDTH)*(1-m_border),-glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border));
    glVertex2f(-glutGet(GLUT_WINDOW_WIDTH)*(1-m_border),glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border));
    glVertex2f(glutGet(GLUT_WINDOW_WIDTH)*(1-m_border),glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border));
    glVertex2f(glutGet(GLUT_WINDOW_WIDTH)*(1-m_border),-glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border));
    glEnd();
}

void glCartesianGraphic::DrawAxisSubdivision(void)
{
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    for(int i=0;i<markers_x*2;i++){
        float dx = glutGet(GLUT_WINDOW_WIDTH)*(1-m_border)/markers_x;
        float x = -glutGet(GLUT_WINDOW_WIDTH)*(1-m_border) + i*dx;
        glVertex2f(x, glutGet(GLUT_WINDOW_HEIGHT)*0.02);
        glVertex2f(x,-glutGet(GLUT_WINDOW_HEIGHT)*0.02);
    }
    glEnd();
    glBegin(GL_LINES);
    for(int i=0;i<markers_y*2;i++){
        float dy = glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border)/markers_x;
        float y = -glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border) + i*dy;
        glVertex2f( glutGet(GLUT_WINDOW_WIDTH)*0.02,y);
        glVertex2f(-glutGet(GLUT_WINDOW_WIDTH)*0.02,y);
    }
    glEnd();
}

void glCartesianGraphic::DrawDataPairs(void){
    // if there is no list to draw
    if(m_timed_pair_list.empty()) {
        return;
    }

    size_t pixels_x = glutGet(GLUT_WINDOW_WIDTH )*(1-m_border*2); // total amount of pixels to draw the lines
    size_t pixels_y = glutGet(GLUT_WINDOW_HEIGHT)*(1-m_border*2); // total amount of pixels to draw the lines
    //float fit_x = pixels_x/count; // if fit>1, we have more data that we can draw
    //float fit_y = pixels_y/count; // if fit>1, we have more data that we can draw
    
    
    
    std::list< boost::shared_ptr<DataPair<boost::posix_time::ptime,float> > >::iterator it = m_timed_pair_list.begin();
    for(;it!=m_timed_pair_list.end();++it){
        //size_t count = *it->count();
        //if( count < m_data_count_to_show )
        {
            //
        }
        
    }
}
