Sample projects for logging on Windows, including:
* `MyEventProvider`: Project for defining logging schemas.
* `MyEventLogger`: C++ project for logging through the provider.
* `MyEventLoggerCs`: C# project for logging through the provider.
* `EventQuery`: C++ project for querying Windows event logs.
* `EventSubscribe`: C++ project for listening to Windows event logs.

## wevtutil tool
[wevtutil](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/wevtutil) can be used to query the Windows Event log from the command-line.

Example queries:
* Last 5 critical or error events from the "System" log: `wevtutil qe "System" /q:"*[System/Level<=2]" /c:5 /rd:true /f:text`
* All events with warning or higher severity from "Application" log the last 24 hours: `wevtutil qe "Application" /q:"*[System[(Level>=1) and (Level<=3) and TimeCreated[timediff(@SystemTime) <= 86400000]]]" /f:text`

`/rd:true` means events are displayed from newest to oldest, and `/f:text` displays them as text instead of XML. 86400000 is the number of milliseconds in 24 hours.

Event severity level values: Critical (`System/Level=1`), Error (`System/Level=2`), Warning (`System/Level=3`), Information (`System/Level=4`).

NOTICE: `System/Level=0` information events have been observed in the "Application" log.

## Example of log channel properties
Logging destination folders are referred to as "channels". Each channel can be configured with maximum size and retention policy settings:  
![image](https://github.com/user-attachments/assets/85c440c0-143b-4d2b-803a-95ed9dcf971b)


## Examples of logged events
Some examples of system problems automatically logged by Windows. All of these events can also be accessed through the Windows Event Log APIs.

### Disk failures
Disk and filesystem problems are automaticaly logged:  
![image](https://github.com/user-attachments/assets/b148863a-3d67-4b81-a64f-4bb66384e136)  
![image](https://github.com/user-attachments/assets/8011024f-be1d-4dd7-8a46-c9b9023a25ec)

Query to retrieve the last 5 disk events:
`wevtutil qe "System" /q:"*/System/Provider[@Name='disk']" /c:5 /rd:true /f:text`

### HW device events
HW device failures are automatically logged:  
![image](https://github.com/user-attachments/assets/723867a3-6bb5-4405-8719-967240e72781)  
![image](https://github.com/user-attachments/assets/9a095ccd-cccd-4e7a-84af-93c47936adb9)


Query to retrieve the last 5 HW events:
`wevtutil qe "Microsoft-Windows-Kernel-PnP/Configuration" /q:"*/System/Provider[@Name='Microsoft-Windows-Kernel-PnP']" /c:5 /rd:true /f:text`

### GPU problems
GPU watchdog recovery:  
![image](https://github.com/user-attachments/assets/ef34e983-696f-4397-9d4c-0d1f1ec73d2c)

High GPU resource usage:  
![image](https://github.com/user-attachments/assets/ea8aaa33-fb50-4640-8506-5c7f01570471)

### Firewall logs
Logging of dropped packets by the firewall: (logging _not_ enabled by default)  
![image](https://github.com/user-attachments/assets/4c01cd9f-2a6b-4963-9d30-c2a2b776a5c7)

### Application whitelising (WDAC) problems
Applications not starting due to insufficient whitelisting:  
![image](https://github.com/user-attachments/assets/c034b43e-fb26-470a-903b-e1c37dc1fef9)

### Background service problems
![image](https://github.com/user-attachments/assets/75f05ab9-e45a-494f-9471-5d9f3a775eb0)

### Application hang or crash
![image](https://github.com/user-attachments/assets/4f5a6796-943e-4948-be40-1f8c4ff30af4)

### Critical OS failure
Reason for unexpected system reboots:  
![image](https://github.com/user-attachments/assets/0164914d-c894-4347-904b-bfa60e65bda7)


### SW installation results
Installation failure:  
![image](https://github.com/user-attachments/assets/5f780f94-44a5-4e63-b27b-bff437054709)  

Installation success:  
![image](https://github.com/user-attachments/assets/275f6497-32fa-44e4-a4b8-dafa4816c468)  

Need to restart afterwards:  
![image](https://github.com/user-attachments/assets/f20751a9-7e54-47c6-9978-6f70d2db4010)  


Query to retrieve the last 5 SW installation events:
`wevtutil qe "Application" /q:"*/System/Provider[@Name='MsiInstaller']" /c:5 /rd:true /f:text`

Event ID code doc: [Windows Installer Event Logging](https://learn.microsoft.com/en-us/windows/win32/msi/event-logging)

## Documentation
* ~~[Event Logging](https://learn.microsoft.com/en-us/windows/win32/eventlog/event-logging)~~ (old APIs)
* [Windows Event Log](https://learn.microsoft.com/en-us/windows/win32/wes/windows-event-log) (new APIs)
* Windows [Event type schema](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-even6/8c61aef7-bd4b-4edb-8dfd-3c9a7537886b)
