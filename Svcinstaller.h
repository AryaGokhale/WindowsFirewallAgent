//To avoid header files to be included multiple times
#pragma once
#include <memory>
void InstallService(
	PCWSTR serviceName, 
	PCWSTR displayName,
	DWORD startType, 
	PCWSTR dependencies,
	PCWSTR account,
	PCWSTR password
);


void UninstallService(PCWSTR serviceName);