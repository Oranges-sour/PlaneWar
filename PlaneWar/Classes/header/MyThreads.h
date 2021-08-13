#ifndef __MY_THREADS_H__
#define __MY_THREADS_H__

#include <functional>
typedef unsigned int UINT;

/**
*针对于大vector更新的多线程拆分更新
*@param vecSize:vector的大小
*@param callFunc:更新函数
*@return 无
*@例子:

std::vector<int> vec;
callInThread(vec.size(),
[&](UINT from, UINT to, UINT threadNumber)
{
        for(int x = from; x < to; ++x)
        {
                vec[x] += 1;
        }
});
*/
void callInThread(
    const size_t vecSize,
    const std::function<void(UINT from, UINT to, UINT threadNumber)>& callFunc);

/**
 *获得callInThread()能使用的最大线程数量
 *@return 最大线程数量
 */
UINT getMaxThreadNumber();

#endif