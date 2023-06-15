# ChecklistLoader
 Loads a text file containing steps for a checklist so you can check through them one at a time.


### Checklist file format
 Files loaded will be read by line according to the following rules:
 1. Characters will be read until the first non-leading whitespace character
 2. These characters will be split on the period (.) character to determine step and sub-step
 3. The first set of non-leading whitespace characters will be ignored
 4. The remaining characters on the line will be read as the text for that step
 5. Leading whitespace characters will cause the relevant step to be treated as a sub-step for the preceeding step with fewer whitespace characters
 6. If there are multiple lines with the same step, only the last value will be retained

### Example checklist file
    1. Testing
      a. Test
      b. Test B
    2.a. Test C
    2.c. Test D
    2. This is a test
    3.	Test 2
 With this example file, you'll end up with a checklist similar to the following
 1. Testing
     1. Test
     2. Test B
 2. This is a test
     1. Test C
     2. Test D
 3. Test 2

## Compilation instructions
 Compilation depends on 2 things:
 1. wxWidgets 3.1.5 or higher has been compiled with static libraries, with the _debug config for debug libraries and the _release config for release libraries
    1. Go to https://www.wxwidgets.org/downloads/ and download the appropriate version (i.e. installer)
    2. Install in a user-accessible directory (for example, C:\wxwidgets)
    3. Set the environment variable WXWIN to the directory noted above (this is used in the include directories for the project)
    4. Open a visual studio command prompt (Tools -> Command Line -> Developer Command Prompt)
    5. Run the following commands to build x86 and x64 libraries both dynamically and statically for debug and release, with appropriate config names so that this solution will compile properly:
```
cd %wxwin%\build\msw
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static BUILD=release CFG=_release
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic BUILD=release CFG=_release
"%vcinstalldir%auxiliary\build\vcvars64.bat"
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static TARGET_CPU=X64 BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic TARGET_CPU=X64 BUILD=debug CFG=_debug
nmake /a /f makefile.vc SHARED=0 RUNTIME_LIBS=static TARGET_CPU=X64 BUILD=release CFG=_release
nmake /a /f makefile.vc SHARED=1 RUNTIME_LIBS=dynamic TARGET_CPU=X64 BUILD=release CFG=_release
```
