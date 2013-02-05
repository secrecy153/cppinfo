@echo off
if exist C:\mozilla-build\msys\bin set PATH=C:\mozilla-build\msys\bin;%PATH%


REM Environment variables:
set TT_MOZ_SRC_DIR=D:\works\ff-release
set TT_PATCH_DIR=D:\works\v18\tete009


REM define restricted pointer attribute:
patch -N %TT_MOZ_SRC_DIR%\mfbt\Attributes.h %TT_PATCH_DIR%\Attributes.h.patch

REM direct access to TLS entry:
patch -N %TT_MOZ_SRC_DIR%\mfbt\ThreadLocal.h %TT_PATCH_DIR%\ThreadLocal.h.patch




REM string:
patch -N %TT_MOZ_SRC_DIR%\xpcom\string\public\nsCharTraits.h %TT_PATCH_DIR%\nsCharTraits.h.patch

REM pl double hashing:
patch -N %TT_MOZ_SRC_DIR%\xpcom\glue\pldhash.cpp %TT_PATCH_DIR%\pldhash.cpp.patch

REM nsDeque::Empty():
patch -N %TT_MOZ_SRC_DIR%\xpcom\glue\nsDeque.cpp %TT_PATCH_DIR%\nsDeque.cpp.patch

REM NS_IsMainThread():
patch -N %TT_MOZ_SRC_DIR%\xpcom\glue\nsThreadUtils.cpp %TT_PATCH_DIR%\nsThreadUtils.cpp.patch




REM SocketConnectContinue:
patch -N %TT_MOZ_SRC_DIR%\nsprpub\pr\src\io\prsocket.c %TT_PATCH_DIR%\prsocket.c.patch

REM PR_GetCurrentThread:
patch -N %TT_MOZ_SRC_DIR%\nsprpub\pr\include\md\_win95.h %TT_PATCH_DIR%\_win95.h.patch




REM avoid optimizer bug with MSVC PGO (Bug 595653):
patch -N %TT_MOZ_SRC_DIR%\js\src\Makefile.in %TT_PATCH_DIR%\js_src_Makefile.in.patch

REM js double hashing:
patch -N %TT_MOZ_SRC_DIR%\js\src\jsdhash.cpp %TT_PATCH_DIR%\jsdhash.cpp.patch

REM use function version of floor() under -Oi -arch:SSE2
patch -N %TT_MOZ_SRC_DIR%\js\src\jsmath.cpp %TT_PATCH_DIR%\jsmath.cpp.patch

REM avoid internal compiler error when applying MSVC PGO:
patch -N %TT_MOZ_SRC_DIR%\js\src\methodjit\Compiler.cpp %TT_PATCH_DIR%\Compiler.cpp.patch

REM avoid the bug of PGO:
patch -N %TT_MOZ_SRC_DIR%\js\src\yarr\YarrInterpreter.cpp %TT_PATCH_DIR%\YarrInterpreter.cpp.patch

REM use SSE2 copy for NewObjectCache::copyCachedToObject():
patch -N %TT_MOZ_SRC_DIR%\js\src\jsobjinlines.h %TT_PATCH_DIR%\jsobjinlines.h.patch

REM use PodCopy in VectorImpl::copyConstruct(wchar_t *, const wchar_t *, const wchar_t *):
patch -N %TT_MOZ_SRC_DIR%\js\public\Vector.h %TT_PATCH_DIR%\Vector.h.patch




REM avoid compile errors with VC8 (and maybe VC9):
patch -N %TT_MOZ_SRC_DIR%\js\src\gc\Root.h %TT_PATCH_DIR%\Root.h.patch
patch -N %TT_MOZ_SRC_DIR%\js\src\jsapi.cpp %TT_PATCH_DIR%\jsapi.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\js\src\jsobj.cpp %TT_PATCH_DIR%\jsobj.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\js\src\jsiter.cpp %TT_PATCH_DIR%\jsiter.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\js\src\vm\ObjectImpl.h %TT_PATCH_DIR%\ObjectImpl.h.patch
patch -N %TT_MOZ_SRC_DIR%\js\src\vm\ObjectImpl.cpp %TT_PATCH_DIR%\ObjectImpl.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\js\src\jsreflect.cpp %TT_PATCH_DIR%\jsreflect.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\xpcom\ds\TimeStamp_windows.cpp %TT_PATCH_DIR%\TimeStamp_windows.cpp.patch




REM gklayout:
patch -N %TT_MOZ_SRC_DIR%\layout\style\nsCSSRuleProcessor.cpp %TT_PATCH_DIR%\nsCSSRuleProcessor.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\layout\base\nsDisplayList.cpp %TT_PATCH_DIR%\nsDisplayList.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\layout\generic\nsHTMLReflowState.cpp %TT_PATCH_DIR%\nsHTMLReflowState.cpp.patch

