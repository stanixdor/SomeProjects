/*****************************************************************/
/*Author: Joaquin Bilbao                                         */
/*Brief description: This contains the declarations for the      */
/*wrappers I created for the MessageBoxA function provided by    */
/*windows                                                        */
/*****************************************************************/
#pragma once
#include <string>
#include <Windows.h>

namespace MessageWindow
{
    int sendMessage(LPCSTR text, LPCSTR windowName, UINT options, HWND windowOwner = NULL);

    int errorMessage(const std::string& WindowTitle, const std::string& ErrorText);
    int warningMessage(const std::string& WindowTitle, const std::string& WarningText);
    
    //Additional info: https://msdn.microsoft.com/en-us/library/windows/desktop/ms645505(v=vs.85).aspx
    int customMessage(const std::string& WindowTitle, const std::string& WindowText, unsigned MessageButtonsFlag, unsigned MessageIconFlag, unsigned MessageModalityFlag = MB_SYSTEMMODAL);
    void resolveMessage(int result);
}