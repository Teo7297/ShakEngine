#pragma once

#include "EngineDefines.h"
#include "GameObject.h"
#include "Background.h"
#include "ShakEngine.h"

namespace shak
{
    enum class CameraResizeBehavior
    {
        Letterbox,
        MaintainSize,
        Squash,
        ViewResize
    };

    class Camera : public GameObject
    {
    public:
        Camera(sf::FloatRect rectangle, CameraResizeBehavior resizeBehavior)
            : GameObject()
            , m_view{ std::make_shared<sf::View>(rectangle) }
            , m_resolution{ sf::Vector2u{static_cast<unsigned int>(rectangle.size.x), static_cast<unsigned int>(rectangle.size.y) } }
            , m_resizeBehavior{ resizeBehavior }
            , m_defaultViewport{ sf::FloatRect{sf::Vector2f{0.f, 0.f}, {1.f, 1.f}} }
        {
            this->setOrigin(rectangle.size / 2.f);
        }
        ~Camera() override = default;

        void move(sf::Vector2f offset) override
        {
            m_view->move(offset);
            GameObject::move(offset);
        }

        void setPosition(sf::Vector2f position) override
        {
            m_view->setCenter(position);
            GameObject::setPosition(position);
        }

        sf::Vector2f getPosition()
        {
            return m_view->getCenter();
        }

        std::shared_ptr<sf::View> GetView() const
        {
            return m_view;
        }

        void SetSize(const sf::Vector2f& size)
        {
            m_view->setSize(size);
        }

        void Zoom(float factor)
        {
            m_view->zoom(factor);
        }

        void SetResizeBehavior(CameraResizeBehavior behavior)
        {
            m_resizeBehavior = behavior;
        }

        void SetResolution(const sf::Vector2u& resolution)
        {
            m_resolution = resolution;
        }

        void SetViewport(const sf::FloatRect& viewport)
        {
            m_view->setViewport(viewport);
            m_defaultViewport = viewport;
        }

        void SetCenter(const sf::Vector2f& center)
        {
            m_view->setCenter(center);
        }

        void Awake() override
        {
            // When the window is resized, apply the letterbox effect
            m_engine->OnResize += [this](const sf::Vector2u& oldSize, const sf::Vector2u& newSize)
                {
                    switch (m_resizeBehavior)
                    {
                    case CameraResizeBehavior::Letterbox:
                        LetterboxResize(newSize);
                        break;

                    case CameraResizeBehavior::MaintainSize:
                        MaintainSizeResize(oldSize, newSize);
                        break;

                    case CameraResizeBehavior::Squash:
                        // No need to do anything here, the squash effect is automatic.
                        m_view->setViewport(m_defaultViewport); // Reset the viewport to the default one
                        break;

                    case CameraResizeBehavior::ViewResize:
                        ViewResize(newSize);
                        break;

                    default:
                        break;
                    }
                };
        }

        // We set the new size and THEN regulate the zoom based on the initial aspectRatio
        //! This implementation "forgets" the zoom previously applied. So if the game requires a particular zoom, it should be set attaching a function to the OnResize event.
        void LetterboxResize(const sf::Vector2u& newSize)
        {
            m_view->setViewport(m_defaultViewport); // Reset the viewport to the default one

            // Compute scaled resolution based on the viewport size
            const float scaleX = m_view->getViewport().size.x;
            const float scaleY = m_view->getViewport().size.y;
            const float scaledResX = static_cast<float>(m_resolution.x) * scaleX;
            const float scaledResY = static_cast<float>(m_resolution.y) * scaleY;

            // Set the new size of the view based on the new resolution and viewport size
            const float newX = static_cast<float>(newSize.x) * scaleX;
            const float newY = static_cast<float>(newSize.y) * scaleY;

            // Compute aspect ratios
            const float newAspectRatio = newX / newY;
            const float targetAspectRatio = scaledResX / scaledResY;

            // Compute zoom factors
            const float zoomX = scaledResX / newX;
            const float zoomY = scaledResY / newY;
            const float zoom = (newAspectRatio > targetAspectRatio) ? zoomY : zoomX;

            //! Important: first we set the size (which resets the zoom) and then we apply the zoom factor.
            m_view->setSize({ newX, newY });
            m_view->zoom(zoom);
        }

        // Resizes the view without changing the zoom. The resize happens using the CENTER as pivot.
        void MaintainSizeResize(const sf::Vector2u& oldSize, const sf::Vector2u& newSize)
        {
            // vp size is inversely proportional to the previous size.
            auto vp = m_view->getViewport();
            auto newVp = sf::FloatRect(
                vp.position,
                {
                    vp.size.x * (static_cast<float>(oldSize.x) / static_cast<float>(newSize.x)),
                    vp.size.y * (static_cast<float>(oldSize.y) / static_cast<float>(newSize.y))
                });
            m_view->setViewport(newVp);
        }

        void ViewResize(const sf::Vector2u& newSize)
        {
            // Set the new size of the view based on the new resolution and viewport size
            const float newX = static_cast<float>(newSize.x);
            const float newY = static_cast<float>(newSize.y);
            m_view->setViewport(m_defaultViewport); // Reset the viewport to the default one
            m_view->setSize({ newX, newY });
        }

        // Hack. We like the camera being a GameObj so we can set it as child of other gameObj. But we don't want the draw function to do anything, so this is an empty function.
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
        }

    private:
        std::shared_ptr<sf::View> m_view;
        sf::Vector2u m_resolution;
        CameraResizeBehavior m_resizeBehavior;
        sf::FloatRect m_defaultViewport;
    };
}

