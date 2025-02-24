#include "LaserDPS.h"
#include "components/AbilitySystem.h"
#include "Ship.h"
#include "components/Health.h"
#include "components/Energy.h"

LaserDPS::LaserDPS(AbilitySystem* abilitySystem)
    : Ability("LaserDPS", 0.75f, 0.0f, 0.f, Type::Active, abilitySystem)
    , m_shipOwner(nullptr)
    , m_shooting(false)
    , m_target(nullptr)
    , m_laserShotPool(10)
    , m_laserTexture(nullptr)
    , m_laserShader(nullptr)
    , m_energyComponent(nullptr)
    , m_energyPerShot(100.f)
{
    m_shipOwner = (Ship*)m_abilitySystem->GetOwner();

    m_energyComponent = m_shipOwner->GetComponent<Energy>();

    SetupCallbacks();
    RegisterCallbacks();

    auto engine = &shak::ShakEngine::GetInstance();
    auto rm = engine->GetResourceManager();
    auto shipData = m_shipOwner->GetShipData();
    m_laserTexture = rm->LoadTexture(shipData.at("laser_texture").ToString(), shipData.at("laser_texture_name").ToString());
    m_laserShader = rm->LoadShader("", shipData.at("laser_shader").ToString(), shipData.at("laser_shader_name").ToString());
    m_laserShader->setUniform("u_texture", *m_laserTexture);
    m_laserShader->setUniform("u_resolution", sf::Glsl::Vec2{ engine->GetWindowSize().x, engine->GetWindowSize().y }); // TODO: this breaks if window is resized(maybe engine event?)
}

void LaserDPS::Update(float dt)
{

}

void LaserDPS::ShootLaser()
{
    if (!m_target)
    {
        this->Deactivate();
        return;
    }

    auto laserShot = m_laserShotPool.Get(m_laserTexture, m_laserShader);
    laserShot->setPosition(m_shipOwner->getPosition());
    laserShot->Init(m_target->getPosition(), sf::Color::Red, LaserShot::Size::Medium, false);
    laserShot->OnHit += [this]()
        {
            if (!m_target) return; // target changed while laser was in flight
            float damage = m_shipOwner->GetShipData().at("base_stats").at("damage").ToFloat();
            bool isCrit = m_shipOwner->IsCritHit();
            damage = isCrit ? damage * 2 : damage;

            float dealt = m_target->GetComponent<Health>()->TakeDamage(damage, isCrit);
            m_shipOwner->OnDamageDealt(dealt);
            m_energyComponent->GainEnergy(m_energyPerShot);
        };

    m_shipOwner->AddChild(laserShot);
    laserShot->SetFollowParent(false);
}

void LaserDPS::SetupCallbacks()
{
    ShootLaserCB = [this](const GameObjectPtr& target, const sf::Vector2f& pos) {ShootLaser();};

    LookAtTarget = [this](const GameObjectPtr& target, const sf::Vector2f& pos) {m_shipOwner->SetLookAtTarget(true);};

    StopLookAtTarget = [this]() {m_shipOwner->SetLookAtTarget(false);};

    Reactivate = [this]()
        {
            if (m_shooting)
            {
                this->Activate();
            }
        };

    AutoAttackStart = [this](const GameObjectPtr& target)
        {
            m_shooting = true;
            m_target = target;
            this->Activate();
        };

    AutoAttackStop = [this]()
        {
            m_shooting = false;
            m_target = nullptr;
        };
}

void LaserDPS::RegisterCallbacks()
{
    OnActivated += ShootLaserCB;
    OnActivated += LookAtTarget;
    OnCooldown += Reactivate;

    m_shipOwner->OnAutoAttackStarted += AutoAttackStart;
    m_shipOwner->OnAutoAttackStopped += AutoAttackStop;
    m_shipOwner->OnAutoAttackStopped += StopLookAtTarget;
}
