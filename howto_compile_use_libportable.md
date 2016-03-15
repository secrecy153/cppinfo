#summary Compile Libportable.
#sidebar Guide\_On

# 获取源码 #
git clone http://code.google.com/p/libportable


# 编译源码 #
1)mingw-w64编译器

需要安装msys(unix like)环境.

cd libportable

make clean

#编译32位dll.

make BITS=32

#或者编译64位dll.

make BITS=64

2)vc编译器

cd libportable

nmake -f Makefile.msvc clean

nmake -f Makefile.msvc

3)交叉编译

cd libportable

make clean

make CROSS\_COMPILING="i686-w64-mingw32-"

# 使用libportable #
1)直接使用二进制dll文件

在Downloads列表下载libportable.7z文件解压.通过Inject PE 程序

静态注入到xul.dll文件.

2)编译时动态链接libportable

补丁如下所示:
```
diff --git a/toolkit/library/Makefile.in b/toolkit/library/Makefile.in
--- a/toolkit/library/Makefile.in
+++ b/toolkit/library/Makefile.in
@@ -454,3 +454,11 @@
 
 $(FINAL_TARGET)/dependentlibs.list: dependentlibs.py $(SHARED_LIBRARY) $(wildcard $(if $(wildcard $(FINAL_TARGET)/dependentlibs.list),$(addprefix $(FINAL_TARGET)/,$(shell cat $(FINAL_TARGET)/dependentlibs.list))))
 	$(PYTHON) $< $(SHARED_LIBRARY) -L $(FINAL_TARGET) $(if $(TOOLCHAIN_PREFIX),$(addprefix -p ,$(TOOLCHAIN_PREFIX))) > $@
+
+ifeq ($(OS_ARCH), WINNT)
+ifdef GNU_CC
+LDFLAGS += -Wl,-u__imp__memset_nontemporal_tt -lportable32
+else
+LDFLAGS += /INCLUDE:"__imp__memset_nontemporal_tt" portable32.lib
+endif
+endif
```