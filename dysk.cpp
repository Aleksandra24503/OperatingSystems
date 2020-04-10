#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>

// maksymalna liczba bloków
#define MAX 100

using namespace std;

int koszt(int poz_wyjsciowa, int poz_docelowa)
{
    if (poz_wyjsciowa > poz_docelowa)
        return poz_wyjsciowa - poz_docelowa;
    else
        return poz_docelowa - poz_wyjsciowa;
}

int FCFS(vector<int> zlecenia, int glowica = 1)
{
    int suma_kosztow = 0;

    cout << "Tutaj algorytm FCFS" << endl;
    for (int i = 0; i < zlecenia.size(); i++)
    {
        cout << "Obsluguje zlecenie " << i << endl;
        cout << "Przesuwam glowice z pozycji " << glowica <<
                " na pozycje " << zlecenia[i] << endl;
        int k = koszt(glowica, zlecenia[i]);
        cout << "Koszt tej operacji to: " << k << endl;
        glowica = zlecenia[i];

        suma_kosztow += k;
    }

    return suma_kosztow;
}

int SSTF(vector<int> zlecenia, int glowica = 1)
{
    int suma_kosztow = 0;
    bool czy_obsluzone[zlecenia.size()] = {}; // ustawiamy wszystko na false

    cout << "Tutaj algorytm SSTF" << endl;

    for (int i = 0; i < zlecenia.size(); i++)
    {
        int zadanie = -1;

        // Szukamy nieobsluzonego zadania, ktore jest najblizej glowicy
        for (int j = 0; j < zlecenia.size(); j++)
            if (czy_obsluzone[j] == false && (zadanie == -1 ||
                    koszt(glowica, zlecenia[j]) < koszt(glowica, zlecenia[zadanie])))
                zadanie = j;

        cout << "Obsluguje zlecenie " << zadanie << endl;
        cout << "Przesuwam glowice z pozycji " << glowica <<
                " na pozycje " << zlecenia[zadanie] << endl;
        int k = koszt(glowica, zlecenia[zadanie]);
        cout << "Koszt tej operacji to: " << k << endl;
        glowica = zlecenia[zadanie];

        czy_obsluzone[zadanie] = true;

        suma_kosztow += k;
    }

    return suma_kosztow;
}

int SCAN(vector<int> zlecenia, int glowica = 1)
{
    cout << "Tutaj algorytm SCAN" << endl;

    vector<pair<int, int>> zlecenia_posortowane;

    for (int i = 0; i < zlecenia.size(); i++)
        zlecenia_posortowane.push_back(make_pair(zlecenia[i], i));
    sort(zlecenia_posortowane.begin(), zlecenia_posortowane.end());

    int dzielenie = upper_bound(zlecenia_posortowane.begin(), zlecenia_posortowane.end(), make_pair(glowica,0)) - zlecenia_posortowane.begin();

    int suma_kosztow = 0;

    for (int i = dzielenie - 1; i >= 0; i--)
    {
        // glowica idzie w lewo

        cout << "Obsluguje zlecenie " << zlecenia_posortowane[i].second << endl;
        cout << "Przesuwam glowice z pozycji " << glowica <<
                " na pozycje " << zlecenia_posortowane[i].first << endl;
        int k = koszt(glowica, zlecenia_posortowane[i].first);
        cout << "Koszt tej operacji to: " << k << endl;
        glowica = zlecenia_posortowane[i].first;

        suma_kosztow += k;
    }

    // glowica dochodzi do 1
    cout << "Glowica dochodzi do poczatku dysku." << endl;
    int k = koszt(glowica, 1);
    cout << "Koszt tej operacji to: " << k << endl;
    glowica = 1;
    suma_kosztow += k;

    for (int i = dzielenie; i < zlecenia.size(); i++)
    {
        // glowica idzie w prawo
        cout << "Obsluguje zlecenie " << zlecenia_posortowane[i].second << endl;
        cout << "Przesuwam glowice z pozycji " << glowica <<
                " na pozycje " << zlecenia_posortowane[i].first << endl;
        int k = koszt(glowica, zlecenia_posortowane[i].first);
        cout << "Koszt tej operacji to: " << k << endl;
        glowica = zlecenia_posortowane[i].first;

        suma_kosztow += k;
    }

    return suma_kosztow;
}

