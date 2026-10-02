/* DOC: https://learn.microsoft.com/en-us/windows/win32/wes/subscribing-to-events */
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <winevt.h>
#include <string>
#include <vector>

#pragma comment(lib, "wevtapi.lib")

DWORD WINAPI SubscriptionCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID pContext, EVT_HANDLE hEvent);
DWORD PrintEvent(EVT_HANDLE hEvent);


int wmain(int argc, wchar_t* argv[]) {
    if (argc < 3) {
        wprintf(L"Usage: EventQuery.exe <channel> <query>\n");
        return -1;
    }

    std::wstring channel = argv[1]; // examples: L"System" or L"Application"
    std::wstring query = argv[2];   // examples: L"*/System[(Level>=1) and (Level<=2)]" or L"*/System/Provider[@Name='MyMoviePlayer']"

    DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hSubscription = NULL;

    // Subscribe to events beginning with the oldest event in the channel. The subscription
    // will return all current events in the channel and any future events that are raised
    // while the application is active.
    hSubscription = EvtSubscribe(NULL, NULL, channel.c_str(), query.c_str(), NULL, NULL,
        (EVT_SUBSCRIBE_CALLBACK)SubscriptionCallback, EvtSubscribeStartAtOldestRecord);
    if (NULL == hSubscription)
    {
        status = GetLastError();

        if (ERROR_EVT_CHANNEL_NOT_FOUND == status)
            wprintf(L"Channel %s was not found.\n", channel.c_str());
        else if (ERROR_EVT_INVALID_QUERY == status)
            // You can call EvtGetExtendedStatus to get information as to why the query is not valid.
            wprintf(L"The query \"%s\" is not valid.\n", query.c_str());
        else
            wprintf(L"EvtSubscribe failed with %lu.\n", status);

        goto cleanup;
    }

    wprintf(L"Hit any key to quit\n\n");
    while (!_kbhit())
        Sleep(10);

cleanup:
    if (hSubscription)
        EvtClose(hSubscription);
}


// The callback that receives the events that match the query criteria. 
DWORD WINAPI SubscriptionCallback(EVT_SUBSCRIBE_NOTIFY_ACTION action, PVOID /*pContext*/, EVT_HANDLE hEvent) {
    DWORD status = ERROR_SUCCESS;

    switch (action) {
        // You should only get the EvtSubscribeActionError action if your subscription flags 
        // includes EvtSubscribeStrict and the channel contains missing event records.
    case EvtSubscribeActionError:
        if (ERROR_EVT_QUERY_RESULT_STALE == (size_t)hEvent) {
            wprintf(L"The subscription callback was notified that event records are missing.\n");
            // Handle if this is an issue for your application.
        } else {
            wprintf(L"The subscription callback received the following Win32 error: %llu\n", (size_t)hEvent);
        }
        break;

    case EvtSubscribeActionDeliver:
        if (ERROR_SUCCESS != (status = PrintEvent(hEvent))) {
            goto cleanup;
        }
        break;

    default:
        wprintf(L"SubscriptionCallback: Unknown action.\n");
    }

cleanup:
    if (ERROR_SUCCESS != status) {
        // End subscription - Use some kind of IPC mechanism to signal
        // your application to close the subscription handle.
    }

    return status; // The service ignores the returned status.
}


// Render the event as an XML string and print it.
DWORD PrintEvent(EVT_HANDLE hEvent) {
    DWORD dwBufferUsed = 0;
    DWORD dwPropertyCount = 0;

    std::vector<wchar_t> buffer;
    if (!EvtRender(NULL, hEvent, EvtRenderEventXml, 0, nullptr, &dwBufferUsed, &dwPropertyCount)) {
        DWORD status = GetLastError();
        if (status == ERROR_INSUFFICIENT_BUFFER) {
            buffer.resize(dwBufferUsed /sizeof(wchar_t));
            EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferUsed, buffer.data(), &dwBufferUsed, &dwPropertyCount);
        }

        status = GetLastError();
        if (status != ERROR_SUCCESS) {
            wprintf(L"EvtRender failed with %d\n", status);
            return status;
        }
    }

    wprintf(L"%s\n\n", buffer.data());
    return ERROR_SUCCESS;
}