REM avoid the fatal error C1001 of compiler when applying PGO:
patch -N %TT_MOZ_SRC_DIR%\layout\style\nsCSSDataBlock.cpp %TT_PATCH_DIR%\nsCSSDataBlock.cpp.patch

REM avoid crash at the time of printing when applying PGO:
patch -N %TT_MOZ_SRC_DIR%\layout\generic\nsContainerFrame.cpp %TT_PATCH_DIR%\nsContainerFrame.cpp.patch




REM libpixman:
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\Makefile.in %TT_PATCH_DIR%\libpixman_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-private.h %TT_PATCH_DIR%\pixman-private.h.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-cpu.c %TT_PATCH_DIR%\pixman-cpu.c.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-edge.c %TT_PATCH_DIR%\pixman-edge.c.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-sse2.c %TT_PATCH_DIR%\pixman-sse2.c.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-trap.c %TT_PATCH_DIR%\pixman-trap.c.patch




REM brush caches for cairo d2d:
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\cairo\src\cairo-d2d-private.h %TT_PATCH_DIR%\cairo-d2d-private.h.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\cairo\src\cairo-d2d-surface.cpp %TT_PATCH_DIR%\cairo-d2d-surface.cpp.patch




REM avoid compile error C2061 and C2065 with VC8:
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\moz-d2d1-1.h %TT_PATCH_DIR%\gfx_2d_moz-d2d1-1.h.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\cairo\cairo\src\moz-d2d1-1.h %TT_PATCH_DIR%\gfx_cairo_moz-d2d1-1.h.patch




REM scaled image cache:
patch -N %TT_MOZ_SRC_DIR%\gfx\thebes\gfxContext.h %TT_PATCH_DIR%\gfxContext.h.patch
patch -N %TT_MOZ_SRC_DIR%\layout\base\nsLayoutUtils.cpp %TT_PATCH_DIR%\nsLayoutUtils.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\image\src\imgFrame.cpp %TT_PATCH_DIR%\imgFrame.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\thebes\gfxUtils.cpp %TT_PATCH_DIR%\gfxUtils.cpp.patch

REM NULL pointer check for entry->GetRequest():
patch -N %TT_MOZ_SRC_DIR%\image\src\imgLoader.cpp %TT_PATCH_DIR%\imgLoader.cpp.patch




REM 2D:
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\Makefile.in %TT_PATCH_DIR%\gfx_2d_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\2D.h %TT_PATCH_DIR%\2D.h.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\Factory.cpp %TT_PATCH_DIR%\Factory.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\Blur.cpp %TT_PATCH_DIR%\Blur.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\DrawTargetD2D.cpp %TT_PATCH_DIR%\DrawTargetD2D.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\ImageScalingSSE2.cpp %TT_PATCH_DIR%\ImageScalingSSE2.cpp.patch




REM qcms:
patch -N %TT_MOZ_SRC_DIR%\gfx\qcms\transform.c %TT_PATCH_DIR%\transform.c.patch




REM call To***Pixels instead of ScaleTo***Pixels, if possible:
patch -N %TT_MOZ_SRC_DIR%\layout\base\FrameLayerBuilder.h %TT_PATCH_DIR%\FrameLayerBuilder.h.patch
patch -N %TT_MOZ_SRC_DIR%\layout\base\FrameLayerBuilder.cpp %TT_PATCH_DIR%\FrameLayerBuilder.cpp.patch

REM use SSE4.1 in some functions of nsRect, if available:
patch -N %TT_MOZ_SRC_DIR%\gfx\2d\BaseRect.h %TT_PATCH_DIR%\BaseRect.h.patch
patch -N %TT_MOZ_SRC_DIR%\gfx\src\nsRect.h %TT_PATCH_DIR%\nsRect.h.patch




REM content:
patch -N %TT_MOZ_SRC_DIR%\content\base\src\nsContentUtils.cpp %TT_PATCH_DIR%\nsContentUtils.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\content\media\nsAudioStream.cpp %TT_PATCH_DIR%\nsAudioStream.cpp.patch

REM avoid crash when applying PGO:
patch -N %TT_MOZ_SRC_DIR%\content\media\Makefile.in %TT_PATCH_DIR%\content_media_Makefile.in.patch




REM svg:
patch -N %TT_MOZ_SRC_DIR%\content\svg\content\src\Makefile.in %TT_PATCH_DIR%\svg_content_src_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\content\svg\content\src\nsSVGFilters.cpp %TT_PATCH_DIR%\nsSVGFilters.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\layout\svg\Makefile.in %TT_PATCH_DIR%\svg_base_src_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\layout\svg\nsSVGMaskFrame.cpp %TT_PATCH_DIR%\nsSVGMaskFrame.cpp.patch




