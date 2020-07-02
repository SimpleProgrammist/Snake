#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <time.h>

using namespace std;

void goXY (int x, int y);
void HideCursor();
void draw (char p[][25], int *x, int *y, int high, int widh);
void options (int *high, int *widh, int *speed);
int game (int boardHigh, int boardWidth, int speed);

class Winner
{
public:
    string nick;
    int score;

    void mySort (Winner t[]);
    void load (Winner t[]);
};

int main()
{
    HideCursor();

    char s = 178;  // Walls
    int point;
    short int chose;

    int boardHigh = 25;
    int boardWidth = 25;
    int speed = 10;

    Winner people[5];
    people[5].load(people);

    fstream f;

    HANDLE hOut;
    hOut = GetStdHandle( STD_OUTPUT_HANDLE );

    for (;;)
    {
        goXY(0,0);
        SetConsoleTextAttribute( hOut, FOREGROUND_BLUE | FOREGROUND_RED);
        cout <<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<< endl;
        cout <<s<<s<<" (1) New game   "<<s<<s<< endl;
        cout <<s<<s<<" (2) Options    "<<s<<s<< endl;
        cout <<s<<s<<" (3) Ranking    "<<s<<s<< endl;
        cout <<s<<s<<" (4) Exit       "<<s<<s<< endl;
        cout <<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<< endl;

        chose = getch() - 48;

        switch (chose)
        {
            case 1:
            {
                point = game(boardHigh, boardWidth, (-10 * speed) + 100);
                if (point > people[4].score)
                {
                    people[4].score = point;
                    cout << "Congratulation! Get your nick: ";
                    getline (cin, people[4].nick);
                }
                people[5].mySort(people);
                f.open ("ranking.txt", ios::out);
                if (f.good() == false)
                {
                    cout << "Error - file not found";
                    return 1;
                }
                for (int i = 0; i < 5; ++i)
                {
                    f << people[i].nick << endl;
                    f << people[i].score << endl;
                }
                f.close ();
                system ("cls");
                break;
            }
            case 2:
            {
                options(&boardHigh, &boardWidth, &speed);
                break;
            }
            case 3:
            {
                system("cls");
                cout << "\t\tRanking\n" << endl;
                for (int i = 0; i < 5; ++i)
                {
                    cout.width (20); cout << left << people[i].nick << "   "; cout.width (11); cout << right << people[i].score << endl;
                }
                cout << "\nPress any button to back";
                getch ();
                system("cls");
                break;
            }
            case 4:
            {
                system("cls");
                cout << "Bye bye";
                Sleep (1000);
                return 0;
            }
        }
    }
    goXY(0,26);

    return 0;
}

