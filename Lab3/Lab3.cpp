#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>

using namespace std;

class IPrintable {
public:
    virtual void printDetails() const = 0;
    virtual string serialize() const = 0; // Needed for Task 6 
    virtual ~IPrintable() {}
};

class BeeHive : public IPrintable {
protected:
    int id;
    int beesCount;
public:
    BeeHive(int i, int count) : id(i), beesCount(count) {}
    virtual void printDetails() const override {
        cout << "[Hive] ID: " << id << ", Bees: " << beesCount << endl;
    }
    // Task 6: Preparing data for file storage 
    string serialize() const override { 
        return "HIVE " + to_string(id) + " " + to_string(beesCount); 
    }
};

class SmartHive : public BeeHive {
public:
    SmartHive(int i, int count) : BeeHive(i, count) {}
    void printDetails() const override final {
        cout << "[SmartHive] ID: " << id << ", Bees: " << beesCount << " (Electronic Monitoring)" << endl;
    }
    string serialize() const override { 
        return "SMARTHIVE " + to_string(id) + " " + to_string(beesCount); 
    }
};

class Person {//////////////////////////// 7 чисто віртуальна функція
protected:
    string name;
public:
    Person(string n) : name(n) {}


    virtual ~Person() {///////////////////////// 4 Віртуальний десктруктор
        cout << "[Person] Destructor called for " << name << endl;
    }

    virtual void work() const = 0;
};


class BeeKeeper : public Person {
private:
    int experience;
public:
    BeeKeeper(string name, int experience) : Person(name), experience(experience) {}
    ~BeeKeeper() override {
        cout << "[BeeKeeper] " << name << " is resting." << endl;
    }

    void work() const override {
        cout << "[BeeKeeper] " << name << " is collecting honey in the apiary." << endl;
    }
};

class BeeHive {
protected:
    int beeCount;
    string material;
    double temperature;
    static int totalHivesCount;

public:
    BeeHive(int count, string mat, double temp)
        : beeCount(count), material(mat), temperature(temp) {
        totalHivesCount++;
    }
    BeeHive() : beeCount(0), material("Plastic"), temperature(20.0) {
        totalHivesCount++;
    }

    virtual ~BeeHive() {
        totalHivesCount--;
    }

    static int getTotalHives() { return totalHivesCount; }


    
    void performMaintenance() const {/////////////////////// 1 не віртуальна функція
        cout << "[BeeHive] Standard maintenance performed on basic hive." << endl;
    }

    virtual void showStatus() const {////////////////////// 2 дві віртуальні функцї в батьківський клас
        cout << "BeeHive : " << beeCount << " bees, Material " << material << endl;
    }

    virtual void collectResource() const {
        cout << "[BeeHive] Collecting standard honey." << endl;
    }

    BeeHive& operator++() {
        this->beeCount += 500;
        return *this;
    }
};

int BeeHive::totalHivesCount = 0;

class SmartHive : public BeeHive {
private:
    string* ipAddress;
    Sensor tempSensor;

public:
    SmartHive(int count, string mat, double temp, string ip)
        : BeeHive(count, mat, temp), tempSensor("Temperature", temp) {
        ipAddress = new string(ip);
    }

    SmartHive(const SmartHive& other)
        : BeeHive(other), tempSensor(other.tempSensor) {
        ipAddress = new string(*other.ipAddress + " (Copy)");
    }
    SmartHive(SmartHive&& other) noexcept
        : BeeHive(move(other)), tempSensor(move(other.tempSensor)), ipAddress(other.ipAddress) {
        other.ipAddress = nullptr;
    }
    SmartHive& operator=(const SmartHive& other) {
        if (this != &other) {
            BeeHive::operator=(other);
            tempSensor = other.tempSensor;
            delete ipAddress;
            ipAddress = new string(*other.ipAddress);
        }
        return *this;
    }
    SmartHive& operator=(SmartHive&& other) noexcept {
        if (this != &other) {
            BeeHive::operator=(move(other));
            tempSensor = move(other.tempSensor);
            delete ipAddress;
            ipAddress = other.ipAddress;
            other.ipAddress = nullptr;
        }
        return *this;
    }

    ~SmartHive() override {
        delete ipAddress;
    }

    void performMaintenance() const {//////////////////////// 1 переназначення=> проблема
        cout << "[SmartHive] Updating firmware and checking sensors..." << endl;
    }

    /////////////////////////////3 перевизначення піртуальних функцій
    ////////////////////////5 final
    void showStatus() const override final {
        cout << "SmartHive [" << (ipAddress ? *ipAddress : "Offline") << "] : "
            << beeCount << " bees, Temp: " << tempSensor.getValue() << "C" << endl;
    }

    void collectResource() const override {
        cout << "[SmartHive] Collecting premium smart-tracked honey." << endl;
    }
};

class HoneyBatch : public IPrintable {////////////////////////// 8 продовження
private:
    double liters;
    string flowerType;
    string* batchNote;

public:
    HoneyBatch(double l, string type = "mixed") : liters(l), flowerType(type) {
        batchNote = new string("Standard Quality");
    }
    ~HoneyBatch() override { delete batchNote; }

    void printDetails() const override {
        cout << "[IPrintable] HoneyBatch: " << liters << " liters of " << flowerType << endl;
    }
};

void inspectHiveWithReference(const BeeHive& hive) {////////////////////// 6 Reference
    cout << "   -> Reference Polymorphism calls: ";
    hive.showStatus();
}

int main() {
    cout << " LAB 5: POLYMORPHISM DEMONSTRATION\n\n";

    SmartHive mySmartHive(50000, "Wood", 34.5, "192.168.1.10");
    BeeHive myBasicHive(30000, "Straw", 25.0);

    cout << "--- 1. Static Method Problem ---" << endl;/////////////////////// 1
    BeeHive* ptrToSmart = &mySmartHive;

    cout << "Calling non-virtual method via Base Pointer: " << endl;
    ptrToSmart->performMaintenance();

    cout << "Calling non-virtual method via Object directly: " << endl;
    mySmartHive.performMaintenance();

    cout << "\n--- 3. Run-time Polymorphism (Base Class Pointer) ---" << endl;/////////////////////// 3 Base class pointer
    ptrToSmart->collectResource();

    cout << "\n--- 6. Run-time Polymorphism (Base Class Reference) ---" << endl;/////////////////////// 6 Base class reference
    inspectHiveWithReference(myBasicHive);
    inspectHiveWithReference(mySmartHive);

    cout << "\n--- 7. Pure Virtual Function & Abstract Classes ---" << endl;//////////////////////// 7 abstract class
    Person* beekeeperPtr = new BeeKeeper("Taras", 5);
    beekeeperPtr->work();
    delete beekeeperPtr;

    cout << "\n--- 8. Interfaces (IPrintable) applied to different classes ---" << endl;/////////////////////// 8
    Sensor mySensor("Humidity", 55.5);
    HoneyBatch myHoney(15, "Acacia");

    IPrintable* elements[2];
    elements[0] = &mySensor;
    elements[1] = &myHoney;

    for (int i = 0; i < 2; i++) {
        elements[i]->printDetails();
    }

    cout << "\n=== End of Lab 5 ===" << endl;
    return 0;
}
