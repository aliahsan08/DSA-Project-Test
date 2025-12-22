# DSA Project Game

## Introduction
This game is a traditional side-scroller game which has 6 different levels all with different difficulties and goals while utilizing various data structures and algorithms as appropriate.

## How to run the project
1) Download Unreal Engine 4.27.2.0
2) As you proceed to build a project, UE4 will prompt you about installing a C++ compiler. It will redirect you to a Visual Studio 2019 community installer (Make sure no VS is installed beforehand)
3) Using git, clone this repository while making sure you utilize LFS (Large FIle Storage) system. Downloading a .zip file will not work.
4) Once cloned, you have to build your VS sln file. For that, you have 2 options:
   1) Right the uproject file -> Click on "Generate Visual Studio Project Files"
   2) If 1) doesn't work, try running this command in your command prompt
      "path\to\Unreal Engine\UE_4.27\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="path\to\project\dsaproject.uproject" -game -rocket -progress
5) Press the sln file. Make sure you have selecte developmentEditor and WIN64 in the top bar
6) Go to Build -> build dsaproject. This will build the project and compile the C++ code.
7) Open the uproject file and it'll take you to the project

## How to run the .exe
1) Simply download the .zip which has the .exe to your PC.
2) Extract the .zip file
3) Press the .exe file and play the game!

## Requirements
You must have a 64-bit architecture windows PC
