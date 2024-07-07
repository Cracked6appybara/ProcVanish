
# Project: ProcVanish

![ProcVanish](https://64.media.tumblr.com/d30fe069cc48e11eeb31ae08293a159e/tumblr_nbtdxg9d6n1szf0nzo1_250.gif)


### Description
This project will be a ring three rootkit by using a DLL and MinHook to hook `NTAPI` functions such as, ` NtQuerySystemInformation`, `NtQueryDirectoryFile`, `NtQueryDirectoryFileEx`, `NtEnumerateKey`, and `NtEnumerateValue` from process that the DLL is injected into. I want to impliment similar techniques that [@bytecode77](https://github.com/bytecode77) used for their r77 rootkit project, I think it will be really good to learn some new techniques and add on some features to make ProcVanish an elite rootkit.

I want to make this project as FUD as possible whilst having up on github. But to do this I will use `indirect syscalls` at any place I can and I will also use `Api Hashing`. Since I am using Visual Studio for this project I will change the compiler settings to be suitable for the malware by not having unecessary functions that could raise potential flags from any AVs. I will do this with `CRT Library Removal`. But when this is implimented it will mean that there will look like there are no other functions being called in the binary because of the `Api Hashing`. So I will use `IAT Camouflage` to make the binary look as benign as possible.

## Features

- [ ]  Hide Directories and Files
- [ ]  Hide Registry Keys
- [x]  Hide Processes From Task Manager, Proc Hacker etc.
- [ ]  Indirect Syscalls
- [x]  Api Hashing
- [ ]  IAT Camouflage & CRT Library Removal
- [ ]  Use the registry to store the Dll.
 
### Features+
- [ ]  Use Windows Services to inject all processes and child processes running on the system
- [ ]  Use a Task Schedular for persistence.



## Authors

- [@bytecode77](https://github.com/bytecode77)
Huge inspiration from [@bytecode77](https://github.com/bytecode77) for his project r77 rootkit.


## Screenshots

![App Screenshot](https://via.placeholder.com/468x300?text=App+Screenshot+Here)

