#pragma once

#include "Core/Types/TypeHelpers.h"
#include <string>

namespace D3DDebugHelperFunctions
{
    inline const char* D3DErrorCodeToString( HRESULT errorCode )
    {
        switch (errorCode)
        {
        case D3D11_ERROR_FILE_NOT_FOUND: return "FILE NOT FOUND";
            case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS: return "TOO MANY UNIQUE STATE OBJECTS";
            case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS: return "TOO MANY UNIQUE VIEW OBJECTS";
            case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD: return "DEFERRED CONTEXT MAP WITHOUT INITIAL DISCARD";
            //case D3DERR_INVALIDCALL: return "INVALID CALL";
            //case D3DERR_WASSTILLDRAWING: return "WAS STILL DRAWING TO SURFACE";
            case E_FAIL: return "GENERAL FAILURE"; 
            case E_INVALIDARG: return "INVALID ARGUMENT";
            case E_OUTOFMEMORY: return "OUT OF MEMORY";
            case S_FALSE: return "None Standard Succes";
            case S_OK: return "No Error";
            default: return "error code maps to no known D3D11 code";
        }
    }

#ifdef _DEBUG
    inline void SetDebugChildName(ID3D11DeviceChild* child, const std::string& name)
    {
        if (child != nullptr)
        {
            child->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.size()), name.c_str());
        }
    }
#else
    inline void SetDebugChildName(ID3D11DeviceChild* child, const std::string& name) { UNUSEDPARAM(child); UNUSEDPARAM(name); }
#endif
}