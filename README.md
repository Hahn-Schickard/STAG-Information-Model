# Template project

## Description
This project works both as a template for your further projects and as an example project with some very basic code. 

In this project you can find example files for: 

* CMakeLists.txt root files
* CMakeLists.txt structure for multiple nested directories
* Linting with clang-tidy
* Including external projects with CMake 
* Writing CMake Macros
* Building shared libraries with cmake
* Writing binaries to a specific location with cmake
* Linking libraries with cmake 
* Writing google tests
* Running google tests 

## Using as a template

When using this repository as a template **MAKE SURE** to delete unused files and comments. 
**DO NOT FORGET** there are comments in every CMakeLists.txt file, it would make for a much cleaner code, if all of these comments were deleted in your project! 

**MAKE SURE** that your repository can be built before committing it to main group! Always expose a root CMakeLists.txt file for other project to use! 

## Windows users

To fully utilize the functionalities of this template GCC compiler toolchain is required. To install GCC on windows please follow [this tutorial](https://git.hahn-schickard.de/opc_ua_dev_group/opcua-project/wikis/Install-GCC-toolchain-on-Windows). 

# Downloading
To use this template you can clone it directly from Git or downloading it as an archive. Short code snipets to do this are provided bellow. 
## Git
```bash 
git clone -b master --single-branch --depth 1 git@git.hahn-schickard.de:opc_ua_dev_group/project_template.git 
cd project_template 
git remote rm origin
git remote add origin [YOUR_REPOSITORY_URI]
git add . 
git commit -m "initial project setup" -s 
git push -u origin master 
```
## Archive
### Linux

```bash
git clone -b master --single-branch --depth 1 git@git.hahn-schickard.de:opc_ua_dev_group/project_template.git project_template/
cp -R project_template/* .
rm -rf .git project_template
```

### Windows

```cmd
git clone -b master --single-branch --depth 1 git@git.hahn-schickard.de:opc_ua_dev_group/project_template.git project_template/
xcopy project_template /e 
rmdir /S .git project_template
```

### From Gitlab 
In gitlab repository view select the Download button showed in the image bellow and select your desired archiving format: 

![Downloading](docs/images/gitlab.gif)

Once downloaded extract the content of archives `project_template/` directory to your future project directory. 

## Dependencies 
### Mandatory 
* cmake
* c++11 compliant compiler
### (Optional) Visual Studio Code:
* clang-format
* clang-tidy
* VSC extentions:
  * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) 
  * [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
  * [CMake Tools](https://marketplace.visualstudio.com/items?itemName=vector-of-bool.cmake-tools)
  * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
  * [Test Explorer UI](https://marketplace.visualstudio.com/items?itemName=hbenl.vscode-test-explorer)
  * [Catch2 and Google Test Explorer (with code lens)](https://marketplace.visualstudio.com/items?itemName=piotrkosek.vscode-gtest-test-adapter-with-code-lens)
  * [Google Test Explorer for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=OpenNingia.vscode-google-test-adapter)
  * [Test Explorer UI](https://marketplace.visualstudio.com/items?itemName=hbenl.vscode-test-explorer)
  * [C++ Intellisense](https://marketplace.visualstudio.com/items?itemName=austin.code-gnu-global)