#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

const int MAX_CARS = 100;
const char FILE_NAME[] = "cars.dat";

struct Car {
    char brand[20];
    char model[20];
    int year;
    char regNumber[15];
    int power;
    bool busy;
    char renterEGN[11];
};

// -------------------------------------------------

void saveToFile(Car cars[], int carCount) {
    ofstream file(FILE_NAME, ios::binary);
    file.write((char*)&carCount, sizeof(carCount));
    file.write((char*)cars, sizeof(Car) * carCount);
    file.close();
}

void loadFromFile(Car cars[], int& carCount) {
    ifstream file(FILE_NAME, ios::binary);
    if (!file) return;
    file.read((char*)&carCount, sizeof(carCount));
    file.read((char*)cars, sizeof(Car) * carCount);
    file.close();
}

// -------------------------------------------------

void printCar(const Car& c) {
    cout << c.brand << " " << c.model
         << ", " << c.year
         << ", Reg: " << c.regNumber
         << ", Power: " << c.power
         << ", Status: " << (c.busy ? "Заета" : "Свободна");
    if (c.busy)
        cout << ", EGN: " << c.renterEGN;
    cout << endl;
}

// -------------------------------------------------

void addCars(Car cars[], int& carCount) {
    int n;
    cout << "Брой коли за добавяне: ";
    cin >> n;

    if (carCount + n > MAX_CARS) {
        cout << "Надвишен максимален брой коли!\n";
        return;
    }

    for (int i = 0; i < n; i++) {
        Car c;
        cout << "Марка: "; cin >> c.brand;
        cout << "Модел: "; cin >> c.model;
        cout << "Година: "; cin >> c.year;
        cout << "Рег. номер: "; cin >> c.regNumber;
        cout << "Мощност: "; cin >> c.power;
        c.busy = false;
        strcpy(c.renterEGN, "-");
        cars[carCount++] = c;
    }
}

// -------------------------------------------------

void printAllCars(Car cars[], int carCount) {
    for (int i = 0; i < carCount; i++)
        printCar(cars[i]);
}

// -------------------------------------------------

void mostPowerfulCars(Car cars[], int carCount) {
    int maxPower = 0;
    for (int i = 0; i < carCount; i++)
        if (cars[i].power > maxPower)
            maxPower = cars[i].power;

    for (int i = 0; i < carCount; i++)
        if (cars[i].power == maxPower)
            printCar(cars[i]);
}

// -------------------------------------------------

int findCarByReg(Car cars[], int carCount, const char* reg) {
    for (int i = 0; i < carCount; i++)
        if (!strcmp(cars[i].regNumber, reg))
            return i;
    return -1;
}

bool renterHasCar(Car cars[], int carCount, const char* egn) {
    for (int i = 0; i < carCount; i++)
        if (cars[i].busy && !strcmp(cars[i].renterEGN, egn))
            return true;
    return false;
}

// -------------------------------------------------

void rentCar(Car cars[], int carCount) {
    char reg[15];
    cout << "Рег. номер: ";
    cin >> reg;

    int idx = findCarByReg(cars, carCount, reg);
    if (idx == -1 || cars[idx].busy) {
        cout << "Невалидна операция\n";
        return;
    }

    char egn[11];
    cout << "ЕГН: ";
    cin >> egn;

    if (renterHasCar(cars, carCount, egn)) {
        char ch;
        cout << "Има вече кола. Още една? (y/n): ";
        cin >> ch;
        if (ch != 'y') return;
    }

    cars[idx].busy = true;
    strcpy(cars[idx].renterEGN, egn);
}

// -------------------------------------------------

void returnCar(Car cars[], int carCount) {
    char reg[15];
    cout << "Рег. номер: ";
    cin >> reg;

    int idx = findCarByReg(cars, carCount, reg);
    if (idx == -1 || !cars[idx].busy) return;

    cars[idx].busy = false;
    strcpy(cars[idx].renterEGN, "-");
}

// -------------------------------------------------

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Car cars[MAX_CARS];
    int carCount = 0;

    loadFromFile(cars, carCount);

    int choice;
    do {
        cout << "\n1. Добавяне\n2. Всички\n3. Най-мощни\n8. Наем\n9. Връщане\n0. Изход\n";
        cin >> choice;

        switch (choice) {
            case 1: addCars(cars, carCount); break;
            case 2: printAllCars(cars, carCount); break;
            case 3: mostPowerfulCars(cars, carCount); break;
            case 8: rentCar(cars, carCount); break;
            case 9: returnCar(cars, carCount); break;
        }
    } while (choice != 0);

    saveToFile(cars, carCount);
    return 0;
}
