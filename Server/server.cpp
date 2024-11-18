/*
    Author: Eddie Kwak
    Class: ECE 4122
    Last Date Modified: 11/17/2024

    Description: This program implements the server-side logic for a robot simulation using SFML
                 for visuals. The server listens for incoming UDP packets on a specified port (8080)
                 and interprets commands from the client. The server visualizes the robot and animates
                 its movements.

*/

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//             PREPROCESSOR DIRECTIVES                //
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//                        MAIN                        //
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // Create UDP socket.
    sf::UdpSocket socket;
    // Server port number.
    unsigned short port = 8080;

    // Bind socket to the port.
    if (socket.bind(port) != sf::Socket::Done)
    {
        std::cerr << "Failed to bind port " << port << "\n";
        return -1;
    }

    // Set socket to non-blocking mode -> allows for continuous movement without waiting for packet.
    socket.setBlocking(false);

    std::cout << "Server listening to port: " << port << "\n";

    // Create window and set maximum framerate.
    sf::RenderWindow window(sf::VideoMode(800, 600), "Robot Simulation");
    window.setFramerateLimit(120);

    // Create robot.
    sf::ConvexShape robot;
    robot.setPointCount(8);

    // Radius of robot.
    float radius = 10.0f;
    sf::Vector2f center(50, 30);

    // Creating robot.
    for (int i = 0; i < 8; ++i) {
        float angle = i * (2 * 3.14 / 8); // Angle in radians
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);
        robot.setPoint(i, sf::Vector2f(x, y));
    }

    // Set robot properties.
    robot.setFillColor(sf::Color::Blue);
    robot.setOutlineColor(sf::Color::White);
    robot.setOutlineThickness(2.0f);
    robot.setPosition(400.f, 300.f); // Middle of the screen

    // Initialize movement speed.
    float speed = 3.0f;

    // Initial movement direciton = no movement.
    sf::Vector2f direction(0.f, 0.f);

    // Clock to measure elapsed time for movement.
    sf::Clock clock; 

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Initialize address of sender, port, and variable to hold incoming packet.
        sf::IpAddress sender;
        unsigned short senderPort;
        sf::Packet packet;

        // Checking for incoming packets.
        if (socket.receive(packet, sender, senderPort) == sf::Socket::Done)
        {
            std::cout << "Client connected successfully from IP: " << sender << " on port: " << senderPort << std::endl;
            int command;
            packet >> command;

            // Interpret the command.
            if (command == 'w') {
                direction = sf::Vector2f(0.0f, -1.0f);
                std::cout << "MOVING UP\n";
            }
            else if (command == 's') {
                direction = sf::Vector2f(0.0f, 1.0f);
                std::cout << "MOVING DOWN\n";
            }
            else if (command == 'a') {
                direction = sf::Vector2f(-1.0f, 0.0f);
                std::cout << "MOVING LEFT\n";
            }
            else if (command == 'd') {
                direction = sf::Vector2f(1.0f, 0.0f);
                std::cout << "MOVING RIGHT\n";
            }
            else if (command == 'g') {
                speed += 2.5f;
                std::cout << "SPEEDING UP\n";
            }
            else if (command == 'h') {
                if (speed - 2.5f < 1.0f) {
                    speed = 5.0f;
                }
                else {
                    speed -= 2.5f;
                }
                std::cout << "SLOWING DOWN\n";
            }
            else if (command == 'q') {
                window.close();
            }
        }

        // Move the robot
        float deltaTime = clock.restart().asSeconds();
        robot.move(direction * speed * deltaTime);

        window.clear();
        window.draw(robot);
        window.display();
    }

    return 0;
}
