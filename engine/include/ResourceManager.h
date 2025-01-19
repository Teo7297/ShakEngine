#pragma once
#include "EngineDefines.h"
#include "TextureAtlas.h"
namespace shak
{
    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager();

        std::shared_ptr<sf::Texture> LoadTexture(const std::string& path, const std::string& name, bool repeated = false, bool smooth = false);
        void UnloadTexture(const std::string& name);
        std::shared_ptr<sf::Texture> GetTexture(const std::string& name) const;

        std::shared_ptr<TextureAtlas> LoadTextureAtlas(const std::string& path, const std::string& name);
        void UnloadTextureAtlas(const std::string& name);
        std::shared_ptr<TextureAtlas> GetTextureAtlas(const std::string& name) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_loadedTextures;
        std::unordered_map<std::string, std::shared_ptr<TextureAtlas>> m_loadedAtlases;
    };
}