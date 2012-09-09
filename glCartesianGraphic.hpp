/* 
 * File:   glCartesianGraphic.hpp
 * Author: jpcordovae
 *
 * Created on May 12, 2012, 2:17 AM
 */

#ifndef GLCARTESIANGRAPHIC_HPP
#define	GLCARTESIANGRAPHIC_HPP

#include "glutwindow.h"
#include <GL/freeglut.h>
#include <boost/shared_ptr.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include <list>
#include "nvMath/nvMath.h"
#include "DataPair.hpp"

enum AXIS_DATA_DEPLOYMENT{ AXIS_FIXED=0x01,
									AXIS_ALL,
									AXIS_LAST_DATA };

enum AXIS_POSITION { AXIS_VERTICAL_CENTERED=0x01,
							AXIS_VERTICAL_TOP=0x02,
							AXIS_VERTICAL_BOTTOM=0x04,
							AXIS_HORIZONTAL_LEFT=0x08,
							AXIS_HORIZONTAL_RIGHT=0x10,
							AXIS_HORIZONTAL_CENTERED=0x12 };

enum TITLE_POSITION {TITLE_UP=0x01,
							TITLE_DOWN,
							TITLE_LEFT,
							TITLE_RIGHT};

class glCartesianGraphic : public cwc::glutWindow 
{
public:
	
	~glCartesianGraphic();

	glCartesianGraphic(int _width, int _height, int _posx, int _posy) // TODO
	{
		
	}
	
	virtual void OnRender(void);

	virtual void OnIdle() {
	}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!

	void OnInit();

	void OnResize(int w, int h);

	virtual void OnClose(void) {
	}

	virtual void OnMouseDown(int button, int x, int y) {
	}

	virtual void OnMouseUp(int button, int x, int y) {
	}

	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y) {
	}

	virtual void OnKeyDown(int nKey, char cAscii) {
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		}
	};

	virtual void OnKeyUp(int nKey, char cAscii) {
		if (cAscii == 'f') {
			SetFullscreen(true);
		} else if (cAscii == 'w') {
			SetFullscreen(false);
		}
	};

	bool AddLine(const std::string &_line_name);
	
	bool AddData(float _x, float _y, const std::string &_line_name)
	{
		std::map<std::string,std::list<nv::vec2f> >::iterator map_it = m_point_list_map.begin();
		for(map_it;map_it!=m_point_list_map.end();++map_it)
		{
			if(map_it->first.compare(_line_name)==0)
			{
				map_it->second.push_back(nv::vec2f(_x,_y));
			}
		}
	}
	
	void SetDrawBox(bool _set_draw_box); // draw a box 
	void SetTitle(const std::string &_title);
	void SetTitlePosition(enum TITLE_POSITION _title_position);
	void SetAxisPosition(enum AXIS_POSITION _axis_position);
	void SetAxisDataDeployment(enum AXIS_DATA_DEPLOYMENT _data_deployment);
	
	void SetBufferSizeToShow(size_t _amount) // how many data we will show
	{
		m_axis_data_deployment = AXIS_LAST_DATA; // warning !!, not thread safe
		m_data_count_to_show = _amount;
	}
	
private:
	void DrawAxis(void);
	void DrawBox(void);
	void DrawTitle(enum TITLE_POSITION _title_position, const std::string &_title);
	void DrawTitle(void);
	void DrawAxisSubdivision(void);
	void DrawDataPairs(void);
	std::string m_title;
	enum TITLE_POSITION m_title_position;
	enum AXIS_POSITION m_axis_position;
	enum AXIS_DATA_DEPLOYMENT m_axis_data_deployment; // how data will be show
	size_t m_data_count_to_show;	// how much data will be show in graphic, 
											// only valid with AXIS_LAST_DATA set
	float m_x_min, m_x_max, m_y_min, m_y_max; // min and max for AXIS_FIXED set
	bool m_bGrid;
	bool m_bDrawBox;
	int markers_x, markers_y;
	boost::mutex m_point_list_map_mtx;
	std::map<std::string,std::list<nv::vec2f> > m_point_list_map;
	bool m_new_data_in_buffer; // check if there is new data to draw, or, has been modified
	float m_axis_x_min, m_axis_x_max; // axis limits
	float m_axis_y_min, m_axis_y_max; // axis limits
	float m_border;
	
	std::list< boost::shared_ptr<DataPair<boost::posix_time::ptime,float> > > m_timed_pair_list;
};

inline AXIS_POSITION operator|(AXIS_POSITION a, AXIS_POSITION b)
{
	return static_cast<AXIS_POSITION>(static_cast<int>(a) | static_cast<int>(b));
}

#endif	/* GLCARTESIANGRAPHIC_HPP */

