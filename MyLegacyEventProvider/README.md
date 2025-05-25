Custom Windows event provider. Used to enable custom log entry types throgh the legacy [Event Logging](https://learn.microsoft.com/en-us/windows/win32/eventlog/event-logging) API.

**WARNING**: Not recommended for new projects.

### Example log entries
![image](https://github.com/user-attachments/assets/e3b68fd8-c165-45ed-9367-d352dfbd5827)

### Installation
The event provider DLL needs to be registered under `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\Application\MyLegacyEventProvider` in the Windows registry to enable the Event Viewer app and APIs to parse log entries.

#### How to install
* Build project.
* Run `regsvr32.exe MyLegacyEventProvider.dll` from an admin command prompt to register the provider.

#### How to uninstall
* Run `regsvr32.exe /u MyLegacyEventProvider.dll` from an admin command prompt to unregister the provider.
