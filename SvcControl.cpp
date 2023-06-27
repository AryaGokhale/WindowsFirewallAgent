#include "SvcControl.h"
#include <strsafe.h>
#include "query.h"
//#include "testrule.h"


Svc* Svc::s_service = NULL;


BOOL Svc::Run(Svc& service)
{
    s_service = &service;

    SERVICE_TABLE_ENTRY DispatchTable[] =
    {
        {LPWSTR(service.svcName), (LPSERVICE_MAIN_FUNCTIONW)ServiceMain},
        {NULL, NULL}

    };
    /*if (!StartServiceCtrlDispatcher(DispatchTable))
    {
        SvcReportEvent(TEXT("StartServiceCtrlDispatcher"));

    }*/
    
    return StartServiceCtrlDispatcher(DispatchTable);

}

VOID WINAPI Svc::SvcCtrlHandler(DWORD dwCtrl)
{
    // Handle the requested control code. 

    switch (dwCtrl)
    {
    case SERVICE_CONTROL_STOP:
        s_service->Stop();

    case SERVICE_CONTROL_PAUSE:
        s_service->Pause();
        break;

    case SERVICE_CONTROL_CONTINUE:
        s_service->Continue();

    case SERVICE_CONTROL_SHUTDOWN:
        s_service->Shutdown();

    default:
        break;
    }

}

void WINAPI Svc::ServiceMain(DWORD dwArgc, PCWSTR* lpszArgv)
{
    // Register the handler function for the service
    s_service->gSvcStatusHandle = RegisterServiceCtrlHandlerW(
        (LPCWSTR)s_service->svcName, (LPHANDLER_FUNCTION)SvcCtrlHandler);
    /*if (s_service->gSvcStatusHandle == 0)
    {
        throw GetLastError();
    }*/

    // Start the service
    s_service->Start(dwArgc, lpszArgv);
}

VOID Svc::ReportSvcStatus(DWORD dwCurrentState,
    DWORD dwWin32ExitCode,
    DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ((dwCurrentState == SERVICE_RUNNING) ||
        (dwCurrentState == SERVICE_STOPPED))
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

/*VOID Svc::SvcReportEvent(PCWSTR pszMessage, WORD wType, DWORD dwEventId, WORD wCategory)
{
    HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
    TCHAR Buffer[80];

    hEventSource = RegisterEventSource(NULL, s_service->svcName);

    if( NULL != hEventSource )
    {

        ReportEvent(hEventSource,        // event log handle
                    EVENTLOG_ERROR_TYPE, // event type
                    0,                   // event category
                    SVC_ERROR,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
}

StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());
*/

//--------------------------------service control commands------------------------------------------------//

void Svc::Start(DWORD dwArgc, PCWSTR* pszArgv)
{
    try
    {
        ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 0);
        onStart(dwArgc, pszArgv);
        ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
    }
    catch (DWORD dwError)
    {
        ReportSvcStatus(SERVICE_STOPPED, dwError, 0);
    }
    //ADD: catch block for event error
}

void Svc::onStart(DWORD dwArgc, PCWSTR* pszArgv)
{
    //runRule();

    BOOL result = RunQuery();
    if(result == true)
    {
        callHandler();
    }

}

//--------------------------------------------------------------------

void Svc::Pause()
{
    try
    {
        ReportSvcStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0);
        onPause();
        ReportSvcStatus(SERVICE_PAUSED, NO_ERROR, 0);
    }
    catch (DWORD dwError)
    {
        ReportSvcStatus(SERVICE_RUNNING, dwError, 0);
    }
    //ADD: catch block for event error
}

void Svc::onPause()
{

}

//--------------------------------------------------------------------

void Svc::Continue()
{
    try
    {
        ReportSvcStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);
        onContinue();
        ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
    }
    catch (DWORD dwError)
    {
        ReportSvcStatus(SERVICE_PAUSED, dwError, 0);
    }
}

void Svc::onContinue()
{

}

//--------------------------------------------------------------------

void Svc::Shutdown()
{
    try
    {
        ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        onStop();
        ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    }
    catch (DWORD dwError)
    {
        ReportSvcStatus(SERVICE_RUNNING, dwError, 0);

    }
}

void Svc::onShutdown()
{

}


void Svc::Stop()
{
    DWORD dwOriginalState = gSvcStatus.dwCurrentState;
    try
    {
        // Tell SCM that the service is stopping.
        ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

        // Perform service-specific stop operations.
        void OnStop();

        // Tell SCM that the service is stopped.
        ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    }
    catch (DWORD dwError)
    {

        ReportSvcStatus(dwOriginalState, dwError, 0);
    }
}

void Svc::onStop()
{

}

Svc::Svc(PCWSTR serviceName, BOOL canStop, BOOL canShutdown, BOOL canPauseContinue)
{
    //s_service->svcName = (serviceName == NULL) ? L"" : serviceName;
    /*if (serviceName != NULL)
    {
        svcName = serviceName;
    }*/

    svcName = serviceName;
    //s_service->svcName = serviceName;

    gSvcStatusHandle = NULL;

    // The service runs in its own process.
    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

    // The service is starting.
    gSvcStatus.dwCurrentState = SERVICE_START_PENDING;

    // The accepted commands of the service.
    DWORD dwControlsAccepted = 0;
    if (canStop)
        dwControlsAccepted |= SERVICE_ACCEPT_STOP;
    if (canShutdown)
        dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
    if (canPauseContinue)
        dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;
    gSvcStatus.dwControlsAccepted = dwControlsAccepted;

    gSvcStatus.dwWin32ExitCode = NO_ERROR;
    gSvcStatus.dwServiceSpecificExitCode = 0;
    gSvcStatus.dwCheckPoint = 0;
    gSvcStatus.dwWaitHint = 0;


}

Svc* Svc::getInstance(PCWSTR serviceName, BOOL canStop, BOOL canPauseContinue , BOOL canShutdown)
{
    s_service = new Svc(serviceName, canStop, canPauseContinue, canShutdown);
    return s_service;
}

void Svc::RunSvc()
{
    Run(*s_service);
    ServiceMain(0, NULL);
    

}

Svc::~Svc()
{
}
