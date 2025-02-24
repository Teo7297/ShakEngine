#pragma once

#include "EngineDefines.h"
#include "Component.h"
#include "ShakEvent.h"

namespace shak
{
    class ShakEngine;
    class GameObject : public sf::Transformable, public sf::Drawable
    {
    public:
        GameObject();
        GameObject(std::shared_ptr<sf::VertexArray> va, std::shared_ptr<sf::Texture> texture = nullptr);
        virtual ~GameObject() override = default;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

        virtual void move(sf::Vector2f offset);

        virtual void setPosition(sf::Vector2f position);

        void rotate(sf::Angle angle);

        void setRotation(sf::Angle angle);

        void rotateAround(sf::Angle angle, const sf::Vector2f& pivot);

        void scale(sf::Vector2f factor);

        void setOrigin(const sf::Vector2f& origin);
        void CenterOrigin();

        // Children
        void AddChild(const std::shared_ptr<GameObject>& child);
        bool RemoveChild(int id);
        bool RemoveChildRecursive(int id);
        void RemoveChildrenRecursive();
        void RemoveChildren();

        std::shared_ptr<GameObject> FindChildRecursive(const std::string& name) const;
        std::shared_ptr<GameObject> FindChildRecursive(int id) const;

        // Components
        template<typename T>
        std::shared_ptr<T> AddComponent()
        {
            std::string compName = typeid(T).name();

            if (m_components.contains(compName))
            {
                std::cerr << "[GameObject] Tried to add a duplicate component of type " << compName << std::endl;
                return std::dynamic_pointer_cast<T>(m_components[compName]);
            }
            auto component = std::make_shared<T>(this);
            m_components[compName] = component;

            return component;
        }

        template<typename T>
        std::shared_ptr<T> GetComponent()
        {
            std::string compName = typeid(T).name();
            if (m_components.contains(compName))
                return std::dynamic_pointer_cast<T>(m_components[compName]);
            return nullptr;
        }

        void GetDrawables(std::vector<std::shared_ptr<GameObject>>& drawables) const;

        template<typename T>
        std::vector<std::shared_ptr<GameObject>> FindChildrenByTypeRecursive() const
        {
            std::vector<std::shared_ptr<GameObject>> result;
            for (const auto& [id, child] : m_children)
            {
                if (std::dynamic_pointer_cast<T>(child))
                    result.push_back(child);
                auto found = child->FindChildrenByTypeRecursive<T>();
                result.insert(result.end(), found.begin(), found.end());
            }
            return result;
        }

        void SetParent(GameObject* parent) { m_parent = parent; }
        GameObject* GetParent() { return m_parent; }

        void SetActive(bool active);
        bool IsActive() const { return m_active; }

        /// @brief Obtain a copy of the children vector of this GameObject
        /// @return std::vector<std::shared_ptr<GameObject>>
        std::vector<std::shared_ptr<GameObject>> GetChildren() const;

        /// @brief Obtain a copy of the components vector of this GameObject
        /// @return std::vector<std::shared_ptr<Components>>
        std::vector<std::shared_ptr<Component>> GetComponents() const;

        void SetShader(const std::shared_ptr<sf::Shader> shader) { m_shader = shader; }
        std::shared_ptr<sf::Shader> GetShader() { return m_shader; }

        void SetVertexArray(const std::shared_ptr<sf::VertexArray> va) { m_vertices = va; }
        std::shared_ptr<sf::VertexArray> GetVertexArray() { return m_vertices; }
        void InitQuadVertexArray(sf::Vector2f size, sf::Color color);

        void SetTexture(const std::shared_ptr<sf::Texture> texture) { m_texture = texture; }
        std::shared_ptr<sf::Texture> GetTexture() { return m_texture; }

        void SetFollowParent(bool follow) { m_followParent = follow; m_rotateWithParent = follow; } //TODO: this should not modify rotation.
        bool GetFollowParent() const { return m_followParent; }
        void SetRotateWithParent(bool rotate) { m_rotateWithParent = rotate; }
        bool GetRotateWithParent() const { return m_rotateWithParent; }

        void SetColor(const sf::Color& color) const;
        void SetTransparency(uint8_t transparency) const;
        void SetZIndex(int zIndex) { m_zIndex = zIndex; }
        int GetZIndex() const { return m_zIndex; }

        void EnablePhysics() { m_physicsEnabled = true; }
        void DisablePhysics() { m_physicsEnabled = false; }
        bool IsPhysicsEnabled() const { return m_physicsEnabled; }

        bool NeedAwake() const { return m_needAwake; }
        void ForwardAwake();

        virtual void OnCollision(const std::shared_ptr<GameObject>& other) {}

        virtual void Awake(); // TODO: forward awake is the same as internal stuff. this awake gets called twice if not overridden. this should be fixed but its not that bad.

        void InternalUpdate(float dt);
        virtual void Update(float dt) {}

        void InternalLateUpdate(float dt);
        virtual void LateUpdate(float dt) {}

        void InternalCleanup();
        // This function is the last one called in the game loop. It is meant to be used to reset flags, etc.
        virtual void Cleanup() {}

        void InternalHandleInput(const sf::Event& event);
        /// @brief Forwards the input event from the root object. This is NOT called if a GUI element is being interacted with
        /// @param event 
        virtual void HandleInput(const sf::Event& event) {}

        void InternalDestroy();
        virtual void OnDestroy() {}

        // Physics
        // Check if a point in world coordinates is inside the object bounding box
        bool IsPointInside(const sf::Vector2f& worldPoint) const;

        bool IsCollidingWithGameObject(const std::shared_ptr<GameObject>& other) const;

        bool IsCollidingWitRect(const sf::FloatRect& rect) const;

        inline bool HasMovedThisFrame() const { return m_movedThisFrame; }
        inline void ResetMovedThisFrame() { m_movedThisFrame = false; }

    private:
        void TrySafeCopy();

    public:
        std::string Name = "GameObject";
        unsigned int Id = -1;
        Event<> OnEnable;
        Event<> OnDisable;

    protected:
        ShakEngine* m_engine;
        std::shared_ptr<sf::VertexArray> m_vertices;
        std::shared_ptr<sf::Texture> m_texture;
        std::shared_ptr<sf::Shader> m_shader;
        // raw pointer because of double ownership, check doubly linked list implementations
        GameObject* m_parent;
        bool m_safeCopyDone;
        std::unordered_map<unsigned int, std::shared_ptr<GameObject>> m_children;
        std::unordered_map<std::string, std::shared_ptr<Component>> m_components;
        std::vector<std::shared_ptr<GameObject>> m_safeChildren;
        std::vector<std::shared_ptr<Component>> m_safeComponents;
        bool m_active;
        bool m_needAwake;
        bool m_followParent, m_rotateWithParent;
        int m_zIndex;
        bool m_physicsEnabled;
        bool m_movedThisFrame;
    };
}

//? Export this type alias to the global scope
// Shared ptr to a shak::GameObject
using GameObjectPtr = std::shared_ptr<shak::GameObject>;
