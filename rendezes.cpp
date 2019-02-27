#include "std_lib_facilities.h"

ostream& operator<<(ostream& os, vector<int>& v) {
	int sor_hossz = 10;

	int i = 0;
	for (int j = 0; j < v.size(); j++) {
		os << v[j] << "\t";
		i++;
		if (i == sor_hossz){
			i = 0;
			os << endl;
		}
	}

	if (i > 0)
		os << endl;

	return os;
}

void buborekos_rendez3(vector<int> &v) {
	int i = v.size()-2;
	while (i >= 0) {
		int last = 0;

		for (int j=0; j<=i; ++j){
			if (v[j+1] < v[j]) {
				int temp = v[j];
				v[j] = v[j+1];
				v[j+1] = temp;
				last = j;
			}
		}

		i = last-1;
	}
}

int main() {
	vector<int> v;
	int n;

	while(cin >> n)
		v.push_back(n);

	cout << "Rendezes elott:" << endl;
	cout << v;

	buborekos_rendez3(v);

	cout << "Rendezes utan:" << endl;
	cout << v;

	return 0;
}
