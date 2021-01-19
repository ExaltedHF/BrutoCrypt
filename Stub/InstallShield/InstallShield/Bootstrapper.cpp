
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

#include "Bootstrapper.h"

 void LoadManagedProject(char* app, int amount) // DllExport
{
    SAFEARRAYBOUND rgsabound[1];
    HRESULT hr; 
    _MethodInfoPtr pMethodInfo = NULL;
    VARIANT retVal;
    VARIANT obj;
    SAFEARRAY* psaStaticMethodArgs;
    VARIANT vtPsa; _AssemblyPtr spAssembly = NULL;   _AppDomainPtr appDomain;

    // Secure a handle to the CLR v4.0
    ICorRuntimeHost* pClr = StartCLR(L"v4.0.30319"); // empty you can start without specifying version (latest maybe?) L""
    if (pClr != NULL) // Let's start without specifying entrypoints 
    {
     
        appDomain = GetDefaultAppDomain(pClr);
       
       // SIZE_T sizeApp = sizeof(app);

        SAFEARRAYBOUND bounds[1];
        bounds[0].cElements = amount;
        bounds[0].lLbound = 0;

        SAFEARRAY* arr = SafeArrayCreate(VT_UI1, 1, bounds); // VT_I4 VT_UI1
        SafeArrayLock(arr);
        memcpy(arr->pvData, app, amount);
        SafeArrayUnlock(arr);

        hr = appDomain->Load_3(arr, &spAssembly);
        if (FAILED(hr)) {
            wprintf(L"Failed to load app");
        }
        hr = spAssembly->get_EntryPoint(&pMethodInfo);
        if (FAILED(hr)) {
            wprintf(L"Failed to get entrypoint");
        }
       // VARIANT retVal =  VARIANT();
      //  hr = pMethodInfo->Invoke_2(0, static_cast<BindingFlags>(BindingFlags_Public | BindingFlags_Static | BindingFlags_InvokeMethod), NULL, NULL, &retVal);

        VARIANT ret;
        VARIANT obj;
        SAFEARRAY* args = SafeArrayCreateVector(VT_VARIANT, 0, 1);

        ZeroMemory(&ret, sizeof(VARIANT));
        ZeroMemory(&obj, sizeof(VARIANT));
        obj.vt = VT_NULL;

        hr = pMethodInfo->Invoke_3(obj, args, &ret);

        //ZeroMemory(&retVal, sizeof(VARIANT));
        //ZeroMemory(&obj, sizeof(VARIANT));

        //obj.vt = VT_NULL;
        //vtPsa.vt = (VT_ARRAY | VT_BSTR);

        //psaStaticMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 0);
        //hr = pMethodInfo->Invoke_3(obj, psaStaticMethodArgs, &retVal);
    }


}
_AppDomainPtr GetDefaultAppDomain(ICorRuntimeHost* runtime) {

    HRESULT hr;
    IUnknownPtr spAppDomainThunk = NULL;
    _AppDomainPtr spDefaultAppDomain = NULL;

    hr = runtime->GetDefaultDomain(&spAppDomainThunk);
    if (FAILED(hr))
    {
        wprintf(L"RuntimeClrHost::GetCurrentAppDomainId failed w/hr 0x%08lx\n", hr);
    }
    hr = spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
    if (FAILED(hr))
    {
        wprintf(L"ICorRuntimeHost::GetDefaultDomain failed w/hr 0x%08lx\n", hr);
    }

    return spDefaultAppDomain;
}

ICorRuntimeHost* StartCLR(LPCWSTR dotNetVersion)
{
    HRESULT hr;

    ICLRMetaHost* pClrMetaHost = NULL;
    ICLRRuntimeInfo* pClrRuntimeInfo = NULL;
    ICorRuntimeHost* pClrRuntimeHost = NULL;

    hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&pClrMetaHost);
    if (hr == S_OK)
    {
        hr = pClrMetaHost->GetRuntime(dotNetVersion, IID_PPV_ARGS(&pClrRuntimeInfo));
        if (hr == S_OK)
        {
            BOOL fLoadable;
            hr = pClrRuntimeInfo->IsLoadable(&fLoadable);
            if ((hr == S_OK) && fLoadable)
            {
                hr = pClrRuntimeInfo->GetInterface(CLSID_CorRuntimeHost,
                    IID_PPV_ARGS(&pClrRuntimeHost));
                if (hr == S_OK)
                {
                    pClrRuntimeHost->Start();
                    return pClrRuntimeHost;
                }
                else {
                    wprintf(L"Failed to get cor runtime host");
                }
            }
            else {
                wprintf(L"Not loadable");
            }
        }
        else {
            wprintf(L"Failed to get runtime");
        }
    }
    else {
        wprintf(L"Failed to create clr instance");
    }

    if (pClrRuntimeHost)
    {
        pClrRuntimeHost->Release();
        pClrRuntimeHost = NULL;
    }
    if (pClrRuntimeInfo)
    {
        pClrRuntimeInfo->Release();
        pClrRuntimeInfo = NULL;
    }
    if (pClrMetaHost)
    {
        pClrMetaHost->Release();
        pClrMetaHost = NULL;
    }

    return NULL;
}
