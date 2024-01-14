#ifndef DISPLAYMNGR_HPP
#define DISPLAYMNGR_HPP

#include "mbed.h"
#include <string>

/**
 * @class DisplayManager
 * @brief Classe pour gérer les opérations d'affichage.
 */
class DisplayManager {
public:
    /**
     * @brief Construit un nouvel objet Display Manager.
     * 
     * @param serial Pointeur vers un objet UnbufferedSerial à utiliser pour la communication série.
     */
    DisplayManager(UnbufferedSerial *serial);

    /**
     * @brief Envoie des coordonnées via la communication série.
     * 
     * Cette fonction convertit les coordonnées en chaînes de caractères, les formate en un message,
     * et envoie le message via la communication série.
     * 
     * @param x La coordonnée x à envoyer.
     * @param y La coordonnée y à envoyer.
     */
    void sendCoordinates(int x, int y);

    void clearScreen();

    bool checkClearOk();

private:
    UnbufferedSerial *commSerie; ///< Pointeur vers un objet UnbufferedSerial pour la communication série.
};

#endif //DISPLAYMNGR_HPP