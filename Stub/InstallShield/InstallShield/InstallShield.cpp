// InstallShield.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "Bootstrapper.h"
#include <fstream>

int main()
{
    TCHAR szFileName[MAX_PATH];
    GetModuleFileName(NULL, szFileName, MAX_PATH);

    HMODULE library = LoadLibraryEx(szFileName, NULL, LOAD_LIBRARY_AS_DATAFILE);
    HRSRC hResource = FindResource(library, L"TEXT", MAKEINTRESOURCE(RT_RCDATA));

    if (hResource)
    {
        HGLOBAL hLoadedResource = LoadResource(library, hResource);

        if (hLoadedResource)
        {
            LPVOID pLockedResource = LockResource(hLoadedResource);

            if (pLockedResource)
            {
                DWORD dwResourceSize = SizeofResource(library, hResource);

                if (0 != dwResourceSize)
                {
                    LoadManagedProject((char*)pLockedResource, (int)dwResourceSize);
                    // Use pLockedResource and dwResourceSize however you want
                }
            }
        }
    }
    

}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
