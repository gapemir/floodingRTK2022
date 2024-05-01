//* More visible comment
//! Warning comment
//? Question comment
// TODO Reminder to implement a feature
// @param param opis parametra

#include <chrono>
#include <ctime>
#include <windows.h>
//#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;
#define A 300



void cls() {
    //system("cls");
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void barva(int mreza[A][A], int i, int j) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (mreza[i][j] == 16)
        SetConsoleTextAttribute(hConsole, 15);
    else
        SetConsoleTextAttribute(hConsole, mreza[i][j] * 16);
}

void poplavla(int mreza[][A], int y, int x, int zdej_barva, int nova_barva, int size) {
    if (x < 0 || x >= size || y < 0 || y >= size)
        return;
    else if (mreza[y][x] != zdej_barva)
        return;
    else if (mreza[y][x] == nova_barva)
        return;

    mreza[y][x] = nova_barva; //zamena barvo celice

    poplavla(mreza, y + 1, x, zdej_barva, nova_barva, size);
    poplavla(mreza, y - 1, x, zdej_barva, nova_barva, size);
    poplavla(mreza, y, x + 1, zdej_barva, nova_barva, size);
    poplavla(mreza, y, x - 1, zdej_barva, nova_barva, size);
}

void printscr(int mreza[][A], int size) { //naprinta mrežo
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            barva(mreza, y, x);
            cout << mreza[y][x];
            if (mreza[y][x] < 10)
                cout << " ";
            SetConsoleTextAttribute(hConsole, 15);
        }
        cout << endl;
    }
    cout << endl;
}

int check(int mreza[][A], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (mreza[0][0] != mreza[i][j])
                return 0;
    return 1;
}

void party_time() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0;; i++) {
        if (i % 16 == 0)
            SetConsoleTextAttribute(hConsole, 15);
        else
            SetConsoleTextAttribute(hConsole, (i % 16) * 16);
        cout << " ";
        cls();
    }
}

void zmaga(int poteze, int mode) {
    if (mode == 1) {
        cls();
        int party = 0;
        cout << "Bravo zmagal si v " << poteze << " potezah.\n\r";
        cout << "A bi su u party mode?\n\rCe bi sou prtisn 1\n\r";
        cin >> party;
        if (party == 1)
            party_time();
    } else {
        cout << "program je porabil " << poteze << " potez";
    }
}

void zacetk(int &mode, int &size, int &color, int &naloga, int &GUI, int &custom) {
    cout << "****************************\n\npozdraulen v igri potaplanje\n\n****************************\n";
    cout << "Izberi\n1-Custom\n2-From file\n";
    cin >> custom;
    if (custom != 1 && custom != 2)
        custom = 0;
    cout << "Izberi\n1-Igraj\n2-Autosolve\n";
    cin >> mode;
    if (mode != 1 && mode != 2)
        mode = 0;
    if (custom == 1) {
        cout << "Velikost: ";
        cin >> size;
        cout << "Barve: ";
        cin >> color;
    } else if (custom == 2) {
        cout << "Stevilka naloge:";
        cin >> naloga;
    }
}

void vpis(int &nova_barva, int color) {
    cin >> nova_barva;
    if (nova_barva > 0 && nova_barva <= color)
        return;
    cout << "stupid\n";
    vpis(nova_barva, color);
}

void load_file(int mreza[A][A], int naloga, int &size, int &color) {
    int nalogaload = 0;
    ifstream inFile("poplaulanje.in");
    string words1;
    int words;
    inFile >> words1;
    inFile >> words;
    while (1) {
        for (int k = 0; k < 4; k++) {
            inFile >> words;
            if (k == 0)
                nalogaload = words;
            else if (k == 1 || k == 2)
                size = words;
            else if (k == 3)
                color = words;
        }
        if (naloga == nalogaload) {
            for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                    inFile >> words;
                    mreza[y][x] = words;
                }
            }
            break;
        } else {
            for (int v = 0; v < size * size; v++) {
                inFile >> words;
            }
        }
    }
}

int ecsa(int mreza[A][A], int size, int color, int &nova_barva ) { //enhanced color search algorithm
    int colr, j = 1;
    int betterc[16];
    for (int i = 0; i < 16; i++)
        betterc[i] = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            colr = mreza[i][j];
            if (colr != nova_barva)
                betterc[colr - 1] += 1;
        }
    colr = betterc[0];
    for (int i = 1; i < 16; i++)
        if (colr < betterc[i]) {
            colr = betterc[i];
            j = i + 1;
        }
    return j;
}

int edrs(int mreza[A][A], int size,int x,int y) {//enhanced diagonal nekineki system
    int b=0,c=0;
    for(int i=0;i<x;i++){
        if (mreza[y][i]==mreza[y][x])
            b++;
        else
            c++;
    }
    for(int i=0;i<y;i++){
        if (mreza[i][x]==mreza[y][x])
            b++;
        else
            c++;
    }
    if(b>=c)
        return false;
    else
        return true;
    cout<<endl;
}

int colors[16];

