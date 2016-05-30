#ifndef CONTROLLER_H
#define CONTROLLER_H

enum class controllertype 
{
	GamePad = 0,
	Xboxgamepad,
	Playbackcontroller,
	KeyBoard,
};

class Controller
{
public:
	Controller() { m_changed = false;}
	virtual ~Controller() {}
	
	virtual void update() = 0;

	void noInput()
	{
		m_leftupdown = m_leftlr = m_rightupdown = m_rightlr = 0.0f;
		m_button1 = m_button2 = m_button3 = m_button4 = m_button5 = m_button6 = m_button7 = m_button8 = m_button9 = m_button10 = false;
	}

	virtual float getLeftUpDown() = 0;
	virtual float getLeftLR() = 0;
	virtual float getRightUpDown() = 0;
	virtual float getRightLR() = 0;
	virtual bool getButton1() = 0;
	virtual bool getButton2() = 0;
	virtual bool getButton3() = 0;
	virtual bool getButton4() = 0;
	virtual bool getButton5() = 0;
	virtual bool getButton6() = 0;
	virtual bool getButton7() = 0;
	virtual bool getButton8() = 0;
	virtual bool getButton9() = 0;
	virtual bool getButton10() = 0;

	void setLeftUpDown(float value);
	void setLeftLR(float value);
	void setRightUpDown(float value);
	void setRightLR(float value);
	void setButton1(bool value);
	void setButton2(bool value);
	void setButton3(bool value);
	void setButton4(bool value);
	void setButton5(bool value);
	void setButton6(bool value);
	void setButton7(bool value);
	void setButton8(bool value);
	void setButton9(bool value);
	void setButton10(bool value);

	controllertype getType() const {return m_type;}
	void setType(controllertype type) {m_type = type;}
protected:
	double		   m_timestamp;
	float m_leftupdown, m_leftlr, m_rightupdown, m_rightlr;
	bool m_button1, m_button2, m_button3, m_button4, m_button5, m_button6, m_button7, m_button8, m_button9, m_button10;
	bool m_changed;
private:
	controllertype m_type;
};
#endif