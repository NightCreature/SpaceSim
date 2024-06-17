#pragma once

#include <type_traits>
#include <concepts>

namespace DebugImguiTypeHelpers
{
    //template<typename T>
    //void OnDebugImgui(T& t)
    //{
    //    if constexpr (std::integral)
    //    {
    //        ImGui::InputInt("Int", &t);
    //    }
    //    else if constexpr (std::floating_point)
    //    {
    //        ImGui::InputFloat("Float", &t);
    //    }
    //    else if constexpr (std::is_same_v<T, std::string>)
    //    {
    //        ImGui::Text("String", t.c_str());
    //    }
    //    else if constexpr (std::is_same_v<T, std::vector>)
    //    {
    //        for (auto& element : t)
    //        {
    //            OnDebugImgui(element);
    //        }
    //    }
    //    else
    //    {
    //        t.OnDebugImgui();
    //    }
    //}
}