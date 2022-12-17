#include "header/MyThreads.h"

#include <functional>
#include <future>
#include <thread>
#include <vector>
using namespace std;

namespace {
const UINT __threadsQuantity = thread::hardware_concurrency();
constexpr UINT MAX_THREAD_NUMBER = 6;
}  // namespace

void callInThread(
    const size_t vecSize,
    const function<void(UINT from, UINT to, UINT threadNumber)>& callFunc)
{
    vector<future<void>> futures;
    const UINT threadsQuantity = getMaxThreadNumber();
    if (vecSize >= threadsQuantity) {
        //һ���߳�����Ҫ���µ�
        const UINT n = vecSize / threadsQuantity;
        for (UINT _x = 0; _x < threadsQuantity - 1; ++_x) {
            futures.push_back(
                async(launch::async, callFunc, n * _x, n * (_x + 1), _x));
        }
        /*���vecSize /threadsQuantity��ʵ��ֵ��Ϊ����ʱ,
            ʹ������n�����и��»��ж�����²���.
            �������һ���߳�ֱ�Ӹ��µ�ĩβ*/
        futures.push_back(async(launch::async, callFunc,
                                n * (threadsQuantity - 1), vecSize,
                                threadsQuantity - 1));
    } else {
        // vector�Ĵ�С���߳�������ʱ,��ֱ�ӿ�vector�Ĵ�С���߳�������
        for (UINT _x = 0; _x < vecSize; ++_x)
            futures.push_back(async(launch::async, callFunc, _x, _x + 1, _x));
    }

    for (auto it = futures.begin(); it != futures.end(); ++it) {
        (*it).get();
    }
}

UINT getMaxThreadNumber()
{
    //���õ��߳�����
    UINT threadsQuantity = __threadsQuantity - 1;
    //��ֹ�����̴߳�������������
    if (threadsQuantity > MAX_THREAD_NUMBER)
        threadsQuantity = MAX_THREAD_NUMBER;
    return threadsQuantity;
}