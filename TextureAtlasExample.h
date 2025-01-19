#include <SFML/Graphics.hpp>
#include <vector>

// Vertex structure
struct Vertex {
    sf::Vector2f position;
    sf::Vector2f texCoords;
};

// Example texture atlas coordinates (assuming a 2x2 atlas)
sf::Vector2f atlasOffsets[4] = {
    {0.0f, 0.0f},   // Top-left
    {0.5f, 0.0f},   // Top-right
    {0.0f, 0.5f},   // Bottom-left
    {0.5f, 0.5f}    // Bottom-right
};

// Function to add a quad to the batch using triangles
void addQuadToBatch(std::vector<Vertex>& vertices, sf::Vector2f position, sf::Vector2f size, int textureIndex) {
    sf::Vector2f offset = atlasOffsets[textureIndex];
    sf::Vector2f texSize = sf::Vector2f(0.5f, 0.5f); // Assuming each texture is half of the atlas

    sf::Vector2f topLeft = position;
    sf::Vector2f topRight = position + sf::Vector2f(size.x, 0.0f);
    sf::Vector2f bottomLeft = position + sf::Vector2f(0.0f, size.y);
    sf::Vector2f bottomRight = position + size;

    sf::Vector2f texTopLeft = offset;
    sf::Vector2f texTopRight = offset + sf::Vector2f(texSize.x, 0.0f);
    sf::Vector2f texBottomLeft = offset + sf::Vector2f(0.0f, texSize.y);
    sf::Vector2f texBottomRight = offset + texSize;

    // First triangle
    vertices.push_back({ topLeft, texTopLeft });
    vertices.push_back({ bottomLeft, texBottomLeft });
    vertices.push_back({ bottomRight, texBottomRight });

    // Second triangle
    vertices.push_back({ topLeft, texTopLeft });
    vertices.push_back({ bottomRight, texBottomRight });
    vertices.push_back({ topRight, texTopRight });
}

// Rendering function
void renderBatch(sf::RenderWindow& window, const std::vector<Vertex>& vertices, sf::Texture& textureAtlas) {
    // Create a vertex array
    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles, vertices.size());

    // Fill vertex array with vertex data
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertexArray[i].position = vertices[i].position;
        auto A = vertices[i].texCoords;
        auto B = sf::Vector2f(textureAtlas.getSize());
        vertexArray[i].texCoords = { A.x * B.x, A.y * B.y };
    }

    // Create a render states object with the texture atlas
    sf::RenderStates states;
    states.texture = &textureAtlas;

    // Draw the vertex array
    window.draw(vertexArray, states);
}

int TextureAtlasExample() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML Batch Rendering with Texture Atlas");

    // Load the texture atlas
    sf::Texture textureAtlas;
    if (!textureAtlas.loadFromFile("../../../textures/bricks.jpg")) {
        return -1;
    }

    // Create a vector to hold the vertices
    std::vector<Vertex> vertices;

    // Add some quads to the batch
    addQuadToBatch(vertices, { 100, 100 }, { 50, 50 }, 0);
    addQuadToBatch(vertices, { 200, 100 }, { 50, 50 }, 1);
    addQuadToBatch(vertices, { 300, 100 }, { 50, 50 }, 2);
    addQuadToBatch(vertices, { 400, 100 }, { 50, 50 }, 3);

    // Main loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                exit(0);
            }
        }

        window.clear();
        renderBatch(window, vertices, textureAtlas);
        window.display();
    }

    return 0;
}