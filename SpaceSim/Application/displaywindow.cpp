#include "displaywindow.h"
//#include "matrixmath.h"
////#include "vld.h"
//#include "matrix44.h"
//#include "particlesystemmanager.h"
//#include "shadermanager.h"
//#include "modelmanager.h"
//#include "keyboard.h"
//#include "playbackcontroller.h"
//
//#include <d3d11.h>
//
//
//
//
//const int TEXTHEIGHT = 16;

//DisplayWindow::DisplayWindow()
//{
//	filestr1.open ("..\\Logs\\stdout.txt");
//	psbuf1 = filestr1.rdbuf();
//	cout.rdbuf(psbuf1);
//	filestr2.open ("..\\Logs\\stderr.txt");
//	psbuf2 = filestr2.rdbuf();
//	cerr.rdbuf(psbuf2);
//	m_fullscreen = false;
//	m_wireframe	 = false;
//	m_gamepad	 = true;
//	m_canmultitexture = false;
//	m_recordingstarted = false;
//	m_playback = false;
//	m_firsttime = true;
//	m_endsequence = false;
//	m_fps = 0;
//	m_previoussecond = 0;
//	m_ondisplaycount = 0;
//	m_buttonpresstime = 0.0f;
//	m_fov = math::gmPI / 4;
//	m_gamestatus = "Game is initilializing";
//#ifdef XBOXCONTROLLER
//	m_controller = new XboxGamepad();
//	XboxGamepad* game  = (XboxGamepad*)m_controller;
//#else
//	m_controller = new Gamepad();
//	Gamepad* game  = (Gamepad*)m_controller;
//#endif
//	if(!game->getOpen())
//	{
//		delete m_controller;
//		m_controller = new Keyboard();
//	}
//	string str;
//	switch (m_controller->getType())
//	{
//	case GamePad:
//		str = "Normal Gamepad, keyboard is still active.";
//		break;
//	case Xboxgamepad:
//		str = "Xbox360 Gamepad, keyboard is still active.";
//		break;
//	case KeyBoard:
//		str = "Keyboard";
//		break;
//	}
//	cout << "Controller type: " << str << endl;
//	initialize();
//}
//
//DisplayWindow::~DisplayWindow()
//{
//	if (m_controller)
//		delete m_controller;
//	if (m_recorder)
//		delete m_recorder;
//	gluDeleteQuadric(m_skysphere);
//}
//
//void DisplayWindow::OnDestroy()
//{
//	m_map.reset();
//	ModelManager::getInstance().cleanup();
//}
//
//void DisplayWindow::initialize()
//{
//	SetSize(1024, 768);
//	if (!SetFullscreen(m_fullscreen))
//	{
//		cerr << "OpenGL: Device doesn\'t support ";
//		cerr << m_fullscreen ? "fullscreen" : "windowed";
//		cerr << " mode" << endl;
//	}
//	if (!SetDoubleBuffer(true))
//		cerr << "OpenGL: Device doesn\'t support double buffering" << endl;
//	SetTitle("Coursework 1: Space Sim");
//	if (!SetDepthBits(32))
//		if (!SetDepthBits(24))
//			if(!SetDepthBits(16))
//				cerr << "OpenGL: Device doesn't support the requested depth buffer" << endl;
//	if (!SetAlphaBits(8))
//		cerr << "OpenGL: Device doesn't support 8 bits alpha" << endl;	
//
//	SetCursor(CRCross);
//	SetPosition(0,0);
//}
//
//void DisplayWindow::checkExtensions()
//{
//	if (!HasExtension("GL_SGIS_generate_mipmap"))
//		m_canautomipmap = false;
//	else
//		m_canautomipmap = true;
//	if (!HasExtension("GL_ARB_vertex_buffer_object"))
//		m_hwbuffers = false;
//	else
//		m_hwbuffers = true;
//}
//
//void DisplayWindow::outputChoosenDeviceOptions()
//{
//	string temp;
//	cout << "OpenGL: Selected settings are:\n";
//	temp = GetFullscreen() ? "true\n" : "false\n";
//	cout << "Fullscreen: " << temp.c_str();
//	temp = GetDoubleBuffer() ? "true\n" : "false\n";
//	cout << "Doublebuffered: " << temp.c_str(); 
//	cout << "Depth buffer: " << GetDepthBits() << "\n";
//	temp = m_canautomipmap ? "true\n" : "false\n";
//	cout << "Auto generate mipmaps: " << temp.c_str();
//	temp = m_canmultitexture ? "true\n" : "false\n";
//	cout << "Multi texturing: " << temp.c_str();
//	cout << "Maximum simultaneous textures: " << m_numtexunits << endl;
//	temp = m_hwbuffers ? "true\n" : "false\n";
//	cout << "Hardware buffers: " << temp.c_str();
//	cout << "Alpha bits: " << GetAlphaBits()  << "\n";
//	temp = (int)m_controller->getType() == (int)GamePad ? "Gamepad\n" : "Keyboard\n";
//	cout << "Controller type: " << temp.c_str();
//}
//
//void DisplayWindow::resetGame()
//
//	m_map.reset();
//	m_map.loadMap(SpaceSim::getInstance().getStringSetting("spacestationmap"));
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective((double)toDegree(m_fov), (double)Width() / (double)Height(), 0.02, 10000.0);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	m_player.reset();
//	Vector3 minimapcampos(-500.0f, 500.0f, 500.0f);
//	Vector3 minmaplookat(m_player.getPosition());
//	m_minimapcam.positionCamera(minimapcampos, -Vector3::zAxis(), Vector3::yAxis());
//	glViewport(0,0, Width(), Height());
//	glEnable(GL_DEPTH_TEST);
//	glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
//	glShadeModel(GL_SMOOTH);
//	glEnable(GL_CULL_FACE);
//	m_skysphere = gluNewQuadric();
//	gluQuadricOrientation(m_skysphere, GLU_INSIDE);
//	gluQuadricTexture(m_skysphere, GLU_TRUE);
//	if (!m_skysphere)
//		cout << "couldn't generate sphere" << endl;
//	else
//		cout << "skysphere generated" << endl;
//	setupLights();
//
//	glEnable(GL_NORMALIZE);
//	glHint(GL_PERSPECTIVE_CORRECTION_Hint, GL_NICEST);
//	glHint(GL_GENERATE_MIPMAP, GL_NICEST);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Color(0.15f, 0.15f, 0.15f).rgba());
//	m_ondisplaycount = 0;
//	m_recordstarttime = 0;
//	m_endsequence = false;
//}
//
//void DisplayWindow::setupLights()
//{
//	for (int i = 0; i < 7; i++)
//	{
//		m_light[i] = Light(i);
//	}
//	m_light[0].setPosition(m_player.getPosition());
//	m_light[0].setDirection(-m_player.getDirection());
//	m_light[0].setLightType(Light::SPOT);
//	m_light[0].setConstantAttenuation(1.0f);
//	m_light[0].setLinearAttenuation(0.01f);
//	m_light[0].setQuadraticAttenuation(0.0f);
//	m_light[0].setCutoff(15);
//	m_light[0].setExponent(90);
//	m_light[1].setPosition(m_player.getPosition());
//	m_light[1].setLightType(Light::POint);
//	m_light[1].setConstantAttenuation(1.0f);
//	m_light[1].setLinearAttenuation(0.02f);
//	m_light[1].setQuadraticAttenuation(0.0f);
//	m_light[2].setPosition(Vector3(200, -175, 125));
//	m_light[2].setLightType(Light::POint);
//	m_light[2].setConstantAttenuation(2.0f);
//	m_light[2].setLinearAttenuation(0.05f);
//	m_light[2].setQuadraticAttenuation(0.0f);
//	m_light[3].setPosition(Vector3(850, -175, 125));
//	m_light[3].setLightType(Light::POint);
//	m_light[3].setConstantAttenuation(3.0f);
//	m_light[3].setLinearAttenuation(0.05f);
//	m_light[3].setQuadraticAttenuation(0.0f);
//	m_light[4].setPosition(Vector3(525, -275, 425));
//	m_light[4].setLightType(Light::POint);
//	m_light[4].setConstantAttenuation(3.0f);
//	m_light[4].setLinearAttenuation(0.05f);
//	m_light[4].setQuadraticAttenuation(0.0f);
//	m_light[5].setPosition(Vector3(200, -300, 650));
//	m_light[5].setLightType(Light::POint);
//	m_light[5].setConstantAttenuation(3.0f);
//	m_light[5].setLinearAttenuation(0.05f);
//	m_light[5].setQuadraticAttenuation(0.0f);
//	m_light[6].setPosition(Vector3(850, -300, 600));
//	m_light[6].setLightType(Light::POint);
//	m_light[6].setConstantAttenuation(3.0f);
//	m_light[6].setLinearAttenuation(0.05f);
//	m_light[6].setQuadraticAttenuation(0.0f);
//	m_light[0].setDiffuse(Color(2.0f, 2.0f, 0.0f));
//	m_light[0].setAmbient(Color(1.0f, 1.0f, 0.0f));
//	m_light[1].setDiffuse(Color::red());
//	m_light[2].setDiffuse(Color(0.20f,2.0f,0.8f));
//	m_light[3].setDiffuse(Color(0.20f,2.0f,0.8f));
//	m_light[4].setDiffuse(Color(0.8f,0.20f,2.0f));
//	m_light[5].setDiffuse(Color(0.8f,0.20f,2.0f));
//	m_light[6].setDiffuse(Color(0.8f,0.20f,2.0f));
//}
//
//void DisplayWindow::OnCreate()
//{
//	m_skysphere = gluNewQuadric();
//	gluQuadricOrientation(m_skysphere, GLU_INSIDE);
//	gluQuadricTexture(m_skysphere, GLU_TRUE);
//	Show();
//	checkExtensions();
//	TextureManager& tm = TextureManager::getInstance();
//	tm.setMipMapSettings(m_canautomipmap, m_canautomipmap ? true : false);
//	SpaceSim& ss = SpaceSim::getInstance();
//	ss.readSettingsFile(m_player);
//	GLWindowEx::OnCreate();
//	m_canmultitexture = glex::Supports( "GL_ARB_multitexture" );
//	if (!m_glex.Load()) {
//		static bool warned=false;
//		if (!warned) {
//			App::MsgPrintf("Failed to load OpenGL extensions");
//			warned=true;
//		}
//	}
//	outputChoosenDeviceOptions();
//	ss.loadTextures();
//	ss.loadModels();
//	m_skytexname = ss.getStringSetting("skysphere");
//	m_desiredforwardspeed = ss.getfloatSetting("forwardspeed");
//	m_desiredrollpitchspeed = ss.getfloatSetting("rollpitchspeed");
//
//	ParticleSystemManager& psm = ParticleSystemManager::getInstance();
//	ShaderManager& shadermanager = ShaderManager::getInstance();
//	shadermanager.setGlexPointer(&m_glex);
//	//shadermanager.createShader("pointlight");
//	//shadermanager.loadCompileLinkShader("pointlight" ,"pointlight.vsh", "pointlight.psh");
//	//shadermanager.activateShader("pointlight");
//
//	resetGame();
//	m_gamestatus = "Game is running";
//}
//
//void DisplayWindow::OnDisplay()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf(m_player.getCameraMatrix().m());
//	Vector3 lightpos = m_player.getPosition();
//	m_light[0].setPosition(lightpos);
//	m_light[0].setDirection(-m_player.getDirection());
//	m_light[1].setPosition(lightpos);
//	m_player.calculateBbox();
//	drawSkySphere();
//	if (!m_map.coreDestroyed() && !m_endsequence)
//	{
//		drawPlayerandScene(false);
//		Vector3 min = m_player.getPosition() - Vector3(1.0f,1.0f,1.0f);
//		Vector3 max = m_player.getPosition() + Vector3(1.0f,1.0f,1.0f);
//		Bbox box(min, max);
//		if (m_collisiondetection)
//		{
//			bool collidedspecial = false;
//			Vector3 collisionnormal;
//			if (m_map.checkCollision(box, m_player.getDirection(), m_player, collidedspecial, collisionnormal))
//			{
//				if (!collidedspecial)
//					m_player.setPosition(m_player.getPrevPosition() + collisionnormal * 0.01f);
//				else
//					resetCamera();
//			}
//		}
//		drawMiniMap();
//	}
//	else if (m_endsequence || m_map.coreDestroyed())
//	{
//		if (m_firsttime)
//		{
//			m_sequencestarttime = 0.0f;
//			m_firsttime = false;
//		}
//		drawEndSequence();
//	}
//	SwapBuffers();
//	m_ondisplaycount++;
//}
//
//void DisplayWindow::drawSkySphere()
//{
//	glPushMatrix();
//	{
//		glColor3f(1,1,1);
//		m_glex.glActiveTexture(GL_TEXTURE0);
//		glEnable(GL_TEXTURE_2D);
//		TextureManager& tm = TextureManager::getInstance();
//		tm.activateTexture(m_skytexname);
//		glTranslatef(m_player.getPosition().x(), m_player.getPosition().y(), m_player.getPosition().z());
//		glRotatef(90, 1.0f, 0.0f, 0.0f);
//		static float angle = 0.0f;
//		angle += 0.001f;
//		glRotatef(angle, 0.0f, 0.0f, 1.0f);
//		glFrontFace(GL_CCW);
//		gluSphere(m_skysphere, 9000.0, 25, 25);
//		m_glex.glActiveTexture(GL_TEXTURE0);
//		glDisable(GL_TEXTURE_2D);
//	}
//	glPopMatrix();
//}
//
//void DisplayWindow::drawText()
//{
//	Material mat1(Color::white(), Color::white(), Color::white(), 0.0f, Color::white());
//	mat1.activate();
//	glEnable2D();	
//	glColor4f(1.0,1.0,1.0, 1.0f);
//	int texty = Height();
//	texty -= TEXTHEIGHT;
//	glRasterPos2i(0, texty);
//	Printf(m_gamestatus.c_str());
//	texty -= TEXTHEIGHT;
//	glRasterPos2i(0, texty);
//	Printf("Aspectratio: %f ", Aspect());
//	texty -= TEXTHEIGHT;
//	fps(texty);
//	texty -= TEXTHEIGHT;
//	glRasterPos2i(0, texty);
//	Printf("Player name: %s", m_player.getName().c_str());
//	texty -= TEXTHEIGHT;
//	glColor3f(0.0,1.0,0.0);
//	glRasterPos2i(0, texty);
//	Printf("Nr of Lives: %i", m_player.getLives());
//	texty -= TEXTHEIGHT;
//	glRasterPos2i(0, texty);
//	Printf("Force field strength: %i", m_player.getFFStrenght());
//	texty -= TEXTHEIGHT;
//	glRasterPos2i(0, texty);
//	Printf("Lasers: %i", m_player.getMaxLasers() - m_player.getUsedLasers());
//	texty -= TEXTHEIGHT;
//	glRasterPos2i(0, texty);
//	if (m_collisiondetection)
//	{
//		glColor4fv(Color::red().rgba());
//		Printf("Collision Detection is Active.");
//	}
//	else
//	{
//		glColor4fv(Color::green().rgba());
//		Printf("Collision Detection is NOT Active.");
//	}
//	glColor4fv(Color::white().rgba());
//	crossair();
//	glDisable2D();
//}
//
//void DisplayWindow::drawMiniMap()
//{
//	glClear(GL_DEPTH_BUFFER_BIT);
//	int width = 0;
//	int height = 0;
//	glViewport(width, height, width + 300, height + 300);
//	glMatrixMode(GL_MODELVIEW);
//	Vector3 minmaplookat(m_player.getPosition());
//	Vector3 minmappos(-500.0f, 500.0f, m_player.getPosition().z());
//	m_minimapcam.positionCamera(minmappos, minmaplookat, Vector3::yAxis());
//	glLoadMatrixf(m_minimapcam.createCamera().m());
//	drawMiniMapBackground(width, height);
//	m_map.drawMiniMap(m_glex);
//	m_player.drawMiniMap(m_glex);
//	glColor3f(0.0f, 0.0f, 0.0f);
//	glEnable2D();
//	float linewidth;
//	glGetfloatv(GL_LINE_WIDTH, &linewidth);
//	glLineWidth(4.0f);
//	glBegin(GL_LINE_STRIP);
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glVertex2i(width, height);
//	glVertex2i(width, height + 300);
//	glVertex2i(width + 300, height + 300);
//	glVertex2i(width + 300, height);
//	glVertex2i(width, height);
//	glEnd();
//	glLineWidth(linewidth);
//	glDisable2D();
//	glViewport(0, 0, Width(), Height());
//	glColor4fv(Color::white().rgba());
//}
//
//void DisplayWindow::drawMiniMapBackground(int width, int height)
//{
//	glEnable2D();
//	glDepthMask(false);
//	glBegin(GL_QUADS);
//	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
//	glVertex2i(width, height);
//	glVertex2i(width + 300, height);
//	glVertex2i(width + 300, height + 300);
//	glVertex2i(width, height + 300);
//	glEnd();
//	glDepthMask(true);
//	glDisable2D();
//}
//
//void DisplayWindow::drawPlayerandScene(bool stopdrawingmap )
//{
//	for (int i = 8; i > 0; i--)
//	{
//		m_glex.glActiveTexture(GL_TEXTURE0 + i);
//		glDisable(GL_TEXTURE_2D);
//	}
//	glEnable(GL_LIGHTING);
//	for (int i = 0; i < 7; i++)
//	{
//		m_light[i].enable();
//	}
//	m_glex.glActiveTexture(GL_TEXTURE0);
//	glEnable(GL_TEXTURE_2D);
//	if (!stopdrawingmap)
//	{
//		glPushMatrix();
//		m_map.draw(m_glex);
//		glPopMatrix();
//	}
//	else
//	{
//		m_map.cleanup();
//	}
//	m_glex.glActiveTexture(GL_TEXTURE0);
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_LIGHTING);
//	m_player.drawLasersAndScorchmarks(m_glex);
//	ParticleSystemManager& psm = ParticleSystemManager::getInstance();
//	psm.drawSystems(m_glex);
//	glEnable(GL_LIGHTING);
//	m_player.draw(m_glex);
//	glDisable(GL_LIGHTING);
//	for (int i = 8; i > 0; i--)
//	{
//		m_glex.glActiveTexture(GL_TEXTURE0 + i);
//		glDisable(GL_TEXTURE_2D);
//	}
//	drawText();
//}
//
//void DisplayWindow::drawEndSequence()
//{
//	static bool stopdrawingmap = false;
//	m_endsequence = true;
//	drawPlayerandScene(stopdrawingmap);
//
//	static bool enddestination = false;
//	static bool emittercreated = false;
//	m_sequencestarttime += m_timestamp;
//	if (m_sequencestarttime > 15.0f  && m_sequencestarttime < (15.0f + m_timestamp))
//	{
//		if (m_controller)
//		{
//			delete m_controller;
//			m_controller = new PlaybackController();
//		}
//		m_player.positionCamera(Vector3(525.0f, -575.0f, 850.0f), Vector3(525.0f, -575.0f, 950.0f), Vector3::yAxis());
//		m_player.setView(Player::thirdperson);
//		m_map.noGunsSpecials();
//	}
//	else if (m_player.getPosition().z() < 3500.0)
//	{
//		m_controller->setRightUpDown(10.0f);
//	}
//	else if (m_player.getPosition().z() > 3500.0 && !enddestination)
//	{
//		m_controller->setRightUpDown(0.0f);
//		m_player.positionCamera(m_player.getPosition(), Vector3(525.0f, -575.0f, -950.0f), Vector3::yAxis());
//		enddestination = true;	
//	}
//	else if (enddestination && !emittercreated)
//	{
//		ParticleEmitter* pe = ParticleSystemManager::getInstance().createEmitter(Vector3(525.0f,-350.0f,1475.0f), Vector3::yAxis());
//		pe->setMaxParticles(7500.0f);
//		pe->setEmissionRate(750.0f);
//		pe->setParticleStartColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
//		pe->setParticleKey1Color(Color(1.0f, 1.0f, 0.0f, 0.8f));
//		pe->setParticleKey1Pos(0.001f);
//		pe->setParticleKey2Color(Color(1.0f, 0.5f, 0.0f, 0.6f));
//		pe->setParticleKey2Pos(0.25f);
//		pe->setParticelEndColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
//		pe->setParticleLifetime(10.0f);
//		pe->setLifeTime(15.0f);
//		pe->setParticleSize(7.5f);
//		pe->setStartVelocity(150.0f);
//		pe->setSpreadAngle(180.0f);
//		pe->setAlive(true);
//		emittercreated = true;
//	}
//	else if (m_sequencestarttime > 25.0f  && m_sequencestarttime < (25.0f + m_timestamp))
//	{
//		stopdrawingmap = true;
//	}
//	else if (m_sequencestarttime > 35.0f  && m_sequencestarttime < (35.0f + m_timestamp))
//		m_gamestatus = "Game has ended restart it to play again";
//}
//
//void DisplayWindow::OnIdle()
//{
//	m_controller->update();
//	if (!m_playback)
//	{
//		m_timestamp = (float)App::GetDeltaTime();
//		m_buttonpresstime -= m_timestamp;
//		m_controller->recordInput(m_timestamp, m_ondisplaycount);
//	}
//	else 
//	{
//		if (m_controller)
//			delete m_controller;
//		m_controller = new PlaybackController();
//		if (m_ondisplaycount < m_recorder->getPlaybackLength() && !m_endsequence)
//		{
//			if (m_ondisplaycount > 1)
//				setPlaybackData();
//			else
//				cout << "skipping frame zero to see if playback will work!!!" << endl;
//		}
//		else
//		{
//			m_controller->noInput();
//			cout << "Playback has reached the end of the playback file or end sequence has begun!" << endl;
//			m_playback = false;
//		}
//	}
//	m_map.move(m_timestamp, m_player);
//	m_player.updateLasers(m_timestamp, m_map);
//	m_controller->update();
//	checkPressedButtons();
//	m_player.pitch(((m_desiredrollpitchspeed * m_controller->getLeftUpDown()))/(1/m_timestamp));
//	m_player.roll(((m_desiredrollpitchspeed * m_controller->getLeftLR()))/(1/m_timestamp));
//	m_player.moveOverPlane(m_strafe, m_updown, -(m_desiredforwardspeed * m_controller->getRightUpDown())/(1/m_timestamp));
//	ParticleSystemManager& psm = ParticleSystemManager::getInstance();
//	psm.proceed(m_timestamp);
//	Redraw();
//}
//
//void DisplayWindow::setPlaybackData()
//{
//	m_timestamp = m_recorder->getFrameTime(m_ondisplaycount);
//	m_controller->setLeftUpDown(m_recorder->getLeftUD(m_ondisplaycount));
//	m_controller->setLeftLR(m_recorder->getLeftLR(m_ondisplaycount));
//	m_controller->setRightUpDown(m_recorder->getRightUD(m_ondisplaycount));
//	m_controller->setRightLR(m_recorder->getRightLR(m_ondisplaycount));
//	m_controller->setButton1(m_recorder->getButton1(m_ondisplaycount));
//	m_controller->setButton2(m_recorder->getButton2(m_ondisplaycount));
//	m_controller->setButton3(m_recorder->getButton3(m_ondisplaycount));
//	m_controller->setButton4(m_recorder->getButton4(m_ondisplaycount));
//	m_controller->setButton5(m_recorder->getButton5(m_ondisplaycount));
//	m_controller->setButton6(m_recorder->getButton6(m_ondisplaycount));
//	m_controller->setButton7(m_recorder->getButton7(m_ondisplaycount));
//	m_controller->setButton8(m_recorder->getButton8(m_ondisplaycount));
//	m_controller->setButton9(m_recorder->getButton9(m_ondisplaycount));
//	m_controller->setButton10(m_recorder->getButton10(m_ondisplaycount));
//}
//
//void DisplayWindow::OnResize(int w, int h)
//{
//	SetSize(w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	if (0 == Height())
//		gluPerspective((double)toDegree(m_fov), Width() / 1.0f, 0.02f, 10000.0f);
//	else if (m_fullscreen)
//		gluPerspective((double)toDegree(m_fov), 1024.0f / 768.0f, 0.02f, 10000.0f);
//	else
//		gluPerspective((double)toDegree(m_fov), (float)Width() / (float)Height(), 0.02f, 10000.0f);
//	changeViewPort();
//}			
//
//void DisplayWindow::OnKeyboard(int key, bool down)
//{
//	handleKeyboardPlayerInput(key, down);
//	switch(key)
//	{
//	case 'c': 
//	case 'C':
//		if (down)
//		{
//			m_collisiondetection = !m_collisiondetection;
//		}
//		break;
//	case 'f': 
//	case 'F':
//		if (down)
//		{
//			m_fullscreen = !m_fullscreen;
//			SetFullscreen(m_fullscreen);
//			changeViewPort();
//		}
//		break;
//	case 'r':
//	case 'R':
//		if(down && !m_recordingstarted)
//		{
//			m_recorder = new Recorder("..\\Recorded games\\rec.dat", true);
//			m_recordingstarted = true;
//			m_controller->setRecording(true, m_recorder);
//			cout << "record function is started! Now recording input to \"..\\Recorded games\\rec.dat\"" << endl;
//			m_recordstarttime = App::GetTime();
//			resetGame();
//			ParticleSystemManager::getInstance().cleanup();
//			m_ondisplaycount = 0;
//			m_gamestatus = "This game is running and is being recorded";
//		}
//		else if(down && m_recordingstarted)
//		{
//			cout << "record function is stopped!" << endl;
//		}
//		break;
//	case 'p':
//	case 'P':
//		if(down && !m_playback)
//		{
//			if (m_recorder)
//			{
//				delete m_recorder;
//			}
//			m_recorder = new Recorder("..\\Recorded games\\rec.dat", false);
//			cout << "Playback function is started! from file \"..\\Recorded games\\rec.dat\"" << endl;
//			cout << "Attempting to read recorded file ..\\Recorded games\\rec.dat" << endl;
//			m_recorder->fillReplayData();
//			cout << "file with recorded data: ..\\Recorded games\\rec.dat read in!!!" << endl;
//			resetGame();
//			ParticleSystemManager::getInstance().cleanup();
//			m_ondisplaycount = 0;
//			m_playback = true;
//			m_gamestatus = "Game is being played back from a previously recorded one";
//		}
//		else if(down && m_playback)
//		{
//			cout << "Playback function is stopped!" << endl;
//		}
//		break;
//	case 27:
//		Close();
//		break;
//	}
//}
//
//void DisplayWindow::handleKeyboardPlayerInput(int key, bool down )
//{
//	switch (key)
//	{
//	case'w':
//	case'W':
//		if (down)
//			m_controller->setRightUpDown(1.0f);
//		else
//			m_controller->setRightUpDown(0.0f);
//		break;
//	case's':
//	case'S':
//		if (down)
//			m_controller->setRightUpDown(-1.0f);
//		else
//			m_controller->setRightUpDown(0.0f);
//		break;
//	case'a':
//	case'A':
//		if (down)
//			m_controller->setLeftLR(1.0f);
//		else
//			m_controller->setLeftLR(0.0f);
//		break;
//	case'd':
//	case'D':
//		if (down)
//			m_controller->setLeftLR(-1.0f);
//		else
//			m_controller->setLeftLR(0.0f);
//		break;
//	case'q':
//	case'Q':
//		if (down)
//			m_controller->setLeftUpDown(-1.0f);
//		else
//			m_controller->setLeftUpDown(0.0f);
//		break;
//	case'e':
//	case'E':
//		if (down)
//			m_controller->setLeftUpDown(1.0f);
//		else
//			m_controller->setLeftUpDown(0.0f);
//		break;
//	case 37:
//		if(down)
//			m_strafe -= 1;
//		m_player.moveOverPlane(m_strafe, m_updown, 0);
//		break;
//	case 38:
//		if(down)
//			m_updown += 1;
//		m_player.moveOverPlane(m_strafe, m_updown, 0);
//		break;
//	case 39:
//		if(down)
//			m_strafe += 1;
//		m_player.moveOverPlane(m_strafe, m_updown, 0);
//		break;
//	case 40:
//		if(down)
//			m_updown -= 1;
//		m_player.moveOverPlane(m_strafe, m_updown, 0);
//		break;
//	}
//}
//
//void DisplayWindow::OnMouseButton(gxbase::GLWindow::MouseButton button, bool down)
//{
//	if (MBLeft == button && down)
//	{
//		m_controller->setButton1(true);
//	}
//	else
//	{
//		m_controller->setButton1(false);
//	}
//}
//
//void DisplayWindow::OnMouseMove(int x, int y)
//{
//	//cout << "x position: " << x << endl;
//	//cout << "y position: " << y << endl;
//}
//
//void DisplayWindow::fps(int textheight)
//{
//	static int counterfps = 0;
//	if ((App::GetTime() - m_previoussecond) < 1)
//		counterfps++;
//	else
//	{
//		m_fps = counterfps;
//		m_previoussecond = (int)App::GetTime();
//		counterfps = 0;
//	}
//	glRasterPos2i(0, textheight);
//	Printf("FPS: %i", m_fps);
//}
//
//void DisplayWindow::changeViewPort()
//{
//	if (m_fullscreen)
//		glViewport(0, 0, 1024, 768);
//	else
//		glViewport(0,0, Width(), Height());
//}
//
//void DisplayWindow::glEnable2D()
//{
//	int vPort[4];
//
//   glGetintegerv(GL_VIEWPORT, vPort);
//
//   glMatrixMode(GL_PROJECTION);
//   glPushMatrix();
//   glLoadIdentity();
//
//   glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
//   glMatrixMode(GL_MODELVIEW);
//   glPushMatrix();
//   glLoadIdentity();
//}
//
//void DisplayWindow::glDisable2D()
//{
//   glMatrixMode(GL_PROJECTION);
//   glPopMatrix();   
//   glMatrixMode(GL_MODELVIEW);
//   glPopMatrix();	
//}
//
//void DisplayWindow::setWireframe(bool wireframe)
//{
//	m_wireframe = wireframe;
//	if(m_wireframe)
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	else
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
//
//void DisplayWindow::checkPressedButtons()
//{
//	if (m_controller->getButton1())
//	{
//		m_player.fireLaser();
//	}
//	if (m_controller->getButton2())
//	{
//		if(!m_recordingstarted)
//		{
//			m_recorder = new Recorder("..\\Recorded games\\rec.dat", true);
//			m_recordingstarted = true;
//			m_controller->setRecording(true, m_recorder);
//			cout << "record function is started! Now recording input to \"..\\Recorded games\\rec.dat\"" << endl;
//			m_recordstarttime = App::GetTime();
//			resetGame();
//			ParticleSystemManager::getInstance().cleanup();
//			m_ondisplaycount = 0;
//			m_gamestatus = "This game is running and is being recorded";
//		}
//	}
//	if (m_controller->getButton3())
//	{
//		if (m_buttonpresstime < 0.0)
//			m_player.switchviews();
//		else
//			m_buttonpresstime = 3.0f;
//	}	
//	if (m_controller->getButton4())
//	{
//		setWireframe(true);
//	}
//	else
//	{
//		setWireframe(false);
//	}
//	if (m_controller->getButton5())
//	{
//	}
//	if (m_controller->getButton6())
//	{
//	}
//	if (m_controller->getButton7())
//	{
//	}
//	if (m_controller->getButton8())
//	{
//	}
//	if (m_controller->getButton9())
//	{
//	}
//	if (m_controller->getButton10())
//	{
//		if (m_playback)
//			cout << "Closing the application from playback on frame: " << m_ondisplaycount << endl;
//		Close();
//	}
//}
//
//void DisplayWindow::drawAxis()
//{
//	//X axis
//	glBegin(GL_LINES);
//		glColor3f(1.0f, 0.0f,0.0f);
//		(-1000.0f, 0.0f, 0.0f);
//		(1000.0f, 0.0f, 0.0f);
//	glEnd();
//	//Y axis
//	glBegin(GL_LINES);
//		glColor3f(0.0f, 1.0f, 0.0f);
//		(0.0f, -1000.0f, 0.0f);
//		(0.0f, 1000.0f, 0.0f);
//	glEnd();
//	//Z axis
//	glBegin(GL_LINES);
//		glColor3f(0.0f, 0.0f, 1.0f);
//		(0.0f, 0.0f, -1000.0f);
//		(0.0f, 0.0f, 1000.0f);
//	glEnd();
//}
//
//void DisplayWindow::crossair()
//{
//	glColor3f(1.0f,1.0f,1.0f);
//	glRasterPos2i(Width()/2-4, Height()/2-4);
//	Printf("+");
//}
//
//void DisplayWindow::resetCamera()
//{
//	m_player.reset();
//}
