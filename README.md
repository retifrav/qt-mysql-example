# Qt MySQL example

<!-- MarkdownTOC -->

- [About](#about)
- [Building](#building)
    - [Qt with MySQL driver](#qt-with-mysql-driver)
        - [Windows](#windows)
        - [Mac OS](#mac-os)
        - [GNU/Linux](#gnulinux)
    - [Application](#application)

<!-- /MarkdownTOC -->

## About

A minimal example for testing *static* MySQL driver with *static* Qt, so that the application would be "self-contained" and not requiring Qt/MySQL shared libraries (*DLLs, in case of Windows*).

## Building

### Qt with MySQL driver

Versions:

- Qt: `6.4.2`
- MySQL driver (*connector*): `8.0.32`
- MySQL server: `8.0.19` (*I had an old version already installed and decided not to update*)

Qt sources can be downloaded [here](https://download.qt.io/official_releases/qt/6.4/6.4.2/single/). It is quite possible that you'll be able to make it work with pre-built Qt installed with the official installer, but I haven't checked that.

Then the MySQL driver (*Oracle calls it "connector"*) can be downloaded from [here](https://dev.mysql.com/downloads/connector/cpp/). I'd recommend getting ZIP archive instead of installer (*I just don't like installers*). Here's the [download link](https://dev.mysql.com/downloads/file/?id=516343) that I used on Windows.

Finally, the MySQL server can be downloaded from [here](https://dev.mysql.com/downloads/mysql/). Again, I'd recommend getting ZIP archive instead of installer. If anything, here's [an article](https://decovar.dev/blog/2016/03/20/mysql-from-archive/) about installing MySQL server without installer (*unfortunately, it's all in russian, but you can probably skip the installation procedure, if you already have server running elsewhere, as you need only headers anyway*). And yes, counter-intuitively enough, you do need MySQL server in addition to the driver/connector, because for whatever reason the headers included in the driver/connector archive are useless, as you need exactly headers that come with MySQL server.

#### Windows

Paths:

- Qt sources: `d:\programs\qt\src\6.4.2`
- MySQL driver/connector: `d:\programs\mysql-connector`
- MySQL server: `d:\programs\mysql`

Assuming that you have MSVC 143 (*Visual Studio 2022*) installed, configure the build like this (*running from `cmd`, not `Git BASH`*):

``` sh
> call d:\programs\vs\vs2022\VC\Auxiliary\Build\vcvars64.bat
> cd d:\programs\qt\src\6.4.2\
> mkdir build
> cd build
> ..\configure -static -release -no-pch -prefix "d:/programs/qt/6.4.2-static" -skip qtwebengine -nomake tests -nomake examples -sql-mysql -- -DMySQL_INCLUDE_DIR="d:/programs/mysql/include" -DMySQL_LIBRARY="d:/programs/mysql-connector/lib64/vs14/mysqlcppconn-static.lib"
```

Pay attention to the `--` after `-sql-mysql`. Also note that `-DMySQL_INCLUDE_DIR` points to the include folder of the server, not the driver/connector (*its headers don't seem to be used for anything*). And then if you want static driver, be careful to provide exactly static variant in the `-DMySQL_LIBRARY` parameter.

If Qt configuration succeeds, you should see that MySQL driver was discovered:

``` sh
...
Qt Sql Drivers:
  DB2 (IBM) .............................. no
  InterBase .............................. no
  MySql .................................. yes
  OCI (Oracle) ........................... no
  ODBC ................................... no
  PostgreSQL ............................. no
  SQLite ................................. yes
    Using system provided SQLite ......... no
...
```

Now you can build and install the Qt:

``` sh
> cmake --build . --parallel
```

On my machine the build finished with something that looked like an error:

``` sh
...
[8903/10043] Generating rep_grammar.cpp, rep_grammar_p.h, repparser.h, repparser.cpp
*** Conflicts: 24 shift/reduce, 28 reduce/reduce
[10043/10043] Linking CXX static library qtbase\qml\QtWebView\qtwebviewquickplugin.lib

>echo %errorlevel%
1
```

But that seemed to be just some weird flux, as I could install it and then successfully use it for building applications:

``` sh
> cmake --build . --parallel
ninja: no work to do.
> cmake --install .
```

#### Mac OS

Haven't checked yet.

#### GNU/Linux

Haven't checked yet.

### Application

Once you have Qt build with MySQL plugin/driver/connector, you can build the sample application (*this repository*).

First configure the build:

``` sh
> cd d:\code\qt\qt-mysql-test
> mkdir build
> cd build
> cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="d:/programs/qt/6.4.2-static" -DMySQL_INCLUDE_DIR="d:/programs/mysql/include" -DMySQL_LIBRARY="d:/programs/mysql-connector/lib64/vs14/mysqlcppconn-static.lib" ..
-- The CXX compiler identification is MSVC 19.33.31629.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: d:/programs/vs/vs2022/VC/Tools/MSVC/14.33.31629/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
-- Using Qt bundled ZLIB.
-- Found WrapZLIB: TRUE
-- Performing Test HAVE_STDATOMIC
-- Performing Test HAVE_STDATOMIC - Success
-- Found WrapAtomic: TRUE
-- Using Qt bundled PCRE2.
-- Found WrapPCRE2: TRUE
-- Found MySQL: d:/programs/mysql-connector/lib64/vs14/mysqlcppconn-static.lib
-- Found ODBC: odbc32.lib
-- Configuring done
-- Generating done
-- Build files have been written to: d:/code/qt/qt-mysql-test/build
```

As you can see, you still need to provide `-DMySQL_INCLUDE_DIR` and `-DMySQL_LIBRARY`, otherwise it will fail like this:

``` sh
CMake Error at d:/programs/qt/6.4.2-static/lib/cmake/Qt6Sql/Qt6QMYSQLDriverPluginTargets.cmake:61 (set_target_properties):
  The link interface of target "Qt6::QMYSQLDriverPlugin" contains:

    MySQL::MySQL

  but the target was not found.  Possible reasons include:

    * There is a typo in the target name.
    * A find_package call is missing for an IMPORTED target.
    * An ALIAS target is missing.
```

complaining that it could not find MySQL either because of missing `MySQL_INCLUDE_DIR` or `MySQL_LIBRARY`.

Once the configuration succeeds, you can build and run the application:

``` sh
> cmake --build .
[4/4] Linking CXX executable qt-mysql-test.exe

> qt-mysql-test.exe
Available SQL drivers:
- QMARIADB
- QMYSQL
- QODBC
- QSQLITE

The QMYSQL driver seems to be OK

Query results: "Some results"

> echo %errorlevel%
0
```
