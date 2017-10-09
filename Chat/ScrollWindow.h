//
//  ScrollWindow.hpp
//  Chat
//
//  Created by David Kopala on 10/8/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef ScrollWindow_hpp
#define ScrollWindow_hpp

#include <stdio.h>
#include <ncurses.h>
#include <string>
#include <math.h>

#include "List.h"

using namespace std;

class ScrollWindow {

public:
    ScrollWindow(WINDOW *win, int newLines, int newCols);
    
    void addLine(string newLine, bool scroll);
    
    void scrollUp();
    void scrollTop();
    void scrollDown();
    void scrollBottom();
    void scrollTo(int index);
    void clearScreen();
    
    void resize(int newLines, int newCols);
    
    string getLine();
    
private:
    WINDOW *window;
    
    List<string> *strings;
    
    int lineCount;
    int colCount;
    
    int currentIndex;//string index above input window
};

#endif /* ScrollWindow_hpp */
