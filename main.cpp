#include "mbed.h"
#include "Sharp.hpp"
#include "DisplayManager.hpp"
#include <cstdio>
#include <cstdlib>

InterruptIn Bouton(BUTTON1);
Timer timerAttente;
float dureeAttente;

bool CmdSuivant;

enum Etat {ATTENTE, CALIBRATION, DESSINE, EFFACE};
Etat etatActuel = ATTENTE;

void startPress() {
    CmdSuivant = true;
}

int map(float x, int in_min, int in_max, int out_min, int out_max) {
    return static_cast<int>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min); //static_cast arrondi vers 0
}

int main() {

    Sharp capteurDroit(PA_0);
    Sharp capteurGauche(PA_1);

    UnbufferedSerial serial(USBTX,USBRX,9600);
    DisplayManager display(&serial);

    Bouton.fall(&startPress);

    float avgDistanceGauche;
    int xCoordToSend;
    float avgDistanceDroite;
    int yCoordToSend;

    while(1){

        switch(etatActuel) {
        case ATTENTE:
        
            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = CALIBRATION;
                timerAttente.start();
            }
            break;

        case CALIBRATION:

            dureeAttente = std::chrono::duration<float>(timerAttente.elapsed_time()).count();

            if (dureeAttente >= 5.0) {
                timerAttente.stop();
                timerAttente.reset();
                etatActuel = DESSINE;
            }
            break;

        case DESSINE:
            avgDistanceGauche = capteurGauche; // Equ à capteurGauche.getAvgDistance(5);
            xCoordToSend = map(avgDistanceGauche,10,80,110, 690);
            avgDistanceDroite = capteurDroit;
            yCoordToSend = map(avgDistanceDroite,10,80,110, 470);
            display.sendCoordinates(xCoordToSend, yCoordToSend);

            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = EFFACE;
            }
            break;

        case EFFACE:
            display.clearScreen();

            etatActuel = ATTENTE;

            break;
    }
        ThisThread::sleep_for(10ms);
    }

    return 0;
}
