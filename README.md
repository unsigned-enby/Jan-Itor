Jan-Itor
========

![Demo](demo.mp4)

Description
===========
- Terminal app using FTXUI to preprocess single word responses
- Cleans and serializes single word responses
- Individual response fields may be of varying formats.
- Jan-Itor may be used to obtain one or more of the following:
   - The serial position of sub-items (relative to a list of target words) in a list of responses
   - A format-standardized version of responses
   - A format-standardized & corrected version of responses

Building
========
- Runs on both windows and unix based systems.
- Requires no external depencies to be installed. The only things needed to build are:
   - C++ build tools.
   - Git (to download the repository)
   - Cmake
   - Make (if on unix) / Ninja(if on Windows).
   - FTXUI (automatically downloaded by Cmake).

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
     you should now be inside your linux (sub-)system
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
```

Using
=====

CSV Menu
--------
   - Supply the given `Input File`, field `Delimiter`, and `Quote`. Then press `Open File`
   - The input file format is as follows:
      - A unique identifier (e.g PID) column
      - One or more response columns belonging to the associated respondents
      - Multiple response columns should not be an issue. But please note that I have not, at this time,
        throughly tested Jan-Itor with more than one response column.
      - Row delimiters are assumed to be either '\n'(unix format) or '\n\r'(Windows' format)
      - A header (i.e column names) are assumed to be present, but are not (at this time) used for anything.
   - The associated data is updated by `IpForm Menu` and the `Make corrections` button of 
     `Serial Positioning & Corrections`. Said data may be saved by supplying a `Output File` and 
     pressing `Save File`

IpForm Menu
-----------
   - `Black Space` & `Removles` may be given via individual characters or the `Ranges` check boxes
   - `Black Space`:
      - Definition:
         - What you want to keep as a single-sub item. Everthing that is not specified by `Black Space`
           or `Removles` is assumed to be a boundry (i.e white space) between two sub-items.
      - Example:
         - If `Black Space` is set to "a-z":
            - "this,that" will become "this" & "that"
         - If `Black Space` is *also* set to ',':
            - "this,that" will stay as "this,that"
   - `Removles`:
      - Definition:
         - What you want to remove from a sub item.
      - Example:
         - If `Removles` is set to "'" and `Black Space` is set to "a-z":
            - "it's an. example" becomes "its", "an", & "example"
   - `Sub-Delim`:
      - The choice made does not affect latter proccessing, as such, you are free to choose whatever you like.
        So long as it is not part of `Black Space`/`Removles` (it may be fine, but I have not tested doing so).
   - `Column`:
      - The column to be cleaned/formated
   - `Clean Column`:
      - Formats the given column based on the supplied choices.

Serial Positioning & Corrections
--------------------------------
   - `Files`:
      - `Target File`:
         - The name of a file containing a list of target words, with one 'target' per line
      - `Save File`:
         - The name of the file to save the positional data to. 
      - `Response Column`:
         - The index of the column to parse for `Unique Words`
      - `PID Column`:
         - The index of the unique identifiers.
      - `Get Words`:
         - Parses the individual responses in `Response Column` based on the `Sub-Delim` set in `IpForm Menu`
         - Displays an alphabetically sorted list of target words at the top of the menu. 
           Please note however, that serial positioning is performed relative to the list 
           as read from the `Target File`
      - `Unique Words`: 
         - Displays a list of the unique words found (minus the target words). You may then search through
           the list and enter in any corrections as you see fit.
      - `Make Corrections`:
         - Updates the individual responses according the corrections supplied in the `Unique Words` list.
         - PLEASE NOTE: if you want to make correction to the responses AND obtain subsequent positional data,
           that you MUST click `Make Corrections` BEFORE `Serialize & Save`.
         - PLEASE NOTE: If a response field contains *both* the 'original' & 'corrected' form of a sub-item,
           then the correction is not made and both forms are kept *as is*.
         - After making corrections, you may save the corrected responses via the `Output File`/`Save File`
           found in `CSV Menu`
      - `Serialize & Save`:
         - Serializes the the sub-items of the individual responses, counts the number of intrusions, 
           and saves said data to the file supplied in the `Save File` field.
         - Saves a log file to `Serial.log` containing:
            - Unique Words found (in their original form)
            - Corrected Words (both the original form and the corrected form)
            - The rows in which any corrections were made.
