#include "assert.h"
#include "logger.h"
#include "core/ftl/source_location.h"
#include "core/platform/os_windows.h"

namespace
{
    static thread_local HHOOK s_AssertMsgBoxHook = nullptr;

    LRESULT CALLBACK CBTProc(int code, WPARAM wparam, LPARAM lparam)
    {
        if (code < 0)
            return CallNextHookEx(s_AssertMsgBoxHook, code, wparam, lparam);

        switch (code)
        {
        case HCBT_ACTIVATE:
            // Get handle to the message box!
            HWND hwnd = (HWND)wparam;

            // Do customization!
            return 0;
        }

        // Call the next hook, if there is one
        return CallNextHookEx(s_AssertMsgBoxHook, code, wparam, lparam);
    }
}

namespace flavo::logger::internal
{
	void HandleAssert(ftl::string_view msg, ftl::source_location src_location)
	{
        // Always log as Fatal
        const ftl::string full_msg = ftl::format("File:\n{}({})\n\nFunction:\n{}\n\nMessage:\n{}", src_location.file_name(), src_location.line(), src_location.function_name(), msg);
        logger::Fatal("{}", full_msg);

        // Install a window hook, so we can intercept the message-box creation, and customize it
        s_AssertMsgBoxHook = SetWindowsHookEx(
            WH_CBT,
            CBTProc,
            NULL,
            GetCurrentThreadId() // Only install for THIS thread!!!
        );

        // Display a standard message box
        // 3 - abort
        // 4 - retry
        // 5 - ignore
        const int retval = MessageBox(nullptr, full_msg.data(), "Assertion failed", MB_ABORTRETRYIGNORE | MB_ICONERROR);

        // Remove the window hook
        UnhookWindowsHookEx(s_AssertMsgBoxHook);

        // Abort if error is not ignored
        if (retval == 3)
        {
            logger::Fatal("Chosen action: Abort");
            abort();
        }
        else if (retval == 4)
        {
            logger::Fatal("Chosen action: Debug (Retry)");
            __debugbreak();
        }
        else if (retval == 5)
        {
            logger::Fatal("Chosen action: Ignore");
        }
        else
        {
            logger::Fatal("Chosen action: Not supported");
            __debugbreak();
        }
	}
}
