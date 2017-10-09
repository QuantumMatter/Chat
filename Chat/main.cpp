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
static ScrollWindow *scrollWindow;

string printableIP(struct in_addr addr) {
    return inet_ntoa(addr);
}

void udpRead(UDPMessage *msg) {
    string ip = printableIP(msg->cliaddr->sin_addr);
    string console = ip + ": " + msg->message;
    scrollWindow->addLine(console, TRUE);
}

int main(int argc, const char * argv[]) {
    //Set up UDP Server
    UDPServer *server = new UDPServer(12345);
    server->readCallback = udpRead;
    
    //Initialize Curses
    initscr();
    //Capture special keys: ie DEL, Arrows
    //keypad(stdscr, TRUE);
    
    //Create different windows
        //Scrolling feed
        //User input
    feed = newwin(LINES-2, COLS, 0, 0);//Down to bottom - 2 lines
    scrollWindow = new ScrollWindow(feed, LINES-2, COLS);
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
            scrollWindow->scrollUp();
            continue;
        } else if (c == KEY_DOWN) {
            scrollWindow->scrollDown();
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
        //scrollWindow->addLine(str, true);
        
        //Broadcast new message
        char *cstr = new char[str.length()];
        strcpy(cstr, str.c_str());
        server->broadcast(cstr);
        
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
