#ifndef BASE_PAGE_H
#define BASE_PAGE_H

#include <map>
#include <string>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

class base_Page : public Layer {
protected:
    void addButton(MenuItem* m, const string& key);

    //无法通过key寻找
    void addButton(MenuItem* m);
    void disableAllButtons();
    void disableButton(const string& key);
    void enableAllButtons();
    void enableButton(const string& key);

private:
    int count = 0;
    map<string, MenuItem*> menuItems;
};

#endif  // !BASE_PAGE_H
