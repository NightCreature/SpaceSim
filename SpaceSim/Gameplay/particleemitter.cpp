#include "Gameplay/particleemitter.h"
#include "Gameplay/particlesystemmanager.h"
#include "Gameplay/particle.h"
#include "Gameplay/square.h"
#include "Math/matrix33.h"
#include "Graphics/texturemanager.h"
#include <iostream>

#include "Core/Resource/RenderResource.h"

#include "D:\SDK\Demo\SpaceSim\SpaceSim\Graphics\D3DDebugHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/Effect.h"
#include "Graphics/EffectCache.h"
#include "Graphics/ShaderCache.h"

namespace ParticleSystem
{

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleEmitterComponentBased::initialise(Resource* resource)
{
    uint32 numberParticles = 100000;
    m_resource = resource;
    m_particleData.createParticles(numberParticles);

    D3D11_BUFFER_DESC bufferDescriptor;
    ZeroMemory(&bufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    bufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescriptor.ByteWidth = (unsigned int)numberParticles * ((4 * sizeof(float) * 2) + sizeof(float));
    bufferDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDescriptor.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDescriptor.StructureByteStride = ((4 * sizeof(float) * 2) + sizeof(float));

    auto resourceHelper = RenderResourceHelper(m_resource);
    ID3D11Device* deviceContext = resourceHelper.getWriteableResource().getDeviceManager().getDevice();
    deviceContext->CreateBuffer(&bufferDescriptor, 0, &m_particleDataBuffer);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDescriptor;
    ZeroMemory(&srvDescriptor, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDescriptor.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDescriptor.Buffer.FirstElement = 0;
    srvDescriptor.Buffer.NumElements = static_cast<uint32>(numberParticles);
    deviceContext->CreateShaderResourceView(m_particleDataBuffer, &srvDescriptor, &m_srv);


    uint32* indecis = new uint32[6 * numberParticles];
    for (uint32 counter = 0; counter < numberParticles * 6; counter += 6)
    {
        indecis[counter] = (counter / 6 * 4);
        indecis[counter + 1] = (counter / 6 * 4) + 2;
        indecis[counter + 2] = (counter / 6 * 4) + 1;
        indecis[counter + 3] = (counter / 6 * 4) + 1;
        indecis[counter + 4] = (counter / 6 * 4) + 2;
        indecis[counter + 5] = (counter / 6 * 4) + 3;
    }
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
    initData.pSysMem = indecis;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    D3D11_BUFFER_DESC indexBufferDescriptor;
    ZeroMemory(&indexBufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDescriptor.ByteWidth = 6 * numberParticles * sizeof(uint32);
    indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDescriptor.CPUAccessFlags = 0;
    indexBufferDescriptor.MiscFlags = 0;
    deviceContext->CreateBuffer(&indexBufferDescriptor, &initData, &m_indexBuffer);

    delete[] indecis;

    m_generators.push_back(new PointGenerator());
    m_updaters.push_back(new EulerUpdater());
    m_emmisionRate = 5500;


    //Test CODE
    ID3D11Device* device = resourceHelper.getWriteableResource().getDeviceManager().getDevice();
    D3D11_BUFFER_DESC wvpBufferDescriptor;
    ZeroMemory(&wvpBufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    wvpBufferDescriptor.ByteWidth = sizeof(float) * 16 * 3;
    wvpBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    if (FAILED(device->CreateBuffer(&wvpBufferDescriptor, 0, &m_constantBuffers[0])))
    {
        MSG_TRACE_CHANNEL("PARTICLE SYSTEM ERROR", "failed to create the World, View and Porjection buffer for this effect");
        return;
    }

    D3DDebugHelperFunctions::SetDebugChildName(m_constantBuffers[0], "Constant Buffer 0  Particle System");

    D3D11_BUFFER_DESC materialDescriptor;
    ZeroMemory(&materialDescriptor, sizeof(D3D11_BUFFER_DESC));
    materialDescriptor.ByteWidth = sizeof(float) * 16;
    materialDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = device->CreateBuffer(&materialDescriptor, 0, &m_constantBuffers[1]);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("PARTICLE SYSTEM ERROR", "failed to create the material buffer for this effect: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
        return;
    }
    D3DDebugHelperFunctions::SetDebugChildName(m_constantBuffers[1], "Constant Buffer 1  Particle System");
}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleEmitterComponentBased::update(double elapsedTime, const Matrix44& view, const Matrix44& projection, const Matrix44& inverseView)
{
    size_t numberOfParticlesToGenerate = static_cast<size_t>(elapsedTime * m_emmisionRate);
    size_t startingParticleIndex = m_particleData.m_aliveParticles;
    size_t lastParticleIndex = std::min(startingParticleIndex + numberOfParticlesToGenerate, m_particleData.m_maxParticles - 1);


    WVPBufferContent wvp;
    wvp.m_world.identity();
    wvp.m_view = view;
    wvp.m_projection = projection;

    for (auto&& generator : m_generators)
    {
        generator->generate(startingParticleIndex, lastParticleIndex, m_particleData);
    }

    for (size_t counter = startingParticleIndex; counter < lastParticleIndex; ++counter)
    {
        m_particleData.wakeParticle(counter);
    }

    for (auto&& updater : m_updaters)
    {
        updater->update(elapsedTime, m_particleData);
    }

    //Run a last updater that is fixed and checks if particles are alive still
    for (size_t counter = 0; counter < m_particleData.m_aliveParticles; ++counter)
    {
        m_particleData.m_lifeTimes[counter] -= static_cast<float>(elapsedTime);
        if (m_particleData.m_lifeTimes[counter] < 0.0f)
        {
            m_particleData.killParticle(counter);
        }
    }

    //Finally extract render information here and send of to render list
    struct PartticleChacheData
    {
        float x, y, z, w;
        float r, g, b, a;
        float size;
    };
    PartticleChacheData* cache = new PartticleChacheData[m_particleData.m_aliveParticles];
    for (size_t counter = 0; counter < m_particleData.m_aliveParticles; ++counter)
    {
        cache[counter].x = m_particleData.m_positionData[counter].x();
        cache[counter].y = m_particleData.m_positionData[counter].y();
        cache[counter].z = m_particleData.m_positionData[counter].z();
        
        cache[counter].a = m_particleData.m_startColor[counter].x();
        cache[counter].r = m_particleData.m_startColor[counter].y();
        cache[counter].g = m_particleData.m_startColor[counter].z();
        cache[counter].b = m_particleData.m_startColor[counter].w();

        cache[counter].size = m_particleData.m_size[counter];
    }

    RenderResourceHelper helper(m_resource);
    auto&& deviceContext = helper.getWriteableResource().getDeviceManager().getDeviceContext();
    //deviceContext->UpdateSubresource(m_particleDataBuffer, 0, 0, (void*)&cache, 0, 0);
    D3D11_MAPPED_SUBRESOURCE out;
    HRESULT hr = deviceContext->Map(m_particleDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &out);
    if (hr != S_OK)
    {
        MSG_TRACE("Something broke hard");
    }
    memcpy(out.pData, cache, sizeof(PartticleChacheData) * m_particleData.m_aliveParticles);
    deviceContext->Unmap(m_particleDataBuffer, 0);
    delete[] cache;//shouldn't allocate and destroy memory if we can avoid this
    
    //This is test code to draw should be really refactored
    {
        PROFILE_EVENT("ParticleEmitter::SubmitInstance", DarkBlue);

        const Effect* effect = helper.getResource().getEffectCache().getEffect(hashString("ParticleSystem.xml"));
        const Technique* technique = effect->getTechnique(hashString("default"));
        deviceContext->UpdateSubresource(m_constantBuffers[0], 0, 0, (void*)&wvp, 0, 0);
        deviceContext->UpdateSubresource(m_constantBuffers[1], 0, 0, (void*)&inverseView, 0, 0);
        

        //this will crash, also we shouldnt set this if the shader id hasnt changed from the previous set
        auto&& shaderCache = helper.getResource().getShaderCache();
        deviceContext->VSSetShader(shaderCache.getVertexShader(technique->getVertexShader()) ? shaderCache.getVertexShader(technique->getVertexShader())->getShader() : nullptr, nullptr, 0);
        deviceContext->HSSetShader(shaderCache.getHullShader(technique->getHullShader()) ? shaderCache.getHullShader(technique->getHullShader())->getShader() : nullptr, nullptr, 0);
        deviceContext->DSSetShader(shaderCache.getDomainShader(technique->getDomainShader()) ? shaderCache.getDomainShader(technique->getDomainShader())->getShader() : nullptr, nullptr, 0);
        deviceContext->GSSetShader(shaderCache.getGeometryShader(technique->getGeometryShader()) ? shaderCache.getGeometryShader(technique->getGeometryShader())->getShader() : nullptr, nullptr, 0);
        deviceContext->PSSetShader(shaderCache.getPixelShader(technique->getPixelShader()) ? shaderCache.getPixelShader(technique->getPixelShader())->getShader() : nullptr, nullptr, 0);

        //technique->setupTechnique();
        deviceContext->VSSetConstantBuffers(0, 2, m_constantBuffers);

        // Set vertex buffer stride and offset.

        {
            uint32 generateVerticesCount = 6; //In the case of a particle system we assume to generate quads in the vertex shader

            deviceContext->VSSetShaderResources(0, 1, &m_srv);
            deviceContext->IASetInputLayout(nullptr);
            deviceContext->IASetVertexBuffers(0, 0, nullptr, 0, 0);
            deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
            deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            deviceContext->DrawIndexed((uint32)m_particleData.m_aliveParticles * generateVerticesCount, 0, 0);
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   Simple Euler motion updater
//! @remark
//-----------------------------------------------------------------------------
void EulerUpdater::update(double elapsedTime, const ParticleData& particleData)
{
    float dt = static_cast<float>(elapsedTime);
    for (size_t counter = 0; counter < particleData.m_aliveParticles; ++counter)
    {
        particleData.m_velocityData[counter] += m_acceleration * dt;
        particleData.m_positionData[counter] += particleData.m_velocityData[counter] * dt;
    }
}

//-----------------------------------------------------------------------------
//! @brief   Simple single point spawner
//! @remark  Only updates the start positionn of a particle
//-----------------------------------------------------------------------------
void PointGenerator::generate(size_t start, size_t end, const ParticleData& particleData)
{
    for (size_t counter = start; counter < end; ++counter)
    {
        particleData.m_positionData[counter] = m_spawnPosition;
        //TODO REMOVE THIS TEST CODE BELOW
        particleData.m_lifeTimes[counter] = 10.0f; //Make this particle live for 100 seconds
    }
}

}