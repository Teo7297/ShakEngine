#include "components/EnergyBar.h"
#include "components/Energy.h"
#include "ShakSprite.h"
#include "ShakEngine.h"

EnergyBar::EnergyBar(shak::GameObject* owner)
    : shak::Component(owner)
    , m_energy(nullptr)
    , m_frontUI(nullptr)
    , m_backgroundUI(nullptr)
    , m_size(300.f, 10.f)
{
    m_onEnergyChange = [this](float energy, float maxEnergy)
        {
            m_frontUI->setScale(sf::Vector2f{ energy / maxEnergy, 1 });
        };
}

EnergyBar::~EnergyBar()
{
}

void EnergyBar::Awake()
{
    m_energy = m_owner->GetComponent<Energy>();
    if (!m_energy)
        throw std::runtime_error("[EnergyBar] Energy component not found on " + m_owner->Name);

    m_energy->OnEnergyChange += m_onEnergyChange;

    // Front
    m_frontUI = std::make_shared<shak::Sprite>(nullptr, nullptr, m_size, sf::Color::Blue);
    m_frontUI->SetZIndex(2000);
    m_frontUI->setPosition(m_owner->getPosition() + sf::Vector2f{ 0.f, -135.f });
    m_frontUI->SetRotateWithParent(false);
    m_frontUI->setOrigin({ 0, 0 });
    m_frontUI->move(-m_size / 2.f);
    m_owner->AddChild(m_frontUI);

    // Back
    m_backgroundUI = std::make_shared<shak::Sprite>(nullptr, nullptr, m_size, sf::Color::Black);
    m_backgroundUI->SetZIndex(1999);
    m_backgroundUI->setPosition(m_owner->getPosition() + sf::Vector2f{ 0.f, -135.f });
    m_backgroundUI->SetRotateWithParent(false);
    m_owner->AddChild(m_backgroundUI);

    // Get initial energy
    m_onEnergyChange(m_energy->GetEnergy(), m_energy->GetMaxEnergy());
}
