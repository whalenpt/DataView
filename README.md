# DataView #

DataView is a library to aid plotting and visualizing data file content
using the Qt framework. Designed primarily for scientific applications,
its main purpose is to allow for easy navigating and visualizing of data files
with a GUI application.

# Dependencies #

Library links with Qt6 modules
<ul>
<li> Qt6::Core </li>
<li> Qt6::Widgets </li>
<li> Qt6::Gui </li>
<li> Qt6::Charts </li>
</ul>

Qt6 >= 6.1.0 must be installed (Qt6::Charts not included in Qt 6.0)
Build is done using CMake

# Installation #

If not installed, get and install <a href = https://doc.qt.io/qt-6/gettingstarted.html>Qt6 </a>,
version greater than or equal to 6.1.0, and download <a href = https://cmake.org/download/>CMake</a>.

The easiest way to build the library and run the demos is to use QtCreator:
<ul>
<li> Open QtCreator </li>
<li> Select Open File or Project from the File Menu </li>
<li> Navigate to the DataView folder downloaded from github and open the CMakeLists.txt file </li>
<li> Build the project to create a dataview library file (shared) </li> 
<li> Run a demo subdirectory to see examples of how the library works </li>
</ul>

To build the DataView library from the terminal (Linux/MacOS) use
```bash
git clone https://github.com/whalenpt/DataView.git
cd DataView
cmake -S . -B build -DCMAKE_PREFIX_PATH=<Path_To_Qt_Folder/Qt_Version/compiler_name>
cd build
make -j4
```
The dataview library file is under the dataview subdirectory of build. Examples
are in the demos subdirectory. For instance, to run the Optics demo change directory
into demos/Optics and run the executable named optics.

A similar procedure can be done for from the Windows Powershell with a few additional
options to the cmake command
```Powershell
cd ParamTree
cmake -S . -B build-mingw -DCMAKE_PREFIX_PATH=<Path_To_Qt_Folder\Qt_Version\mingw_version>
			  -DCMAKE_CXX_COMPILER=<Path_To_Qt_Folder\Tools\mingw_version\bin\g++.exe>
			  -G "Unix Makefiles"
cd build-mingw 
make -j4
```
To run demos on the Windows build, the executables need to be able to find the Qt DLLs (for
instance by adding the folder `<Path_To_Qt_Folder\Qt_Version_#\mingw_version\bin>` to the
windows environment PATH variable) and the generated paramtree DLL (for instance by
copying this to the directory of the demo or also adding it to the path). Alternatively,
cmake without the "Unix Makefiles" compiler flag can create a Visual Studio project file and
the library can be created in that setting as well. 

# License #
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
The Qt code used in the dataview library is available with a GPLv3 license 
but check [Qt.LICENSE](./Qt.LICENSE) for more information on reuse restrictions and
limitations. Note that the Qt6::Charts module uses the more restrictive GPL license
and this module touches most parts of the dataview library.

# Contact #
Patrick Whalen - whalenpt@gmail.com













































