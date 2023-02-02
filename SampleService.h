//#include "SvcControl.h"
//
//
//class SampleService : public Svc
//{
//public:
//
//    SampleService(PCWSTR pszServiceName,
//        BOOL fCanStop = TRUE,
//        BOOL fCanShutdown = TRUE,
//        BOOL fCanPauseContinue = FALSE);
//    virtual ~SampleService(void);
//    void Run();
//    //void onInstall(DWORD dwArgc, PCWSTR* lpszArgv);
//
//protected:
//
//    virtual void onStart(DWORD dwArgc, PCWSTR* pszArgv);
//    virtual void onStop();
//
//    void ServiceWorkerThread(void);
//    
//
//private:
//
//    BOOL m_fStopping;
//    HANDLE m_hStoppedEvent;
//};

#include "SvcControl.h"


class SampleService
{
public:

    SampleService(PCWSTR pszServiceName,
        BOOL fCanStop = TRUE,
        BOOL fCanShutdown = TRUE,
        BOOL fCanPauseContinue = FALSE);
    virtual ~SampleService(void);
    void Run();
    //void onInstall(DWORD dwArgc, PCWSTR* lpszArgv);

protected:

    virtual void onStart(DWORD dwArgc, PCWSTR* pszArgv);
    virtual void onStop();

    void ServiceWorkerThread(void);

    Svc* s_service;

private:

    BOOL m_fStopping;
    HANDLE m_hStoppedEvent;
};