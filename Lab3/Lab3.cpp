#include <iostream>
#include <string>

using namespace std;

class BeeHive {
private:

	int beeCount;
	string material;
	double temperature;

	static int totalHivesCount;

public:
	BeeHive(int count, string mat, double temp)

		: beeCount(count), material(mat), temperature(temp) {
		totalHivesCount++;
		cout << "BeeHive create of(" << material << ")." << endl;

	}
	BeeHive() : beeCount(0), material("Plastic"), temperature(20.0) {

		cout << "BeeHive created empty" << endl;

	}
	static int getTotalHives() {
		return totalHivesCount;
	}

	void showStatus() {

		cout << "BeeHive : " << beeCount << " bees, Material " << material << endl;

	}

	BeeHive& operator++() {
		this->beeCount += 500;
		return *this;
	}
	friend ostream& operator<<(ostream& os, const BeeHive& hive);
	friend istream& operator>>(istream& is, BeeHive& hive);
};

int BeeHive::totalHivesCount = 0;

ostream& operator<<(ostream& os, const BeeHive& hive) {
	os << "Hive" << hive.material << "  Bees:" << hive.beeCount << endl;
	return os;
}

istream& operator>>(istream& is, BeeHive& hive) {
	cout << "Enter material and bee count: ";
	is >> hive.material >> hive.beeCount;
	return is;
}

class BeeKeeper {
private:
	string name;
	int experience;

public:
	BeeKeeper(string name, int experience) {
		this->name = name;
		this->experience = experience;
		cout << "BeeKepeer " << this->name << " working" << endl;

	}

	~BeeKeeper() {
		cout << "BeeKeeper " << name << " is resting" << endl;

	}

	void collectHoney() {

		cout << name << " is collecting honey" << endl;

	}

};

class HoneyBatch {

private:

	double liters;
	string flowerType;

	string* batchNote;

public:

	HoneyBatch(double l, string type = "mixed") : liters(l), flowerType(type) {
		batchNote = new string("Standart Quality");
		cout << "HoneyBatch was collected - " << liters << "l honey(" << flowerType << ")" << endl;

	}

	HoneyBatch(const HoneyBatch& other) : liters(other.liters), flowerType(other.flowerType) {
		batchNote = new string(*other.batchNote + " (Copy)");
		cout << "Copy constructor called for " << flowerType << "batch" << endl;
	}
	HoneyBatch(HoneyBatch&& other)noexcept
		: liters(other.liters), flowerType(move(other.flowerType)), batchNote(other.batchNote) {
		other.batchNote = nullptr;
		cout << "Move constructor called for " << flowerType << endl;
	}

	~HoneyBatch() {
		delete batchNote;
	}
	HoneyBatch operator+(const HoneyBatch& other) {
		return HoneyBatch(this->liters + other.liters, this->flowerType + "&" + other.flowerType);
	}

	void printInfo() const {
		cout << "Batch: " << flowerType << " - " << liters << " liters" << endl;
	}
};



int main() {

	cout << "Hives count: " << BeeHive::getTotalHives() << endl;

	BeeHive mainHive(999, "wood", 35.9);

	BeeHive ecoHive(3000, "Straw", 32);
	ecoHive.showStatus();
	++mainHive;

	cout << "Updated hive: " << mainHive << endl;
	HoneyBatch acacia(10, "Acacia");
	HoneyBatch acaciaCopy = acacia;
	HoneyBatch merged = acacia + HoneyBatch(5, "Rice");

	merged.printInfo();
	cout << "Total Hives: " << BeeHive::getTotalHives() << endl;


	return 0;

}