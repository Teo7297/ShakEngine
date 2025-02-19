#include "MachineGun.h"
#include "components/AbilitySystem.h"
#include "Ship.h"
#include "components/Health.h"
#include "components/Energy.h"
#include "MathExtensions.h"

MachineGun::MachineGun(AbilitySystem* abilitySystem)
    : Ability("MachineGun", 1.0f, 999999.0f, 0.f, Type::Active, abilitySystem)
    , m_target(nullptr)
    , m_laserShotPool(10)
    , m_laserTexture(nullptr)
    , m_laserShader(nullptr)
    , m_projectileTimer(0.f)
    , m_projectileCooldown(1.f / 20.f)
    , m_costPerShot(50.f)
{
    SetupCallbacks();
    RegisterCallbacks();

    auto engine = &shak::ShakEngine::GetInstance();
    auto rm = engine->GetResourceManager();
    auto shipData = m_shipOwner->GetShipData();
    m_laserTexture = rm.LoadTexture(shipData.at("laser_texture").ToString(), shipData.at("laser_texture_name").ToString());
    m_laserShader = rm.LoadShader("", shipData.at("laser_shader").ToString(), shipData.at("laser_shader_name").ToString());
    m_laserShader->setUniform("u_texture", *m_laserTexture);
    m_laserShader->setUniform("u_resolution", sf::Glsl::Vec2{ engine->GetWindowSize().x, engine->GetWindowSize().y }); // TODO: this breaks if window is resized(maybe engine event?)
}

void MachineGun::Update(float dt)
{
    if (!m_isActive)
        return;

    if (!m_target || !m_energy->HasEngouhEnergy(m_costPerShot))
        this->Deactivate();

    m_projectileTimer += dt;
    if (m_projectileTimer >= m_projectileCooldown)
    {
        m_projectileTimer = 0.f;
        this->ShootLaser();
        this->RestartCooldown(); // Always reset the cooldown with casted abilities
    }
}

void MachineGun::ShootLaser()
{
    m_energy->UseEnergy(m_costPerShot);
    auto laserShot = m_laserShotPool.Get(m_laserTexture, m_laserShader);
    laserShot->setPosition(m_shipOwner->getPosition());
    laserShot->Init(m_target->getPosition() + shak::randVec2f(-50.f, 50.f), sf::Color::Yellow, LaserShot::Size::Small, false);
    laserShot->OnHit += OnLaserHit;

    m_shipOwner->AddChild(laserShot);
    laserShot->SetFollowParent(false);
}

void MachineGun::SetupCallbacks()
{
    LookAtTarget = [this](const GameObjectPtr& target, const sf::Vector2f& pos) {m_shipOwner->SetLookAtTarget(true);};

    AutoAttackOrStop = [this]()
        {
            if (m_shipOwner->GetTarget() == m_target)
                m_shipOwner->OnAutoAttackStarted(m_target);
            else
                m_shipOwner->SetLookAtTarget(false);
        };

    StopAutoAttack = [this](const GameObjectPtr& target, const sf::Vector2f& pos) {m_shipOwner->OnAutoAttackStopped();};

    StoreTarget = [this](const GameObjectPtr& target, const sf::Vector2f& pos) {m_target = target;};

    SetIsLaserShootingOnShip = [this](const GameObjectPtr& target, const sf::Vector2f& pos) {m_shipOwner->StartLaserAttack();};

    StopIsLaserShootingOnShip = [this]() {m_shipOwner->StopLaserAttack();};

    OnLaserHit = [this]()
        {
            if (!m_target) return; // target changed while laser was in flight
            auto shipOwner = (Ship*)m_abilitySystem->GetOwner();
            float damage = shipOwner->GetShipData().at("base_stats").at("damage").ToFloat();
            float dealt = m_target->GetComponent<Health>()->TakeDamage(damage);
            m_shipOwner->OnDamageDealt(dealt);
        };

    DeactivateAbility = [this]()
        {
            this->Deactivate();
        };
}

void MachineGun::RegisterCallbacks()
{
    OnActivated += StopAutoAttack;
    OnActivated += LookAtTarget;
    OnActivated += StoreTarget;
    OnActivated += SetIsLaserShootingOnShip;
    OnDeactivated += StopIsLaserShootingOnShip;
    OnDeactivated += AutoAttackOrStop;

    m_shipOwner->OnTargetChanged += DeactivateAbility;
}
