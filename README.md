# AOC Patcher

### What is AOC Patcher? ###
AOC Patcher is a WiiU homebrew. It launches WiiU DLC (experimental) from SD card.

### How to build? ###
To build the main application devkitPPC is required as well as some additionally libraries. Download the libogc and portlibs packages from the release pages and put them in your devkitPro path. Replace any existing files. If not yet done export the path of devkitPPC and devkitPRO to the evironment variables DEVKITPRO and DEVKITPPC.
All remaining is to enter the main application path and enter "make". You should get a AocPatcher.elf and a AocPatcher_dbg.elf in the main path.

### How do I use this? ###
- Put the elf into the app path on your SD Card (e.g. /wiiu/apps/AocPatcher/AocPatcher.elf) and insert the sd card into your WiiU (You can also use the sendelf method)
- Launch homebrew launcher
- Select AocPatcher and start it.

### Folder structure DLC ###
-Place the DLC into the titles path on your SD Card  /wiiu/games/titles/[Title Id]/[AOC Title].

(e.g. for Super Smash Bros EUR /wiiu/games/titles/0005000010145000/aoc0005000c10145000

### Credits ###
* Dimok
* Maschell
* and several more contributers

Masqchips