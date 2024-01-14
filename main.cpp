#include "mbed.h"
#include "rtos.h"
#include <atomic>
#include "Sharp.hpp"
#include "DisplayManager.hpp"
#include <cstdio>
#include <cstdlib>

InterruptIn Bouton(BUTTON1);

bool CmdSuivant;

enum Etat {ATTENTE, CALIBRATION, DESSINE, EFFACE};
Etat etatActuel = ATTENTE;

int map(float x, int in_min, int in_max, int out_min, int out_max) {
    return static_cast<int>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min); //static_cast arrondi vers 0
}

// Premier thread managé par rtos
int main() {

    Sharp capteurDroit(PA_0);

    UnbufferedSerial serial(USBTX,USBRX,9600);
    DisplayManager display(&serial);

    Bouton.fall(&startPress);

    float avgDistance;
    int yCoordToSend;

    while(1){

        switch(etatActuel) {
        case ATTENTE:
        
            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = CALIBRATION;
            }
            break;

        case CALIBRATION:

            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = DESSINE;
            }
            break;

        case DESSINE:
            avgDistance = capteurDroit; // Equ à capteurDroit.getAvgDistance(5);
            yCoordToSend = map(avgDistance,10,80,10, 590);
            display.sendCoordinates(400, round(yCoordToSend));

            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = EFFACE;
            }
            break;

        case EFFACE:
            display.clearScreen();
            if (display.checkClearOk()) {
                etatActuel = ATTENTE;
            }
            break;
    }

        //printf("Etat calibration : %s\n", CmdSuivant ? "true" : "false");
        ThisThread::sleep_for(10ms);
    }

    return 0;
}