int CSCAN(vector<int> zlecenia, int glowica = 1)
{
    cout << "Tutaj algorytm CSCAN" << endl;

    vector<pair<int, int>> zlecenia_posortowane;

    for (int i = 0; i < zlecenia.size(); i++)
        zlecenia_posortowane.push_back(make_pair(zlecenia[i], i));
    sort(zlecenia_posortowane.begin(), zlecenia_posortowane.end());

    int dzielenie = upper_bound(zlecenia_posortowane.begin(), zlecenia_posortowane.end(), make_pair(glowica,0)) - zlecenia_posortowane.begin();

    int suma_kosztow = 0;

    for (int i = dzielenie - 1; i >= 0; i--)
    {
        // glowica idzie w lewo

        cout << "Obsluguje zlecenie " << zlecenia_posortowane[i].second << endl;
        cout << "Przesuwam glowice z pozycji " << glowica <<
                " na pozycje " << zlecenia_posortowane[i].first << endl;
        int k = koszt(glowica, zlecenia_posortowane[i].first);
        cout << "Koszt tej operacji to: " << k << endl;
        glowica = zlecenia_posortowane[i].first;

        suma_kosztow += k;
    }

    // glowica dochodzi do 1
    cout << "Glowica dochodzi do poczatku dysku." << endl;
    int k = koszt(glowica, 1);
    cout << "Koszt tej operacji to: " << k << endl;
    glowica = 1;
    suma_kosztow += k;

    cout << "Glowica dochodzi do konca dysku." << endl;
    k = koszt(glowica, MAX);
    cout << "Koszt tej operacji to: " << k << endl;
    glowica = MAX;
    suma_kosztow += k;

    for (int i = zlecenia.size()-1; i >= dzielenie; i--)
    {
        // glowica idzie w lewo
        cout << "Obsluguje zlecenie " << zlecenia_posortowane[i].second << endl;
        cout << "Przesuwam glowice z pozycji " << glowica <<
                " na pozycje " << zlecenia_posortowane[i].first << endl;
        int k = koszt(glowica, zlecenia_posortowane[i].first);
        cout << "Koszt tej operacji to: " << k << endl;
        glowica = zlecenia_posortowane[i].first;

        suma_kosztow += k;
    }

    return suma_kosztow;
}

int EDF(vector<pair<int, int>> zlecenia, int glowica = 1)
// zlecenia.first <- numer bloku ktory chcemy odczytac
// zlecenia.second <- deadline
{
    int zadanie;
    int suma_kosztow = 0; // to uznajemy jednoczesnie za czas
    int ile_obsluzonych = 0;
    bool czy_obsluzone[zlecenia.size()] = {}; // ustawiamy wszystko na false

    cout << "Tutaj algorytm EDF" << endl;

    do
    {
        zadanie = -1;

        for (int i = 0; i < zlecenia.size(); i++)
            if (czy_obsluzone[i] == false && suma_kosztow + koszt(glowica, zlecenia[i].first) <= zlecenia[i].second
                && (zadanie == -1 || zlecenia[i].second < zlecenia[zadanie].second))
                zadanie = i;

        if (zadanie != -1)
        {
            cout << "Obsluguje zlecenie " << zadanie << endl;
            cout << "Przesuwam glowice z pozycji " << glowica <<
                    " na pozycje " << zlecenia[zadanie].first << endl;
            int k = koszt(glowica, zlecenia[zadanie].first);
            cout << "Koszt tej operacji to: " << k << endl;
            glowica = zlecenia[zadanie].first;

            czy_obsluzone[zadanie] = true;
            ile_obsluzonych++;

            suma_kosztow += k;
        }

    } while (zadanie != -1);

    cout << "Udalo sie obsluzyc " << ile_obsluzonych << " z " << zlecenia.size() << " zadan." << endl;

    return suma_kosztow;
}

int main()
{
    int N, glowica, koszt_algorytmu;
    vector<int> V;
    string odpowiedz;

    cout << "Podaj ilosc elementow: ";
    cin >> N;

    cout << "Wpisac elementy losowo? (T/N): ";
    cin >> odpowiedz;

    if (odpowiedz == "T")
    {
        srand(time(NULL));
        for (int i = 0; i < N; i++)
            V.push_back(rand()%(MAX-1) + 1);
    }
    else
    {
        cout << "Podaj elementy:" << endl;
        for (int i = 0; i < N; i++)
        {
            int val;
            cin >> val;
            V.push_back(val);
        }
    }

    cout << "Podaj glowice: ";
    cin >> glowica;
    cout << endl;

    koszt_algorytmu = FCFS(V, glowica);
    cout << "Koszt calego algorytmu to: " << koszt_algorytmu << endl << endl;
    koszt_algorytmu = SSTF(V, glowica);
    cout << "Koszt calego algorytmu to: " << koszt_algorytmu << endl << endl;
    koszt_algorytmu = SCAN(V, glowica);
    cout << "Koszt calego algorytmu to: " << koszt_algorytmu << endl << endl;
    koszt_algorytmu = CSCAN(V, glowica);
    cout << "Koszt calego algorytmu to: " << koszt_algorytmu << endl << endl;

    vector<pair<int, int>> V2;

    for (int i = 0; i < N; i++)
    {
        V2.push_back(make_pair(V[i], rand() % (2*MAX) + MAX));
    }

    koszt_algorytmu = EDF(V2, glowica);
    cout << "Koszt calego algorytmu to: " << koszt_algorytmu << endl << endl;

    return 0;
}
