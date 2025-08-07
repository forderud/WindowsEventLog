/* DOC: https://learn.microsoft.com/en-us/windows/win32/wes/consuming-events */
#include "Channel.hpp"
#include "Event.hpp"
#include "Provider.hpp"


int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        wprintf(L"Usage:\n");
        wprintf(L"  XPath query: EventQuery.exe qe <channel> <query>\n");
        wprintf(L"  List all channels: EventQuery.exe channels\n");
        wprintf(L"  List all providers: EventQuery.exe providers\n");
        wprintf(L"Examples:\n");
        wprintf(L"  List all MyMoviePlayer usage events: EventQuery.exe qe MyMoviePlayer/Operational  */System/Provider[@Name='MyMoviePlayer']\n");
        return 1;
    }

    std::wstring mode = argv[1];

    if (mode == L"channels") {
        wprintf(L"List of Channels:\n");
        std::vector<std::wstring> channels = EnumerateChannels();
        for (const std::wstring& channel : channels) {
            wprintf(L"  %s\n", channel.c_str());
#if 0
            wprintf(L"\n");
            PrintChannelProperties(channel);
            wprintf(L"\n");
#endif
        }
    } else if (mode == L"providers") {
        wprintf(L"List of providers:\n");
        std::vector<std::wstring> providers = EnumerateProviders();
        for (const std::wstring& provider : providers)
            wprintf(L"  %s\n", provider.c_str());

    } else if (mode == L"qe") {
        if (argc < 4) {
            wprintf(L"ERROR: Channel and query arguments missing.\n");
            return -1;
        }

        // perform event query
        std::wstring channel = argv[2]; // examples: L"System", L"Application", L"Microsoft-Windows-Kernel-PnP/Configuration"
        std::wstring query = argv[3];   // examples: L"*/System[(Level>=1) and (Level<=2)]", L"*/System/Provider[@Name='MyMoviePlayer']"
        EventQuery(channel, query, 10);
    } else {
        wprintf(L"ERROR: Incorrect arguments.\n");
        return -1;
    }

    return 0;
}
