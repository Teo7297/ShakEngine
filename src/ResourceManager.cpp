#include "ResourceManager.h"

#define SHADERS_PATH "assets/shaders/"

CMRC_DECLARE(shak);

namespace shak
{
    ResourceManager::ResourceManager()
        : m_embeddedFilesystem(cmrc::shak::get_filesystem())
        , m_prefix("")
    {
    }

    void ResourceManager::Clear()
    {
        m_loadedTextures.clear();
        m_loadedImages.clear();
        m_loadedAtlases.clear();
        m_loadedShaders.clear();
        m_loadedFonts.clear();
        m_loadedSounds.clear();
        m_loadedMusic.clear();
    }

    std::shared_ptr<sf::Texture> ResourceManager::LoadTexture(const std::string& path, const std::string& name, bool repeated, bool smooth)
    {
        if(m_loadedTextures.contains(name))
            return m_loadedTextures[name];

        auto texture = std::make_shared<sf::Texture>();
        fs::path actualPath = m_prefix / path;

        bool success = false;
        // Check if the texture is in the embedded filesystem
        if(m_embeddedFilesystem.exists(actualPath.string()))
        {
            auto file = m_embeddedFilesystem.open(actualPath.string());
            success = texture->loadFromMemory(file.begin(), file.size());
        }
        else
        {
            success = texture->loadFromFile(actualPath);
        }

        if(!success)
        {
            std::cerr << "Failed to load texture [" << name << "] at location: " << actualPath << std::endl;
            return nullptr;
        }

        texture->setRepeated(repeated);
        texture->setSmooth(smooth);

        m_loadedTextures[name] = texture;
        return texture;
    }

    void ResourceManager::UnloadTexture(const std::string& name)
    {
        if(m_loadedTextures.contains(name))
            m_loadedTextures.erase(name);
    }

    std::shared_ptr<sf::Texture> ResourceManager::GetTexture(const std::string& name) const
    {
        if(m_loadedTextures.contains(name))
            return m_loadedTextures.at(name);
        return nullptr;
    }

    std::shared_ptr<TextureAtlas> ResourceManager::LoadTextureAtlas(const std::string& path, const std::string& name)
    {
        if(m_loadedAtlases.contains(name))
            return m_loadedAtlases[name];

        fs::path actualPath = m_prefix / path;
            auto atlas = std::make_shared<TextureAtlas>(actualPath);
            m_loadedAtlases[name] = atlas;
            return atlas;
    }

    void ResourceManager::UnloadTextureAtlas(const std::string& name)
    {
        if(m_loadedAtlases.contains(name))
            m_loadedAtlases.erase(name);
    }

