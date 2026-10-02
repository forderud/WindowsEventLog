#include <cassert>
#include <iostream>
#include "EventLogger.hpp"
#include "../MyLegacyEventProvider/MyLegacyEventProvider.h" // custom event provider


int wmain() {
    // open event provider
    EventLogger log(L"MyLegacyEventProvider"); // or L"Application" or L"System"

    // generate log entries defined by the provider
    {
        WORD type = EVENTLOG_ERROR_TYPE; //  or other EVENTLOG_xxx type
        WORD category = UI_CATEGORY; // source-specific category
        DWORD eventId = MSG_INVALID_COMMAND; // entry in the message file associated with the event source
        const BYTE data[] = "Extra command details";

        wprintf(L"Writing log entry...\n");
        log.ReportUserData(type, category, eventId, sizeof(data), data);
    }
    {
        WORD type = EVENTLOG_ERROR_TYPE; //  or other EVENTLOG_xxx type
        WORD category = DATABASE_CATEGORY; // source-specific category
        DWORD eventId = MSG_BAD_FILE_CONTENTS; // entry in the message file associated with the event source
        const wchar_t* messages[] = { L"somefile.txt" };

        wprintf(L"Writing log entry...\n");
        log.ReportInsertStrings(type, category, eventId, std::size(messages), messages);
    }
    {
        WORD type = EVENTLOG_WARNING_TYPE; // or other EVENTLOG_xxx type
        WORD category = NETWORK_CATEGORY; // source-specific category
        DWORD eventId = MSG_RETRIES; // entry in the message file associated with the event source
        const wchar_t* messages[] = { L"25", L"zero" };

        wprintf(L"Writing log entry...\n");
        log.ReportInsertStrings(type, category, eventId, std::size(messages), messages);
    }
    {
        WORD type = EVENTLOG_INFORMATION_TYPE; //  or other EVENTLOG_xxx type
        WORD category = UI_CATEGORY; // source-specific category
        DWORD eventId = MSG_COMPUTE_CONVERSION; // entry in the message file associated with the event source
        const wchar_t* messages[] = { L"8", L"2" };

        wprintf(L"Writing log entry...\n");
        log.ReportInsertStrings(type, category, eventId, std::size(messages), messages);
    }

    wprintf(L"[done]\n");
    return 0;
}
