#pragma once
#include <Windows.h>
#include <comdef.h>
#include <string>
#include <stdexcept>


/** C++ RAII wrapper for legacy "Event Logging". */
class EventLogger {
public:
    EventLogger(const wchar_t* provider) {
        m_log = RegisterEventSourceW(NULL, provider);
        if (!m_log) {
            _com_error err(GetLastError());
            wprintf(L"ERROR: RegisterEventSourceW failed (%s)\n", err.ErrorMessage());
            throw std::runtime_error("RegisterEventSourceW failed");
        }
    }

    EventLogger(EventLogger&& obj) {
        std::swap(m_log, obj.m_log);
    }

    ~EventLogger() {
        DeregisterEventSource(m_log);
        m_log = 0;
    }

    /** Write log entry with insertion strings. */
    void ReportInsertStrings(WORD type, WORD category, DWORD eventId, WORD stringCount, const wchar_t* strings[]) {
        BOOL ok = ReportEventW(m_log, type, category, eventId, NULL, stringCount, /*raw data bytes*/0, strings, /*raw data*/NULL);
        if (!ok) {
            _com_error err(GetLastError());
            wprintf(L"ERROR: ReportEventW failed (%s)\n", err.ErrorMessage());
            throw std::runtime_error("ReportEventW failed");
        }
    }

    /** Write log entry with extra user-defined binary data. */
    void ReportUserData(WORD type, WORD category, DWORD eventId, DWORD rawdataSize, const BYTE* rawdata) {
        BOOL ok = ReportEventW(m_log, type, category, eventId, NULL, /*strCount*/0, rawdataSize, /*strings*/NULL, (void*)rawdata);
        if (!ok) {
            _com_error err(GetLastError());
            wprintf(L"ERROR: ReportEventW failed (%s)\n", err.ErrorMessage());
            throw std::runtime_error("ReportEventW failed");
        }
    }

private:
    HANDLE m_log = 0;
};
