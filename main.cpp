#include "mbed.h"
#include "rtos.h"
#include <atomic>
#include "Sharp.hpp"
#include "DisplayManager.hpp"
#include <cstdio>
#include <cstdlib>

InterruptIn Bouton(BUTTON1);
Timer Appuie; 

bool CmdReprendre;
bool CmdSuivant;

enum Etat {ATTENTE, CALIBRATION, DESSINE, EFFACE};
Etat etatActuel = ATTENTE;

InterruptIn Bouton(BUTTON1);
Timer Appuie;

Thread thread;
bool clearOKrecu(false);

bool CmdReprendre;
bool CmdSuivant;

enum Etat {ATTENTE, CALIBRATION, DESSINE, EFFACE};
Etat etatActuel = ATTENTE;

void checkClearOk() {
    char retourProcessing[10];
    while (!clearOKrecu) {
        scanf("%s", retourProcessing);
        if (strcmp(retourProcessing, "clearOk") == 0) {
            clearOKrecu = true;
        }
    }
}

void ClearScreen() {
    printf("clear\n");
}

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
        CmdReprendre=true;
        CmdSuivant=false;
    } else if(DureeAppuie >= 3.0){
        CmdReprendre=false;
        CmdSuivant=true;
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

    int lower_bound_x = 10; // borne inférieure
    int upper_bound_x = 790; // borne supérieure
    int lower_bound_y = 10; // borne inférieure
    int upper_bound_y = 590; // borne supérieure
    int x,y;

    srand(time(NULL)); // initialiser le générateur de nombres aléatoires

    while(1){

        x = lower_bound_x + rand() % (upper_bound_x - lower_bound_x + 1); // générer un nombre aléatoire entre lower_bound et upper_bound
        y = lower_bound_y + rand() % (upper_bound_y - lower_bound_y + 1);

        int avgDistance = capteurDroit.getAvgDistance(5);

        int yCoordToSend = map(avgDistance,7,40,10, 590);

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

            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = EFFACE;
            }
            break;
        case EFFACE:
            if (ClearScreen()) {
                if (CmdSuivant) {
                    etatActuel = ATTENTE;
                } else if (CmdReprendre) {
                    etatActuel = DESSINE;
                }
            }
            break;
    }

        display.sendCoordinates(400, abs(yCoordToSend));

        switch(etatActuel) {
        case ATTENTE:

            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = CALIBRATION;
            }
            break;
        case CALIBRATION:

            // if (CmdSuivant) {
            //     CmdSuivant = false;
            //     etatActuel = DESSINE;
            // }
            break;
        case DESSINE:
            
            if (CmdSuivant) {
                CmdSuivant = false;
                etatActuel = EFFACE;
            }
            break;
        case EFFACE:

            ClearScreen();
            thread.start(checkClearOk);
            //thread.join();

            if (clearOKrecu) {
                thread.terminate();
                if (CmdSuivant) {
                    etatActuel = ATTENTE;
                    clearOKrecu = false;
                } else if (CmdReprendre) {
                    etatActuel = DESSINE;
                    clearOKrecu = false;
                }
            }
            break;
    }

        //printf("Point y : %d\n", yCoordToSend);
        

        // double tension = capteurDroit.readRawValue();
        // double distance = capteurDroit.getRawDistance();

        //printf("Distance estimée : %d\n", avgDistance);

        //printf("Etat clearScreen : %s\n", CmdReprendre ? "true" : "false");
        //ThisThread::sleep_for(1000ms);
        //printf("Etat calibration : %s\n", CmdSuivant ? "true" : "false");
        ThisThread::sleep_for(100ms);
    }

    return 0;
}
