#include "ShaderParameter.h"
#include "DeviceManagerD3D12.h"
#include "DescriptorHeapManager.h"

#include "Logging/LoggingMacros.h"

#include <variant>

//std::visit([](auto&& arg) {
//    using T = std::decay_t<decltype(arg)>;
//    if constexpr (std::is_same_v<T, int>)
//        std::cout << "int with value " << arg << '\n';
//    else if constexpr (std::is_same_v<T, long>)
//        std::cout << "long with value " << arg << '\n';
//    else if constexpr (std::is_same_v<T, double>)
//        std::cout << "double with value " << arg << '\n';
//    else if constexpr (std::is_same_v<T, std::string>)
//        std::cout << "std::string with value " << std::quoted(arg) << '\n';
//    else
//        static_assert(always_false_v<T>, "non-exhaustive visitor!");
//    }, w);

///-----------------------------------------------------------------------------
///! @brief   
///! @remark std::variant<ConstantBuffer<WVPBufferContent>, ConstantBuffer<MaterialContent>, ConstantBuffer<PerFrameConstants>>
/////-----------------------------------------------------------------------------
//void ShaderParameter::CreateConstantBuffer(const DeviceManager& deviceManager, DescriptorHeap& heap)
//{
//    auto index = m_data.index();
//    switch (index)
//    {
//    case 0:
//    {
//        auto data = std::get_if<0>(&m_data);
//        m_cbData.Create(deviceManager, heap, sizeof(*data));
//        //This can now call into the constant buffer manager maybe? or should this have the material instead probably cleaner
//    }
//    break;
//    case 1:
//    {
//        auto data = std::get_if<1>(&m_data);
//        m_cbData.Create(deviceManager, heap, sizeof(*data));
//    }
//    break;
//    case 2:
//    {
//        auto data = std::get_if<2>(&m_data);
//        m_cbData.Create(deviceManager, heap, sizeof(*data));
//    }
//    break;
//    default:
//        //MSG_TRACE_CHANNEL("ShaderParameter", "Failed to create constant buffer because the type of the variant doesnt match or is not set: %d", m_data.index());
//        break;
//    }
//
//}
//
/////-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ShaderParameter::Print() const
{
    std::stringstream str;
    str << "RootParamIndex :" << m_rootParamIndex << "\n";
    str << "Name Hash :" << m_nameHash << "\n";
    auto index = m_data.index();
    switch (index)
    {
    case 0:
    {
        str << "Data Type : WVPBuffer\n";
    }
    break;
    case 1:
    {
        str << "Data Type : Material Buffer\n";
    }
    break;
    case 2:
    {
        str << "Data Type : Perframe constants\n";
    }
    break;
    case 3:
    {
        str << "Data Type : Texture Data\n";
    }
    break;
    default:
        //MSG_TRACE_CHANNEL("ShaderParameter", "Failed to create constant buffer because the type of the variant doesnt match or is not set: %d", m_data.index());
        break;
    }
    
    MSG_TRACE_CHANNEL("ShaderParameter", "%s", str.str().c_str() );
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PrintParameters(const ShaderParameters& parameters)
{
    for (const auto& param : parameters)
    {
        param.Print();
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
size_t GetVariantSize(size_t index)
{
    switch (index)
    {
    case 0:
        return sizeof(std::variant_alternative_t<0, ShaderParameter::ShaderParameterData>);
    case 1:
        return sizeof(std::variant_alternative_t<1, ShaderParameter::ShaderParameterData>);
    case 2:
        return sizeof(std::variant_alternative_t<2, ShaderParameter::ShaderParameterData>);

    }

    return 0;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
ShaderParameter::ShaderParameterData GetVariant(const std::string& name)
{
    if (name == "WVPConstants" || name == "ShadowConstants")
    {
        return WVPData();
    }
    else if (name == "LightParameters")
    {
        return LightParameters();
    }
    else if (name == "MaterialConstants")
    {
        return MaterialConstants();
    }

    return ShaderParameter::ShaderParameterData();
}
