# zlib_base64_memfile
Test/evaluate using cmake with zlib from vcpkg and do some base64 encode&amp;decode 

# Build
The test was using VScode with the Visual Studio 2019 Community. Tried mingw first but its a bit tricky to get it using vcpkg (triplet/target mapped correctly)

# Base64
This code was downloaded and edited some to return std::vector<char> instead of std::string.
  
```
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
```
# Vcpkg / prerequesites
The project use d:\vcpkg as root.  Installation is easy enough (https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160) - in this case on windows:
```
d:\> git clone https://github.com/microsoft/vcpkg
d:\> cd vcpkg
d:\vcpkg> bootstrap-vcpkg.bat
d:\vcpkg> vcpkg.exe install zlib
d:\vcpkg> vcpkg.exe install gtest
```

# Unit test
I like googletest - and size we use vcpkg for zlib, a simple "vcpkg install gtest" do the trick
