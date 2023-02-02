//#pragma once
//#include <Windows.h>
//#include <memory>
//class Svc
//{
//	public:
//
//		Svc(PCWSTR serviceName, BOOL canStop = TRUE, BOOL canPauseContinue = TRUE, BOOL canShutdown = FALSE);
//	
//		//deleting copy constructor
//		Svc(const Svc& obj) = delete;
//
//		//get instance 
//		//static Svc* getInstance();
//
//
//		//Register services with SCM 
//		static BOOL Run(Svc &service);
//
//		//Virtual destructor (desived class object referenced with base class pointer error handled)
//		virtual ~Svc(void);
//
//		static void WINAPI ServiceMain(DWORD dwArgc, PCWSTR* lpszArgv);
//
//		//Stop service
//		void Stop();
//		void Start(DWORD dwArgc, PCWSTR* pszArgv);
//		void Pause();
//		void Continue();
//		void Shutdown();
//
//protected:
//
//	//static void WINAPI ServiceMain(DWORD dwArgc, PCWSTR* lpszArgv);
//	virtual void onStart(DWORD dwArgc, PCWSTR *pszArgv);
//	virtual void onStop();
//	virtual void onPause();
//	virtual void onContinue();
//	virtual void onShutdown();
//
//	/*void Start(DWORD dwArgc, PCWSTR* pszArgv);
//	void Pause();
//	void Continue();
//	void Shutdown();*/
//	//void SvcReportEvent(PCWSTR pszMessage, WORD wType, DWORD dwEventId, WORD wCategory);
//	
//	//Set service status 
//	VOID ReportSvcStatus(DWORD dwCurrentState,
//		DWORD dwWin32ExitCode,
//		DWORD dwWaitHint);
//
//	
//	static void SvcCtrlHandler(DWORD dwCtrl);
//	//Singleton service instance : static pointer that points to the instance of the class
//	static Svc* s_service;
//	PCWSTR  svcName;
//
//	SERVICE_STATUS          gSvcStatus;
//	SERVICE_STATUS_HANDLE   gSvcStatusHandle;
//	HANDLE                  ghSvcStopEvent = NULL;
//
//
//};


#pragma once
#include <Windows.h>
#include <memory>
#include "Handler.h"
class Svc
{
public:


	//Svc(PCWSTR serviceName, BOOL canStop = TRUE, BOOL canPauseContinue = TRUE, BOOL canShutdown = FALSE);

	//deleting copy constructor
	Svc(const Svc& obj) = delete;

	//not assignable
	void operator=(const Svc&) = delete;

	//get instance 
	static Svc* getInstance(PCWSTR serviceName, BOOL canStop = TRUE, BOOL canPauseContinue = TRUE, BOOL canShutdown = FALSE);


	//Register services with SCM 
	static BOOL Run(Svc& service);


	static void WINAPI ServiceMain(DWORD dwArgc, PCWSTR* lpszArgv);

	//Stop service
	void Stop();
	void Start(DWORD dwArgc, PCWSTR* pszArgv);
	void Pause();
	void Continue();
	void Shutdown();
	static void RunSvc();

private:
	//Singleton service instance : static pointer that points to the instance of the class
	static Svc* s_service;

protected:

	Svc(PCWSTR serviceName, BOOL canStop = TRUE, BOOL canPauseContinue = TRUE, BOOL canShutdown = FALSE);

	~Svc();

	//static void WINAPI ServiceMain(DWORD dwArgc, PCWSTR* lpszArgv);
	virtual void onStart(DWORD dwArgc, PCWSTR* pszArgv);
	virtual void onStop();
	virtual void onPause();
	virtual void onContinue();
	virtual void onShutdown();
	

	/*void Start(DWORD dwArgc, PCWSTR* pszArgv);
	void Pause();
	void Continue();
	void Shutdown();*/
	//void SvcReportEvent(PCWSTR pszMessage, WORD wType, DWORD dwEventId, WORD wCategory);

	//Set service status 
	VOID ReportSvcStatus(DWORD dwCurrentState,
		DWORD dwWin32ExitCode,
		DWORD dwWaitHint);


	static void SvcCtrlHandler(DWORD dwCtrl);
	
	PCWSTR  svcName;

	SERVICE_STATUS          gSvcStatus;
	SERVICE_STATUS_HANDLE   gSvcStatusHandle;
	HANDLE                  ghSvcStopEvent = NULL;


};