#pragma once

#include <vector>

namespace InputDefines
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
        RightStickClick,

        NumInputActions
    };

    XControllerInput FindControllerInputIdForHashId(unsigned int hashId) const;
private:
    unsigned int m_inputNameHash[NumInputActions];
};

}