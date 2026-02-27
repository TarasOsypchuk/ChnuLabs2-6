#include <iostream>
#include <string>

using namespace std;

class BeeHive {
private:
	int beeCount;
	string material;
	double temperature;

public:
	BeeHive(int count, string mat, double temp)
		: beeCount(count), material(mat), temperature(temp) {
		cout << "BeeHive create of(" << material << ")." << endl;
	}

	BeeHive() : beeCount(0), material("Plastic"), temperature(20.0) {
		cout << "BeeHive created empty" << endl;
	}
	void showStatus() {
		cout << "BeeHive : " << beeCount << " bees, Material " << material << endl;
	}

};

class BeeKeeper {
private:
	string name;
	int experience;
public:
	BeeKeeper(string n, int exp): name(n), experience(exp) {
		cout << "BeeKepeer " << name << " working" << endl;
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
public:
	HoneyBatch(double l, string type = "mixed") : liters(l), flowerType(type) {
		cout << "HoneyBatch was collected - " << liters << "l honey(" << flowerType << ")" << endl;
	}
	HoneyBatch() : HoneyBatch(1, "test") {
		cout << "HoneyButch test production" << endl;
	}
};

int main() {
	BeeHive mainHive(10000, "wood", 35.2);
	BeeHive emptyHive;
	emptyHive.showStatus();

	HoneyBatch batch1(5.5, "Acacia");
	HoneyBatch batch2(3);
	HoneyBatch testBatch;
	{
		BeeKeeper keeper("JunJun", 99);
		keeper.collectHoney();
	}
	return 0;
}