#pragma once
#include "EngineDefines.h"
#include <vector>
#include <cmath>

// Struct to represent a point in the trail
struct TrailPoint {
    sf::Vector2f position;
    float ttl; // Time-to-live in seconds
    sf::Color color;

    TrailPoint(const sf::Vector2f& pos, float timeToLive)
        : position(pos), ttl(timeToLive), color(sf::Color::White) {
    }
};

class TrailRenderer {
public:
    TrailRenderer(float pointTTL = 1.0f) : pointTTL(pointTTL) {}

    // Add a new point to the trail
    void addPoint(const sf::Vector2f& position) {
        trailPoints.emplace_back(position, pointTTL);
    }

    // Update the trail points (fading and removal)
    void update(float deltaTime) {
        for (auto& point : trailPoints) {
            point.ttl -= deltaTime; // Reduce TTL
            float alpha = std::max(0.f, point.ttl / pointTTL); // Normalize TTL to 0-1
            point.color.a = (alpha * 255); // Fade out alpha
        }

        // Remove expired points
        trailPoints.erase(
            std::remove_if(trailPoints.begin(), trailPoints.end(),
                [](const TrailPoint& p) { return p.ttl <= 0.f; }),
            trailPoints.end());
    }

    // Render the trail
    void draw(sf::RenderWindow& window) const {
        sf::VertexArray trail(sf::PrimitiveType::LineStrip);

        for (const auto& point : trailPoints) {
            sf::Vertex vertex;
            vertex.position = point.position;
            vertex.color = point.color;
            trail.append(vertex);
        }

        window.draw(trail);
    }

private:
    std::vector<TrailPoint> trailPoints; // List of trail points
    float pointTTL; // Lifetime of each point in seconds
};

int LineTrailExample() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Trail Renderer Example");
    window.setFramerateLimit(60);

    // Create the trail renderer
    TrailRenderer trailRenderer(1.0f); // Points last 1 second

    // Moving object (e.g., a circle)
    sf::CircleShape object(10.f);
    object.setFillColor(sf::Color::Red);

    // Time management
    sf::Clock clock;
    float timeElapsed = 0.f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            {
                exit(0);
            }
        }

        // Delta time
        float deltaTime = clock.restart().asSeconds();

        // Move the object in a circular path
        timeElapsed += deltaTime;
        sf::Vector2f objectPosition(
            400.f + std::cos(timeElapsed * 2.f) * 200.f, // X-coordinate
            300.f + std::sin(timeElapsed * 2.f) * 200.f  // Y-coordinate
        );

        object.setPosition(objectPosition - sf::Vector2f(10.f, 10.f));

        // Update the trail with the new position
        trailRenderer.addPoint(objectPosition);
        trailRenderer.update(deltaTime);

        // Clear the window
        window.clear(sf::Color::Black);

        // Draw the trail and the object
        trailRenderer.draw(window);
        window.draw(object);

        // Display everything
        window.display();
    }

    return 0;
}