Jan-Itor
========

![Demo](demo.mp4)

Description
===========
- Terminal app using FTXUI to preprocess single word responses
- Cleans and serializes single word responses
- Response fields may be of varying formats.
- Jan-Itor may be used to obtain one or more of the following:
   - The serial position (relative to a list of target words) of sub-items in a column of responses.
   - A format-standardized version of responses.
   - A format-standardized & corrected version of responses.
   - Unique words present in responses.

Building
========
- Runs on both windows and Unix based systems.
- Requires no external dependencies to be installed. The only things needed to build are:
   - C++ build tools.
   - Git (to download the repository)
   - CMake
   - Make (if on Unix) / Ninja(if on Windows).
   - FTXUI (automatically downloaded by CMake).

Linux instructions
------------------
```bash
git clone https://github.com/unsigned-enby/Jan-Itor.git
cd ./Jan-Itor
mkdir build && cd ./build
cmake ..
make -j$(nproc)
```

Windows
-------
   - While Jan-Itor can be natively built on Windows, you may find it preferable 
     to use 'Windows Subsytem for Linux'. Your welcome to use either.
     While WSL will take (slightly) longer, and use (a moderate amount of) more disk space,
     it will leave you with a virtual Linux system installed (that you may even use to run GUI apps!)
     I will give instructions for both.

WSL:
----
   - First open up a Windows Terminal and type `wsl --install` and accept the requested permissions.
   - After doing this, you will need to restart your computer.
   - After rebooting, open the Terminal once again and type `wsl --install` 
     (you will not need to reboot this time, this will just install the Ubuntu-Linux system itself).
   - After it finishes installing, simply run WSL with the `wsl` command; 
     you should now be inside your Linux (sub-)system
   - Run these commands:
```bash
sudo apt update
sudo apt upgrade # Not strictly required but ultimately recomended
sudo apt install build-essentials cmake git
```
   - Then simply follow the instructions for Linux!

Building natively on Windows:
-----------------------------
   - First open a Windows Terminal and run these commands:
```ps1
winget install Microsoft.VisualStudio.2022.BuildTools --force --override "--wait --passive --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.VisualStudio.Component.Windows11SDK.22000"
winget install Git.Git Ninja-build.Ninja cmake
```
- close and reopen the terminal
- In the drop down menu located on the tab bar, click `Developer PowerShell for VS 2022`
```ps1
git clone https://github.com/unsigned-enby/Jan-Itor.git
cd .\Jan-Itor
mkdir build
cd .\build
cmake -G Ninja ..
ninja -C .
