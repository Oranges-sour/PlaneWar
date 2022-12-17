#ifndef __XML_MANAGER_H__
#define __XML_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
#include <vector>
using namespace std;

class XMLManager {
public:
    ~XMLManager();
    static void pushFileToManager(const string& fileName);
    static void removeAllFileToManager();
    static string getString(const string& key);

private:
    static vector<cocos2d::__Dictionary*> dicPool;
};

#endif
