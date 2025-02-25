#pragma once
#include "EngineDefines.h"
#include "TextureAtlas.h"
#include "cmrc/cmrc.hpp"

namespace shak
{
    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager() = default;

        // Texture
        std::shared_ptr<sf::Texture> LoadTexture(const std::string& path, const std::string& name, bool repeated = false, bool smooth = false);
        void UnloadTexture(const std::string& name);
        std::shared_ptr<sf::Texture> GetTexture(const std::string& name) const;

        // TextureAtlas
        std::shared_ptr<TextureAtlas> LoadTextureAtlas(const std::string& path, const std::string& name);
        void UnloadTextureAtlas(const std::string& name);
        std::shared_ptr<TextureAtlas> GetTextureAtlas(const std::string& name) const;

        // Shader
        std::shared_ptr<sf::Shader> LoadShader(const fs::path& vpath, const fs::path& fpath, const std::string& name);
        void UnloadShader(const std::string& name);
        std::shared_ptr<sf::Shader> GetShader(const std::string& name) const;

        // Fonts
        std::shared_ptr<sf::Font> LoadFont(const fs::path& path, const std::string& name);
        void UnloadFont(const std::string& name);
        std::shared_ptr<sf::Font> GetFont(const std::string& name) const;

        // Sound (short in-memory sounds)
        std::shared_ptr<sf::Sound> LoadSound(const fs::path& path, const std::string& name);
        void UnloadSound(const std::string& name);
        std::shared_ptr<sf::Sound> GetSound(const std::string& name) const;

        // Music (long memory-streamed music)
        std::shared_ptr<sf::Music> LoadMusic(const fs::path& path, const std::string& name);
        void UnloadMusic(const std::string& name);
        std::shared_ptr<sf::Music> GetMusic(const std::string& name) const;
    
    private:
        struct SoundData
        {
            std::shared_ptr<sf::SoundBuffer> buffer;
            std::shared_ptr<sf::Sound> sound;
        };

    private:
        cmrc::embedded_filesystem m_embeddedFilesystem;

        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_loadedTextures;
        std::unordered_map<std::string, std::shared_ptr<TextureAtlas>> m_loadedAtlases;
        std::unordered_map<std::string, std::shared_ptr<sf::Shader>> m_loadedShaders;
        std::unordered_map<std::string, std::shared_ptr<sf::Font>> m_loadedFonts;
        std::unordered_map<std::string, SoundData> m_loadedSounds;
        std::unordered_map<std::string, std::shared_ptr<sf::Music>> m_loadedMusic;
    };
}