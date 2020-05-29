#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <set>

using namespace std;

class proces
{
private:
    int it;
    int zbior_stron;
    vector<int> ostatnie_strony;
public:
    proces() : zbior_stron(20), it(0)
    {
        ostatnie_strony.resize(5);
        for (int i = 0; i < 5; i++)
            ostatnie_strony[i] = i % zbior_stron;
    }

    void set_zbior_stron(int zs)
    {
        zbior_stron = zs;
    }

    int get_zbior_stron()
    {
        return zbior_stron;
    }

    void set_ostatnie_strony(int wartosc)
    {
        ostatnie_strony.resize(wartosc);
        for (int i = 0; i < wartosc; i++)
            ostatnie_strony[i] = i % zbior_stron;
        if (it > wartosc) it = 0;
    }

    int rzadanie()
    {
        if (rand() % 5 < 2) // z prawdopodobieństwem 40% losujemy nowš stronę
            ostatnie_strony[it] = rand() % zbior_stron;

        int odpowiedz = ostatnie_strony[it];

        /*it++;
        if (it == ostatnie_strony.size())
            it = 0;*/
        it = rand() % ostatnie_strony.size();

        return odpowiedz;
    }
};

enum metoda
{
    proporcjonalny,
    rowny,
    sterowanie_czestoscia,
    strefowy
};

class systemik
{
private:
    int low;
    int high;
    int seed;
    vector<pair<int, int>> ramka; // nr procesu ; nr strony
    vector<int> ostatnie_zgloszenie;
    vector<proces*> procesy;

    void inicjuj_ramki(metoda m)
    {
        if (m == rowny || m == sterowanie_czestoscia || m == strefowy)
        {
            for (int i = 0; i < ramka.size(); i++)
            {
                ramka[i] = make_pair(procesy.size() * i / ramka.size(), -1);
                ostatnie_zgloszenie[i] = -1;
            }
        }

        if (m == proporcjonalny)
        {
            int suma = 0;

            for (int i = 0; i < procesy.size(); i++)
                suma += procesy[i]->get_zbior_stron(); // sumuje ile kazdy z procesow ma stron

            // ile ramek ma dostac pierwszy
            // ~ ile stron ma / suma stron wszystkich * ilosc ramek

            int z = 0;
            int current = procesy[0]->get_zbior_stron();

            for (int i = 0; i < ramka.size(); i++)
            {
                if (i * double(suma) / ramka.size() > current)
                {
                    z++;
                    current += procesy[z]->get_zbior_stron();
                }

                ramka[i].first = z; // proces
                ramka[i].second = -1; // nr strony (pisze wyzej)
                ostatnie_zgloszenie[i] = -1;
            }
        }

        /*cout << "DEBUG" << endl;
        for (int i = 0; i < ramka.size(); i++)
        {
            cout << "(" << ramka[i].first << ", " << ramka[i].second << ")" << endl;
        }*/
    }

    void redeleguj_ramki(vector<int> ilosc_ramek, vector<bool> aktywne_procesy)
    {
        vector<vector<pair<int, int>>> stare_dane;

        stare_dane.resize(procesy.size());

        for (int i = 0; i < ramka.size(); i++)
            stare_dane[ramka[i].first].push_back(make_pair(ramka[i].second, ostatnie_zgloszenie[i]));

        for (int i = 0; i < procesy.size(); i++)
            sort(stare_dane[i].begin(), stare_dane[i].end(), [](pair<int, int> a, pair<int, int> b) {return a.second > b.second;});

        int pozycja = 0;

        for (int i = 0; i < procesy.size(); i++)
            if (aktywne_procesy[i])
                for (int j = 0; j < ilosc_ramek[i]; j++)
                    if (j < stare_dane[j].size())
                    {
                        ramka[pozycja] = make_pair(i, stare_dane[i][j].first);
                        ostatnie_zgloszenie[pozycja] = stare_dane[i][j].second;

                        pozycja++;
                    }
                    else
                    {
                        ramka[pozycja] = make_pair(i, -1);
                        ostatnie_zgloszenie[pozycja] = -1;

                        pozycja++;
                    }
    }
public:
    systemik() : seed(0), low(10), high(40)
    {
        ramka.resize(40);
        ostatnie_zgloszenie.resize(40);
    }

    void set_low(int l)
    {
        low = l;
    }

    void set_high(int h)
    {
        high = h;
    }

    void set_seed(int s)
    {
        seed = s;
    }

    void dodaj_proces(proces* p)
    {
        procesy.push_back(p);
    }

    void set_ramki(int r)
    {
        ramka.resize(r);
        ostatnie_zgloszenie.resize(r);
    }

