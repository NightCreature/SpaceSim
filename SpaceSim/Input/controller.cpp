#include "controller.h"

void Controller::setLeftUpDown(float value) 
{
	if (m_leftupdown != value)
	{
		m_leftupdown = value;
		m_changed = true;
	}
}
void Controller::setLeftLR(float value) 
{
	if (m_leftlr != value)
	{
		m_leftlr = value;
		m_changed = true;
	}
}
void Controller::setRightUpDown(float value)
{
	if (m_rightupdown != value)
	{
		m_rightupdown = value;
		m_changed = true;
	}
}
void Controller::setRightLR(float value)
{
	if (m_rightlr != value)
	{
		m_rightlr = value;
		m_changed = true;
	}
}
void Controller::setButton1(bool value)
{
	if (m_button1 != value)
	{
		m_button1 = value;
		m_changed = true;
	}
}
void Controller::setButton2(bool value)
{
	if (m_button2 != value)
	{
		m_button2 = value;
		m_changed = true;
	}
}
void Controller::setButton3(bool value)
{
	if (m_button3 != value)
	{
		m_button3 = value;
		m_changed = true;
	}
}
void Controller::setButton4(bool value)
{
	if (m_button4 != value)
	{
		m_button4 = value;
		m_changed = true;
	}
}
void Controller::setButton5(bool value)
{
	if (m_button5 != value)
	{
		m_button5 = value;
		m_changed = true;
	}
}
void Controller::setButton6(bool value)
{
	if (m_button6 != value)
	{
		m_button6 = value;
		m_changed = true;
	}
}
void Controller::setButton7(bool value)
{
	if (m_button7 != value)
	{
		m_button7 = value;
		m_changed = true;
	}
}
void Controller::setButton8(bool value)
{
	if (m_button8 != value)
	{
		m_button8 = value;
		m_changed = true;
	}
}
void Controller::setButton9(bool value)
{
	if (m_button9 != value)
	{
		m_button9 = value;
		m_changed = true;
	}
}
void Controller::setButton10(bool value)
{
	if (m_button10 != value)
	{
		m_button10 = value;
		m_changed = true;
	}
}