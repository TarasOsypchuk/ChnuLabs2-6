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

class HoneyBatch : public IPrintable {
    string flowerSource;
    double weight;
public:
    HoneyBatch(string source, double w) : flowerSource(source), weight(w) {}
    void printDetails() const override {
        cout << "[Honey] Source: " << flowerSource << ", Weight: " << weight << "kg" << endl;
    }
    string serialize() const override { 
        return "HONEY " + flowerSource + " " + to_string(weight); 
    }
};


class BeekeepingSystem {
    /////////////////////////////////Smart Pointers
    vector<unique_ptr<IPrintable>> inventory; 
    const string DATA_FILE = "data.txt";
    const string HISTORY_FILE = "history.txt";

    ////////////////////////////// 8: Save user action history 
    void logAction(const string& action) {
        ofstream log(HISTORY_FILE, ios::app);
        if (log) log << "Action Log: " << action << endl;
    }

public:
    ////////////////////////////4 update/add information 
    void addItem(unique_ptr<IPrintable> item) {
        inventory.push_back(move(item));
        logAction("Added new item to system.");
    }

    ///////////////////////////////////5: User view 
    void showInventory() {
        if (inventory.empty()) {
            cout << "No data available." << endl;
            return;
        }
        for (const auto& item : inventory) {
            item->printDetails();
        }
        logAction("Viewed inventory items.");
    }

    //////////////////////////6: Data save
    void saveToFiles() {
        ofstream out(DATA_FILE);
        // Task 9: Exception handling for file operations 
        if (!out) throw runtime_error("Critical Error: Could not open data file for saving!");
        
        for (const auto& item : inventory) {
            out << item->serialize() << endl;
        }
        logAction("Saved current state to file.");
    }

    //////////////////////// 7 Loading
    void loadFromFiles() {
        ifstream in(DATA_FILE);
        if (!in) return; 

        string type;
        while (in >> type) {
            if (type == "HIVE") {
                int id, count; in >> id >> count;
                inventory.push_back(make_unique<BeeHive>(id, count));
            } else if (type == "SMARTHIVE") {
                int id, count; in >> id >> count;
                inventory.push_back(make_unique<SmartHive>(id, count));
            } else if (type == "HONEY") {
                string src; double w; in >> src >> w;
                inventory.push_back(make_unique<HoneyBatch>(src, w));
            }
        }
    }
};

int main() {
    BeekeepingSystem system;
    // Task 7: Load data on startup 
    system.loadFromFiles();

    ////////////////////// 1  2: Console menu with two types (Admin/User) 
    int role;
    cout << "Select Access Level:\n1. Administrator\n2. Standard User\nChoice: ";
    cin >> role;

    if (role == 1) {
        /////////////////////3: Administrator must have a password 
        string password;
        cout << "Enter Admin Password: ";
        cin >> password;

        if (password != "chnu_ipz_2026") {
            cout << "Invalid password. Access denied." << endl;
            return 0;
        }

        int choice = -1;
        while (choice != 0) {
            ////////////////////////1 Console menu for interaction 
            cout << "\n--- ADMIN MENU ---\n1. Add Smart Hive\n2. Add Honey\n3. View All\n4. Save to File\n0. Exit\nChoice: ";
            cin >> choice;

            //////////////////////////9: Processing exceptional situations 
            try {
                if (choice == 1) {
                    int id, count;
                    cout << "ID and Bee Count: ";
                    if (!(cin >> id >> count)) throw invalid_argument("Numeric input required!");
                    system.addItem(make_unique<SmartHive>(id, count));
                } else if (choice == 2) {
                    string src; double w;
                    cout << "Honey Source and Weight: ";
                    cin >> src >> w;
                    system.addItem(make_unique<HoneyBatch>(src, w));
                } else if (choice == 3) {
                    system.showInventory();
                } else if (choice == 4) {
                    system.saveToFiles();
                    cout << "System state saved successfully." << endl;
                }
            } catch (const exception& e) {
                cout << "Caught Exception: " << e.what() << endl;
                cin.clear(); // Clear error flags
                cin.ignore(1000, '\n'); // Clear buffer
            }
        }
    } else {
        ///////////////////////////////////5: Standard user can only view functionality 
        cout << "\n--- USER VIEW ---\n";
        system.showInventory();
    }
    return 0;
}
