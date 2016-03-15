#summary Portable configuration example.
#sidebar Guide\_On

# portable.ini示例 #
```
;SafeEx				--安全增强(0 不启用 1 启用)
;Portable			--便携式(0 不启用 1 启用)
;EnableWhiteList		--启用白名单,最严格的安全策略(0 不启用 1 启用)
;CreateCrashDump		--进程崩溃后建立dmp文件(0 不启用 1 启用)
;Bosskey			--是否启用老板键(0 不启用 1 启用)
[General]
SafeEx=
Portable=1
EnableWhiteList=0
PortableDataPath=../profiles
CreateCrashDump=0
ProcessAffinityMask=0
Bosskey=1
ProxyExe=

;支持环境变量,如 MOZILLA_DISABLE_PLUGINS=1(见Iceweasel源码补丁), 为禁止插件启动
;TmpDataPath为自定义临时文件路径(包括网页缓存与mozilla临时文件,需设置Portable=1)
;NpluginPath为自定义插件路径
;VimpPentaHome为Pentadactyl或Vimperator插件的HOME目录,支持相对路径
[Env]
TmpDataPath=
NpluginPath=
VimpPentaHome=

;模块与进程白名单( <=14 ),支持通配符匹配,支持windows环境变量
;6--14条添加了注释,如需启用请删除前面的";"符号
[whitelist]
Path1=xxxxxx
Path2=xxxxxx
Path3=xxxxxx
Path4=xxxxxx
Path5=C:\Program Files\Internet Explorer\iexplore.exe
;Path6=C:\Windows\System32\rundll32.exe
;Path7=*xThunder.exe
;Path8=*opentext22.dll
;Path9=*WordStrokeHelper32.dll
;Path10=*AM32-*.dll
;Path11=*CLCLHook.dll
;Path12=C:\WINDOWS\system32\cmd.exe
;Path13=
;Path14=

;当 Bosskey=1 时生效,如果下面的值设置出错或未设置,默认老板键为(Ctrl+Shift+~).
;Hotkey设置必须是虚拟键码(Virtual Key Codes)的十进制格式,以+号分开.
;例如, 02+192 (热键为 Ctrl+~)  02+04+188 (热键为 Ctrl+Shift+,)
[attach]
Hotkey=02+192
ExPath="../local/goagent.exe,0"
```

# 配置说明 #
## `SafeEx`开关与白名单: ##

当设置`SafeEx=1`时,安全模块会拦截所有第三方软件的dll加载,以及外部非GUI程序的运行与一些危险的底层api操作.

并且会对JAVA 和SMARTPDF插件进行注入保护.

如果你不知道为什么要这样做,请设置`SafeEx=0` (设置为1会带给你更多的困惑).

如果你一定要启用安全模块,请留意`[whitelist]`字段的白名单说明,下面是一些网友可能会遇到的问题.

1)为什么我的xThunder扩展不能调用迅雷下载了?
> 需要删除`;Path7=*xThunder.exe` 条目前面的;符号.
2)为什么我的奶牛不能在firefox或Iceweasel上不起作用了?
> 需要删除`;Path10=*AM32-*.dll` 条目前面的;符号,并确认firefox或Iceweasel进程加入了过滤选项.
3)为什么我的有道词典不能屏幕取词了?
> 需要删除`;Path9=*WordStrokeHelper32.dll` 条目前面的;符号.
4)为什么我的灵格斯词典不能屏幕取词了?
> 需要删除`;Path8=*opentext22.dll` 条目前面的;符号.
5)为什么我的CLCL剪贴板不能工作了?
> 需要删除`;Path11=*CLCLHook.dll` 条目前面的;符号.
6)为什么我的批处理调用不能运行了.
> 需要删除`;Path12=C:\WINDOWS\system32\cmd.exe` 条目前面的;符号.
7)什么我的解雇ie Internet选项打不开?
> 这是因为你开启了`EnableWhiteList=1`的缘故(最严格的安全控制),

> 这时候你需要删除`;Path6=C:\Windows\System32\rundll32.exe` 条目前面的;符号.
8)为什么Strokeit鼠标手势不起作用了?
> 请设置`SafeEx=0`.

> 或者把它的dll文件加入白名单,例如:

> Path12=E:\soft\StrokeIt\mhook.dll
9) 一些未知的为什么 等等..........
> 请设置`SafeEx=0`.

## 新增`ProxyExe`开关变量: ##

当设置`ProxyExe=1`时,libportable可启动(关闭)外部进程.而

`[attach]`

ExPath="../local/goagent.exe,0"

../local/goagent.exe为外部进程的路径,支持bat批处理,vbs脚本.

后面的",0"代表尽可能隐藏外部进程的窗口.

## 移除`Nocompatete`开关变量: ##

不再兼容tete的便携式配置.而是把文件放在当前配置目录顶层.

解决了一些人抱怨便携式目录过深的问题.

## 移除`GdiBatchLimit`开关变量: ##

因为几乎没有人再去使用它.

# 热键使用 #
简单说下vk 热键的键码，如下:
```
常数名称                             十六进制值          十进制值       对应按键

MOD_ALT					0x1		1		ALT 键

MOD_CONTROL			        0x2		2		CONTROL键

MOD_SHIFT				0x4		4		SHIFT键

MOD_WIN				        0x8 		8		WIN键

VK_0到VK_9			       0x30-0x39       48-57	        0_9键

VK_A到VK_Z			       0x41-0x5A       65-90	        A_Z键

VK_F1到 VK_F12			       0x70-0x7B       112-123	        F1_F12键
```