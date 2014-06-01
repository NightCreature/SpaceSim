#ifndef BOX_H
#define BOX_H

#include "model.h"
#include "Vector3.h"
#include "face.h"

class Box : public Model
{
public:
	Box(Resource* resource);
	Box(Resource* resource, const Vector3& lowerleft, const Vector3& upperright, bool dynamic, bool gentexcoords = false);
	~Box();
    void initialise(const ShaderInstance& shaderInstance);

    void setTexture(std::string texname) {m_texname = texname;}
	std::string getTexture() const {return m_texname;}
	void setLowerLeft(const Vector3& ll) {m_lowerleft = ll;}
	void setUpperRight(const Vector3& ur) {m_upperright = ur;}
	Vector3 getLowerLeft() const {return m_lowerleft;}
	Vector3 getUpperRight() const {return m_upperright;}
	void setGenTexs(bool gen) {m_gentexcoords = gen;}
	bool getGenTexs() const {return m_gentexcoords;}
    void setDynamic() { m_dynamic = true; }

protected:
private:
	std::string	m_texname;
	Vector3		m_lowerleft;
	Vector3		m_upperright;
    bool        m_dynamic;
	bool		m_gentexcoords;
};
#endif