void braincheck(int mreza2[][A], int y, int x, int zdej_barva,int size) {
    if (x < 0 || x >= size || y < 0 || y >= size)
        return;
    else if (mreza2[y][x] != zdej_barva)
        return;

    colors[zdej_barva-1]+=1;

    braincheck(mreza2, y + 1, x, zdej_barva, size);
    braincheck(mreza2, y, x + 1, zdej_barva, size);
    braincheck(mreza2, y - 1, x, zdej_barva, size);
    braincheck(mreza2, y, x - 1, zdej_barva, size);
}

void brain(int mreza[A][A], int size, int zdej_barva, int &nova_barva ){
    int mreza2[A][A],colr=0;
    nova_barva+=1;
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            mreza2[i][j]=mreza[i][j];

    poplavla(mreza2, 0, 0, zdej_barva, nova_barva, size);
    braincheck(mreza2, 0, 0, nova_barva, size);

}

void autosolve2(int mreza[A][A], int naloga, int size, int color, int &poteze, int zdej_barva, int &nova_barva, int &GUI, int custom) { //diagonala
    int y = 0, x = 0, prej_barva = 0, y1 = 0, x1 = 0, y2 = 0, x2 = 0, m = 0, brk = 0;
    bool k = 0, k1 = 0, k2 = 0, g=false;
    cout << "GUI:";
    cin >> GUI;
    if (custom == 2)
        load_file(mreza, naloga, size, color);
    x1 = size / 2;
    y1 = x1;
    x2 = x1;
    y2 = x1;
    for (poteze = 0;; poteze++) {
        if (GUI == 1)
            printscr(mreza, size);
        if (check(mreza, size) == 1) {
            zmaga(poteze, 2);
            break;
        }
        if ((x >= size - 1 || y >= size - 1) && poteze != 0) {
            nova_barva = ecsa(mreza, size, color, nova_barva);
            /*nova_barva=0;
            for(int i=0;i<16;i++)
                brain(mreza,size,zdej_barva,nova_barva);
            //todo tuki treba loopat da check use barve in pol se zalaufa ta da se sprazni
                for(int i=0;i<16;i++)
                    colors[i]=0;*/
        } else if (mreza[y][x] != 0 || poteze == 0) {
            prej_barva = mreza[0][0];
            while (1) {
                if (m % 3 == 0) {
                    if (k == true)
                        y++;
                    else
                        x++;
                    k = !k;
                    nova_barva = mreza[y][x];
                    if (mreza[y][x] != prej_barva)
                        break;
                } else if (m % 3 == 1) {
                    if (k1 == true)
                        y1++;
                    else
                        x1--;
                    k1 = !k1;
                    nova_barva = mreza[y1][x1];
                    if (mreza[y1][x1] != prej_barva)
                        break;
                } else if (m % 3 == 2) {
                    if (k2 == true)
                        y2--;
                    else
                        x2++;
                    k2 = !k2;
                    nova_barva = mreza[y2][x2];
                    if (mreza[y2][x2] != prej_barva)
                        break;
                }
                if (y1 == size - 1 || x1 == 0 || y2 == 0 || x2 == size - 1) {
                    brk = 1;
                    m = 0;
                    nova_barva = 1;
                }
            }
            if (y >= size / 2 && x >= size / 2 && brk == 0) {
                if (m == 0){
                    g=edrs(mreza, size, x-1, y-1);
                    if(g==true)
                    m++;
                }
                else if (g==true)
                    m++;
            }
        }
        if (nova_barva == 0)
            nova_barva = ecsa(mreza, size, color, nova_barva);
        zdej_barva = mreza[0][0];
        poplavla(mreza, 0, 0, zdej_barva, nova_barva, size);
        cout << x << " " << y << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << poteze <<" "<<nova_barva<< endl;
    }
    cout << g<<endl;
    cout << endl;
}

void play(int mreza[A][A], int naloga, int size, int color, int &poteze, int zdej_barva, int &nova_barva, int custom) {
    if (custom == 2)
        load_file(mreza, naloga, size, color);
    while (1) {
        printscr(mreza, size);
        if (check(mreza, size) == 1) {
            zmaga(poteze, 1);
            break;
        }
        vpis(nova_barva, color);
        poteze++;
        zdej_barva = mreza[0][0];
        poplavla(mreza, 0, 0, zdej_barva, nova_barva, size);
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    srand(time(NULL));
    int zdej_barva = 0, nova_barva = 1, poteze = 0, size = 0, color = 0, mode = 0, naloga = 0, GUI = 0, custom = 0; //x in y sta kjer zacne preverjat in spreminjat barve
    int mreza[A][A], next[A][A];
    zacetk(mode, size, color, naloga, GUI, custom);
    for (int i = 0; i < A; i++) {
        for (int j = 0; j < A; j++) {
            mreza[i][j] = 0;
        }
    }
    if (mode == 1) {
        play(mreza, naloga, size, color, poteze, zdej_barva, nova_barva, custom);
    } else if (mode == 2) {
        autosolve2(mreza, naloga, size, color, poteze, zdej_barva, nova_barva, GUI, custom);
    } else if (0) {
        for (int i = 0; i < A; i++)
            for (int j = 0; j < A; j++) {
                next[i][j] = mreza[i][j];
            }

        zdej_barva = mreza[0][0];
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << duration.count() / 1000.0 << "ms\n";
    return 0;
}
