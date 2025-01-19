#pragma once

#include "EngineDefines.h"

namespace shak
{

    struct TextureCoordinates
    {
        sf::Vector2f topLeft;
        sf::Vector2f bottomLeft;
        sf::Vector2f topRight;
        sf::Vector2f bottomRight;
    };

    class TextureAtlas
    {
    public:
        TextureAtlas(const fs::path& path);
        ~TextureAtlas();

        TextureCoordinates GetTextureCoords(int id) const;

        inline std::shared_ptr<sf::Texture> GetAtlasTexture() const { return m_atlas; }
        inline int GetCount() const { return m_count; }

    private:
        void Parse(const fs::path& path);
        sf::IntRect ExtractRegion(const std::string& line);

    private:
        fs::path m_texturePath;
        bool m_smooth = false;
        bool m_repeat = false;
        sf::Vector2i m_atlasSize;
        std::shared_ptr<sf::Texture> m_atlas;
        std::unordered_map<int, sf::IntRect> m_regions;
        int m_count = 0;
    };
}
