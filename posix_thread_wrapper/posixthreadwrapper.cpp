#include "posixthreadwrapper.h"
PosixThreadWrapper::PosixThreadWrapper(RUN_FUNC_POINTER RunFuncPointer):runFuncPointer(RunFuncPointer)
{

}

void PosixThreadWrapper::start()
{
    int retVal=0;
    retVal=pthread_create(&threadID,NULL,startRoutine,(void*)this);
}

void PosixThreadWrapper::join()
{
    pthread_join(threadID,NULL);
}
void *PosixThreadWrapper::startRoutine(void *thiz)
{
    PosixThreadWrapper* tmpThiz=(PosixThreadWrapper*)thiz;
    if(tmpThiz->runFuncPointer!=NULL)
        (*(tmpThiz->runFuncPointer))();
    else
        tmpThiz->run();
    pthread_exit(NULL);
}

void PosixThreadWrapper::run()
{
}

PosixThreadWrapper::~PosixThreadWrapper()
{
    join();
}
