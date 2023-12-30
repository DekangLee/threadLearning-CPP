#include "threadPool.hpp"
int main()
{
    threadPool *pool = threadPoolCreate(20,20,5);
    threadPoolDestroy(pool);
}