#include "ResourceManager.h"
namespace shak
{
    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    std::shared_ptr<sf::Texture> ResourceManager::LoadTexture(const std::string& path, const std::string& name, bool repeated, bool smooth)
    {
        if (m_loadedTextures.find(name) != m_loadedTextures.end())
            return m_loadedTextures[name];

        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path))
        {
            std::cerr << "Failed to load texture [" << name << "] at location: " << path << std::endl;
            return nullptr;
        }
        texture->setRepeated(repeated);
        texture->setSmooth(smooth);

        m_loadedTextures[name] = texture;
        return texture;
    }

    void ResourceManager::UnloadTexture(const std::string& name)
    {
        if (m_loadedTextures.find(name) != m_loadedTextures.end())
            m_loadedTextures.erase(name);
    }

    std::shared_ptr<sf::Texture> ResourceManager::GetTexture(const std::string& name) const
    {
        if (m_loadedTextures.find(name) != m_loadedTextures.end())
            return m_loadedTextures.at(name);
        return nullptr;
    }

    std::shared_ptr<TextureAtlas> ResourceManager::LoadTextureAtlas(const std::string& path, const std::string& name)
    {
        if (m_loadedAtlases.find(name) != m_loadedAtlases.end())
            return m_loadedAtlases[name];

        auto atlas = std::make_shared<TextureAtlas>(path);
        m_loadedAtlases[name] = atlas;
        return atlas;
    }

    void ResourceManager::UnloadTextureAtlas(const std::string& name)
    {
        if (m_loadedAtlases.find(name) != m_loadedAtlases.end())
            m_loadedAtlases.erase(name);
    }

    std::shared_ptr<TextureAtlas> ResourceManager::GetTextureAtlas(const std::string& name) const
    {
        if (m_loadedAtlases.find(name) != m_loadedAtlases.end())
            return m_loadedAtlases.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Shader> ResourceManager::LoadShader(const fs::path& vpath, const fs::path& fpath, const std::string& name)
    {
        if (m_loadedShaders.find(name) != m_loadedShaders.end())
            return m_loadedShaders[name];

        auto shader = std::make_shared<sf::Shader>();
        bool success = false;

        if (vpath == "")
            success = shader->loadFromFile(fpath, sf::Shader::Type::Fragment);
        else if (fpath == "")
            success = shader->loadFromFile(vpath, sf::Shader::Type::Vertex);
        else
            success = shader->loadFromFile(vpath, fpath);

        if (!success)
        {
            std::cerr << "Failed to load shader [" << name << "] at location: " << vpath << ", " << fpath << std::endl;
            return nullptr;
        }

        m_loadedShaders[name] = shader;
        return shader;
    }

    void ResourceManager::UnloadShader(const std::string& name)
    {
    }

    std::shared_ptr<sf::Shader> ResourceManager::GetShader(const std::string& name) const
    {
        return std::shared_ptr<sf::Shader>();
    }
}
