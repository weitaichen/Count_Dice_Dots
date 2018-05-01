# Count_Dice_Dots
Detect and count dots on dice using C++ with OpenCV on visual studio (a task from Nascent technology)

## How can you get started
https://docs.opencv.org/2.4/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html<br />
https://www.youtube.com/watch?v=l4372qtZ4dc<br />
Above are tutorial webisites which tell you how to install OpenCV library on visual studio platform, following are steps:<br />
* Download OpenCV on https://opencv.org/releases.html<br />
* Setting environment variable:PC->Properties->Advanced system setting->Environment variables->Path<br />
->Edit->New->OpenCV bin directory<br />
* Open new projects: Open Visual studio->File->New project->Visual C++->Win32 Console application->Finish<br />
* Choose configuration: Configuration manager->Active solution platform->New->Choose x86 or x64 platform <br />
* Set include path: Project->Properties->C/C++->Additional include directories->Opencv include directory<br />
* Add library directories:Project->Properties->linker->General->Additional library directories->Opencv lib directory<br />
* Add library dependency:Project->Properties->linker->Input->Additional dependecies->opencv_world300d.lib<br />
* Download msvcp120d.dll and msvcr120d.dll to main.exe folder <br />
* Copy all file into your project folder<br />
* Build project and find main.exe directory, put lena.png file into that folder <br />
* Open cmd.exe and go to your debug directory where main.exe was generated <br />
* Enter command "main.exe lena.png 1" <br />
* Have fun! <br />


## How does this work
This program is intended to people who need to test consecutive image processing. After each command, there will be a file output.png generated in your main.exe directory. <br />
* In cmd.exe, go to your debug directory where main.exe was generated. <br />
* Enter command "main.exe image_name method parameter1 parameter2...."<br />
* For example, you can do input image->Otsu's thresholding->Sobel filter, which is a well known method for edge detection. Following are steps:"<br />
* (Show color image) In cmd.exe: main.exe lena.png 2
<p align="center"><img src="/image/lena.png" height="30%" width="30%"></p><br />
* (Apply Sobel filter) In cmd.exe: main.exe output.png 9 3
<p align="center"><img src="/image/sobel.png" height="30%" width="30%"></p><br />
* (Apply Otsu's thresholding) In cmd.exe: main.exe output.png 3
<p align="center"><img src="/image/otsu.png" height="30%" width="30%"></p><br />
