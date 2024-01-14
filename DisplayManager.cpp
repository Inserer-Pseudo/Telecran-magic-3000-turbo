#include "DisplayManager.hpp"

DisplayManager::DisplayManager(UnbufferedSerial *serial) : commSerie(serial) {}

void DisplayManager::sendCoordinates(int x, int y) {
        std::string x_str = std::to_string(x); // Attention : consomme de la mémoire
        std::string y_str = std::to_string(y);
        std::string message = x_str + "," + y_str + ",\n";
        commSerie->write(message.c_str(), message.length());
    }

void DisplayManager::clearScreen() {
    std::string message = "clear\n";
    commSerie->write(message.c_str(), message.length());
}

// bool DisplayManager::checkClearOk() {
//     char retourProcessing[10];
//     //while (!clearOKrecu) {
//         scanf("%s", retourProcessing);
//         if (strcmp(retourProcessing, "clearOk") == 0) {
//             clearOKrecu = true;
//         }
//     //}
// }

bool DisplayManager::checkClearOk() {
    char retourProcessing[10];
    int octetsLu = commSerie->read(retourProcessing, sizeof(retourProcessing) - 1);
    if (octetsLu > 0) {
        retourProcessing[octetsLu] = '\0';
        if (strcmp(retourProcessing, "clearOk") == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
            return false;
    }
}