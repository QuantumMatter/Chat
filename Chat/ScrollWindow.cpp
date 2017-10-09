//
//  ScrollWindow.cpp
//  Chat
//
//  Created by David Kopala on 10/8/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "ScrollWindow.h"

ScrollWindow::ScrollWindow(WINDOW *win, int newLines, int newCols)
{
    window = win;
    scrollok(window, TRUE);
    lineCount = newLines;
    colCount = newCols;
    
    strings = new List<string>();
    currentIndex = 0;
}

void ScrollWindow::addLine(string newLine, bool scroll)
{
    strings->addCopy(newLine);
    if (scroll) {
        scrollBottom();
        currentIndex = strings->count()-1;
    }
}

void ScrollWindow::scrollTo(int index)
{
    clearScreen();
    currentIndex = index;
    currentIndex = min(strings->count() - 1, currentIndex);
    currentIndex = max(currentIndex, strings->count() - lineCount);
    if (currentIndex < 0) {
        currentIndex = 0;
    }
    for (int i = 0; i < currentIndex; i++) {//max(min(strings->count(), lineCount), 0); i++) {
        wmove(window, lineCount-1-i, 0);
        string str = *strings->get(currentIndex - i);
        for (int j = 0; j < str.length(); j++) {
            waddch(window, str[j]);
        }
    }
    wrefresh(window);
}

void ScrollWindow::scrollDown()
{
    scrollTo(currentIndex+1);
}

void ScrollWindow::scrollBottom()
{
    clearScreen();
    currentIndex = strings->count() -1;
    for (int i = 0; i < max(min(strings->count(), lineCount), 0); i++) {
        wmove(window, lineCount-1-i, 0);
        string str = *strings->get(currentIndex - i);
        for (int j = 0; j < str.length(); j++) {
            waddch(window, str[j]);
        }
    }
    wrefresh(window);
}

void ScrollWindow::scrollUp()
{
    scrollTo(currentIndex-1);
}

void ScrollWindow::scrollTop() {
    clearScreen();
    for (int i = 0; i < lineCount; i++) {
        wmove(window, lineCount-1-i, 0);
        string str = *strings->get(lineCount-1-i);
        for (int j = 0; j < str.length(); j++) {
            waddch(window, str[j]);
        }
    }
}

void ScrollWindow::clearScreen() {
    for (int i = 0; i < lineCount; i++) {
        wmove(window, i, 0);
        for (int j = 0; j < colCount; j++) {
            waddch(window, ' ');
        }
    }
}