int game(int boardHigh, int boardWidth, int speed)
{
    char w = 219;
    char s = 178;

    char board[25][25];
    char movement = 'd';
    int xw, yw, lenght = 4;
    int hxw[10000] = {0};
    int hyw[10000] = {0};
    int xj, yj;
    int i, j, cycle = -1;

    for (i = 0; i < boardHigh; ++i)
    {
        for (j = 0; j < boardWidth; j++)
        {
            if ((i == 0 || i == (boardHigh - 1)) || (j == 0 || j == (boardWidth - 1))) board[i][j] = 's';
                else board[i][j] = 'p';
        }
    }

    srand (time(NULL));

    xw = rand()% (boardHigh - 10) + 1;
    yw = rand()% (boardWidth - 10)+ 1;
    board [xw][yw] = 'g';
    draw (board, &xj, &yj, boardHigh, boardWidth);

    HANDLE hOut;
    hOut = GetStdHandle( STD_OUTPUT_HANDLE );

    for (;;)
    {
        goXY(0,0);
        ++cycle;
        hxw[cycle] = xw;
        hyw[cycle] = yw;

        if (cycle >= lenght)
        {
            board[hxw[cycle-lenght]][hyw[cycle-lenght]] = 'p';
        }

        for (i = 0; i < boardHigh; ++i)
        {
            for (j = 0; j < boardWidth; j++)
            {
                switch (board[i][j])
                {
                    case 'p':   /// Background
                    {
                        SetConsoleTextAttribute( hOut, BACKGROUND_BLUE );
                        cout << "  ";
                        break;
                    }
                    case 'w':   /// Snake
                    case 'g':   /// Snake's head
                    {
                        SetConsoleTextAttribute( hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << w << w;
                        break;
                    }
                    case 's':   /// Wall
                    {
                        SetConsoleTextAttribute( hOut, FOREGROUND_RED );
                        cout<< w << w; break;
                    }

                    case 'j':   /// Food
                    {
                        SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << w << w; break;
                    }
                }
            }
            cout << endl;
        }

        Sleep (speed);

        if (kbhit())
        {
            int pomr = movement;
            movement = getch();

            if (movement == -32)    /// arrow keys
            {
                movement = getch();
                switch (movement)
                {
                    case 72: movement = 'w'; break;
                    case 75: movement = 'a'; break;
                    case 77: movement = 'd'; break;
                    case 80: movement = 's'; break;
                }
            }

            if (movement != 'w' && movement != 's' && movement != 'a' && movement != 'd' && movement != 'p' && movement != 'q' && movement != 'e' && movement != 'z' && movement != 'c') movement = pomr;
        }

        switch (movement)
        {
            case 'w': --xw; break;
            case 's': ++xw; break;
            case 'a': --yw; break;
            case 'd': ++yw; break;
            case 'q': {--xw; --yw; break;}
            case 'e': {--xw; ++yw; break;}
            case 'z': {++xw; --yw; break;}
            case 'c': {++xw; ++yw; break;}
            case 'p':
            {
                while (!(kbhit())){}
            }
        }

        if (board[xw][yw] == 's' || board[xw][yw] == 'w') break;
            else if (board[xw][yw] == 'j')
            {
                ++lenght;
                draw (board, &xj, &yj, boardHigh, boardWidth);
            }
        board[xw][yw] = 'g';
        board[hxw[cycle-1]][hyw[cycle-1]] = 'w';
    }

    goXY(16,9);  cout <<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s;
    goXY(16,10); cout <<s<<s<<"                 "<<s<<s;
    goXY(16,11); cout <<s<<s<<" Your score: "; cout.width (3); cout<< lenght<< ' ' <<s<<s;
    goXY(16,12); cout <<s<<s<<"                 "<<s<<s;
    goXY(16,13); cout <<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s;
    Sleep (1000);
    system("cls");

    return lenght;
}

void draw (char p[][25], int *x, int *y, int high, int widh)
{
    do
    {
        *x = rand()%high-4 + 1;
        *y = rand()%widh-4 + 1;
    } while (p[*x][*y] != 'p');
    p[*x][*y] = 'j';
}


void goXY (int x, int y)
{
    HANDLE hCon;
    COORD dwPos;

    dwPos.X=x;
    dwPos.Y=y;

    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

void HideCursor()
{
    ::HANDLE hConsoleOut =::GetStdHandle( STD_OUTPUT_HANDLE );
    ::CONSOLE_CURSOR_INFO hCCI;
    ::GetConsoleCursorInfo( hConsoleOut, & hCCI );
    hCCI.bVisible = FALSE;
    ::SetConsoleCursorInfo( hConsoleOut, & hCCI );
}

void options(int *high, int *widh, int *speed)
{
    char s = 178;
    short int chose;
    int x;
    do
    {
        system ("cls");
        cout <<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<< endl;
        cout <<s<<s<<" (1) Board high  (11 - 25) "; cout.width (2); cout<<*high<<' '<<s<<s<< endl;
        cout <<s<<s<<" (2) Board width (11 - 25) "; cout.width (2); cout<<*widh<<' '<<s<<s<< endl;
        cout <<s<<s<<" (3) Bpeed  (1 - 10)       ";     cout.width (2); cout << *speed <<' '<<s<<s<< endl;
        cout <<s<<s<<" (4) Return                   " <<s<<s<< endl;
        cout <<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<<s<< endl;

        chose = getch() - 48;
        system ("cls");

        switch (chose)
        {
            case 1:
            {
                cout <<"board high = ";
                cin >> x;
                if (x < 10 || x > 25) chose = 4;
                    else *high = x;
                break;
            }
            case 2:
            {
                cout <<"board width = ";
                cin >> x;
                if (x < 10 || x > 25) chose = 4;
                    else *widh = x;
                break;
            }
            case 3:
            {
                cout <<"Speed = ";
                cin >> x;
                if (x < 1 || x > 10) chose = 4;
                    else *speed = x;
                break;
            }
        }
    } while (chose != 4);
}

void Winner::mySort (Winner t[])
{
    int x, i;
    string miano;
    for (i = 4; i > 0; --i)
    {
        if (t[i].score > t[i-1].score)
        {
            x = t[i].score;           miano = t[i].nick;
            t[i].score = t[i-1].score;  t[i].nick = t[i-1].nick;
            t[i-1].score = x;         t[i-1].nick = miano;
        } else break;
    }
}

void Winner::load (Winner t[])
{
    int i = 1, nr_os = 0;  // people counters
    std::string line;

    std::fstream f;
    f.open ("ranking.txt", std::ios::in);
    if (f.good() == false)
    {
        std::ofstream outfile ("ranking.txt");
        for (int j = 0; j < 5; ++j)
        {

            outfile << "Nobody" << std::endl << "0" << std::endl;
            t[j].nick = "Nobody";
            t[j].score = 0;
        }
        outfile.close();
        return;
    }

    while (getline(f, line))
    {
        switch (i)
        {
            case 1: t[nr_os].nick = line; break;
            case 2: t[nr_os].score = atoi(line.c_str()); i = 0; ++nr_os; break;
        }
        ++i;
    }

    f.close ();
}
