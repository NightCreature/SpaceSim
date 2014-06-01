#include "shadermanager.h"

#include <iostream>
#include <fstream>



ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager instance;
	return instance;
}
