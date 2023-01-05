#include "Graphics\D3D12\ShaderCompiler.h"

#include "Core\StringOperations\StringHelperFunctions.h"

#include <combaseapi.h>
#include <string>
#include "..\Shaders.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool ShaderCompiler::Initialise()
{
    HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_dxcUtils));
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to initialise Dxc Library intergace");
        return false;
    }


    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler));
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to initialise the DxC compiler");
        return false;

    }

    hr = m_dxcUtils->CreateDefaultIncludeHandler(&m_includeHandler);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to initialise the default include handler");

    }

    return true;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags
///-----------------------------------------------------------------------------
bool ShaderCompiler::CreateShader(const std::filesystem::path& filePath, const std::wstring& entrypoint, const std::wstring& profileName, size_t compilerFlags, CreatedShaderObjects& shaderObject) const
{
    uint32_t codePage = CP_UTF8;
    IDxcBlobEncoding* shaderSourceFromFile = nullptr;
    const std::wstring& wideFilePath = filePath.wstring();
    HRESULT hr = m_dxcUtils->LoadFile(wideFilePath.c_str(), &codePage, &shaderSourceFromFile);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("Shaders", "Failed to load shader file: %s", filePath.c_str());
        return false;
    }

    DxcBuffer shaderSourceCode;
    shaderSourceCode.Ptr = shaderSourceFromFile->GetBufferPointer();
    shaderSourceCode.Size = shaderSourceFromFile->GetBufferSize();
    shaderSourceCode.Encoding = DXC_CP_ACP; //BOM UTF8, UTF16 or ANSI

    std::vector<LPCWSTR> arguments =
    {
        wideFilePath.c_str(),       // Optional shader source file name for error reporting and for PIX shader source view.  
        L"-E", entrypoint.c_str(),  // Entry point.
        L"-T", profileName.c_str(), // Target.
        L"-Zs",                     // Enable debug information (slim format)
        L"-Fo", L"myshader.bin",    // Optional. Stored in the pdb. 
        L"-Fd", L"myshader.pdb",    // The file name of the pdb. This must either be supplied or the autogenerated file name must be used.
        L"-Qstrip_reflect",         // Strip reflection into a separate blob. 
        L"HV", L"2021"              // Enable templates in shaders
    };

    IDxcResult* results = nullptr;
    hr = m_compiler->Compile(&shaderSourceCode, arguments.data(), arguments.size(), m_includeHandler, IID_PPV_ARGS(&results));

    //
   // Print errors if present.
   //
    IDxcBlobUtf8* errors = nullptr;
    results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
    // Note that d3dcompiler would return null if no errors or warnings are present.
    // IDxcCompiler3::Compile will always return an error buffer, but its length
    // will be zero if there are no warnings or errors.
    if (errors != nullptr && errors->GetStringLength() != 0)
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Compiler Errors/Warnings Output: %s", errors->GetStringPointer());
    }

    HRESULT hrStatus;
    results->GetStatus(&hrStatus);
    if (FAILED(hrStatus))
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to compile shader: %s", filePath.string().c_str());
        return false;
    }

    //Write output to a file
    //CComPtr<IDxcBlob> pShader = nullptr;
    //CComPtr<IDxcBlobUtf16> pShaderName = nullptr;
    //pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pShader), &pShaderName);
    //if (pShader != nullptr)
    //{
    //    FILE* fp = NULL;

    //    _wfopen_s(&fp, pShaderName->GetStringPointer(), L"wb");
    //    fwrite(pShader->GetBufferPointer(), pShader->GetBufferSize(), 1, fp);
    //    fclose(fp);
    //}

    //
    // Save pdb.
    //
    //CComPtr<IDxcBlob> pPDB = nullptr;
    //CComPtr<IDxcBlobUtf16> pPDBName = nullptr;
    //pResults->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&pPDB), &pPDBName);
    //{
    //    FILE* fp = NULL;

    //    // Note that if you don't specify -Fd, a pdb name will be automatically generated.
    //    // Use this file name to save the pdb so that PIX can find it quickly.
    //    _wfopen_s(&fp, pPDBName->GetStringPointer(), L"wb");
    //    fwrite(pPDB->GetBufferPointer(), pPDB->GetBufferSize(), 1, fp);
    //    fclose(fp);
    //}

    ////
    //// Print hash.
    ////
    //CComPtr<IDxcBlob> pHash = nullptr;
    //pResults->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(&pHash), nullptr);
    //if (pHash != nullptr)
    //{
    //    wprintf(L"Hash: ");
    //    DxcShaderHash* pHashBuf = (DxcShaderHash*)pHash->GetBufferPointer();
    //    for (int i = 0; i < _countof(pHashBuf->HashDigest); i++)
    //        wprintf(L"%.2x", pHashBuf->HashDigest[i]);
    //    wprintf(L"\n");
    //}

    ////
    //// Demonstrate getting the hash from the PDB blob using the IDxcUtils::GetPDBContents API
    ////
    //CComPtr<IDxcBlob> pHashDigestBlob = nullptr;
    //CComPtr<IDxcBlob> pDebugDxilContainer = nullptr;
    //if (SUCCEEDED(pUtils->GetPDBContents(pPdb, &pHashDigestBlob, &pDebugDxilContainer)))
    //{
    //    // This API returns the raw hash digest, rather than a DxcShaderHash structure.
    //    // This will be the same as the DxcShaderHash::HashDigest returned from
    //    // IDxcResult::GetOutput(DXC_OUT_SHADER_HASH, ...).
    //    wprintf(L"Hash from PDB: ");
    //    const BYTE* pHashDigest = (const BYTE*)pHashDigestBlob->GetBufferPointer();
    //    assert(pHashDigestBlob->GetBufferSize() == 16); // hash digest is always 16 bytes.
    //    for (int i = 0; i < pHashDigestBlob->GetBufferSize(); i++)
    //        wprintf(L"%.2x", pHashDigest[i]);
    //    wprintf(L"\n");

    //    // The pDebugDxilContainer blob will contain a DxilContainer formatted
    //    // binary, but with different parts than the pShader blob retrieved
    //    // earlier.
    //    // The parts in this container will vary depending on debug options and
    //    // the compiler version.
    //    // This blob is not meant to be directly interpreted by an application.
    //}

    //
    // Get separate reflection.
    //
    IDxcBlob* reflectionData;
    results->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflectionData), nullptr);
    if (reflectionData != nullptr)
    {
        // Optionally, save reflection blob for later here.

        // Create reflection interface.
        DxcBuffer ReflectionData;
        ReflectionData.Encoding = DXC_CP_ACP;
        ReflectionData.Ptr = reflectionData->GetBufferPointer();
        ReflectionData.Size = reflectionData->GetBufferSize();

        m_dxcUtils->CreateReflection(&ReflectionData, IID_PPV_ARGS(&shaderObject.m_reflectionObject));
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to get reflection object for: %s", filePath.string().c_str());
            shaderObject.m_reflectionObject = nullptr;
            return false;
        }

        // Use reflection interface here.

    }

    hr = results->GetOutput(DXC_OUT_ROOT_SIGNATURE, IID_PPV_ARGS(&shaderObject.m_rootSignatureBlob), nullptr);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to get compiled root signature for: %s", filePath.string().c_str());
        shaderObject.m_rootSignatureBlob = nullptr;
        return false;
    }


    hr = results->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderObject.m_shaderObject), nullptr);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderCompiler", "Failed to get compiled shader object for: %s", filePath.string().c_str());
        shaderObject.m_shaderObject = nullptr;
        return false;
    }

    

    return true;
}

