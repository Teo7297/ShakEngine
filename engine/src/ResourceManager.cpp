#include "ResourceManager.h"
namespace shak
{
    std::shared_ptr<sf::Texture> ResourceManager::LoadTexture(const std::string& path, const std::string& name, bool repeated, bool smooth)
    {
        if (m_loadedTextures.contains(name))
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
        if (m_loadedTextures.contains(name))
            m_loadedTextures.erase(name);
    }

    std::shared_ptr<sf::Texture> ResourceManager::GetTexture(const std::string& name) const
    {
        if (m_loadedTextures.contains(name))
            return m_loadedTextures.at(name);
        return nullptr;
    }

    std::shared_ptr<TextureAtlas> ResourceManager::LoadTextureAtlas(const std::string& path, const std::string& name)
    {
        if (m_loadedAtlases.contains(name))
            return m_loadedAtlases[name];

        auto atlas = std::make_shared<TextureAtlas>(path);
        m_loadedAtlases[name] = atlas;
        return atlas;
    }

    void ResourceManager::UnloadTextureAtlas(const std::string& name)
    {
        if (m_loadedAtlases.contains(name))
            m_loadedAtlases.erase(name);
    }

    std::shared_ptr<TextureAtlas> ResourceManager::GetTextureAtlas(const std::string& name) const
    {
        if (m_loadedAtlases.contains(name))
            return m_loadedAtlases.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Shader> ResourceManager::LoadShader(const fs::path& vpath, const fs::path& fpath, const std::string& name)
    {
        if (m_loadedShaders.contains(name))
            return m_loadedShaders[name];

        auto shader = std::make_shared<sf::Shader>();
        bool success;

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
        if (m_loadedShaders.contains(name))
            m_loadedShaders.erase(name);
    }

    std::shared_ptr<sf::Shader> ResourceManager::GetShader(const std::string& name) const
    {
        if (m_loadedShaders.contains(name))
            return m_loadedShaders.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Font> ResourceManager::LoadFont(const fs::path& path, const std::string& name)
    {
        if (m_loadedFonts.contains(name))
            return m_loadedFonts[name];

        auto font = std::make_shared<sf::Font>();
        if (!font->openFromFile(path))
        {
            std::cerr << "Failed to load font [" << name << "] at location: " << path << std::endl;
            return nullptr;
        }

        m_loadedFonts[name] = font;
        return font;
    }

    void ResourceManager::UnloadFont(const std::string& name)
    {
        if (m_loadedFonts.contains(name))
            m_loadedFonts.erase(name);
    }

    std::shared_ptr<sf::Font> ResourceManager::GetFont(const std::string& name) const
    {
        if (m_loadedFonts.contains(name))
            return m_loadedFonts.at(name);
        return nullptr;
    }
}