    std::shared_ptr<TextureAtlas> ResourceManager::GetTextureAtlas(const std::string& name) const
    {
        if(m_loadedAtlases.contains(name))
            return m_loadedAtlases.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Image> ResourceManager::LoadSFImage(const fs::path& path, const std::string& name)
    {
        if(m_loadedImages.contains(name))
            return m_loadedImages[name];

        auto image = std::make_shared<sf::Image>();
        fs::path actualPath = m_prefix / path;

        bool success = false;
        // Check if the image is in the embedded filesystem
        if(m_embeddedFilesystem.exists(actualPath.string()))
        {
            auto file = m_embeddedFilesystem.open(actualPath.string());
            success = image->loadFromMemory(file.begin(), file.size());
        }
        else
        {
            success = image->loadFromFile(actualPath);
        }

        if(!success)
        {
            std::cerr << "Failed to load image [" << name << "] at location: " << actualPath << std::endl;
            return nullptr;
        }

        m_loadedImages[name] = image;
        return image;
    }

    void ResourceManager::UnloadSFImage(const std::string& name)
    {
        if(m_loadedImages.contains(name))
            m_loadedImages.erase(name);
    }

    std::shared_ptr<sf::Image> ResourceManager::GetSFImage(const std::string& name) const
    {
        if(m_loadedImages.contains(name))
            return m_loadedImages.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Shader> ResourceManager::LoadShader(const fs::path& vpath, const fs::path& fpath, const std::string& name)
    {
        if(m_loadedShaders.contains(name))
            return m_loadedShaders[name];

        auto shader = std::make_shared<sf::Shader>();
        bool success;

        if(vpath == "")
        {
            // TODO: check why we are using this macro instead of the m_prefix
            std::string internalFragPath = SHADERS_PATH + fpath.filename().string();
            if(m_embeddedFilesystem.exists(internalFragPath))
            {
                const auto fs = m_embeddedFilesystem.open(internalFragPath);
                success = shader->loadFromMemory(fs.begin(), sf::Shader::Type::Fragment);
            }
            else
                success = shader->loadFromFile(fpath, sf::Shader::Type::Fragment);
        }
        else if(fpath == "")
        {
            std::string internalVertPath = SHADERS_PATH + vpath.filename().string();
            if(m_embeddedFilesystem.exists(internalVertPath))
            {
                const auto vs = m_embeddedFilesystem.open(internalVertPath);
                success = shader->loadFromMemory(vs.begin(), sf::Shader::Type::Vertex);
            }
            else
                success = shader->loadFromFile(vpath, sf::Shader::Type::Vertex);
        }
        else
        {
            std::string internalFragPath = SHADERS_PATH + fpath.filename().string();
            std::string internalVertPath = SHADERS_PATH + vpath.filename().string();

            if(m_embeddedFilesystem.exists(internalFragPath) && m_embeddedFilesystem.exists(internalVertPath))
            {
                const auto fs = m_embeddedFilesystem.open(internalFragPath);
                const auto vs = m_embeddedFilesystem.open(internalVertPath);
                success = shader->loadFromMemory(vs.begin(), fs.begin());
            }
            else
                success = shader->loadFromFile(vpath, fpath);
        }

        if(!success)
        {
            std::cerr << "Failed to load shader [" << name << "] at location: " << vpath << ", " << fpath << std::endl;
            return nullptr;
        }

        m_loadedShaders[name] = shader;
        return shader;
    }

    void ResourceManager::UnloadShader(const std::string& name)
    {
        if(m_loadedShaders.contains(name))
            m_loadedShaders.erase(name);
    }

    std::shared_ptr<sf::Shader> ResourceManager::GetShader(const std::string& name) const
    {
        if(m_loadedShaders.contains(name))
            return m_loadedShaders.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Font> ResourceManager::LoadFont(const fs::path& path, const std::string& name)
    {
        if(m_loadedFonts.contains(name))
            return m_loadedFonts[name];

        auto font = std::make_shared<sf::Font>();
        fs::path actualPath = m_prefix / path;

        bool success = false;
        // Check if the font is in the embedded filesystem
        if(m_embeddedFilesystem.exists(actualPath.string()))
        {
            auto file = m_embeddedFilesystem.open(actualPath.string());
            success = font->openFromMemory(file.begin(), file.size());
        }
        else
        {
            success = font->openFromFile(actualPath);
        }

        if(!success)
        {
            std::cerr << "Failed to load font [" << name << "] at location: " << actualPath << std::endl;
            return nullptr;
        }

        m_loadedFonts[name] = font;
        return font;
    }

    void ResourceManager::UnloadFont(const std::string& name)
    {
        if(m_loadedFonts.contains(name))
            m_loadedFonts.erase(name);
    }

    std::shared_ptr<sf::Font> ResourceManager::GetFont(const std::string& name) const
    {
        if(m_loadedFonts.contains(name))
            return m_loadedFonts.at(name);
        return nullptr;
    }

    std::shared_ptr<sf::Sound> ResourceManager::LoadSound(const fs::path& path, const std::string& name)
    {
        if(m_loadedSounds.contains(name))
            return m_loadedSounds[name].sound;

        fs::path actualPath = m_prefix / path;
        auto buffer = std::make_shared<sf::SoundBuffer>();

        bool success = false;
        // Check if the sound is in the embedded filesystem
        if(m_embeddedFilesystem.exists(actualPath.string()))
        {
            auto file = m_embeddedFilesystem.open(actualPath.string());
            success = buffer->loadFromMemory(file.begin(), file.size());
        }
        else
        {
            success = buffer->loadFromFile(actualPath);
        }

        if(!success)
        {
            std::cerr << "Failed to load sound [" << name << "] at location: " << actualPath << std::endl;
            return nullptr;
        }

        auto sound = std::make_shared<sf::Sound>(*buffer);
        m_loadedSounds[name] = { buffer, sound };
        return sound;
    }

    void ResourceManager::UnloadSound(const std::string& name)
    {
        if(m_loadedSounds.contains(name))
            m_loadedSounds.erase(name);
    }

    std::shared_ptr<sf::Sound> ResourceManager::GetSound(const std::string& name) const
    {
        if(m_loadedSounds.contains(name))
            return m_loadedSounds.at(name).sound;
        return nullptr;
    }

    std::shared_ptr<sf::Music> ResourceManager::LoadMusic(const fs::path& path, const std::string& name)
    {
        if(m_loadedMusic.contains(name))
            return m_loadedMusic[name];

        auto music = std::make_shared<sf::Music>();
        fs::path actualPath = m_prefix / path;

        bool success = false;
        // Check if the music is in the embedded filesystem
        if(m_embeddedFilesystem.exists(actualPath.string()))
        {
            auto file = m_embeddedFilesystem.open(actualPath.string());
            // For music, we need to create a temporary file since SFML's Music can't load from memory
            // But this is a placeholder - you might need to implement a custom solution
            std::cerr << "Loading music from embedded resources is not implemented yet." << std::endl;
            success = false;
        }
        else
        {
            success = music->openFromFile(actualPath);
        }

        if(!success)
        {
            std::cerr << "Failed to load music [" << name << "] at location: " << actualPath << std::endl;
            return nullptr;
        }

        m_loadedMusic[name] = music;
        return music;
    }

    void ResourceManager::UnloadMusic(const std::string& name)
    {
        if(m_loadedMusic.contains(name))
            m_loadedMusic.erase(name);
    }

    std::shared_ptr<sf::Music> ResourceManager::GetMusic(const std::string& name) const
    {
        if(m_loadedMusic.contains(name))
            return m_loadedMusic.at(name);
        return nullptr;
    }
}
