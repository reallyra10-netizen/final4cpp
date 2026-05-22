#include "ConsoleUtils.hpp"
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;
static HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

// color 
void setColor  (int c) { SetConsoleTextAttribute(hCon, c); }
void resetColor()      { SetConsoleTextAttribute(hCon, 7); }
void ms        (int t) { Sleep(t); }
void cls       ()      { system("cls"); }

void hideCursor()
{
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(hCon, &ci);
}

void showCursor()
{
    CONSOLE_CURSOR_INFO ci = {10, TRUE};
    SetConsoleCursorInfo(hCon, &ci);
}

void gotoxy(int x, int y)
{
    COORD c; c.X = x; c.Y = y;
    SetConsoleCursorPosition(hCon, c);
}

void setupConsole()
{
    system("mode con: cols=72 lines=38");
    SetConsoleTitleA("  Worker Management System  v2.0");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
}

// box style
static const string TL="╔", TR="╗", BL="╚", BR="╝";
static const string H ="═", V ="║";
static const string ML="╠", MR="╣";
static const string SL="╟", SR="╢", SH="─";

const int IW = 68;

string rep(const string &s, int w)
{
    string r; r.reserve(w * s.size());
    for (int i = 0; i < w; i++) r += s;
    return r;
}

string center(const string &s, int w)
{
    int len = (int)s.size();
    if (len >= w) return s.substr(0, w);
    int l = (w - len) / 2, r = w - len - l;
    return string(l, ' ') + s + string(r, ' ');
}

string left_pad(const string &s, int w)
{
    int len = (int)s.size();
    if (len >= w) return s.substr(0, w);
    return s + string(w - len, ' ');
}

// animation 
void typeText(const string &t, int color, int delay)
{
    setColor(color);
    for (char c : t) { cout << c; cout.flush(); ms(delay); }
    resetColor();
}

void animStr(const string &s, int color, int delay)
{
    setColor(color);
    for (char c : s) { cout << c; cout.flush(); ms(delay); }
    resetColor();
}

void put(const string &s, int color)
{
    setColor(color); cout << s; resetColor();
}

void progressBar(int color, int steps, int delay)
{
    put("  [", 8);
    for (int i = 0; i < steps; i++)
    {
        cout << "\r";
        put("  [", 8);
        put(rep("#", i+1), color);
        put(rep(" ", steps-i-1), 8);
        put("]", 8);
        put("  " + to_string((i+1)*100/steps) + "%  ", 15);
        cout.flush();
        ms(delay);
    }
    cout << std::endl;
}

void spinner(const string &label, int color, int dur)
{
    const char fr[] = {'|','/','-','\\'};
    int steps = dur / 60;
    for (int i = 0; i < steps; i++)
    {
        setColor(color);
        cout << "\r  " << fr[i%4] << "  " << label << "   ";
        resetColor(); cout.flush(); ms(60);
    }
    cout << "\r" << string(50,' ') << "\r";
}

void pulse(const string &msg, int color, int times)
{
    for (int i = 0; i < times; i++)
    {
        setColor(color);   cout << "\r  " << msg; cout.flush(); ms(80);
        setColor(color-2 < 0 ? color : color-2);
        cout << "\r  " << msg; cout.flush(); ms(80);
    }
    setColor(color); cout << "\r  " << msg << std::endl; resetColor();
}

//  drawing
void drawTop(int borderColor, int)
{
    setColor(borderColor);
    cout << TL << rep(H, IW+2) << TR << std::endl;
    resetColor();
}

void drawBot(int borderColor, int)
{
    setColor(borderColor);
    cout << BL << rep(H, IW+2) << BR << std::endl;
    resetColor();
}

void drawMid(int borderColor, int)
{
    setColor(borderColor);
    cout << ML << rep(H, IW+2) << MR << std::endl;
    resetColor();
}

void drawSep(int borderColor)
{
    setColor(borderColor);
    cout << SL << rep(SH, IW+2) << SR << std::endl;
    resetColor();
}

void row(const string &content, int borderColor, int textColor, bool centered)
{
    string padded = centered ? center(content, IW) : left_pad(" "+content, IW);
    setColor(borderColor); cout << V;
    setColor(textColor);   cout << " " << padded << " ";
    setColor(borderColor); cout << V << std::endl;
    resetColor();
}

void rowEmpty(int borderColor)
{
    setColor(borderColor);
    cout << V << string(IW+2, ' ') << V << std::endl;
    resetColor();
}

// page 
void pageHeader(const string &title, int borderColor, int titleColor)
{
    cout << std::endl;
    drawTop(borderColor);
    row(title, borderColor, titleColor, true);
    drawMid(borderColor);
    cout << std::endl;
}

void pageFooter(int borderColor)
{
    cout << std::endl;
    drawBot(borderColor);
}

void menuRow(const string &text, int borderColor, int textColor, int)
{
    row(text, borderColor, textColor);
}


void animSuccess(const string &msg) { cout << std::endl; pulse("  OK  " + msg, 10); }
void animError  (const string &msg) { cout << std::endl; pulse("  !!  " + msg, 12); }

void waitEnter()
{
    cout << std::endl;
    setColor(8); cout << "  Press ENTER to continue ..."; resetColor();
    cin.ignore(1000,'\n'); cin.get();
}

string readPassword()
{
    string password;
    int ch;

    while ((ch = _getch()) != '\r')
    {
        if (ch == 0 || ch == 0xE0)
        {
            _getch();
            continue;
        }

        if (ch == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
            continue;
        }

        if (ch < 32 || ch > 126)
        {
            continue;
        }

        password.push_back(static_cast<char>(ch));
        cout << '*';
    }

    cout << std::endl;
    return password;
}

bool readInt(int &out)
{
    string inp; cin >> inp;
    for (size_t i = 0; i < inp.size(); i++)
    {
        char c = inp[i];
        if (!isdigit(c) && !(c=='-' && i==0))
        { animError("invalid input! enter a number."); cin.clear(); return false; }
    }
    try { out = stoi(inp); return true; }
    catch (...) { animError("number out of range."); return false; }
}
