#pragma once
#include <windows.h>
#include <evntprov.h>
#include <stdexcept>


/** ETW event provider registration wrapper class. */
class EventHandle {
public:
    EventHandle(const GUID* providerGuid) {
        DWORD status = EventRegister(
            providerGuid, // GUID that identifies the provider
            NULL,         // Callback not used
            NULL,         // Context noot used
            &m_provider   // Used when calling EventWrite and EventUnregister
        );
        if (ERROR_SUCCESS != status) {
            wprintf(L"EventRegister failed with %lu\n", status);
            throw std::runtime_error("EventRegister failed");
        }
    }

    EventHandle(EventHandle&& obj) {
        std::swap(m_provider, obj.m_provider);
    }

    ~EventHandle() {
        EventUnregister(m_provider);
        m_provider = 0;
    }

    void Write(const EVENT_DESCRIPTOR* EventDescriptor, size_t UserDataCount, EVENT_DATA_DESCRIPTOR* UserData) {
        DWORD status = EventWrite(m_provider, EventDescriptor, (ULONG)UserDataCount, UserData);
        if (status != ERROR_SUCCESS) {
            wprintf(L"EventWrite failed with 0x%x", status);
            throw std::runtime_error("EventWrite failed");
        }
    }

private:
    REGHANDLE m_provider = 0;
};

/** WARNING: Does NOT copy the data. */
static EVENT_DATA_DESCRIPTOR EventData(const void* DataPtr, size_t DataSize) {
    EVENT_DATA_DESCRIPTOR desc{};
    EventDataDescCreate(&desc, DataPtr, (ULONG)DataSize);
    return desc;
}
