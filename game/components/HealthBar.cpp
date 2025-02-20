#include "components/HealthBar.h"
#include "components/Health.h"
#include "ShakSprite.h"
#include "ShakEngine.h"

HealthBar::HealthBar(shak::GameObject* owner)
    : shak::Component(owner)
    , m_health(nullptr)
    , m_hpFrontUI(nullptr)
    , m_hpBackgroundUI(nullptr)
    , m_size(300.f, 10.f)
{
    m_onHealthChange = [this](float health, float maxHealth)
        {
            float hpRatio = health / maxHealth;
            sf::Color frontColor;
            if (hpRatio > 0.5f)
                frontColor = sf::Color::Green;
            else if (hpRatio > 0.25f)
                frontColor = sf::Color::Yellow;
            else
                frontColor = sf::Color::Red;

            m_hpFrontUI->setScale(sf::Vector2f{ hpRatio, 1 });
            m_hpFrontUI->SetColor(frontColor);
        };
}

HealthBar::~HealthBar()
{
}

void HealthBar::Awake()
{
    m_health = m_owner->GetComponent<Health>();
    if (!m_health)
        throw std::runtime_error("[HealthBar] Health component not found on " + m_owner->Name);

    m_health->OnHealthChange += m_onHealthChange;

    // Front
    m_hpFrontUI = std::make_shared<shak::Sprite>(nullptr, nullptr, m_size);
    m_hpFrontUI->SetZIndex(2000);
    m_hpFrontUI->setPosition(m_owner->getPosition() + sf::Vector2f{ 0.f, -150.f });
    m_hpFrontUI->SetRotateWithParent(false);
    m_hpFrontUI->setOrigin({ 0, 0 });
    m_hpFrontUI->move(-m_size / 2.f);
    m_owner->AddChild(m_hpFrontUI);

    // Back
    m_hpBackgroundUI = std::make_shared<shak::Sprite>(nullptr, nullptr, m_size, sf::Color::Black);
    m_hpBackgroundUI->SetZIndex(1999);
    m_hpBackgroundUI->setPosition(m_owner->getPosition() + sf::Vector2f{ 0.f, -150.f });
    m_hpBackgroundUI->SetRotateWithParent(false);
    m_owner->AddChild(m_hpBackgroundUI);

    // Get initial health
    m_onHealthChange(m_health->GetHealth(), m_health->GetMaxHealth());
}
