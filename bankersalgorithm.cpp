#include <iostream>

using namespace std;

int main()
{
    int nprocesses; // iloœæ procesów
    int nresources; // iloœæ zasobów

    cout << "Enter number of processes: ";
    cin  >> nprocesses;

    cout << "Enter number of resources: ";
    cin  >> nresources;

    int  allocation[nprocesses][nresources];
    int  maxr[nprocesses][nresources];
    int  need[nprocesses][nresources];
    int  freeresources[nresources];
    bool finish[nprocesses] = {};

    cout << "Enter allocation of resources." << endl;
    for (int i = 0; i < nprocesses; i++)
    {
        cout << "  Process " << i+1 << ":" << endl;
        for (int j = 0; j < nresources; j++)
        {
            cout << "    Resource " << j+1 << ": ";
            cin >> allocation[i][j];
        }
    }

    cout << "Enter maximum of resources." << endl;
    for (int i = 0; i < nprocesses; i++)
    {
        cout << "  Process " << i+1 << ":" << endl;
        for (int j = 0; j < nresources; j++)
        {
            cout << "    Resource " << j+1 << ": ";
            cin >> maxr[i][j];
        }
    }

    cout << "Enter free resources." << endl;
    for (int i = 0; i < nresources; i++)
    {
        cout << "  Resource " << i+1 << ": ";
        cin >> freeresources[i];
    }

    for (int i = 0; i < nprocesses; i++)
        for (int j = 0; j < nresources; j++)
            need[i][j] = maxr[i][j] - allocation[i][j];

    cout << "Need: " << endl;
    for (int i = 0; i < nprocesses; i++)
    {
        for (int j = 0; j < nresources; j++)
            cout << need[i][j] << " ";
        cout << endl;
    }


    bool change = true;
    while (change)
    {
        change = false;

        for (int i = 0; i < nprocesses; i++)
            if (!finish[i])
            {
                bool can_we_finish = true;

                for (int j = 0; j < nresources; j++)
                    if (need[i][j] > freeresources[j])
                        can_we_finish = false;

                if (can_we_finish)
                {
                    finish[i] = true;
                    cout << "Process " << i+1 << " can finish!" << endl;
                    change = true;

                    for (int j = 0; j < nresources; j++)
                        freeresources[j] += allocation[i][j];
                }
                else
                {
                    cout << "Process " << i+1 << " can't be executed!" << endl;
                }
            }
    }

    return 0;
}
