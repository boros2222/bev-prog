#include "std_lib_facilities.h"

ostream& operator<<(ostream& os, vector<int>& v) {
	int sor_hossz = 10;

	int i = 0;
	for (int j = 0; j < v.size(); j++) {
		os << v[j] << " ";
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

class LZWBinFa {
	struct Elem {
		int adat;
		Elem* bal;
		Elem* jobb;
	};

	Elem* uj_elem(int n) {
		Elem* temp = new Elem;
		temp->adat = n;
		temp->bal = NULL;
		temp->jobb = NULL;

		return temp;
	}

	Elem* gyoker_ptr;
	Elem* aktualis_ptr;

	vector<int> levelek_vec;

public:
	LZWBinFa() {
		gyoker_ptr = uj_elem(-1);
		aktualis_ptr = gyoker_ptr;
	}

	const Elem* gyoker() const { return gyoker_ptr; }

	vector<int> levelek() const { return levelek_vec; }

	bool beszur(int n) {
		if (n == 0) {
			if (aktualis_ptr->bal == NULL) {
				aktualis_ptr->bal = uj_elem(0);
				aktualis_ptr = gyoker_ptr;

				return 1;
			}
			else {
				aktualis_ptr = aktualis_ptr->bal;

				return 0;
			}
		}
		else if (n != 0) {
			if (aktualis_ptr->jobb == NULL) {
				aktualis_ptr->jobb = uj_elem(1);
				aktualis_ptr = gyoker_ptr;

				return 1;
			}
			else {
				aktualis_ptr = aktualis_ptr->jobb;

				return 0;
			}
		}
	}

	void inorder(const Elem* pointer, vector<int> &v) {
		if (pointer != NULL) {
			inorder(pointer->bal, v);
			v.push_back(pointer->adat);
			inorder(pointer->jobb, v);
		}
	}

	void preorder(const Elem* pointer, vector<int> &v) {
		if (pointer != NULL) {
			v.push_back(pointer->adat);
			preorder(pointer->bal, v);
			preorder(pointer->jobb, v);
		}
	}

	void postorder(const Elem* pointer, vector<int> &v) {
		if (pointer != NULL) {
			postorder(pointer->bal, v);
			postorder(pointer->jobb, v);
			v.push_back(pointer->adat);
		}
	}

	void agak(const Elem* pointer, vector<int> &v) {
		if (pointer != NULL) {
			v.push_back(pointer->adat);

			if (pointer->bal == NULL && pointer->jobb == NULL) {
				cout << "Utvonal: " << endl << v;
				levelek_vec.push_back(v.size()-1);
			}

			agak(pointer->bal,v);
			agak(pointer->jobb,v);
			v.pop_back();
		}
	}

	~LZWBinFa() {
		torol(gyoker_ptr);
		aktualis_ptr = NULL;
	}

	void torol(Elem* pointer) {
		if (pointer != NULL) {
			torol(pointer->bal);
			torol(pointer->jobb);

			delete pointer;
		}
	}
};

double szoras_kiszamol(vector<int> v, double a) {
	double szoras, szamlalo = 0;

	for(auto iter = v.begin(); iter != v.end(); ++iter)
		szamlalo += (*iter - a)*(*iter - a);

	szoras = sqrt(szamlalo / v.size());

	return szoras;
}

int main() {
	LZWBinFa fa;
	int n;

	cout << "Kerek biteket (0 vagy 1):" << endl;
	
	while(cin >> n)
		cout << "Bekerult (bit=" << (n==0 ? 0 : 1) << "): " << (fa.beszur(n) ? "igen" : "nem") << endl;
	cout << endl;

	vector<int> v, levelek;

	cout << "preorder bejaras:" << endl;
	fa.preorder(fa.gyoker(), v);
	cout << v;
	v.clear();

	cout << "inorder bejaras:" << endl;
	fa.inorder(fa.gyoker(), v);
	cout << v;
	v.clear();

	cout << "postorder bejaras:" << endl;
	fa.postorder(fa.gyoker(), v);
	cout << v << endl;
	v.clear();

	fa.agak(fa.gyoker(), v);

	levelek = fa.levelek();
	cout << endl << "Levelek: " << levelek;
	cout << "Levelek szama: " << levelek.size() << endl;

	int osszeg = 0;
	double atlag;
	for(auto iter = levelek.begin(); iter != levelek.end(); ++iter)
		osszeg += *iter;
	atlag = (double) osszeg / levelek.size();
	cout << "Osszeg: " << osszeg << endl;
	cout << "Atlag: " << atlag << endl;

	double szoras;
	szoras = szoras_kiszamol(levelek, atlag);
	cout << "Szoras: " << szoras << endl;

	return 0;
}
