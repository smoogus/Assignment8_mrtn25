// Assignment 8 - Inheritance

#include <iostream>
#include <cmath>

const double PI = 3.14159265358979323846;
const double EPSILON_0 = 8.854187817e-12;  // permittivity of free space (F/m)
const double MU_0 = 4.0 * PI * 1e-7;       // permeability of free space (T*m/A)

// Base class: Field
class Field {
protected:
    double *value;  // dynamically allocated array of 3 components

public:
    // Default constructor: components = 0
    Field() {
        value = new double[3];
        value[0] = value[1] = value[2] = 0.0;
    }

    // Constructor that assigns components
    Field(double x, double y, double z) {
        value = new double[3];
        value[0] = x;
        value[1] = y;
        value[2] = z;
    }

    // Copy constructor (deep copy)
    Field(const Field &other) {
        value = new double[3];
        for (int i = 0; i < 3; i++) value[i] = other.value[i];
    }

    // Assignment operator (deep copy)
    Field &operator=(const Field &other) {
        if (this != &other) {
            for (int i = 0; i < 3; i++) value[i] = other.value[i];
        }
        return *this;
    }

    virtual ~Field() { delete[] value; }

    double getX() const { return value[0]; }
    double getY() const { return value[1]; }
    double getZ() const { return value[2]; }

    void setX(double x) { value[0] = x; }
    void setY(double y) { value[1] = y; }
    void setZ(double z) { value[2] = z; }

    double calculateMagnitude() const {
        return std::sqrt(value[0] * value[0] + value[1] * value[1] + value[2] * value[2]);
    }

    // Prints the three components (and the overall magnitude)
    void printMagnitude() const {
        std::cout << "Components: (" << value[0] << ", " << value[1] << ", "
                  << value[2] << ")" << std::endl;
        std::cout << "Magnitude:  " << calculateMagnitude() << std::endl;
    }
};

// Derived class: Electric_Field
class Electric_Field : public Field {
private:
    double calculatedE;  // stores E computed from Gauss' law

public:
    Electric_Field() : Field(), calculatedE(0.0) {}

    Electric_Field(double x, double y, double z) : Field(x, y, z), calculatedE(0.0) {}

    // Copy constructor: calls base copy constructor
    Electric_Field(const Electric_Field &other)
        : Field(other), calculatedE(other.calculatedE) {}

    Electric_Field &operator=(const Electric_Field &other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedE = other.calculatedE;
        }
        return *this;
    }

    // Gauss' law for a spherical charge: E = Q / (4 pi r^2 eps0)
    // Q in Coulombs, r in meters. Result stored in calculatedE.
    void calculateElectricField(double Q, double r) {
        calculatedE = Q / (4.0 * PI * r * r * EPSILON_0);
    }

    double getCalculatedE() const { return calculatedE; }

    // e1 = e2 + e3
    Electric_Field operator+(const Electric_Field &rhs) const {
        return Electric_Field(value[0] + rhs.value[0],
                              value[1] + rhs.value[1],
                              value[2] + rhs.value[2]);
    }

    friend std::ostream &operator<<(std::ostream &os, const Electric_Field &e) {
        os << "Electric_Field(" << e.value[0] << ", " << e.value[1] << ", "
           << e.value[2] << ")";
        return os;
    }
};

// Derived class: Magnetic_Field
class Magnetic_Field : public Field {
private:
    double calculatedB;  // stores B computed from Ampere's law

public:
    Magnetic_Field() : Field(), calculatedB(0.0) {}

    Magnetic_Field(double x, double y, double z) : Field(x, y, z), calculatedB(0.0) {}

    Magnetic_Field(const Magnetic_Field &other)
        : Field(other), calculatedB(other.calculatedB) {}

    Magnetic_Field &operator=(const Magnetic_Field &other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedB = other.calculatedB;
        }
        return *this;
    }

    // Ampere's law for a long straight wire. I in Amps, r in meters.
    void calculateMagneticField(double I, double r) {
        calculatedB = MU_0 * I / (2.0 * PI * r);
    }

    double getCalculatedB() const { return calculatedB; }

    Magnetic_Field operator+(const Magnetic_Field &rhs) const {
        return Magnetic_Field(value[0] + rhs.value[0],
                              value[1] + rhs.value[1],
                              value[2] + rhs.value[2]);
    }

    friend std::ostream &operator<<(std::ostream &os, const Magnetic_Field &b) {
        os << "Magnetic_Field(" << b.value[0] << ", " << b.value[1] << ", "
           << b.value[2] << ")";
        return os;
    }
};

// main
int main() {
    std::cout << "=== Constructors and printMagnitude ===" << std::endl;
    Electric_Field E(0.0, 1e5, 1e3);
    Magnetic_Field B(2e-3, 0.0, 5e-4);

    std::cout << "Electric field:" << std::endl;
    E.printMagnitude();
    std::cout << "Magnetic field:" << std::endl;
    B.printMagnitude();

    std::cout << "\n=== Gauss' law (spherical charge) ===" << std::endl;
    double Q = 2e-6;  // 2 microcoulombs
    double rE = 0.5;  // 0.5 m
    E.calculateElectricField(Q, rE);
    std::cout << "Q = " << Q << " C, r = " << rE << " m" << std::endl;
    std::cout << "E = " << E.getCalculatedE() << " N/C" << std::endl;

    std::cout << "\n=== Ampere's law (current-carrying wire) ===" << std::endl;
    double I = 10.0;  // 10 A
    double rB = 0.05; // 5 cm
    B.calculateMagneticField(I, rB);
    std::cout << "I = " << I << " A, r = " << rB << " m" << std::endl;
    std::cout << "B = " << B.getCalculatedB() << " T" << std::endl;

    std::cout << "\n=== Overloaded + operator ===" << std::endl;
    Electric_Field e1, e2(1.0, 2.0, 3.0), e3(4.0, 5.0, 6.0);
    e1 = e2 + e3;
    std::cout << "e2 = " << e2 << std::endl;
    std::cout << "e3 = " << e3 << std::endl;
    std::cout << "e1 = e2 + e3 = " << e1 << std::endl;

    Magnetic_Field b1, b2(1e-3, 2e-3, 3e-3), b3(4e-3, 5e-3, 6e-3);
    b1 = b2 + b3;
    std::cout << "b2 = " << b2 << std::endl;
    std::cout << "b3 = " << b3 << std::endl;
    std::cout << "b1 = b2 + b3 = " << b1 << std::endl;

    std::cout << "\n=== Overloaded << operator (default constructed) ===" << std::endl;
    Electric_Field eDefault;
    Magnetic_Field bDefault;
    std::cout << eDefault << std::endl;
    std::cout << bDefault << std::endl;

    return 0;
}
