#include "mbed.h"
#include "rtos.h"
#include <atomic>
#include "Sharp.hpp"
#include "DisplayManager.hpp"
#include <cstdio>
#include <cstdlib>

InterruptIn Bouton(BUTTON1);
Timer Appuie;

Thread thread;

bool CmdEfface;
bool CmdSuivant;

enum Etat {ATTENTE, CALIBRATION, DESSINE, EFFACE};
Etat etatActuel = ATTENTE;

// PRODUIT EN CROIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void startPress(){
    Appuie.start();
}

void stopPress(){
    Appuie.stop();

    auto DureeAppuie = std::chrono::duration<float>(Appuie.elapsed_time()).count();
    if (DureeAppuie >= 0.2 && DureeAppuie < 3.0){
        CmdEfface=false;
        CmdSuivant=true;
    } else if(DureeAppuie >= 3.0){
        CmdEfface=true;
        CmdSuivant=false;
    }

    Appuie.reset();
}

// Premier thread managé par rtos
int main() {

    Sharp capteurDroit(PA_0);

    UnbufferedSerial serial(USBTX,USBRX,9600);
    DisplayManager display(&serial);

    Bouton.fall(&startPress);
    Bouton.rise(&stopPress);
    int avgDistance;
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
            yCoordToSend = map(avgDistance,7,40,10, 590);
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

        //printf("Point y : %d\n", yCoordToSend);
        

        // double tension = capteurDroit.readRawValue();
        // double distance = capteurDroit.getRawDistance();

        //printf("Distance estimée : %d\n", avgDistance);

        //printf("Etat clearScreen : %s\n", CmdEfface ? "true" : "false");
        //ThisThread::sleep_for(1000ms);
        //printf("Etat calibration : %s\n", CmdSuivant ? "true" : "false");
        ThisThread::sleep_for(100ms);
    }

    return 0;
}
