#include "header/base_Page.h"

#include <cstdlib>

void base_Page::addButton(MenuItem* m, const string& key)
{
    menuItems.insert({key, m});
}

void base_Page::addButton(MenuItem* m)
{
    //没有key要自己生成key
    char key[256] = "";
    snprintf(key, sizeof(key), "%d", count);
    ++count;
    this->addButton(m, key);
}

void base_Page::disableAllButtons()
{
    for (auto& it : menuItems) {
        if (it.second) {
            it.second->setEnabled(false);
        }
    }
}

void base_Page::disableButton(const string& key)
{
    auto n = menuItems.find(key);
    n->second->setEnabled(false);
}

void base_Page::enableAllButtons()
{
    for (auto& it : menuItems) {
        if (it.second) {
            it.second->setEnabled(true);
        }
    }
}

void base_Page::enableButton(const string& key)
{
    auto n = menuItems.find(key);
    n->second->setEnabled(true);
}
