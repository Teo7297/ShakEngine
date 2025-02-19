#include "LifeDrain.h"
#include "Ship.h"
#include "components/AbilitySystem.h"
#include "components/Health.h"

LifeDrain::LifeDrain(AbilitySystem* abilitySystem)
    : Ability("LifeDrain", 1.f, 15.f, 250.f, Type::Active, abilitySystem)
    , m_stackedDamage(0.f)
    , m_damagePerStack(1000.f)
    , m_buffStacks(0)
    , m_lifeDrainPerStack(50.f)
    , m_health(nullptr)
{
    m_health = m_shipOwner->GetComponent<Health>();

    SetupCallbacks();
    RegisterCallbacks();
}

void LifeDrain::Update(float dt)
{
}

void LifeDrain::DrainHealth(float damageDealt)
{
    if (!m_isActive) return;
    m_stackedDamage += damageDealt;
    if (m_stackedDamage >= m_damagePerStack)
    {
        m_buffStacks++;
        m_stackedDamage = 0.f; // This avoids building multiple stacks with 1 hit
    }

    if (m_buffStacks == 0) return;

    float received = m_health->Heal(m_lifeDrainPerStack * m_buffStacks);
    m_shipOwner->OnHealReceived(received);

    std::cout << "stacked damage: " << m_stackedDamage << std::endl;
    std::cout << "buff stacks: " << m_buffStacks << std::endl;
    std::cout << "healed: " << received << std::endl;
}

void LifeDrain::SetupCallbacks()
{
    ResetProperties = [this](const GameObjectPtr& target, const sf::Vector2f& pos)
        {
            m_stackedDamage = 0.f;
            m_buffStacks = 0;
        };
}

void LifeDrain::RegisterCallbacks()
{
    OnActivated += ResetProperties;

    m_shipOwner->OnDamageDealt += std::bind(&LifeDrain::DrainHealth, this, std::placeholders::_1);
}