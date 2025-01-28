#include "Player.h"

#include "LaserShot.h"

Player::Player(std::shared_ptr<ShakEngine> engine, std::shared_ptr<shak::TextureAtlas> atlas, std::shared_ptr<sf::Texture> laserTexture, std::shared_ptr<sf::Shader> laserShader)
    : Ship(atlas, { { 75.f, -31.f }, { 15.f, -26.f }, { 75.f, +41.f }, { 16.f, +23.f } }), m_engine(engine), m_laserTexture(laserTexture), m_laserShader(laserShader)
{
}

static bool shooting = false;
static int fcount = 0;
void Player::HandleInput(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Space)
        {
            shooting = !shooting;
        }
    }

    Ship::HandleInput(event);
}

void Player::Awake()
{
    m_target = std::dynamic_pointer_cast<Ship>(m_engine->FindGameObjectByName("Alien"));
    Ship::Awake();
}

void Player::Update(float dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        // Move if click outside of ship
        auto mousePos = m_engine->GetMouseWorldPos();
        bool updateDestination = !this->IsPointInside(mousePos);

        // Click valid only if inside window
        auto windowSize = m_engine->GetWindowSize();
        auto mousePosPixel = m_engine->GetMousePixelPos();
        updateDestination &= mousePosPixel.x >= 0 && mousePosPixel.x <= windowSize.x;
        updateDestination &= mousePosPixel.y >= 0 && mousePosPixel.y <= windowSize.y;

        if (updateDestination)
        {
            m_destination = mousePos;
            this->UpdateDirection();
        }
    }
    this->UpdateLookDirection();
    this->UpdateTextureCoords();

    static float time = 0.f;
    time += dt;
    m_laserShader->setUniform("u_time", time);

    if (shooting)
    {
        m_dpsTimer += dt;
    }
    else
    {
        m_dpsTimer = 0.f;
        m_totalDamage = 0.f;
    }

    if (shooting && ++fcount % 10 == 0)
    {
        float dmg = Shoot();
        m_totalDamage += dmg;
        m_dps = m_totalDamage / m_dpsTimer;
    }

    if (!shooting)
        m_lookAtTarget = false;

    Ship::Update(dt);
}

float Player::Shoot()
{
    if (!m_target)
        return 0.f;

    m_lookAtTarget = true;

    for (int i = m_laserIndex; i < m_lasers.size(); i += 2)
    {
        auto& laser = m_lasers[i];
        auto direction = m_target->getPosition() - laser->getPosition();
        direction = direction.normalized();
        sf::Angle laserAngle = direction.angle();
        auto shotLeft = m_laserShotPool.Get(sf::Color::Blue, LaserShot::Size::Small, true, m_laserTexture, m_laserShader);
        shotLeft->setPosition(laser->getPosition());
        shotLeft->Init(m_target->getPosition(), laserAngle); // must be called after set position!!
        shotLeft->SetFollowParent(false);
        this->AddChild(shotLeft);
    }
    m_laserIndex = (m_laserIndex + 1) % 2; // alternate front and back lasers

    std::cout << m_laserShotPool.GetTotalCount() << " lasers in pool" << std::endl;

    return m_target->TakeDamage(m_damage + std::rand() % 10000);
}
