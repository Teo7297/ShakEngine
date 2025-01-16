#include "TextureAtlas.h"

namespace shak
{

    TextureAtlas::TextureAtlas(const fs::path& path)
    {
        Parse(path);
        m_atlas = std::make_shared<sf::Texture>(m_texturePath.string());
        m_atlas->setRepeated(m_repeat);
        m_atlas->setSmooth(m_smooth);
    }

    TextureAtlas::~TextureAtlas()
    {
    }

    TextureCoordinates TextureAtlas::GetTextureCoords(int id) const
    {
        auto regionIt = m_regions.find(id);
        if (regionIt == m_regions.cend())
        {
            std::cerr << "[Texture Atlas] Region not found: " << id << std::endl;
            return TextureCoordinates{};
        }

        const auto& region = regionIt->second;

        TextureCoordinates coords
        {
            .topLeft = { (float)region.position.x, (float)region.position.y },
            .bottomLeft = { (float)region.position.x, (float)region.position.y + (float)region.size.y },
            .topRight = { (float)region.position.x + (float)region.size.x, (float)region.position.y },
            .bottomRight = { (float)region.position.x + (float)region.size.x, (float)region.position.y + (float)region.size.y }
        };

        return coords;
    }

    void TextureAtlas::Parse(const fs::path& path)
    {
        std::ifstream stream(path);
        if (!stream.is_open())
        {
            std::cerr << "Failed to open file: " << path << std::endl;
            return;
        }

        std::string line;
        int lineCount = 0;
        bool isId = true;
        int currentId = -1;
        while (std::getline(stream, line))
        {
            if (lineCount++ == 0)
            {
                fs::path texturePath = line;
                m_texturePath = path.parent_path() / texturePath;
            }

            else if (line._Starts_with("size:"))
            {
                std::vector<int> size;
                std::string prefix = "size:";
                size_t prefixLength = prefix.length();

                // Check if the string starts with the prefix
                if (line.substr(0, prefixLength) == prefix)
                {
                    // Extract the part of the string after the prefix
                    std::string sizeStr = line.substr(prefixLength);

                    // Use a string stream to parse the values
                    std::stringstream ss(sizeStr);
                    std::string token;
                    while (std::getline(ss, token, ','))
                    {
                        size.push_back(std::stoi(token));
                    }

                    m_atlasSize = { size[0], size[1] };
                }
            }

            else if (line._Starts_with("filter:Linear"))
            {
                // m_atlas->setSmooth(true);
                m_smooth = true;
            }

            else if (line._Starts_with("repeat:none"))
            {
                m_repeat = false;
            }

            else if (isId)
            {
                currentId = std::stoi(line);
                isId = false;
            }
            else
            {
                m_regions[currentId] = ExtractRegion(line);
                m_count++;
                isId = true;
            }
        }
    }

    sf::IntRect TextureAtlas::ExtractRegion(const std::string& line)
    {
        std::vector<int> bounds;
        std::string prefix = "bounds:";
        size_t prefixLength = prefix.length();

        // Check if the string starts with the prefix
        if (line.substr(0, prefixLength) == prefix)
        {
            // Extract the part of the string after the prefix
            std::string boundsStr = line.substr(prefixLength);

            // Use a string stream to parse the values
            std::stringstream ss(boundsStr);
            std::string token;
            while (std::getline(ss, token, ','))
            {
                bounds.push_back(std::stoi(token));
            }
        }

        return sf::IntRect{ {bounds[0], bounds[1]}, {bounds[2], bounds[3]} };
    }

}