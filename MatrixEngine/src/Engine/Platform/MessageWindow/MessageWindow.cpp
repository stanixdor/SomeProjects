/*****************************************************************/
/*Author: Joaquin Bilbao                                         */
/*Brief description: This contains the implementation for the    */
/*wrappers I created for the MessageBoxA function provided by    */
/*windows                                                        */
/*****************************************************************/
#include <iostream>
#include "Platform\Time\FrameRateController.h"
#include "Platform/MessageWindow/MessageWindow.h"
#include "Window\WindowManager.h"
#include "Window\Window.h"
//#define BEANNOYING


namespace MessageWindow
{
    /*!
    *****************************************************************************
    \fn
        int sendMessage(LPCSTR text, LPCSTR windowName, UINT options, HWND windowOwner)

    \brief
        wrapper for the windows function used to create message boxes
    \param
        LPCSTR text: text of the window
        LPCSTR windowName: name of the window
        UINT options: flags for the window
        HWND windowOwner: owner of the window (null by default)
    \return
        the return value that windows function MessageBoxA returns
    *******************************************************************************/ 
    int sendMessage(LPCSTR text, LPCSTR windowName, UINT options, HWND windowOwner)
    {
        return MessageBoxA(windowOwner, text, windowName, options);
    }

    /*!
    *****************************************************************************
    \fn
    void errorMessage(const std::string& WindowTitle, const std::string& ErrorText)

    \brief
        wrapper the wrapper sendMessage done, since we are mainly going to have 
    warning boxes and error boxes, I predefined those two boxes.

    Its worth noting that if the user accepts the error message too fast another identical
    window will pop up. This was done to prevent users from not reading the errors.

    \param
    const std::string& WindowTitle: name of the window
    const std::string& ErrorText: error message

    \return
    none, void function.
    *******************************************************************************/
    int errorMessage(const std::string& WindowTitle, const std::string& ErrorText)
    {
        int returnValue;

        //average reading speed = 20characters/s
        FRC::Timer windowLifeTime;
        returnValue = sendMessage(ErrorText.c_str(), WindowTitle.c_str(), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL | MB_TOPMOST);
         
        #ifdef BEANNOYING
        //if the user closes the window really fast (AKA he didn't read it) the same message will pop again.
        f32 timeSpent = windowLifeTime.GetTimeSinceStart();
        f32 timeThatShouldBeSpent = static_cast<f32>(ErrorText.size() / 20);
        //we dont want to hold people for too long
        if (timeThatShouldBeSpent > 5.f)
            timeThatShouldBeSpent = 5.f;

        if (timeSpent < timeThatShouldBeSpent)
            errorMessage(WindowTitle, ErrorText);
        #endif

        return returnValue;
    }

    /*!
    *****************************************************************************
    \fn
    void warningMessage(const std::string& WindowTitle, const std::string& WarningText)


    \brief
    wrapper the wrapper sendMessage, since we are mainly going to have
    warning boxes and error boxes, I predefined those two boxes.

    Its worth noting that if the user accepts the error message too fast another identical
    window will pop up. This was done to prevent users from not reading the errors.

    \param
    const std::string& WindowTitle: name of the window
    const std::string& WarningText: warning message

    \return
    none, void function.
    *******************************************************************************/
    int warningMessage(const std::string& WindowTitle, const std::string& WarningText)
    {
        int returnValue;
        //average reading speed = 20characters/s
        FRC::Timer windowLifeTime;
        returnValue = sendMessage(WarningText.c_str(), WindowTitle.c_str(), MB_YESNO | MB_ICONWARNING | MB_TASKMODAL | MB_TOPMOST, Window::WindowManager::Instance()->GetWindow()->getHandler());

        #ifdef BEANNOYING
        //if the user closes the window really fast (AKA he didn't read it) the same message will pop again.
        f32 timeSpent = windowLifeTime.GetTimeSinceStart();
        f32 timeThatShouldBeSpent = static_cast<f32>(WarningText.size() / 20);
        //we dont want to hold people for too long
        if (timeThatShouldBeSpent > 5.f)
            timeThatShouldBeSpent = 5.f;

        if (timeSpent < timeThatShouldBeSpent)
            warningMessage(WindowTitle, WarningText);
        #endif
        return returnValue;
    }

    /*!
    *****************************************************************************
    \fn
    void customMessage(const std::string& WindowTitle, const std::string& WindowText, unsigned MessageButtonsFlag, unsigned MessageIconFlag, unsigned MessageModalityFlag)

    \brief
    wrapper the wrapper sendMessage. this should be used when neither errorMessage or warningMessage would be adequate calls.
    Its worth noting that if the user accepts the error message too fast another identical
    window will pop up. This was done to prevent users from not reading the errors.

    For aditional info check: https://msdn.microsoft.com/en-us/library/windows/desktop/ms645505(v=vs.85).aspx
    
    \param
    const std::string& WindowTitle: name of the window
    const std::string& WindowText: text that will appear on the window
    unsigned MessageButtonsFlag: flag for the button that we will want to implement
    MessageIconFlag: flag for the icon the message box will have
    MessageModalityFlag: Type of modality of the window, its defaulted to MB_SYSTEMMODAL.

    \return
    none, void function.
    *******************************************************************************/
    int customMessage(const std::string& WindowTitle, const std::string& WindowText, unsigned MessageButtonsFlag, unsigned MessageIconFlag, unsigned MessageModalityFlag)
    {

        int returnValue;
        //average reading speed = 20characters/s
        FRC::Timer windowLifeTime;
        returnValue = sendMessage(WindowText.c_str(), WindowTitle.c_str(), MessageButtonsFlag | MessageIconFlag | MessageModalityFlag);

        #ifdef BEANNOYING
        //if the user closes the window really fast (AKA he didn't read it) the same message will pop again.
        f32 timeSpent = windowLifeTime.GetTimeSinceStart();
        f32 timeThatShouldBeSpent = static_cast<f32>(WindowTitle.size() / 20);
        //we dont want to hold people for too long
        if (timeThatShouldBeSpent > 5.f)
            timeThatShouldBeSpent = 5.f;

        if (timeSpent < timeThatShouldBeSpent)
            customMessage(WindowTitle, WindowText, MessageButtonsFlag, MessageIconFlag, MessageModalityFlag);
        #endif
        return returnValue;
    }

    /*!
    *****************************************************************************
    \fn
    void resolveMessage(int result)

    \brief
        This function is DEPRECATED, I leave it here in case we need it on the future.
    \param
        int result: returned value from sendMessage()
    \return
        none
    *******************************************************************************/ 
    void resolveMessage(int result)
    {
        if (result == IDYES)
        {
            std::cout << "selected yes" << std::endl;

        }
        else if (result == IDNO)
        {
            std::cout << "selected no" << std::endl;
        }
    }

}
