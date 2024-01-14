#include "Sharp.hpp"

Sharp::Sharp(PinName pin) : analogInput(pin) {
    this->distanceMin = defautDistanceMin;
    this->distanceMax = defautDistanceMax;
    this->coeff = defautCoeff;
    this->decalage = defautDecalage;
}

Sharp::Sharp(PinName pin, int distanceMin, int distanceMax, float coeff, float decalage)  : analogInput(pin) {
    this->distanceMin = distanceMin;
    this->distanceMax = distanceMax;
    this->coeff = coeff;
    this->decalage = decalage;
}

Sharp::operator float() {return getAvgDistance(5);}

double Sharp::readRawValue() {
    double rawValue = analogInput.read() * 5;
    return rawValue;
}

float Sharp::getRawDistance() {
    float distance = coeff / (readRawValue() - decalage);
    if (distance > distanceMax) return distanceMax;
    if (distance < distanceMin) return distanceMin;
    return distance;
}

float Sharp::getAvgDistance(int n) {
    float moy = 0;
    for(int i = 0; i < n; i++) {
        moy += getRawDistance();
        ThisThread::sleep_for(5ms);
    }
    return (moy / n);
}

