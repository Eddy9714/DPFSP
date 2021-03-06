#include <random>

using namespace std;

class Random {
	private:
		mt19937 gen;

	public:
		Random() {
			std::random_device rd;
			gen.seed(rd());
		}

		int randIntU(int a, int b) {
			uniform_int_distribution<int> distrib(a, b);
			return distrib(gen);
		}
};