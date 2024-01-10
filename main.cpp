#include "mbed.h"
#include <cstdio>
#include <cstdlib>

InterruptIn Bouton(BUTTON1);
Timer Appuie; 

bool CmdReprendre;
bool CmdSuivant;

enum Etat {ATTENTE, CALIBRATION, DESSINE, EFFACE};
Etat etatActuel = ATTENTE;

class Sharp {
private:
    AnalogIn analogInput;

public:
    Sharp(PinName pin) : analogInput(pin) {}

    //tension en V
    double readRawValue() {
        double rawValue = analogInput.read() * 5;
        return rawValue;
  }

    //distance en cm
    int getRawDistance() {
        double distance = 13 * pow(readRawValue(), -1); // d'apres la datasheet
        distance = 27.86 / (readRawValue() - 0.42);
        if (distance > 80) return 80;
        if (distance < 7) return 7;
        return round(distance);
    }

    //moyenne sur n lecture de distance
    int getAvgDistance(int n) {
        double moy = 0;
        for(int i = 0; i < n; i++) {
            moy += getRawDistance();
            //ToDo virer ca et implementer interruption
            ThisThread::sleep_for(10ms);
        }
        return round(moy / n); // Calculer et retourner la moyenne
    }

    // //Trop fort, permet de juste appeler valeur = capteurDroit au lieu de capteurDroit.getDistance
    // operator double(); //dans .hpp

    // Sharp::operator double() { //dans.cpp
    // return getDistance();
    // }
};

class comm {
public:
  comm(UnbufferedSerial *serial) : _serial(serial) {}

  void sendCoordinates(int x, int y) {
    /*
    char x_byte = (x & 0xFF);
    char y_byte = (y & 0xFF);
    uint8_t fusion = (x_byte << 8) | y_byte;
    
    uint8_t data[4] = {
        static_cast<uint8_t>(x & 0xFF),
        static_cast<uint8_t>((x >> 8) & 0xFF),
        static_cast<uint8_t>(y & 0xFF),
        static_cast<uint8_t>((y >> 8) & 0xFF),
    };
    */
    
    printf("%d,%d,\n", x, y);
    //_serial->write(&x_byte, sizeof(x_byte));
  }

private:
  UnbufferedSerial *_serial;
};

bool ClearScreen() {
    printf("Clear\n");
    /* On attend désormais la validation du processing*/
    char Check_Processing[10];
    scanf("%s", Check_Processing);
    if (strcmp(Check_Processing, "ClearOk") == 0) {
    /*si processing envoi le champ de validation alors on retourne True */
        return true;
    } else {
        return false;
    }
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

int main() {

    Sharp capteurDroit(PA_0);

    UnbufferedSerial serial(USBTX,USBRX,9600);
    comm display(&serial);

    //Configuration des interruptions
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

        //printf("Point y : %d\n", yCoordToSend);
        //display.sendCoordinates(400, abs(yCoordToSend));

        // double tension = capteurDroit.readRawValue();
        // double distance = capteurDroit.getRawDistance();

        //printf("Distance estimée : %d\n", avgDistance);

        printf("Etat clearScreen : %s\n", CmdReprendre ? "true" : "false");
        ThisThread::sleep_for(1000ms);
        printf("Etat calibration : %s\n", CmdSuivant ? "true" : "false");
        ThisThread::sleep_for(1000ms);
    }

    return 0;
}
