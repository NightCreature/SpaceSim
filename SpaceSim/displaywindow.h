#pragma once

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

//#include "controller.h"
//#include "light.h"
//#include "material.h"
//#include "mesh.h"
//#include "..\SpaceSim\Camera.h"
//#include "vector4.h"
//#include "texturemanager.h"
//#include "texture.h"
//#include "face.h"
//#include "box.h"
//#include "MapLoader.h"
//#include "player.h"
//
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <D3D11.h>

//class DisplayWindow : public gxbase::GLWindowEx
//{
//public:
//	DisplayWindow();
//	~DisplayWindow();
//	void initialize();
//	void checkExtensions();
//	void outputChoosenDeviceOptions();
//	void OnCreate();
//	void OnDestroy();
//
//	void OnDisplay();
//	void drawSkySphere();
//	void drawText();
//	void OnIdle();
//	void drawEndSequence();
//
//	void drawPlayerandScene( bool stopdrawingmap );
//	void OnResize(int w, int h);
//	void OnKeyboard(int key, bool down);
//	void handleKeyboardPlayerInput(int key, bool down );
//	void OnMouseButton(MouseButton button, bool down);
//	void OnMouseMove(int x, int y);
//	void setPlaybackData();
//protected:
//private:
//	void fps(int textheight);
//	void changeViewPort();
//	void glEnable2D();
//	void glDisable2D();
//	void setWireframe(bool wireframe = false);
//	void crossair();
//	void drawAxis();
//	void resetCamera();
//	void drawMiniMap();
//	void drawMiniMapBackground(int width, int height);
//	void resetGame();
//
//	void setupLights();
//	void checkPressedButtons();
//
//	bool m_fullscreen;
//	bool m_canautomipmap;
//	bool m_wireframe;
//	bool m_gamepad;
//	bool m_hwbuffers;
//	bool m_canmultitexture;
//	bool m_collisiondetection;
//	bool m_recordingstarted;
//	bool m_playback;
//	bool m_firsttime;
//	bool m_endsequence;
//	int m_fps;
//	int m_previoussecond;
//	int m_numtexunits;
//	int m_ondisplaycount;
//	float m_fov;
//	float m_timestamp;
//	float m_desiredforwardspeed;
//	float m_desiredrollpitchspeed;
//	float m_buttonpresstime;
//	float m_sequencestarttime;
//	float m_strafe;
//	float m_updown;
//	double m_recordstarttime;
//	Controller* m_controller;
//	Camera m_minimapcam;
//	Light m_light[8];
//	GLUquadric* m_skysphere;
//	std::string m_skytexname;
//	Map m_map;
//	Player m_player;
//	glex m_glex;
//	Recorder* m_recorder;
//	float			   m_laserspeed;
//	std::list<Laser*>   m_lasers;
//	std::list<Laser*>::iterator m_lasersit;
//	std::streambuf *psbuf1;
//	std::ofstream filestr1;
//	std::streambuf *psbuf2;
//	std::ofstream filestr2;
//	std::string m_gamestatus;
//};
