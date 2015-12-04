#ifndef POSIXTHREADWRAPPER_H
#define POSIXTHREADWRAPPER_H
#include <pthread.h>
#include <unistd.h>
typedef void(*RUN_FUNC_POINTER)();

class PosixThreadWrapper
{
private:
    pthread_t threadID;
    RUN_FUNC_POINTER runFuncPointer;
    static void* startRoutine(void *thiz);
protected:
    virtual void run();
public:
    PosixThreadWrapper(RUN_FUNC_POINTER RunFuncPointer=0);
    void start();
    void join();
    virtual ~PosixThreadWrapper();
};

#endif // POSIXTHREADWRAPPER_H
