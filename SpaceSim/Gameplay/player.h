#ifndef PLAYER_H
#define PLAYER_H

#include "Math/vector3.h"
#include <string>
#include <list>
#include "Graphics/Camera.h"
#include "Gameplay/laser.h"
#include "Gameplay/scorchmark.h"
#include "Gameplay/particleemitter.h"
#include "Gameplay/bbox.h"
#include "Graphics/Camera.h"
#include "Gameplay/GameObject.h"
#include "Core/tinyxml2.h"

class MapLoader;
class RenderInstance;
class Input;
class CameraManager;

class Player : public GameObject
{
public:
    enum class camstate
    {
        firstperson,
        thirdperson,
    };

    Player(Resource* resource) : GameObject(resource) { m_name = "player"; }
    ~Player();

    void deserialise( const tinyxml2::XMLElement* node );
    void initialize();
    void reset();
    void cleanup();

    void loadFromFile(const std::string& filename);
    void save(const std::string& filename) const;

    void update( float elapsedTime, const Input& input);

    void draw();
    //void drawForceField(GLUquadric* sphere, const Vector3& translate);
    void drawMiniMap();
    void drawLasersAndScorchmarks();

    virtual bool collision( const Bbox& bbox, const Vector3& dir );
    virtual float collision( const Vector3& position, const Vector3& dir, Vector3& normal );
    virtual void handleMessage( const MessageSystem::Message& msg );

    void getsHit();
    void getsKilled();

    void fireLaser();
    void updateLasers(float elapsedtime, MapLoader& m_map);
    void createScorchMark(const Vector3& pos, const Vector3 &normal);

    //void positionCamera(const Vector3& eye, const Vector3& lookat, const Vector3& up) {m_camera->positionCamera(eye, lookat, up); m_prevposition = m_position; m_position = m_camera->getEye(); m_direction = m_camera->getLookAt();}
    //void roll(float angle) {m_camera->roll(angle); m_prevposition = m_position; m_position = m_camera->getEye(); m_direction = m_camera->getLookAt();}
    //void pitch(float angle) {m_camera->pitch(angle); m_prevposition = m_position; m_position = m_camera->getEye(); m_direction = m_camera->getLookAt();}
    //void yaw(float angle) {m_camera->yaw(angle); m_prevposition = m_position; m_position = m_camera->getEye(); m_direction = m_camera->getLookAt();}
    //void moveOverPlane(float deltau, float deltav, float deltan) {m_camera->moveOverPlane(deltau, deltav, deltan); m_prevposition = m_position; m_position = m_camera->getEye(); m_direction = m_camera->getLookAt();}
    void switchviews();

    bool isGameOver () {return m_gameover;}
    Vector3 getPosition() const {return m_position;}
    Vector3 getPrevPosition() const {return m_prevposition;}
    Vector3 getDirection() const {return m_direction;}
    int getLives() const {return m_lives;}
    int getFFStrenght() const {return m_ffstrength;}
    float getLaserSpeed() const {return m_laserspeed;}
    int getMaxLasers() const {return m_maxlasers;}
    int     getUsedLasers() const {return m_count;}
    std::string getName() const { return "player"; }
    //void setPosition(const Vector3& pos) {m_position = pos; m_camera->setEye(pos);}
    void setDirection(const Vector3& dir) {m_direction = dir;}
    void setStartPosition(const Vector3& pos) {m_startposition = pos;}
    void setLaserSpeed(const float speed) {m_laserspeed = speed;}
    void setMaxLasers(const int maxlasers) {m_maxlasers = maxlasers;}
    void setView(const camstate state) {m_camstate = state;}
    Matrix44 getCameraMatrix() const  {return m_camera->getCamera();}
    Bbox getBoundingBox() { return Bbox(); } //Needs to return the drawable bbox of this object
protected:
private:
    bool isPlayerDead();
    void eraseLaser(std::list<Laser*>::iterator& it, Laser* l);

    std::string m_defaultname;
    std::string m_gameObjectName;
    std::list<Laser*> m_lasers;
    std::list<Laser*>::iterator m_lasersit;
    std::list<ScorchMark*> m_scorchmarks;
    std::list<ScorchMark*>::iterator m_scorchit;
    Vector3 m_position;
    Vector3 m_startposition;
    Vector3 m_prevposition;
    Vector3 m_direction;
    const Camera* m_camera;
    camstate m_camstate;
    int m_lives;
    int m_ffstrength;
    int m_maxlasers;
    int m_count;
    int m_defaultlives;
    int m_defaultffstrength;
    float m_laserspeed;
    float m_lasergentime;
    float m_ffangle;
    bool m_gameover;
    bool m_hit;
    //GLUquadric*		 m_forcefieldinside;
    //GLUquadric*		 m_forcefieldoutside;

    //ECS::Entity m_entity;
};
#endif