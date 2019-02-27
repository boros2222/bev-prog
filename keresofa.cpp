#include "std_lib_facilities.h"

struct elem {
	int adat;
	elem* bal;
	elem* jobb;
};

using elem_ptr = elem*;
using vektor = vector<int>;

elem_ptr uj_levelelem (int ertek);
bool fa_beszur (elem_ptr & gyoker, int ertek);
    // A fa gyökerének mutatóját referenciaként adom át,
    // Í­gy módosí­tható lesz a függvényen belül.
int fa_magassag(elem_ptr gyoker);
int fa_elemszam (elem_ptr gyoker);
int fa_osszegez (elem_ptr gyoker);
void fa_preorder (elem_ptr gyoker, vektor & v);
    // A vektorok referenciaként való átadásával a
    // módosíthatóság mellett további előnyt jelent
    // az adatmásolások elmaradása --egy vektorban
    // nagyon sok elem lehet.
void fa_inorder (elem_ptr gyoker, vektor & v);
void fa_postorder (elem_ptr gyoker, vektor & v);
void fa_agak (elem_ptr gyoker, vektor & v);
void fa_torol (elem_ptr & gyoker);
void kiir_vektor (vektor & v, int sor_hossz);
void kiir_fa (elem_ptr gyoker, int magassag);


int main (){

    elem_ptr gyoker = NULL;   // 'gyoker' egy üres fára mutat
    int i;
    bool siker=true;
    vektor v;

    while (siker && cin>>i) {
	v.push_back(i);
	siker=fa_beszur(gyoker, i);
    }
    cout << "Input adatok:\n";
    kiir_vektor(v,10);
    v.clear();

    if (gyoker != NULL){ // azaz, ha sikerült adatot beolvasni
	int elemszam = fa_elemszam(gyoker);
	int magassag = fa_magassag(gyoker);
	int osszeg = fa_osszegez(gyoker);
	double atlag = osszeg / ((double) elemszam); // A '(double)' segí­tségével
		    // kényszerí­tem a fordí­tót, hogy az osztás előtt konvertálja át
		    // az 'elemszam' értékét valós számmá, és í­gy ne egész-osztással
		    // határozza meg az 'atlag' értékét.

	kiir_fa(gyoker, magassag);
	
	cout << "A fa elemszáma: " << elemszam << '\n';
	cout << "A fa magassága: " << magassag << '\n';
	cout << "A fa elemeinek átlaga: " << setprecision(9) << atlag << '\n';
					    // 9 értékes jegyet írjon ki

	fa_preorder(gyoker,v);
	cout << "\nA fa preorder bajárásának sorrendje:\n";
	kiir_vektor(v,10);
	v.clear();

	fa_inorder(gyoker,v);
	cout << "\nA fa inorder bajárásának sorrendje:\n";
	kiir_vektor(v,10);
	v.clear();

	fa_postorder(gyoker,v);
	cout << "\nA fa postorder bajárásának sorrendje:\n";
	kiir_vektor(v,10);
	v.clear();

	cout << "\nA fa ágai:\n";
	fa_agak(gyoker,v);

	fa_torol(gyoker); // Dinamikus memória kezelés egyik sarkalatos pontja,
		// hogy a lefoglalt tárterületeket szabadítsuk fel, ha már
		// nincs rájuk szükségünk.
		// Igaz, a főprogram 'return' előtti utolsó utasításánál
		// ez csupán elvi jelentőséggel bír, de függvényekben
		// már nagyon fontos lehet ez a tevékenység, ha el akarjuk kerülni,
		// hogy senki által nem használt tárterületek miatt ne maradjon hely
		// a későbbiekben esedékes tevékenységek számára.
	gyoker = NULL; // A 'fa_torol'-ben lévő 'delete' csak felszabadította a
	    // lefoglalt memória területet, de változatlanul hagyta 'gyoker' értékét.
	    // Ez hibákhoz vezethetne a későbbiekben, pl.:
	    // if (gyoker != NULL) cout << gyoker->adat;
	if (gyoker != NULL) // Ha "kikommentezzük" az előző utasítást ....
	    cout << "Oops: gyoker nem változott NULL-ra!!!\n";
    }

    return 0;
}

