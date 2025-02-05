#include "Component.h"
#include "GameObject.h"

namespace shak
{
    Component::Component(GameObject* owner)
        : m_owner(owner)
    {
    }
}