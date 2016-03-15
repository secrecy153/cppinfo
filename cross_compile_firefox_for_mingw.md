#summary Cross Compile Firefox.
#sidebar Guide\_On

# 交叉编译 #
使用mingw交叉编译firefox:

官方的编译教程在这里

https://developer.mozilla.org/en/docs/Cross_Compile_Mozilla_for_Mingw32

但它写的不全,且错误不少.

1)准备操作系统平台(WINDOWS/LINUX),我这里介绍在LINUX下的交叉编译

因为在LINUX下编译速度要快很多,并且编译工具也易于获取,这里以debian/ubuntu/fedora为例.

2)安装交叉编译工具链

编译工具python perl automake多数系统已经内置了,如果没有请自行安装.

接下来你可以自己编译最新版的gcc和binutils,但并不建议这样做.

你可以下载我编译好的 [mingw32\_4.8.2\_cross\_compiler\_linux64.tar.xz](http://cppinfo.googlecode.com/files/mingw32_4.8.2_cross_compiler_linux64.tar.xz)

或者 debian/ubuntu 获取gcc(i686-w64-mingw32)和binutils:

aptitude install binutils-mingw-w64-i686 gcc-mingw-w64-i686 g++-mingw-w64-i686

fedora命令如下:

yum install mingw32-gcc mingw32-gcc-c++

安装完成后查看一下:

i686-w64-mingw32-g++ -v

如果一切正常,下一步你需要确认mingw64 crt库所在的位置. fedora下的位置在

` /usr/i686-w64-mingw32/sys-root/mingw `目录

3)更新mingw-w64 crt

社区编译的crt库可能已经过时,在编译firefox的时候会遇到许多错误,这里要自行编译最新版的crt.

#获取源码

` svn co svn://svn.code.sf.net/p/mingw-w64/code/trunk mingw-w64-svn `

#编译头

` mkdir tmp_build && cd tmp_build `

` ../mingw-w64-svn/mingw-w64-headers/configure --host=i686-w64-mingw32 --prefix=/usr/i686-w64-mingw32/sys-root/mingw --enable-sdk=all --disable-secure-api --enable-idl `

` make && make install && rm -rf * `

#编译库

` ../mingw-w64-svn/mingw-w64-crt/configure --host=i686-w64-mingw32 --prefix=/usr/i686-w64-mingw32/sys-root/mingw `

` make && make install && rm -rf * `

#编译widl

../mingw-w64-svn/mingw-w64-tools/widl/src/configure --target=i686-w64-mingw32 --prefix=/usr

` make && make install && rm -rf * `

注意,编译头文件的时候我使用了` --disable-secure-api `参数,这意味着我禁用了微软所谓的安全api以便支持winxp系统.

还有一种方法是` --enable-secure-api ` 启用安全api, 然后在编译firefox的时候链接到msvcr100之类的高版本crt库以便支持winxp.

第一种方法需要大量修改firefox源码(补丁可以在我的git trunk里看到),第二种方法需要修改编译参数,我选择第一种方法.

4)获取firefox源码编译

` hg clone http://hg.mozilla.org/mozilla-central/ ff-nightly `

` cd ff-nightly `

` vim .mozconfig `

` make -f client.mk build `

.mozconfig 文件以及补丁可以在Source Browse查看到.

实验性的构建版本下载  [Iceweasel v29.0 Chinese (Simplified) Win32](http://cppinfo.googlecode.com/files/Iceweasel-v29.0.zh_CN.mingw32.7z)

5)链接libportable获取更多高级特性.

libportable支持便携式,老板键,以及安全增强 [使用libportable](howto_compile_use_libportable#%e4%bd%bf%e7%94%a8libportable.md).

针对mingw编译版
我们可以启用tcmalloc库,代替jemalloc,因为我们很难实现在交叉编译的时候引入jemalloc.

6)现有的问题

mingw编译出来的firefox还存在一些问题.

known bugs:

在win7/8系统上开启硬件加速将导致firefox启动时崩溃.

关闭firefox主窗口时可能会导致某些线程崩溃。






