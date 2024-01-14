#include "Sharp.hpp"

Sharp::Sharp(PinName pin) : analogInput(pin) {
    distanceMin = defautDistanceMin;
    distanceMax = defautDistanceMax;
    coeff = defautCoeff;
    decalage = defautDecalage;
}

Sharp::Sharp(PinName pin, int distanceMin, int distanceMax, float coeff, float decalage)  : analogInput(pin) {
    this->distanceMin = distanceMin;
    this->distanceMax = distanceMax;
    this->coeff = coeff;
    this->decalage = decalage;
}

Sharp::operator double() {return getAvgDistance(5);}

double Sharp::readRawValue() {
    double rawValue = analogInput.read() * 5;
    return rawValue;
}

int Sharp::getRawDistance() {
    //double distance = coeff * pow(readRawValue(), -1); // d'apres la datasheet
    double distance = coeff / (readRawValue() - decalage);
    if (distance > distanceMax) return distanceMax;
    if (distance < distanceMin) return distanceMin;
    return round(distance);
}

int Sharp::getAvgDistance(int n) {
    double moy = 0;
    for(int i = 0; i < n; i++) {
        moy += getRawDistance();
        //ToDo virer ca et implementer interruption
        ThisThread::sleep_for(10ms);
    }
    return round(moy / n); // Calculer et retourner la moyenne
}

