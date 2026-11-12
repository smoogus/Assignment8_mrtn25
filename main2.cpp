// Lab Assignment 8: Field Inheritance and Operator Overloading
// Electric_Field and Magnetic_Field inherit from a common Field base class.

#include <iostream>
#include <cmath>
#include <stdexcept>

class Field {
protected:
    double* value;  // Dynamic array of three components (x, y, z)

public:
    // Default constructor: all three components are zero.
    Field() : Field(0.0, 0.0, 0.0) {}

    // Constructor with three component values.
    Field(double x, double y, double z) : value(new double[3]{x, y, z}) {}

    // Copy constructor: make an independent copy of the array.
    Field(const Field& other)
        : value(new double[3]{other.value[0], other.value[1], other.value[2]}) {}

    // Copy assignment: copy components, not the pointer address.
    Field& operator=(const Field& other) {
        if (this != &other) {
            for (int i = 0; i < 3; i++) {
                value[i] = other.value[i];
            }
        }
        return *this;
    }

    virtual ~Field() {
        delete[] value;
    }

    // Magnitude: square root of the sum of component squares.
    double magnitude() const {
        return std::sqrt(value[0] * value[0] + value[1] * value[1] +
                         value[2] * value[2]);
    }

    // Prints the three components and the overall magnitude.
    void printMagnitude() const {
        std::cout << "Components: ( " << value[0] << ", " << value[1]
                  << ", " << value[2] << " )\n";
        std::cout << "Magnitude: " << magnitude() << '\n';
    }
};

class Electric_Field : public Field {
private:
    double calculatedElectricField;

public:
    Electric_Field() : Field(), calculatedElectricField(0.0) {}

    // Calls the Field constructor to initialize the components.
    Electric_Field(double x, double y, double z)
        : Field(x, y, z), calculatedElectricField(0.0) {}

    Electric_Field(const Electric_Field& other)
        : Field(other), calculatedElectricField(other.calculatedElectricField) {}

    Electric_Field& operator=(const Electric_Field& other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedElectricField = other.calculatedElectricField;
        }
        return *this;
    }

    // Gauss's law for a spherical charge: E = Q / (4*pi*r^2*epsilon0).
    void calculateElectricField(double charge, double radius) {
        if (radius <= 0.0) {
            throw std::invalid_argument("Electric field radius must be positive.");
        }
        const double PI = std::acos(-1.0);
        const double EPSILON_0 = 8.854187817e-12;
        calculatedElectricField = charge / (4.0 * PI * radius * radius * EPSILON_0);
    }

    double getCalculatedElectricField() const {
        return calculatedElectricField;
    }

    // Adds corresponding vector components.
    Electric_Field operator+(const Electric_Field& other) const {
        return Electric_Field(value[0] + other.value[0],
                              value[1] + other.value[1],
                              value[2] + other.value[2]);
    }

    friend std::ostream& operator<<(std::ostream& out, const Electric_Field& field) {
        out << "( " << field.value[0] << ", " << field.value[1]
            << ", " << field.value[2] << " )";
        return out;
    }
};

class Magnetic_Field : public Field {
private:
    double calculatedMagneticField;

public:
    Magnetic_Field() : Field(), calculatedMagneticField(0.0) {}

    Magnetic_Field(double x, double y, double z)
        : Field(x, y, z), calculatedMagneticField(0.0) {}

    Magnetic_Field(const Magnetic_Field& other)
        : Field(other), calculatedMagneticField(other.calculatedMagneticField) {}

    Magnetic_Field& operator=(const Magnetic_Field& other) {
        if (this != &other) {
            Field::operator=(other);
            calculatedMagneticField = other.calculatedMagneticField;
        }
        return *this;
    }

    // Ampere's law for a long, straight wire: B = I / (2*pi*r*mu0).
    void calculateMagneticField(double current, double radius) {
        if (radius <= 0.0) {
            throw std::invalid_argument("Magnetic field radius must be positive.");
        }
        const double PI = std::acos(-1.0);
        const double MU_0 = 4.0 * PI * 1e-7;
        calculatedMagneticField = current / (2.0 * PI * radius * MU_0);
    }

    double getCalculatedMagneticField() const {
        return calculatedMagneticField;
    }

    Magnetic_Field operator+(const Magnetic_Field& other) const {
        return Magnetic_Field(value[0] + other.value[0],
                              value[1] + other.value[1],
                              value[2] + other.value[2]);
    }

    friend std::ostream& operator<<(std::ostream& out, const Magnetic_Field& field) {
        out << "( " << field.value[0] << ", " << field.value[1]
            << ", " << field.value[2] << " )";
        return out;
    }
};

int main() {
    // Objects built with the constructors; printMagnitude shows their components.
    Electric_Field electric(0, 100000, 1000);
    Magnetic_Field magnetic(0, 3e-6, 4e-6);

    std::cout << "++ Constructor Values ++\n";
    std::cout << "Electric Field:\n";
    electric.printMagnitude();
    std::cout << "Magnetic Field:\n";
    magnetic.printMagnitude();
    std::cout << '\n';

    // Calculate fields with made-up charge, current, and distances.
    electric.calculateElectricField(1e-9, 0.1);
    magnetic.calculateMagneticField(5.0, 0.1);
    std::cout << "Calculated Electric Field (Q = 1e-9 C, r = 0.1 m): "
              << electric.getCalculatedElectricField() << " N/C\n";
    std::cout << "Calculated Magnetic Field (I = 5 A, r = 0.1 m): "
              << magnetic.getCalculatedMagneticField() << " T\n\n";

    Electric_Field e1, e2(1, 2, 3), e3(4, 5, 6);
    e1 = e2 + e3;
    std::cout << "++ Electric Field Addition ++\n";
    std::cout << "E2 components:\n= " << e2 << '\n';
    std::cout << "E3 components:\n= " << e3 << '\n';
    std::cout << "E2 + E3 components:\n= " << e1 << "\n\n";

    Magnetic_Field b1, b2(1e-6, 2e-6, 3e-6), b3(4e-6, 5e-6, 6e-6);
    b1 = b2 + b3;
    std::cout << "++ Magnetic Field Addition ++\n";
    std::cout << "B2 components:\n= " << b2 << '\n';
    std::cout << "B3 components:\n= " << b3 << '\n';
    std::cout << "B2 + B3 components:\n= " << b1 << "\n\n";

    Electric_Field electricCopy(e1);
    Magnetic_Field magneticCopy(b1);
    std::cout << "Electric Field Copy:\n= " << electricCopy << '\n';
    std::cout << "Magnetic Field Copy:\n= " << magneticCopy << '\n';
    return 0;
}
