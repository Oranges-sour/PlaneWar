#ifndef __MY_THREADS_H__
#define __MY_THREADS_H__

#include <functional>
typedef unsigned int UINT;

/**
*����ڴ�vector���µĶ��̲߳�ָ���
*@param vecSize:vector�Ĵ�С
*@param callFunc:���º���
*@return ��
*@����:

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
 *���callInThread()��ʹ�õ�����߳�����
 *@return ����߳�����
 */
UINT getMaxThreadNumber();

#endif