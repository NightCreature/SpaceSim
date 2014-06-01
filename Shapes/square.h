#ifndef SQUARE_H
#define SQUARE_H
#include "vector2.h"

#include "..\SpaceSim\Camera.h"
#include "model.h"

class Camera;
class Matrix44;

class Square : public Model
{
public:
	Square(Resource* resource) : Super(resource) {}
	Square(Resource* resource, float radius);
	~Square() {}

    void initialise(const ShaderInstance& shaderInstance);

	void transform (Camera& cam);
    protected:
private:
	Vector2 m_lowerleft;
	Vector2 m_upperright;
};
#endif