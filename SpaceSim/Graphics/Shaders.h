#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"

#include "Core/tinyxml2.h"
#include <d3d11.h>
#include <string>

class DeviceManager;

class VertexShader
{
public:
    VertexShader() {}
    void cleanup() { m_shader->Release(); }

    void deserialise(const tinyxml2::XMLElement* element);

    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() const { return m_fileName; }
    const ID3DBlob* getShaderBlob() const { return m_vertexShaderBlob; }

    ID3D11VertexShader* getShader() const { return m_shader; }

    HASH_ELEMENT_DEFINITION;
private:
    ID3DBlob* m_vertexShaderBlob;
    ID3D11VertexShader* m_shader;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
};

class HullShader
{
public:
    HullShader() {}
    void cleanup() { m_shader->Release(); }

    void deserialise(const tinyxml2::XMLElement* element);
    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() { return m_fileName; }

    ID3D11HullShader* getShader() const { return m_shader; }

    HASH_ELEMENT_DEFINITION;
private:
    ID3D11HullShader* m_shader;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
};

class DomainShader
{
public:
    DomainShader() {}
    void cleanup() { m_shader->Release(); }
    
    void deserialise(const tinyxml2::XMLElement* element);
    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() { return m_fileName; }

    ID3D11DomainShader* getShader() const { return m_shader; }

    HASH_ELEMENT_DEFINITION;
private:
    ID3D11DomainShader* m_shader;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
};

class GeometryShader
{
public:
    GeometryShader() {}
    void cleanup() { m_shader->Release(); }

    void deserialise(const tinyxml2::XMLElement* element);
    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() { return m_fileName; }

    ID3D11GeometryShader* getShader() const { return m_shader; }

    HASH_ELEMENT_DEFINITION;
private:
    ID3D11GeometryShader* m_shader;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
};

class PixelShader
{
public:
    PixelShader() {}
    void cleanup() { m_shader->Release(); }

    void deserialise(const tinyxml2::XMLElement* element);
    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() { return m_fileName; }

    ID3D11PixelShader* getShader() const { return m_shader; }

    HASH_ELEMENT_DEFINITION;
private:
    ID3D11PixelShader* m_shader;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
};

class ComputeShader
{
public:
    ComputeShader() {}
    void cleanup() { m_shader->Release(); }

    void deserialise(const tinyxml2::XMLElement* element);
    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() { return m_fileName; }

    ID3D11ComputeShader* getShader() const { return m_shader; }

    HASH_ELEMENT_DEFINITION;
private:
    ID3D11ComputeShader* m_shader;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
};
