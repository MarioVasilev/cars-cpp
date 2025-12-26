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

Car cars[MAX_CARS];
int carCount = 0;

// -------------------------------------------------

void saveToFile() {
    ofstream file(FILE_NAME, ios::binary);
    file.write((char*)&carCount, sizeof(carCount));
    file.write((char*)cars, sizeof(Car) * carCount);
    file.close();
}

void loadFromFile() {
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

void addCars() {
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

void printAllCars() {
    for (int i = 0; i < carCount; i++)
        printCar(cars[i]);
}

// -------------------------------------------------

void mostPowerfulCars() {
    int maxPower = 0;
    for (int i = 0; i < carCount; i++)
        if (cars[i].power > maxPower)
            maxPower = cars[i].power;

    cout << "Автомобили с най-голяма мощност:\n";
    for (int i = 0; i < carCount; i++)
        if (cars[i].power == maxPower)
            printCar(cars[i]);
}

// -------------------------------------------------

void searchByYear() {
    int y;
    cout << "Въведи година: ";
    cin >> y;

    for (int i = 0; i < carCount; i++)
        if (cars[i].year == y)
            printCar(cars[i]);
}

// -------------------------------------------------

void sortByPowerDesc() {
    for (int i = 0; i < carCount - 1; i++)
        for (int j = i + 1; j < carCount; j++)
            if (cars[i].power < cars[j].power)
                swap(cars[i], cars[j]);
}

// -------------------------------------------------

void freeCarsSorted() {
    Car temp[MAX_CARS];
    int cnt = 0;

    for (int i = 0; i < carCount; i++)
        if (!cars[i].busy)
            temp[cnt++] = cars[i];

    for (int i = 0; i < cnt - 1; i++)
        for (int j = i + 1; j < cnt; j++)
            if (strcmp(temp[i].regNumber, temp[j].regNumber) > 0)
                swap(temp[i], temp[j]);

    for (int i = 0; i < cnt; i++)
        printCar(temp[i]);
}

// -------------------------------------------------

void searchByBrandModel() {
    char b[20], m[20];
    cout << "Марка: "; cin >> b;
    cout << "Модел: "; cin >> m;

    for (int i = 0; i < carCount; i++)
        if (!strcmp(cars[i].brand, b) && !strcmp(cars[i].model, m))
            printCar(cars[i]);
}

// -------------------------------------------------

int findCarByReg(const char* reg) {
    for (int i = 0; i < carCount; i++)
        if (!strcmp(cars[i].regNumber, reg))
            return i;
    return -1;
}

bool renterHasCar(const char* egn) {
    for (int i = 0; i < carCount; i++)
        if (cars[i].busy && !strcmp(cars[i].renterEGN, egn))
            return true;
    return false;
}

// -------------------------------------------------

void rentCar() {
    char reg[15];
    cout << "Рег. номер: ";
    cin >> reg;

    int idx = findCarByReg(reg);
    if (idx == -1) {
        cout << "Не е открит автомобил с този номер\n";
        return;
    }

    if (cars[idx].busy) {
        cout << "Автомобилът вече е ангажиран\n";
        return;
    }

    char egn[11];
    cout << "ЕГН на наемателя: ";
    cin >> egn;

    if (renterHasCar(egn)) {
        char choice;
        cout << "Лицето вече има кола. Да се даде ли още една? (y/n): ";
        cin >> choice;
        if (choice != 'y') {
            cout << "Отказано наемане\n";
            return;
        }
    }

    cars[idx].busy = true;
    strcpy(cars[idx].renterEGN, egn);
    cout << "Колата е наета успешно\n";
}

// -------------------------------------------------

void returnCar() {
    char reg[15];
    cout << "Рег. номер за връщане: ";
    cin >> reg;

    int idx = findCarByReg(reg);
    if (idx == -1 || !cars[idx].busy) {
        cout << "Невалидна операция\n";
        return;
    }

    cars[idx].busy = false;
    strcpy(cars[idx].renterEGN, "-");
    cout << "Колата е върната\n";
}

// -------------------------------------------------

void menu() {
    cout << "\n1. Добавяне на коли";
    cout << "\n2. Извеждане на всички коли";
    cout << "\n3. Най-мощни коли";
    cout << "\n4. Търсене по година";
    cout << "\n5. Подреждане по мощност";
    cout << "\n6. Свободни коли (сортирани)";
    cout << "\n7. Търсене по марка и модел";
    cout << "\n8. Наемане на кола";
    cout << "\n9. Връщане на кола";
    cout << "\n0. Изход\n";
}

// -------------------------------------------------

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    loadFromFile();

    int choice;
    do {
        menu();
        cout << "\nИзбор: ";
        cin >> choice;

        switch (choice) {
            case 1: addCars(); break;
            case 2: printAllCars(); break;
            case 3: mostPowerfulCars(); break;
            case 4: searchByYear(); break;
            case 5: sortByPowerDesc(); break;
            case 6: freeCarsSorted(); break;
            case 7: searchByBrandModel(); break;
            case 8: rentCar(); break;
            case 9: returnCar(); break;
        }
    } while (choice != 0);

    saveToFile();
    return 0;
}
