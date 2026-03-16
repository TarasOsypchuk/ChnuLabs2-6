#include <iostream>
#include <string>

using namespace std;

class Sensor {
private:
    string type;
    double value;
public:
    Sensor(string t = "Temperature", double v = 0.0) : type(t), value(v) {
        cout << "   [Sensor] " << type << " initialized." << endl;
    }
    ~Sensor() {
        cout << "   [Sensor] " << type << " destroyed." << endl;
    }
    double getValue() const { return value; }
};

class Person {
protected:
    string name;
public:
    Person(string n) : name(n) {
        cout << "[Person] Constructor called for " << name << endl;
    }
    virtual ~Person() {
        cout << "[Person] Destructor called for " << name << endl;
    }
    virtual void work() = 0;
};

class BeeKeeper : public Person {
private:
    int experience;
public:
    BeeKeeper(string name, int experience) : Person(name), experience(experience) {
        cout << "[BeeKeeper] " << this->name << " is ready to work." << endl;
    }
    ~BeeKeeper() override {
        cout << "[BeeKeeper] " << name << " is resting." << endl;
    }
    void work() override {
        cout << name << " is collecting honey in the apiary." << endl;
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
        cout << "[BeeHive] Created of (" << material << ")." << endl;
    }
    BeeHive() : beeCount(0), material("Plastic"), temperature(20.0) {
        totalHivesCount++;
        cout << "[BeeHive] Created empty." << endl;
    }

    virtual ~BeeHive() {
        totalHivesCount--;
        cout << "[BeeHive] Destroyed." << endl;
    }

    static int getTotalHives() { return totalHivesCount; }

    virtual void showStatus() const {
        cout << "BeeHive : " << beeCount << " bees, Material " << material << endl;
    }

    BeeHive& operator++() {
        this->beeCount += 500;
        return *this;
    }
    friend ostream& operator<<(ostream& os, const BeeHive& hive);
};

int BeeHive::totalHivesCount = 0;

ostream& operator<<(ostream& os, const BeeHive& hive) {
    os << "Hive " << hive.material << " | Bees:" << hive.beeCount;
    return os;
}

class SmartHive : public BeeHive {
private:
    string* ipAddress;
    Sensor tempSensor;

public:
    SmartHive(int count, string mat, double temp, string ip)
        : BeeHive(count, mat, temp), tempSensor("Temperature", temp) {
        ipAddress = new string(ip);
        cout << "[SmartHive] Connected to IP: " << *ipAddress << endl;
    }

    SmartHive(const SmartHive& other)
        : BeeHive(other), tempSensor(other.tempSensor) {
        ipAddress = new string(*other.ipAddress + " (Copy)");
        cout << "[SmartHive] Copy constructor called." << endl;
    }

    SmartHive(SmartHive&& other) noexcept
        : BeeHive(move(other)), tempSensor(move(other.tempSensor)), ipAddress(other.ipAddress) {
        other.ipAddress = nullptr;
        cout << "[SmartHive] Move constructor called." << endl;
    }

    SmartHive& operator=(const SmartHive& other) {
        cout << "[SmartHive] Copy assignment operator called." << endl;
        if (this != &other) {
            BeeHive::operator=(other);
            tempSensor = other.tempSensor;
            delete ipAddress;
            ipAddress = new string(*other.ipAddress);
        }
        return *this;
    }

    SmartHive& operator=(SmartHive&& other) noexcept {
        cout << "[SmartHive] Move assignment operator called." << endl;
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
        cout << "[SmartHive] Destroyed." << endl;
    }

    void showStatus() const override {
        cout << "SmartHive [" << (ipAddress ? *ipAddress : "Offline") << "] : "
            << beeCount << " bees, Temp: " << tempSensor.getValue() << "C" << endl;
    }
};

class HoneyBatch {
private:
    double liters;
    string flowerType;
    string* batchNote;

public:
    HoneyBatch(double l, string type = "mixed") : liters(l), flowerType(type) {
        batchNote = new string("Standard Quality");
        cout << "[HoneyBatch] Collected - " << liters << "l (" << flowerType << ")" << endl;
    }

    HoneyBatch(const HoneyBatch& other) : liters(other.liters), flowerType(other.flowerType) {
        batchNote = new string(*other.batchNote + " (Copy)");
    }

    HoneyBatch(HoneyBatch&& other) noexcept
        : liters(other.liters), flowerType(move(other.flowerType)), batchNote(other.batchNote) {
        other.batchNote = nullptr;
    }

    ~HoneyBatch() { delete batchNote; }

    HoneyBatch operator+(const HoneyBatch& other) {
        return HoneyBatch(this->liters + other.liters, this->flowerType + "&" + other.flowerType);
    }

    void printInfo() const {
        cout << "Batch: " << flowerType << " - " << liters << " liters" << endl;
    }
};

int main() {
    cout << "--- 1. Testing Person and BeeKeeper (Upward Hierarchy) ---" << endl;
    BeeKeeper master("Taras", 5);
    master.work();

    cout << "\n--- 2. Testing SmartHive and Composition (Downward & Has-A) ---" << endl;
    SmartHive node1(50000, "Wood", 34.5, "192.168.1.10");
    node1.showStatus();

    cout << "\n--- 3. Testing Rule of 5 in Derived Class ---" << endl;
    SmartHive node2 = node1;
    SmartHive node3(10000, "Plastic", 33.0, "192.168.1.11");
    node3 = move(node1);

    cout << "\n--- 4. Original Functionality Check ---" << endl;
    HoneyBatch acacia(10, "Acacia");
    HoneyBatch merged = acacia + HoneyBatch(5, "Linden");
    merged.printInfo();

    cout << "\nTotal Hives active: " << BeeHive::getTotalHives() << endl;

    cout << "\n--- 5. Destructor Sequence Check ---" << endl;
    return 0;
}