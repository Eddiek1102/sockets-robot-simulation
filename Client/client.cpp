/*
    Author: Eddie Kwak
    Class: ECE 4122
    Last Date Modified: 11/17/2024

    Description: This program implements the client-side logic for an application using SFML for visuals. The client connects to a server
                 over UDP and allows the user to send commands to control movements or change setting (speed, direction). The commands are 
                 sent to the server as UDP packets.

*/


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//             PREPROCESSOR DIRECTIVES                //
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
#include <iostream>
#include <SFML/Network.hpp>



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//                      MAIN                          //
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // Create UDP socket.
    sf::UdpSocket socket;
    // Variable to store server address.
    sf::IpAddress server;
    // Temp string to store user input.
    std::string temp;
    // Predefined server port number.
    unsigned short serverPort = 8080;

    // Repeat until server connection.
    do
    {
        std::cout << "Enter the address of the server to connect to: ";
        // Read user input and get rid of the newline character.
        std::cin >> static_cast<std::string>(temp);
        // Assign server to user input.
        server = temp;
    } while (server == sf::IpAddress::None);

    // Bind the socket to available port.
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }

    std::cout << "Connected to server: " << server << "\n";
    std::cout << "Port: " << serverPort << "\n";

    // Variable to store command inputs.
    char input;
    // Packet for sending.
    sf::Packet packet;

    // Loop to continuously accept and send new commands.
    while (true)
    {
        // Prompt user for input and read.
        std::cout << "Enter command (w/s/a/d to move, g for faster, h for slower, q to quit): ";
        std::cin >> input;

        // Clear the packet.
        packet.clear();
        // Add the user input to the packet.
        packet << input;

        // Send the packet to the server.
        if (socket.send(packet, server, serverPort) != sf::Socket::Done)
        {
            std::cerr << "Failed to send packet." << std::endl;
        }

        // Exit the loop if the user prompts to quit the application.
        if (input == 'q')
        {
            break;
        }
    }

    return 0;
}