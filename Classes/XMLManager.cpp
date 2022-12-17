#include "header/XMLManager.h"

USING_NS_CC;
using namespace std;

vector<__Dictionary*> XMLManager::dicPool(20, nullptr);

XMLManager::~XMLManager()
{
    for (size_t x = 0; x < dicPool.size(); ++x) {
        if (dicPool[x]) {
            dicPool[x]->release();
            dicPool[x] = nullptr;
        }
    }
}

void XMLManager::pushFileToManager(const string& fileName)
{
    auto dic = __Dictionary::createWithContentsOfFile(fileName.c_str());
    dic->retain();
    for (size_t x = 0; x < dicPool.size(); ++x) {
        if (dicPool[x] == nullptr) {
            dicPool[x] = dic;
            return;
        }
    }

    //当池满的时候无法添加新的文件
    dic->release();
    cocos2d::log(
        ">from class XMLManager::pushFileToManager(...) --> dicPool is full! "
        "Cannot push more file!");
}

void XMLManager::removeAllFileToManager()
{
    for (size_t x = 0; x < dicPool.size(); ++x) {
        if (dicPool[x]) {
            dicPool[x]->release();
            dicPool[x] = nullptr;
        }
    }
}

string XMLManager::getString(const string& key)
{
    const __String* result = nullptr;
    for (size_t x = 0; x < dicPool.size(); ++x) {
        auto dic = dicPool[x];
        if (dic) {
            auto str = dic->valueForKey(key);
            if (str->length() == 0)
                continue;
            else {
                result = str;
                break;
            }
        }
    }
    if (result)
        return string(result->getCString());
    else
        return string();
}
