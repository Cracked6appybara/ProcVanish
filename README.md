# Project: ProcVanish

![ProcVanish](https://64.media.tumblr.com/d30fe069cc48e11eeb31ae08293a159e/tumblr_nbtdxg9d6n1szf0nzo1_250.gif)


### Description
This project will be a ring three rootkit by using a DLL and MinHook to hook `NTAPI` functions such as, ` NtQuerySystemInformation`, `NtQueryDirectoryFile`, `NtQueryDirectoryFileEx`, `NtEnumerateKey`, and `NtEnumerateValue` from process that the DLL is injected into. I want to impliment similar techniques that [@bytecode77](https://github.com/bytecode77) used for their r77 rootkit project, I think it will be really good to learn some new techniques and add on some features to make ProcVanish an elite rootkit.

I want to make this project as FUD as possible whilst having up on github. But to do this I will use `indirect syscalls` at any place I can and I will also use `Api Hashing`. Using both of these techniques will hide the functions used from the IAT and from using `Indirect Syscalls` it will hide the fact that syscalls are being called from inside the binary. 
## Features

- [ ]  Hide Directories and Files
- [ ]  Hide Registry Keys
- [x]  Hide Processes From Task Manager, Proc Hacker etc.
- [ ]  Indirect Syscalls
- [ ]  Api Hashing
- [ ]  IAT Camouflage
- [ ]  Use the registry to store the Dll.
 
### Features+
- [ ]  Use Windows Services to inject all processes and child processes running on the system
- [ ]  Use a Task Schedular for persistence.



## Authors

- [@bytecode77](https://github.com/bytecode77)
Huge inspiration from [@bytecode77](https://github.com/bytecode77) for his project r77 rootkit.

