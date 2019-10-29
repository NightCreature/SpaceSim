#pragma once

//#include "Core/Profiler/GPUProfiler.h"
//#include <d3d11.h>

//struct D3D11GPUProflingEvent : public ProfilingEvent
//{
//    void Begin();
//    void End();
//
//    void CreateEvent();
//private:
//    ID3D11Query* m_querry;
//};
//
//class D3D11GPUProfiler : public Profiler
//{
//public:
//    D3D11GPUProfiler() {}
//    virtual ~D3D11GPUProfiler() {}
//
//    virtual void Init(Resource* resource) override;
//    virtual void CaptureEvent(const ProfilingEvent& event) override;
//    virtual void Start() override;
//    virtual void End() override;
//
//private:
//};