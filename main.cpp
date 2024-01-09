#include "mbed.h"
#include <cstdlib>

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

// PRODUIT EN CROIX !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {

    Sharp capteurDroit(PA_0);

    UnbufferedSerial serial(USBTX,USBRX,9600);

    comm display(&serial);

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

        //printf("Point y : %d\n", yCoordToSend);
        display.sendCoordinates(400, abs(yCoordToSend));

        // double tension = capteurDroit.readRawValue();
        // double distance = capteurDroit.getRawDistance();

        //printf("Distance estimée : %d\n", avgDistance);

        ThisThread::sleep_for(10ms);
    }

    return 0;
}
