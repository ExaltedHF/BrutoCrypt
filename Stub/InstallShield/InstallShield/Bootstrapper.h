#pragma once
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

// For exporting functions without name-mangling
#define DllExport extern "C" __declspec( dllexport )

#import <mscorlib.tlb> raw_interfaces_only			\
    	high_property_prefixes("_get","_put","_putref")		\
    	rename("ReportEvent", "InteropServices_ReportEvent")	\
	rename("or", "InteropServices_or")
using namespace mscorlib;
#pragma endregion

// Our sole export for the time being
 void LoadManagedProject(char* app, int amount); //DllExport

// Not exporting, so go ahead and name-mangle
ICorRuntimeHost* StartCLR(LPCWSTR dotNetVersion);

_AppDomainPtr GetDefaultAppDomain(ICorRuntimeHost* host);