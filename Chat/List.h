//
//  List.hpp
//  Playground
//
//  Created by David Kopala on 8/13/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef List_h
#define List_h

#include <stdio.h>
#include <stdlib.h>

template <class T>
class List {
    
public:
    List() {
        list = (ListItem *)malloc(sizeof(ListItem));
        list->value = NULL;
        list->next = NULL;
    };
    void add(T *value) {
        ListItem *item = list;
        while (item->next != NULL) {
            item = item->next;
        }
        item->next = (ListItem *)malloc(sizeof(ListItem));
        item->next->value = value;
        item->next->next = NULL;
    };
    void addCopy(T value) {
        T *val = (T *)malloc(sizeof(T));
        *val = value;
        
        ListItem *item = list;
        while (item->next != NULL) {
            item = item->next;
        }
        item->next = (ListItem *)malloc(sizeof(ListItem));
        item->next->value = val;
        item->next->next = NULL;
    };
    int count() {
        int count = 0;
        ListItem *item = list;
        while ((item = item->next) != NULL) {
            count += 1;
        }
        return count;
    };
    T *first() {
        if (list->next != NULL) {
            return list->next->value;
        } else {
            return NULL;
        }
    };
    T *last() {
        if(list->next != NULL) {
            ListItem *last = list;
            while(last->next != NULL) {
                last = last->next;
            }
            return last->value;
        } else {
            return NULL;
        }
    };
    T *get(int index) {
        int currentIndex = 0;
        ListItem *currentItem = list;
        while((currentItem = currentItem->next) != NULL) {
            if (currentIndex == index) {
                return currentItem->value;
            } else {
                currentIndex += 1;
            }
        }
        return NULL;
    };
    
private:
    struct ListItem {
        ListItem *next;
        T *value;
    };
    struct ListItem *list;
    
};

#endif /* List_h */
