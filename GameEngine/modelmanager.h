#ifndef MODELMANGER_H
#define MODELMANGER_H

#include <string>
#include <vector>
#include <map>

class Model;
class Resource;

class ModelManager
{
public:
	ModelManager() {}
	~ModelManager();

    void cleanup();

    Model* LoadModel(Resource* resource, const std::string& fileName) const;

protected:
private:
    typedef std::pair<unsigned int, Model*> GeometryTreePair;
    typedef std::map< unsigned int, Model* >::iterator GeometryTreeIterator;
    std::map< unsigned int, Model* > m_loadedModels;
};
#endif