#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;

float frand()
{
    return float(rand()) / RAND_MAX;
}

const bool log1 = false;
const bool log_2 = false;
const bool log3 = false;

int main()
{
    int N = 3;
    double p = 0.9; // 80%
    double r = 0.5; // 50%
    int z = N;

    string odpowiedz;
    cout << "Czy chcesz podac wlasne parametry? (tak/nie) ";
    cin >> odpowiedz;

    if (odpowiedz == "tak")
    {
        cout << "Podaj wartosc N: ";
        cin >> N;
        cout << "Podaj wartosc p: ";
        cin >> p;
        cout << "Podaj wartosc r: ";
        cin >> r;
        cout << "Podaj wartosc z: ";
        cin >> z;
    }

    double obc[N][3]    = {}; // obciazenie kazdego z procesorow
    int    migracje[3]  = {};
    int    zapytania[3] = {};

    srand(time(NULL));

    int ilosc_iteracji = N * (p / 0.06);

    for (int i = 0; i < ilosc_iteracji; i++)
    {
        int x = rand() % N;
        double moc = frand() / 10; // dostaje liczbe z przedzialu 0% - 10%

        if (log1 || log_2 || log3)
            cout << "Procesor " << x << " otrzymal zadanie o mocy " << moc * 100 << "%" << endl;

        // 1
        {
            if (log1)
            {
                cout << "Metoda 1:" << endl;
                for (int i = 0; i < N; i++)
                    cout << "Procesor " << i << ": " << obc[i][0] * 100 << "%" << endl;
                cout << endl;
            }

            int y = rand() % N;
            int ile_prob = 1;
            if (log1)
                cout << "Proba " << ile_prob << " wylosowany procesor " << y << " " << obc[y][0] * 100 << "/" << p * 100 << endl;
            while (obc[y][0] > p && ile_prob <= z)
            {
                y = rand() % N;
                ile_prob++;
                if (log1)
                    cout << "Proba " << ile_prob << " wylosowany procesor " << y << " " << obc[y][0] * 100 << "/" << p * 100 << endl;
            }
            zapytania[0] += ile_prob;
            if (ile_prob <= z)
                obc[y][0] += moc;
            else
                obc[x][0] += moc;
        }

        // 2
        {
            if (log_2)
            {
                cout << "Metoda 2:" << endl;
                for (int i = 0; i < N; i++)
                    cout << "Procesor " << i << ": " << obc[i][1] * 100 << "%" << endl;
                cout << endl;
            }

            if (obc[x][1] < p)
                obc[x][1] += moc;
            else
            {
                int y = rand() % N;

                if (log_2)
                    cout << "Wylosowany procesor " << y << " " << obc[y][1] * 100 << "/" << p * 100 << endl;
                zapytania[1]++;
                while (obc[y][1] > p)
                {
                    zapytania[1]++;
                    y = rand() % N;
                    if (log_2)
                        cout << "Wylosowany procesor " << y << " " << obc[y][1] * 100 << "/" << p * 100 << endl;
                }
                obc[y][1] += moc;
            }
        }

        // 3
        {
            if (log3)
            {
                cout << "Metoda 3:" << endl;
                for (int i = 0; i < N; i++)
                    cout << "Procesor " << i << ": " << obc[i][2] * 100 << "%" << endl;
                cout << endl;
            }

            if (obc[x][2] < r)
            {
                int y = rand() % N;
                int ile_prob = 1;
                while (obc[y][2] < p && ile_prob <= z)
                {
                    y = rand() % N;
                    ile_prob++;
                }
                if (obc[y][2] >= p)
                {
                    if (log3)
                        cout << "Procesor " << x << " przejmuje 5% obciazenia procesora " << y << endl;
                    migracje[2]++;
                    obc[y][2] -= 0.05;
                    obc[x][2] += 0.05;
                }
                obc[x][2] += moc;
            }
            else if (obc[x][2] < p)
            {
                obc[x][2] += moc;
            }
            else
            {
                int y = rand() % N;

                if (log3)
                    cout << "Wylosowany procesor " << y << " " << obc[y][2] * 100 << "/" << p * 100 << endl;

                zapytania[2]++;
                while (obc[y][2] > p)
                {
                    zapytania[2]++;
                    y = rand() % N;
                    if (log3)
                        cout << "Wylosowany procesor " << y << " " << obc[y][2] * 100 << "/" << p * 100 << endl;
                }
                obc[y][2] += moc;
            }
        }

        if (log1 || log_2 || log3)
            cout << endl << "---------------------" << endl << endl;
    }

    for (int metoda = 0; metoda < 3; metoda++)
    {
        cout << "Metoda " << metoda << ":" << endl;

        double srednia = 0;

        for (int i = 0; i < N; i++)
            srednia += obc[i][metoda];
        srednia /= N;

        cout << "Srednia: " << srednia << endl;

        double odchylenie = 0;

        for (int i = 0; i < N; i++)
            odchylenie += (srednia - obc[i][metoda]) * (srednia - obc[i][metoda]);
        odchylenie /= N;
        odchylenie = sqrt(odchylenie);

        cout << "Odchylenie: " << odchylenie << endl;

        cout << "Zapytania: " << zapytania[metoda] << endl;

        cout << "Migracje: " << migracje[metoda] << endl << endl;
    }

    return 0;
}