elem_ptr uj_levelelem(int ertek){
//  elem_ptr uj = new elem;
    // Dinamikus memóriakezelés egyik sarkalatos pontja, hogy vajon van-e elgendő
    // hely az újabb adatok tárolására.
    // Gondot okozhat a túl sok adat érkezése, vagy ha valamilyen oknál fogva
    // a programunk csak a memória kis hányada fölött rendelkezhetne.
    // A 'new' memória lefoglaló utasítás alap esetben egy kivételt váltana ki ilyenkor.
    
    // Ha azonban az alábbi formában használjuk 'new' függvényt, akkor ez nem fog bekövetkezni.
    // (Jelenleg nem kí­vánunk foglalkozni a kivételkezelés témakörével.)
    // Helyette a visszatérési érték 'NULL' lesz, azaz nem fog mutatni sehová,
    // ezzel jelezve, hogy nem sikerült a kért helyfoglalás a memóriában.
    elem_ptr uj = new (nothrow) elem;
	// 'nothow' ==> ne váltson ki kivételt (exception-t)
	// Így viszont nekünk kell ellenőriznünk, hogy 
	// rendben lezajlott-e a helyfoglalás:
    if (uj == NULL){
	cerr << "Nincs már szabad hely a memóriában\n";
	// 'cerr' olyan, mint 'cout' csak akkor is a képernyőre ír, ha a
	// normál kimenetet máshová irányítottuk ($ ./keresőfa > fajl-nev).
	
	// Ide jöhetne egy:
	// exit (EXIT_FAILURE);
	// De ebben az esetben a már beí­rt adatokat sem tudnánk feldolgozni.
	// (Ilyenkor a 'fa_beszur' is lehetne 'void' értékű függvény, amely
	// nem is vizsgálná az 'uj_levelelem' értékét.)
	// Persze gyakran nincs is értelme csupán az adatok egy részét feldolgozni.
    }
    else{
	uj->adat = ertek;
	uj->bal = uj->jobb = NULL;
    }

    return uj;
}

bool fa_beszur (elem_ptr & gyoker, int ertek){
    if (gyoker != NULL){

	if (gyoker->adat == ertek){
	    cerr << "Ilyen érték (" << ertek << ") már van a fában!\n";
	    return true;	// Ha ezt az 'if'-et töröljük, akkor minden elemet be fog 
	}		// tenni a fába, azaz egy érték többször is előforduhat benne.
	// 'Keresőfában' nem lehetnek azonos értékű elemek, de más jellegű alkalmazásoknál
	// ez is megfontolható lehet, pl. ha az összes input adat rendezése a célunk.

	if (gyoker->adat > ertek)
	    return fa_beszur(gyoker->bal, ertek);
	else
	    return fa_beszur(gyoker->jobb, ertek);
    }
    else{
	gyoker = uj_levelelem(ertek);
	return (gyoker != NULL);
	// Ha egy adatnak már nincs hely a memóriában, akkor a többinek sem lesz.
	// Ezért a függvényértéken keresztül jelezzük a hí­vónak, hogy már nincs
	// értelme több adatot beolvasni az inputból.
    }
}

int fa_magassag(elem_ptr gyoker){
    if (gyoker == NULL)
	return 0;
    int b = fa_magassag(gyoker->bal);
    int j = fa_magassag(gyoker->jobb);
    if (b > j)
	return b + 1;
    else
	return j + 1;
}

int fa_elemszam (elem_ptr gyoker){
    if (gyoker == NULL)
	return 0;
    return fa_elemszam(gyoker->bal) + 1 + fa_elemszam(gyoker->jobb);
}

int fa_osszegez (elem_ptr gyoker){
    if(gyoker == NULL)
	return 0;
    return fa_osszegez(gyoker->bal) + gyoker->adat + fa_osszegez(gyoker->jobb);
}

void fa_preorder (elem_ptr gyoker, vektor & v){
    if (gyoker != NULL){
	v.push_back(gyoker->adat); // Általánosan: feldolgoz(gyoker->adat);
	fa_preorder(gyoker->bal,v);
	fa_preorder(gyoker->jobb,v);
    }
}

void fa_inorder (elem_ptr gyoker, vektor & v){
    if (gyoker != NULL){
	fa_inorder(gyoker->bal,v);
	v.push_back(gyoker->adat); // Általánosan: feldolgoz(gyoker->adat);
	fa_inorder(gyoker->jobb,v);
    }
}

