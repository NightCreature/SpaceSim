#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "controller.h"

class PlaybackController : public Controller
{
public:
	PlaybackController()
	{
		setType((controllertype)0);
		m_leftupdown = 0.0f; 
		m_leftlr = 0.0f;
		m_rightupdown = 0.0f;
		m_rightlr = 0.0f;
		m_button1 = false;
		m_button2 = false;
		m_button3 = false;
		m_button4 = false;
		m_button5 = false;
		m_button6 = false;
		m_button7 = false;
		m_button8 = false; 
		m_button9 = false;
		m_button10 = false;
	}
	~PlaybackController() {}
	void update() {}

	float getLeftUpDown() {return m_leftupdown;}
	float getLeftLR() {return m_leftlr;}
	float getRightUpDown() {return m_rightupdown;}
	float getRightLR() {return m_rightlr;}
	bool getButton1() {return m_button1;}
	bool getButton2() {return m_button2;}
	bool getButton3() {return m_button3;}
	bool getButton4() {return m_button4;}
	bool getButton5() {return m_button5;}
	bool getButton6() {return m_button6;}
	bool getButton7() {return m_button7;}
	bool getButton8() {return m_button8;}
	bool getButton9() {return m_button9;}
	bool getButton10() {return m_button10;}
};
#endif