#pragma once

#include <string>

class Message
{
public:
    explicit Message(const std::string& target) : m_target(target) {}
    ~Message() {}
    const std::string& getTarget() const { return m_target; }
private:
    std::string m_target;
};


class ActivationMessage : public Message
{
public:
    ActivationMessage(const std::string& target, bool activate) : Message(target), m_activate(activate) {}
    bool shouldActivate() const { return m_activate; }
private:
    bool m_activate;
};