void fa_postorder (elem_ptr gyoker, vektor & v){
    if (gyoker != NULL){
	fa_postorder(gyoker->bal,v);
	fa_postorder(gyoker->jobb,v);
	v.push_back(gyoker->adat); // Általánosan: feldolgoz(gyoker->adat);
    }
}

void fa_agak (elem_ptr gyoker, vektor & v){
    // A fa gyökerétől a levélelemekig vezető utakat gyűjti össze
    // és í­ratja ki a kimenetre.
    if (gyoker != NULL){
	v.push_back(gyoker->adat);
	if (gyoker->bal == gyoker->jobb) // ez csak úgy fordulhat elő, ha mindkettő NULL értékű
					// (legalábbis helyes működés mellett, azaz programhiba nélkül)
	    kiir_vektor(v,v.size());
	fa_agak(gyoker->bal,v);
	fa_agak(gyoker->jobb,v);
	v.pop_back();
    }
}

void fa_torol (elem_ptr & gyoker){
    if (gyoker != NULL){
	fa_torol (gyoker->bal);
	fa_torol (gyoker->jobb);
	delete gyoker;
    }
}

void kiir_vektor (vektor & v, int sor_hossz){
    int i=0;
    for (int j=0; j < v.size(); j++){
	cout << v[j] <<", ";
	i++;
	if (i == sor_hossz){
	    i=0;
	    cout << '\n';
	}
    }
    if (i > 0)
	cout << '\n';
}

//--------------------------------------------------

using vector_fa = vector<elem_ptr>;

// A kupac-hoz hasonlóan képezem le egy vektorba a fát.
// Így könnyebb lesz 'kirajzolni'.
// (Ugyanakkor rendkí­vül pazarló tárolási mód, ezért csak kis fákra használom.)
void masol_fa_vector(elem_ptr gyoker, vector_fa & vf, int index){
    if (gyoker != NULL){
	vf[index] = gyoker;
	masol_fa_vector(gyoker->bal, vf, index+index+1);
	masol_fa_vector(gyoker->jobb, vf, index+index+2);
    }
}

void kiir_fa (elem_ptr gyoker, int magassag){
// Ez az algoritmus feltételezi, hogy a legalsó szinten
// --legalábbis közvetlen egymás mellett-- nincsenek
// 99-nél nagyobb, ill. -9-nél kisebb számok, azaz
// ezen a szinten minden szám leírható legfeljebb 2 karakterrel.
    
    if ((gyoker == NULL) || (magassag > 6)){ // Ha magas a fa ==> nagyon széles a legalsó sor,
					     // ill. nagyon sok eleme lenne a 'vf' vector-nak.
					     // Inkább nem 'rajzoljuk' ki.
	cout << endl; // endl: sor-vége jel ('\n').
	return;
    }
    int n = 1;
    int m = magassag;
    while (m>0){
	m--;
	n += n;
    }
    vector_fa vf(n);
    masol_fa_vector(gyoker, vf, 0);
    n += n/2;
    m = n;
    for (elem_ptr gy = gyoker; gy != NULL ; gy=gy->bal)
	m = m/2;
    m-=2;
    int elso = 0;
    int kiirva;
    int pozicio;
    cout << "\nA fa 'rajza':\n";
    for (int szint = 0; szint < magassag; szint++ ){
	kiirva = 0;
	for (int i = elso; i <= elso+elso; i++){
	    if (vf[i] != NULL){
		pozicio = n*(i-elso)+(n+1)/2-m;
		cout << setw(pozicio-kiirva) << vf[i]->adat;
		kiirva = pozicio;
	    }
	}
	cout << '\n';
	kiirva = 0;
	for (int i = elso; i <= elso+elso; i++){
	    if (vf[i] != NULL){
		if (vf[i]->bal != NULL){
		    pozicio = n*(i-elso)+(n+1)/2-m-n/8-1;
		    cout << setw(pozicio-kiirva) << '/';
		    kiirva = pozicio;    
		}
		if (vf[i]->jobb != NULL){
		    pozicio = n*(i-elso)+(n+1)/2-m+n/8;
		    cout << setw(pozicio-kiirva) << '\\';
		    kiirva = pozicio;    
		}
	    }
	}
	cout << '\n';
	elso += elso+1;
	n /= 2;
    }
    
    
}
