#pragma once

#include "EngineDefines.h"

namespace shak
{
    class ShakEngine;
    struct Drawable
    {
        Drawable(std::shared_ptr<sf::Drawable> d, std::shared_ptr<sf::RenderStates> s) : drawable{ d }, renderStates{ s } {}
        ~Drawable() {}
        std::shared_ptr<sf::Drawable> drawable;
        std::shared_ptr<sf::RenderStates> renderStates;
    };

    class GameObject : public sf::Transformable, public sf::Drawable
    {
    public:
        GameObject();
        GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture = nullptr);
        virtual ~GameObject() = default;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        virtual void move(sf::Vector2f offset);

        void rotate(sf::Angle angle);

        void rotateAround(sf::Angle angle, const sf::Vector2f& pivot);

        void scale(sf::Vector2f factor);

        void setOrigin(const sf::Vector2f& origin);

        void AddChild(std::shared_ptr<GameObject> child);
        bool RemoveChild(int id);
        bool RemoveChildRecursive(int id);
        std::shared_ptr<GameObject> FindChildRecursive(std::string name) const;
        std::shared_ptr<GameObject> FindChildRecursive(int id) const;

        template<typename T>
        std::vector<std::shared_ptr<GameObject>> FindChildrenByTypeRecursive() const
        {
            std::vector<std::shared_ptr<GameObject>> result;
            for (const auto& child : m_children)
            {
                if (std::dynamic_pointer_cast<T>(child))
                    result.push_back(child);
                auto found = child->FindChildrenByTypeRecursive<T>();
                result.insert(result.end(), found.begin(), found.end());
            }
            return result;
        }

        inline void SetParent(GameObject* parent) { m_parent = parent; }
        inline GameObject* GetParent() { return m_parent; }

        inline void SetActive(bool active) { m_active = active; }
        inline bool IsActive() const { return m_active; }

        /// @brief Obtain a copy of the children vector of this GameObject
        /// @return std::vector<std::shared_ptr<GameObject>>
        std::vector<std::shared_ptr<GameObject>> GetChildren() const;

        inline void SetShader(const std::shared_ptr<sf::Shader> shader) { m_shader = shader; }
        inline std::shared_ptr<sf::Shader> GetShader() { return m_shader; }

        inline void SetVertexArray(const std::shared_ptr<sf::VertexArray> va) { m_vertices = va; }
        inline std::shared_ptr<sf::VertexArray> GetVertexArray() { return m_vertices; }

        inline void SetTexture(const std::shared_ptr<sf::Texture> texture) { m_texture = texture; }
        inline std::shared_ptr<sf::Texture> GetTexture() { return m_texture; }

        inline void SetFollowParent(bool follow) { m_followParent = follow; }
        inline bool GetFollowParent() const { return m_followParent; }

        void SetColor(const sf::Color& color);
        void SetTransparency(uint8_t transparency);
        inline void SetZIndex(int zIndex) { m_zIndex = zIndex; }

        virtual void Awake();

        virtual void Update(float dt);

        virtual void HandleInput(const sf::Event& event);

        virtual void OnDestroy();

        // Physics
        // Check if a point in world coordinates is inside the object bounding box
        bool IsPointInside(const sf::Vector2f& worldPoint) const;

    public:
        std::string Name = "GameObject";
        unsigned int Id = -1;

    protected:
        ShakEngine* m_engine;
        std::shared_ptr<sf::VertexArray> m_vertices;
        std::shared_ptr<sf::Texture> m_texture;
        std::shared_ptr<sf::Shader> m_shader;
        // raw pointer because of double ownership, check doubly linked list implementations
        GameObject* m_parent;
        std::vector<std::shared_ptr<GameObject>> m_children;
        bool m_active;
        bool m_followParent;
        int m_zIndex;
    };
}