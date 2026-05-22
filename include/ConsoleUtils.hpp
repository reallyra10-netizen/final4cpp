#pragma once
#include <string>

using std::string;

// color
void setColor(int c);
void resetColor();
void ms(int t);
void cls();
void hideCursor();
void showCursor();
void gotoxy(int x, int y);
void setupConsole();

// string 
string rep(const string &s, int w);
string center(const string &s, int w);
string left_pad(const string &s, int w);

// animation 
void typeText(const string &t, int color, int delay = 8);
void animStr (const string &s, int color, int delay = 2);
void put     (const string &s, int color);
void progressBar(int color, int steps = 28, int delay = 12);
void spinner (const string &label, int color, int dur = 400);
void pulse   (const string &msg,   int color, int times = 2);

// box 
extern const int IW;

void drawTop(int borderColor, int delay = 0);
void drawBot(int borderColor, int delay = 0);
void drawMid(int borderColor, int delay = 0);
void drawSep(int borderColor);
void row     (const string &content, int borderColor, int textColor, bool centered = false);
void rowEmpty(int borderColor);

// page 
void pageHeader(const string &title, int borderColor, int titleColor);
void pageFooter(int borderColor);
void menuRow   (const string &text,  int borderColor, int textColor, int delay = 0);

void animSuccess(const string &msg);
void animError  (const string &msg);
void waitEnter  ();
bool readInt    (int &out);
string readPassword();