    void wykonaj(metoda m, int iteracji)
    {
        int czas = 0;
        int numer_iteracji = 0;
        int zlicz_bledy = 0;
        int zakonczone_procesy = 0;
        vector<int> poszczegolne_bledy;
        vector<int> poszczegolne_bledy_w_iteracji;
        vector<int> ilosc_ramek_dla_procesu;
        vector<int> ilosc_iteracji_procesu;
        vector<bool> aktywne_procesy;
        set<int> strefa[procesy.size()];

        poszczegolne_bledy.resize(procesy.size(), 0);
        poszczegolne_bledy_w_iteracji.resize(procesy.size(), 0);
        aktywne_procesy.resize(procesy.size(), false);
        ilosc_iteracji_procesu.resize(procesy.size(), 0);
        ilosc_ramek_dla_procesu.resize(procesy.size(), 0);

        srand(seed);

        inicjuj_ramki(m);
        for (int i = 0; i < ramka.size(); i++)
        {
            ilosc_ramek_dla_procesu[ramka[i].first]++;
            aktywne_procesy[ramka[i].first] = true;
        }

        while (zakonczone_procesy < procesy.size())
        {
            cout << "Iteracja nr " << ++numer_iteracji << endl;
            for (int i = 0; i < procesy.size(); i++)
            {
                if (aktywne_procesy[i])
                    cout << "Proces " << i << " otrzymal " << ilosc_ramek_dla_procesu[i] << " ramek." << endl;
                else
                    cout << "Proces " << i << " nieaktywny." << endl;
            }

            for (int i = 0; i < procesy.size(); i++)
            {
                poszczegolne_bledy_w_iteracji[i] = 0;
                strefa[i].clear();
            }

            for (int z = 0; z < 100; z++, czas++)
                for (int j = 0; j < procesy.size(); j++)
                    if (aktywne_procesy[j])
                    {
                        int strona = procesy[j]->rzadanie();

                        strefa[j].insert(strona);

                        // szukamy strony w naszych ramkach
                        int k = 0;
                        while (k < ramka.size() && ramka[k] != make_pair(j, strona)) k++;

                        if (k < ramka.size())
                        {
                            // znalezlismy ramke!
                            ostatnie_zgloszenie[k] = czas;
                        }
                        else
                        {
                            // nie znalezlismy ramki
                            zlicz_bledy++;
                            poszczegolne_bledy[j]++;
                            poszczegolne_bledy_w_iteracji[j]++;

                            // szukamy ramki danego procesu do ktorego zgloszenie bylo najdawniej
                            for (int l = 0; l < ramka.size(); l++)
                            {
                                if (ramka[l].first == j && (k == ramka.size() || ostatnie_zgloszenie[l] < ostatnie_zgloszenie[k]))
                                    k = l;
                            }
                            ramka[k] = make_pair(j, strona);
                            ostatnie_zgloszenie[k] = czas;
                        }
                    }

            for (int j = 0; j < procesy.size(); j++)
                if (aktywne_procesy[j])
                    if (++ilosc_iteracji_procesu[j] == iteracji)
                    {
                        aktywne_procesy[j] = false;
                        ilosc_ramek_dla_procesu[j] = 0;
                        zakonczone_procesy++;
                    }

            if (m == sterowanie_czestoscia)
            {
                for (int i = 0; i < procesy.size(); i++)
                    if (aktywne_procesy[i])
                    {
                        if (poszczegolne_bledy_w_iteracji[i] < low)
                            ilosc_ramek_dla_procesu[i]--;
                        else if (high < poszczegolne_bledy_w_iteracji[i])
                            ilosc_ramek_dla_procesu[i]++;
                    }
            }

            if (m == strefowy)
            {
                for (int i = 0; i < procesy.size(); i++)
                    if (aktywne_procesy[i])
                        ilosc_ramek_dla_procesu[i] = strefa[i].size();
            }

            if (m == sterowanie_czestoscia || m == strefowy)
            {
                vector<int> nrp; // numery procesow;

                for (int i = 0; i < procesy.size(); i++)
                    nrp.push_back(i);

                sort(nrp.begin(), nrp.end(), [=](int a, int b){return ilosc_iteracji_procesu[a] < ilosc_iteracji_procesu[b];});

                int uzyte = 0;

                for (int i = 0; i < nrp.size(); i++)
                    if (ilosc_iteracji_procesu[nrp[i]] < iteracji && uzyte + ilosc_ramek_dla_procesu[nrp[i]] <= ramka.size())
                    {
                        aktywne_procesy[nrp[i]] = true;
                        uzyte += ilosc_ramek_dla_procesu[nrp[i]];
                    }
                    else aktywne_procesy[nrp[i]] = false;
            }
        }

        cout << endl << "Wybrana metoda: ";
        if (m == proporcjonalny)
            cout << "proporcjonalna" << endl;
        if (m == rowny)
            cout << "rowny" << endl;
        if (m == sterowanie_czestoscia)
            cout << "sterowanie czestoscia" << endl;
        if (m == strefowy)
            cout << "strefowa" << endl;
        cout << zlicz_bledy << " bledow." << endl;
        for (int i = 0; i < procesy.size(); i++)
            cout << "proces " << i << ": " << poszczegolne_bledy[i] << " bledow." << endl;
        cout << endl;
    }
};

int main()
{
    int N;
    systemik S;
    proces *p;
    string odpowiedz;

    cout << "Chcesz samemu dobrac parametry? (T/N) ";
    cin >> odpowiedz;

    if (odpowiedz == "T")
    {
        cout << "Podaj ilosc procesow: ";
        cin >> N;

        p = new proces[N];

        for (int i = 0; i < N; i++)
        {
            int stron, ostatnie;

            cout << "Podaj ile stron ma miec proces " << i << ": ";
            cin >> stron;
            cout << "Podaj ile ma pamietac ostatnich stron: ";
            cin >> ostatnie;

            p[i].set_zbior_stron(stron);
            p[i].set_ostatnie_strony(ostatnie);
        }
    }
    else
    {
        N = 3;
        int stron[3] = {10, 20, 30};
        int ostatnie[3] = {5, 3, 10};

        p = new proces[N];

        for (int i = 0; i < N; i++)
        {
            p[i].set_zbior_stron(stron[i]);
            p[i].set_ostatnie_strony(ostatnie[i]);
        }
    }

    for (int i = 0; i < N; i++)
        S.dodaj_proces(&p[i]);

    cout << endl;

    //S.wykonaj(proporcjonalny, 1);
    //S.wykonaj(rowny, 1);
    S.wykonaj(sterowanie_czestoscia, 5);
    S.wykonaj(strefowy, 5);

    delete[] p;

    return 0;
}
