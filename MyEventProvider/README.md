Manifest-based log provider sample


Based on [Writing Manifest-based Events](https://learn.microsoft.com/en-us/windows/win32/etw/writing-manifest-based-events).

### How to install
From an admin command prompt:
```
wevtutil.exe im MyEventProvider.man /rf:%DLL_PATH% /mf:%DLL_PATH%
```

### How to uninstall
From an admin command prompt:
```
wevtutil.exe um MyEventProvider.man
```


## If experiencing access errors
Observed problem:
```
**** Warning: Publisher MyEventProvider resources could not be found or are not accessible
to the EventLog service account (NT SERVICE\EventLog).
```

Solution: Grant `Users` access to `MyEventProvider.dll`.
