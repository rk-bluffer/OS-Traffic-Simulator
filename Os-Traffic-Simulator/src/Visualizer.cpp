#include <SFML/Graphics.hpp>
#include "../include/Intersection.h"
#include <thread>
#include <vector>
#include <iostream> // Added for debug prints

using namespace std;

// Define Colors
const sf::Color ROAD_COLOR(50, 50, 50);      // Dark Grey
const sf::Color GRASS_COLOR(34, 139, 34);    // Forest Green
const sf::Color CAR_NORMAL(255, 255, 0);     // Yellow
const sf::Color CAR_EMERGENCY(255, 0, 255);  // Magenta/Purple

// Main Visualizer Function
void runVisualizer(Intersection* intersection) {
    // Debug Print: Check if function is called
    cout << "[GUI] Initializing Graphics Engine..." << endl;

    // 1. Create the Window (800x600 pixels)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Smart City OS Simulator");
    window.setFramerateLimit(60); // Cap at 60 FPS for smoothness

    if (!window.isOpen()) {
        cout << "[GUI ERROR] Could not create the window!" << endl;
        return;
    }
    cout << "[GUI] Window created successfully!" << endl;

    // 2. Setup Graphical Assets (Simple Shapes)
    
    // -- ROADS --
    sf::RectangleShape roadV(sf::Vector2f(100, 600)); // Vertical Road
    roadV.setPosition(350, 0); 
    roadV.setFillColor(ROAD_COLOR);

    sf::RectangleShape roadH(sf::Vector2f(800, 100)); // Horizontal Road
    roadH.setPosition(0, 250); 
    roadH.setFillColor(ROAD_COLOR);

    // -- TRAFFIC LIGHTS --
    sf::CircleShape lights[4]; 
    // Positions: North, East, South, West corners
    sf::Vector2f lightPos[] = {{300, 200}, {500, 200}, {500, 400}, {300, 400}};
    for(int i=0; i<4; i++) {
        lights[i].setRadius(15);
        lights[i].setPosition(lightPos[i]);
    }

    // 3. Main Graphics Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: click X
            if (event.type == sf::Event::Closed) window.close();
            
            // Close window: press Escape key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                cout << "[GUI] Escape key pressed. Closing..." << endl;
                window.close();
            }
        }

        // --- FETCH DATA FROM SHARED MEMORY ---
        int greenLaneIndex;
        // Thread-safe read of the cars to draw
        vector<Vehicle> carsToDraw = intersection->getDisplayState(greenLaneIndex);

        // --- DRAWING PHASE ---
        window.clear(GRASS_COLOR); // 1. Draw Background

        // 2. Draw Roads
        window.draw(roadV);
        window.draw(roadH);

        // 3. Draw Traffic Lights (Red/Green logic)
        for(int i=0; i<4; i++) {
            if(i == greenLaneIndex) lights[i].setFillColor(sf::Color::Green);
            else lights[i].setFillColor(sf::Color::Red);
            window.draw(lights[i]);
        }

        // 4. Draw Cars
        for(const auto& car : carsToDraw) {
            sf::RectangleShape carShape(sf::Vector2f(30, 30));
            
            // Set Color based on Priority
            if(car.isEmergency) carShape.setFillColor(CAR_EMERGENCY);
            else carShape.setFillColor(CAR_NORMAL);

            // Calculate Screen Position based on Direction and Queue Position
            // (Simplified visual queueing)
            float spacing = 40.0f;
            int visualQueuePos = car.id % 5; // Wrap around so they don't go off screen

            switch(car.dir) {
                case NORTH: carShape.setPosition(385, 200 - (visualQueuePos * spacing)); break;
                case SOUTH: carShape.setPosition(385, 400 + (visualQueuePos * spacing)); break;
                case WEST:  carShape.setPosition(200 - (visualQueuePos * spacing), 285); break;
                case EAST:  carShape.setPosition(600 + (visualQueuePos * spacing), 285); break;
            }
            window.draw(carShape);
        }

        window.display(); // Flip the screen buffer
    }
    cout << "[GUI] Visualizer loop ended." << endl;
}