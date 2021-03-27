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

		Random(unsigned int seed) {
			gen.seed(seed);
		}

		int randIntU(int a, int b) {
			uniform_int_distribution<int> distrib(a, b);
			return distrib(gen);
		}

		void dueIndiciRandom(unsigned int dimensione, int& i1, int& i2) {
			i1 = randIntU(0, dimensione - 1);
			i2 = (i1 + 1 + randIntU(0, dimensione - 2)) % dimensione;
		}

		double randDouble(double a, double b) {
			uniform_real_distribution<double> distrib(a, b);
			return distrib(gen);
		}

		void impostaSeed(unsigned int seed) {
			gen.seed(seed);
		}
};