REM canvas:
patch -N %TT_MOZ_SRC_DIR%\content\canvas\src\Makefile.in %TT_PATCH_DIR%\canvas_src_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\content\canvas\src\CanvasUtils.cpp %TT_PATCH_DIR%\CanvasUtils.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\content\canvas\src\CanvasUtils.h %TT_PATCH_DIR%\CanvasUtils.h.patch
patch -N %TT_MOZ_SRC_DIR%\content\canvas\src\nsCanvasRenderingContext2DAzure.cpp %TT_PATCH_DIR%\nsCanvasRenderingContext2DAzure.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\content\canvas\src\nsCanvasRenderingContext2D.cpp %TT_PATCH_DIR%\nsCanvasRenderingContext2D.cpp.patch




REM plug-in:
patch -N %TT_MOZ_SRC_DIR%\dom\plugins\base\nsPluginNativeWindowWin.cpp %TT_PATCH_DIR%\nsPluginNativeWindowWin.cpp.patch




REM make NS_ASSERT() the same behavior as official builds:
patch -N %TT_MOZ_SRC_DIR%\toolkit\content\debug.js %TT_PATCH_DIR%\debug.js.patch




REM avoid an error LNK1120 for tete009 builds:
patch -N %TT_MOZ_SRC_DIR%\security\nss\lib\freebl\Makefile %TT_PATCH_DIR%\freebl_Makefile.patch
patch -N %TT_MOZ_SRC_DIR%\security\nss\lib\freebl\manifest.mn %TT_PATCH_DIR%\freebl_manifest.mn.patch




REM embeds OpenMP manifest for tete009 builds:
patch -N %TT_MOZ_SRC_DIR%\toolkit\xre\nsWindowsWMain.cpp %TT_PATCH_DIR%\nsWindowsWMain.cpp.patch




REM apply the patch of bug 526038 to avoid crash:
patch -N %TT_MOZ_SRC_DIR%\image\decoders\icon\win\nsIconChannel.cpp %TT_PATCH_DIR%\nsIconChannel.cpp.patch




REM jemalloc:
patch -N %TT_MOZ_SRC_DIR%\mozglue\build\mozglue.def.in %TT_PATCH_DIR%\mozglue.def.in.patch
patch -N %TT_MOZ_SRC_DIR%\memory\mozjemalloc\jemalloc.c %TT_PATCH_DIR%\jemalloc.c.patch




REM nsWindow & WinUtils:
patch -N %TT_MOZ_SRC_DIR%\widget\windows\nsWindow.cpp %TT_PATCH_DIR%\nsWindow.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\widget\windows\WinUtils.cpp %TT_PATCH_DIR%\WinUtils.cpp.patch




REM set MOZ_UA_BUILDID=20100101:
patch -N %TT_MOZ_SRC_DIR%\browser\branding\aurora\configure.sh %TT_PATCH_DIR%\branding_aurora_configure.sh.patch
patch -N %TT_MOZ_SRC_DIR%\browser\branding\nightly\configure.sh %TT_PATCH_DIR%\branding_nightly_configure.sh.patch


REM avoid the fatal error C1001 of compiler when applying PGO to Thunderbird:
patch -N %TT_MOZ_SRC_DIR%\ipc\ipdl\Makefile.in %TT_PATCH_DIR%\ipc_ipdl_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\dom\plugins\ipc\Makefile.in %TT_PATCH_DIR%\dom_plugins_ipc_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\dom\ipc\Makefile.in %TT_PATCH_DIR%\dom_ipc_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\netwerk\base\src\Makefile.in %TT_PATCH_DIR%\netwerk_base_src_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\security\manager\ssl\src\Makefile.in %TT_PATCH_DIR%\security_manager_ssl_src_Makefile.in.patch
patch -N %TT_MOZ_SRC_DIR%\media\webrtc\trunk\tools\gyp\pylib\gyp\generator\mozmake.py %TT_PATCH_DIR%\mozmake.py.patch




REM Bug 832641 patches:
patch -N %TT_MOZ_SRC_DIR%\layout\xul\base\src\nsMenuPopupFrame.cpp %TT_PATCH_DIR%\nsMenuPopupFrame.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\widget\android\nsWindow.cpp %TT_PATCH_DIR%\android_nsWindow.cpp.patch
patch -N %TT_MOZ_SRC_DIR%\widget\nsIWidget.h %TT_PATCH_DIR%\nsIWidget.h.patch


pause

