#ifndef SHARP_HPP
#define SHARP_HPP

#include "mbed.h"
#include <cmath>

constexpr int defautDistanceMin = 10; ///< Distance minimale par défaut en cm.
constexpr int defautDistanceMax = 80; ///< Distance maximale par défaut en cm.
constexpr float defautCoeff = 27.86; ///< Coefficient par défaut pour le calcul de la distance.
constexpr float defautDecalage = 0.42; ///< Offset par défaut pour le calcul de la distance.

/**
 * @class Sharp
 * @brief Classe pour gérer les capteurs de distance Sharp.
 */
class Sharp {
private:
    AnalogIn analogInput; ///< Entrée analogique pour lire les données du capteur.
    int distanceMin;
    int distanceMax;
    float coeff;
    float decalage;

public:
    /**
     * @brief Construit un nouvel objet Sharp.
     * 
     * @param pin Le pin connecté au capteur.
     */
    Sharp(PinName pin);

    /**
     * @brief Construit un nouvel objet Sharp.
     * 
     * @param pin Le pin connecté au capteur.
     */
    Sharp(PinName pin, int distanceMin, int distanceMax, float coeff, float decalage);

    /**
     * @brief Surcharge de l'opérateur double.
     * 
     * Permet à l'objet d'être utilisé comme un double, renvoyant la distance lue à partir du capteur.
     * 
     * @return La distance lue à partir du capteur.
     */
    operator float();

    /**
     * @brief Lit la valeur brute du capteur.
     * 
     * @return La lecture brute du capteur en volts.
     */
    double readRawValue();

    /**
     * @brief Obtient la distance brute lue par le capteur.
     * 
     * @return La distance brute en cm, retournée comme un entier plus la valeur multipliée par 10.
     */
    float getRawDistance();

    /**
     * @brief Obtient la distance moyenne lue par le capteur sur un certain nombre de lectures.
     * 
     * @param n Le nombre de lectures à moyenner.
     * @return La distance moyenne en cm, retournée comme un entier plus la valeur multipliée par 10.
     */
    float getAvgDistance(int n);

    [[nodiscard]] int getDistanceMin() const noexcept { return distanceMin; }
    [[nodiscard]] int getDistanceMax() const noexcept { return distanceMax; }
    [[nodiscard]] float getCoeff() const noexcept { return coeff; }
    [[nodiscard]] float getDecalage() const noexcept { return decalage; }

    void setDistanceMin(int distanceMin) { this->distanceMin = distanceMin; }
    void setDistanceMax(int distanceMax) { this->distanceMax = distanceMax; }
    void setCoeff(float coeff) { this->coeff = coeff; }
    void setDecalage(float decalage) { this->decalage = decalage; }
};

#endif //SHARP_HPP