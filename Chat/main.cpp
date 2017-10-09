//
//  main.cpp
//  Chat
//
//  Created by David Kopala on 10/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include <iostream>
#include <arpa/inet.h>
#include <ncurses.h>
#include <signal.h>
#include <math.h>

#include "UDPServer.h"
#include "UDPMessage.h"
#include "ScrollWindow.h"

using namespace std;

WINDOW *feed;
WINDOW *input;

string printableIP(struct in_addr addr) {
    return inet_ntoa(addr);
}

void udpRead(UDPMessage *msg) {
    string ip = printableIP(msg->cliaddr->sin_addr);
    string console = ip + ": " + msg->message;
    cout<<console<<endl;
}

int line = 0;
List<string> lines;
void addLineToFeed(WINDOW *win, string str) {
    lines.addCopy(str);
    //If window is full, move everything up one line
    for (int i = 0; i < min(LINES - 4, lines.count()+1); i++) {
        string *newline = lines.get(lines.count() - 1);
        wmove(feed, LINES - 4 - i, 0);
        for (int j = 0; j < newline->length(); j++) {
            waddch(feed, (*newline)[j]);
        }
    }
}

void zeroFill(char *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = '\0';
    }
}

void writeToInput(string str) {
    
}

int main(int argc, const char * argv[]) {
    //Initialize Curses
    initscr();
    //Capture special keys: ie DEL, Arrows
    //keypad(stdscr, TRUE);
    
    //Create different windows
        //Scrolling feed
        //User input
    feed = newwin(LINES-2, COLS, 0, 0);//Down to bottom - 2 lines
    ScrollWindow *scroll = new ScrollWindow(feed, LINES-2, COLS);
    input = newwin(1, COLS, LINES-1, 0);//Last line
    
    //Fill second to last line with flat line
    wmove(stdscr, LINES-2, 0);
    for (int i = 0; i < COLS; i++) {
        waddch(stdscr, '-');
    }
    wrefresh(stdscr);
    
    nodelay(input, TRUE);
    keypad(input, TRUE);
    string str = "";
    while (str != "[quit]") {
        int c = wgetch(input);
        if (c == ERR) {
            continue;
            //NO INPUT
        } else if (c == KEY_UP) {
            scroll->scrollUp();
            continue;
        } else if (c == KEY_DOWN) {
            scroll->scrollDown();
            continue;
        } else if ((c == KEY_DC) || (c == 127)) {
            str = str.substr(0, str.length()-1);
            //Clear input
            wmove(input, 0, 0);
            for (int i = 0; i < COLS; i++) {
                waddch(input, ' ');
            }
            wmove(input, 0, 0);
            for (int i = 0; i < str.length(); i++) {
                waddch(input, str[i]);
            }
            continue;
        } else if (c != '\n') {
            str += c;
            continue;
        }
        
        //Print new message in feed
        scroll->addLine(str, true);
        
        //Clear input
        wmove(input, 0, 0);
        for (int i = 0; i < COLS; i++) {
            waddch(input, ' ');
        }
        wmove(input, 0, 0);
        
        //Update screen
        wrefresh(feed);
        wrefresh(input);
        str = "";
    }
    
    endwin();
}
