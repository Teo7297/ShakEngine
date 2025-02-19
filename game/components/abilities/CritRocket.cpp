#include "CritRocket.h"
#include "Ship.h"
#include "components/AbilitySystem.h"
#include "components/Health.h"
#include "Rocket.h"

CritRocket::CritRocket(AbilitySystem* abilitySystem)
    : Ability("CritRocket", 1.f, 0.f, 150.f, Type::Active, abilitySystem)
    , m_damage(2000.f)
{
    SetupCallbacks();
    RegisterCallbacks();

    auto rm = shak::ShakEngine::GetInstance().GetResourceManager();
    m_rocketTexture = rm.LoadTexture("assets/textures/rocket.png", "rocket");
    m_rocketShader = rm.LoadShader("", "assets/shaders/rocket.fs", "rocket");
}

void CritRocket::Update(float dt)
{
}

void CritRocket::ShootRocket(const GameObjectPtr& target)
{

    auto rocket = std::make_shared<Rocket>(m_rocketTexture, m_rocketShader);
    m_shipOwner->AddChild(rocket);
    rocket->SetFollowParent(false);
    rocket->Init(m_shipOwner->getPosition(), std::dynamic_pointer_cast<Ship>(target));
    rocket->OnHit += OnTargetHit;
}

void CritRocket::SetupCallbacks()
{
    ShootRocketCB = [this](const GameObjectPtr& target, const sf::Vector2f& pos)
        {
            ShootRocket(target);
        };

    OnTargetHit = [this](const std::shared_ptr<Ship>& hitTarget)
        {
            float dealt = hitTarget->GetComponent<Health>()->TakeDamage(m_damage);
            m_shipOwner->OnDamageDealt(dealt);
        };


    OnCritHit = [this]()
        {
            m_damage *= 2.f;
        };
}

void CritRocket::RegisterCallbacks()
{
    OnActivated += ShootRocketCB;
    m_shipOwner->OnCriticalHitDealt += OnCritHit;
}