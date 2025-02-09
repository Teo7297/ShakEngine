#include "Player.h"

#include "LaserShot.h"
#include "Animation.h"
#include "components/Health.h"
#include "components/TestComp.h"
#include "components/AbilitySystem.h"
#include "components/abilities/TestAbility.h"

Player::Player(std::shared_ptr<shak::TextureAtlas> atlas, std::shared_ptr<sf::Texture> laserTexture, std::shared_ptr<sf::Shader> laserShader, std::shared_ptr<shak::TextureAtlas> deathAnimation)
    : Ship(atlas, { { 75.f, -31.f }, { 15.f, -26.f }, { 75.f, +41.f }, { 16.f, +23.f } }, deathAnimation), m_laserTexture(laserTexture), m_laserShader(laserShader)
{
    Name = "Player";
}

void Player::Awake()
{
    Ship::Awake();

    auto as = AddComponent<AbilitySystem>();
    as->AddAbility<TestAbility>("TestAbility");
}

static int fcount = 0;
void Player::HandleInput(const sf::Event& event)
{
    if (auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Space)
        {
            m_shooting = !m_shooting;
        }

        else if (key->code == sf::Keyboard::Key::R)
        {
            m_engine->TestQuadtree();
        }

        else if (key->code == sf::Keyboard::Key::D)
        {
            auto vec = m_engine->FindGameObjectsByType<Ship>();
            for (auto& ship : vec)
            {
                if (ship->Name == "Player")
                    continue;
                auto health = std::dynamic_pointer_cast<Ship>(ship)->GetComponent<Health>();
                health->TakeDamage(m_damage);
            }
        }
        else if (key->code == sf::Keyboard::Key::A)
        {
            this->GetComponent<AbilitySystem>()->UseAbility("TestAbility");
        }
    }

    else if (auto key = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (key->button == sf::Mouse::Button::Left)
        {
            auto vec = m_engine->FindGameObjectsByType<Ship>();
            for (auto& ship : vec)
            {
                if (ship->Name == "Player")
                    continue;
                else if (ship->IsPointInside(m_engine->GetMouseWorldPos()))
                {
                    auto newTarget = std::dynamic_pointer_cast<Ship>(ship);
                    if (m_target)
                    {
                        if (newTarget->Id == m_target->Id)
                            continue;
                        else
                        {
                            // Stop shooting and deactivate aim
                            m_shooting = false;
                            m_target->ToggleAimSprite(false);
                        }
                    }
                    m_target = newTarget;
                    m_target->ToggleAimSprite(true);
                    m_targetWasSelected = true;
                    break;
                }
            }
        }
    }
    else if (auto key = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (key->button == sf::Mouse::Button::Left)
        {
            m_targetWasSelected = false;
        }
    }

    Ship::HandleInput(event);
}

void Player::Update(float dt)
{
    if (!m_targetWasSelected) // we can move
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
    }
    this->UpdateLookDirection();
    this->UpdateTextureCoords();

    static float time = 0.f;
    time += dt;
    m_laserShader->setUniform("u_time", time);

    if (!m_target)
        m_shooting = false;

    if (m_shooting)
    {
        m_dpsTimer += dt;
    }
    else
    {
        m_dpsTimer = 0.f;
        m_totalDamage = 0.f;
    }

    if (++fcount % 100 == 0 && m_shooting)
    {
        float dmg = Shoot();
        m_totalDamage += dmg;
        m_dps = m_totalDamage / m_dpsTimer;
    }

    // if (fcount % 100 == 0.f)
    // {
    //     auto vec = m_engine->FindGameObjectsByType<Ship>();
    //     for (auto& ship : vec)
    //     {
    //         if (ship->Name == "Player")
    //             continue;
    //         std::dynamic_pointer_cast<Ship>(ship)->TakeDamage(m_damage + std::rand() % 10000);
    //     }
    // }

    if (!m_shooting)
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
        auto shot = m_laserShotPool.Get(sf::Color::Blue, LaserShot::Size::Small, true, m_laserTexture, m_laserShader);
        shot->setPosition(laser->getPosition());
        shot->Init(m_target->getPosition(), laserAngle); // must be called after set position!!
        shot->SetFollowParent(false);
        this->AddChild(shot);
        auto onLaserHitCB = std::bind(&Player::OnLaserHit, this);
        if (shot->NeedAwake()) // This is a hack because Event cannot support contains for now
            shot->OnHit.Add(onLaserHitCB);
    }
    m_laserIndex = (m_laserIndex + 1) % 2; // alternate front and back lasers


    return 0.f;
}
