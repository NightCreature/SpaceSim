#pragma once

#include <D3D11.h>

class DeviceManager;

class IndexBuffer
{
public:
    IndexBuffer(void) { m_buffer = 0;}
    ~IndexBuffer(void) { cleanup(); }

    void createBuffer(const DeviceManager& deviceManager, unsigned int bufferSize, void* data, bool dynamic, unsigned int bindFlag );
    void cleanup();

    ID3D11Buffer* getBuffer() const { return m_buffer; }
    void setNumberOfIndecis(unsigned int numberOfIndecis) { m_numberOfIndecis = numberOfIndecis; }
    unsigned int getNumberOfIndecis() const { return m_numberOfIndecis; }
private:
    ID3D11Buffer* m_buffer;
    unsigned int m_numberOfIndecis;
};
