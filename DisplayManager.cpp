#include "DisplayManager.hpp"

DisplayManager::DisplayManager(UnbufferedSerial *serial) : commSerie(serial) {}

void DisplayManager::sendCoordinates(int x, int y) {
        std::string x_str = std::to_string(x); // Attention : consomme de la mémoire
        std::string y_str = std::to_string(y);
        std::string message = x_str + "," + y_str + ",\n";
        commSerie->write(message.c_str(), message.length());
    }