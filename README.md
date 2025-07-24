# MFC_Application
基于MFC的c++桌面应用开发小项目

项目目的：程序在登录后先读入data中的数据（登录界面数据存在数据库sqlite里，即test.db），然后利用c++中的 EasyX 库将原始数据建图后保存在data目录下，后利用上面上传的data文件夹的穿过气体后的光谱数据计算气体吸光度，在已经知道气体种类和浓度的情况下利用迭代重加权拟合气体吸光度和气体浓度（代码中加入了c++的 Eigen 库方便实现）。最后菜单界面可以读入气体原始光谱数据，计算吸光度后带入拟合后的函数，从而反推出气体浓度。

注意事项：需要下载sqlite，Eigen，EasyX 等，同时.exe目录下一定要有data文件夹和sqlite.dll依赖项，在vs中运行时需要将字符集改成多字节字符集，然后添加sqlite的依赖项（项目属性里面VC++目录下，包含目录需要添加inc文件夹，库目录需要添加lib文件夹，连接器里面的附加依赖项需要添加sqlite.lib），项目也还有bug和未完成的地方，请多谅解。

C++ Desktop Application Development Mini-Project Based on MFC

Project Objective: After successful login (login interface data is stored in an SQLite database, i.e., `test.db`), the program first reads the data from the `data` directory. It then utilizes the EasyX library in C++ to graph the raw data and saves the generated graph in the `data` directory. Subsequently, the program uses the spectral data of light passing through the gas (located in the uploaded `data` folder) to calculate the gas absorbance. Given the known gas type and concentration, it employs an iteratively reweighted fitting method to establish the relationship between gas absorbance and gas concentration (the Eigen library is incorporated in the code to facilitate this implementation). Finally, the menu interface allows users to load the original spectral data of a gas sample. After calculating its absorbance, this value is input into the fitted function to inversely determine the gas concentration.

Important Notes:
1.  Dependencies: SQLite, Eigen, and EasyX libraries need to be downloaded and installed.
2.  Runtime Requirements: The `.exe` directory must contain the `data` folder and the `sqlite.dll` dependency.
3.  Visual Studio Configuration:
       Change the character set to **Multi-byte Character Set.
       Add SQLite dependencies in the project properties:
           Under `VC++ Directories`:
              Include Directories: Add the path to the `inc` folder.
              Library Directories: Add the path to the `lib` folder.
           Under `Linker` -> `Input` -> Additional Dependencies: Add `sqlite.lib`.
4.  Project Status: Please be aware that the project may still contain bugs and unfinished parts. We appreciate your understanding and patience.
