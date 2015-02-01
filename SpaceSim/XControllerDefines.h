#pragma once

#include <map>

namespace Input
{

class XControllerDefines
{
public:
    XControllerDefines();
    ~XControllerDefines() {}

    enum XControllerInput
    {
        LeftStickPositiveX,
        LeftStickNegativeX,
        LeftStickPositiveY,
        LeftStickNegativeY,
        RightStickPositiveX,
        RightStickNegativeX,
        RightStickPositiveY,
        RightStickNegativeY,
        LeftTrigger,
        RightTrigger,
        LeftShoulderButton,
        RightShoulderButton,
        BackButton,
        StartButton,
        AButton,
        BBUtton,
        XButton,
        YButton,
        POVLeft,
        POVRight,
        POVUp,
        POVDown,
        LeftStickClick,
        RightStickClick
    };

    typedef std::map<unsigned int, XControllerInput> NameHashToXInput;
    typedef std::pair<unsigned int, XControllerInput> NameHashToXInputPair;
    NameHashToXInput m_XInputToAction;
};

}