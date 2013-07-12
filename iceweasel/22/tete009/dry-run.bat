



REM Environment variables:
set TT_MOZ_SRC_DIR=%BUILDDIR%
set TT_PATCH_DIR=E:\Mozilla\tete009_patch\tete\Fx22.0




REM define restricted pointer attribute:
patch --dry-run -N %TT_MOZ_SRC_DIR%\mfbt\Attributes.h %TT_PATCH_DIR%\Attributes.h.patch




REM string:
patch --dry-run -N %TT_MOZ_SRC_DIR%\xpcom\string\public\nsCharTraits.h %TT_PATCH_DIR%\nsCharTraits.h.patch

REM pl double hashing:
patch --dry-run -N %TT_MOZ_SRC_DIR%\xpcom\glue\pldhash.cpp %TT_PATCH_DIR%\pldhash.cpp.patch

REM nsDeque::Empty():
patch --dry-run -N %TT_MOZ_SRC_DIR%\xpcom\glue\nsDeque.cpp %TT_PATCH_DIR%\nsDeque.cpp.patch




REM SocketConnectContinue:
patch --dry-run -N %TT_MOZ_SRC_DIR%\nsprpub\pr\src\io\prsocket.c %TT_PATCH_DIR%\prsocket.c.patch




REM avoid optimizer bug with MSVC PGO (Bug 595653):
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\Makefile.in %TT_PATCH_DIR%\js_src_Makefile.in.patch

REM js double hashing:
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\jsdhash.cpp %TT_PATCH_DIR%\jsdhash.cpp.patch

REM use function version of floor() under -Oi -arch:SSE2
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\jsmath.cpp %TT_PATCH_DIR%\jsmath.cpp.patch

REM avoid internal compiler error when applying MSVC PGO:
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\methodjit\Compiler.cpp %TT_PATCH_DIR%\Compiler.cpp.patch

REM avoid the bug of PGO:
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\yarr\YarrInterpreter.cpp %TT_PATCH_DIR%\YarrInterpreter.cpp.patch

REM use SSE2 copy for NewObjectCache::copyCachedToObject():
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\jsobjinlines.h %TT_PATCH_DIR%\jsobjinlines.h.patch

REM use PodCopy in VectorImpl::copyConstruct(wchar_t *, const wchar_t *, const wchar_t *):
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\public\Vector.h %TT_PATCH_DIR%\Vector.h.patch




REM avoid compile errors with VC8 (and maybe VC9):
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\jsapi.cpp %TT_PATCH_DIR%\jsapi.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\jsiter.cpp %TT_PATCH_DIR%\jsiter.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\vm\ObjectImpl.h %TT_PATCH_DIR%\ObjectImpl.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\vm\ObjectImpl.cpp %TT_PATCH_DIR%\ObjectImpl.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\jsreflect.cpp %TT_PATCH_DIR%\jsreflect.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\builtin\Eval.cpp %TT_PATCH_DIR%\Eval.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\js\src\methodjit\PolyIC.cpp %TT_PATCH_DIR%\PolyIC.cpp.patch

REM avoid compile errors with VC8:
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\base\public\nsINode.h %TT_PATCH_DIR%\nsINode.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\skia\src\gpu\gl\GrGpuGL.h %TT_PATCH_DIR%\GrGpuGL.h.patch




REM gklayout:
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\style\nsCSSRuleProcessor.cpp %TT_PATCH_DIR%\nsCSSRuleProcessor.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\base\nsDisplayList.cpp %TT_PATCH_DIR%\nsDisplayList.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\generic\nsHTMLReflowState.cpp %TT_PATCH_DIR%\nsHTMLReflowState.cpp.patch

REM avoid the fatal error C1001 of compiler when applying PGO:
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\style\nsCSSDataBlock.cpp %TT_PATCH_DIR%\nsCSSDataBlock.cpp.patch

REM avoid crash at the time of printing when applying PGO:
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\generic\nsContainerFrame.cpp %TT_PATCH_DIR%\nsContainerFrame.cpp.patch




REM libpixman:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\Makefile.in %TT_PATCH_DIR%\libpixman_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-private.h %TT_PATCH_DIR%\pixman-private.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-implementation.c %TT_PATCH_DIR%\pixman-implementation.c.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-edge.c %TT_PATCH_DIR%\pixman-edge.c.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-sse2.c %TT_PATCH_DIR%\pixman-sse2.c.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\libpixman\src\pixman-trap.c %TT_PATCH_DIR%\pixman-trap.c.patch




REM brush caches for cairo d2d:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\cairo\src\cairo-d2d-private.h %TT_PATCH_DIR%\cairo-d2d-private.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\cairo\src\cairo-d2d-surface.cpp %TT_PATCH_DIR%\cairo-d2d-surface.cpp.patch




REM avoid compile error C2061 and C2065 with VC8:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\moz-d2d1-1.h %TT_PATCH_DIR%\gfx_2d_moz-d2d1-1.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\cairo\cairo\src\moz-d2d1-1.h %TT_PATCH_DIR%\gfx_cairo_moz-d2d1-1.h.patch

REM avoid fatal error LNK1169 with VC8:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\harfbuzz\src\hb-open-type-private.hh %TT_PATCH_DIR%\hb-open-type-private.hh.patch




REM scaled image cache:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\thebes\gfxContext.h %TT_PATCH_DIR%\gfxContext.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\base\nsLayoutUtils.cpp %TT_PATCH_DIR%\nsLayoutUtils.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\image\src\imgFrame.cpp %TT_PATCH_DIR%\imgFrame.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\thebes\gfxUtils.cpp %TT_PATCH_DIR%\gfxUtils.cpp.patch

REM NULL pointer check for entry->GetRequest():
patch --dry-run -N %TT_MOZ_SRC_DIR%\image\src\imgLoader.cpp %TT_PATCH_DIR%\imgLoader.cpp.patch




REM 2D:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\Makefile.in %TT_PATCH_DIR%\gfx_2d_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\2D.h %TT_PATCH_DIR%\2D.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\Factory.cpp %TT_PATCH_DIR%\Factory.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\Blur.cpp %TT_PATCH_DIR%\Blur.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\DrawTargetD2D.cpp %TT_PATCH_DIR%\DrawTargetD2D.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\SSEHelpers.h %TT_PATCH_DIR%\SSEHelpers.h.patch




REM call To***Pixels instead of ScaleTo***Pixels, if possible:
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\base\FrameLayerBuilder.h %TT_PATCH_DIR%\FrameLayerBuilder.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\base\FrameLayerBuilder.cpp %TT_PATCH_DIR%\FrameLayerBuilder.cpp.patch

REM use SSE4.1 in some functions of nsRect, if available:
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\2d\BaseRect.h %TT_PATCH_DIR%\BaseRect.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\gfx\src\nsRect.h %TT_PATCH_DIR%\nsRect.h.patch




REM content:
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\base\src\nsContentUtils.cpp %TT_PATCH_DIR%\nsContentUtils.cpp.patch

REM avoid crash when applying PGO:
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\media\Makefile.in %TT_PATCH_DIR%\content_media_Makefile.in.patch




REM svg:
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\svg\content\src\Makefile.in %TT_PATCH_DIR%\svg_content_src_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\layout\svg\Makefile.in %TT_PATCH_DIR%\svg_Makefile.in.patch




REM canvas:
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\canvas\src\Makefile.in %TT_PATCH_DIR%\canvas_src_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\canvas\src\CanvasUtils.cpp %TT_PATCH_DIR%\CanvasUtils.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\canvas\src\CanvasUtils.h %TT_PATCH_DIR%\CanvasUtils.h.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\content\canvas\src\CanvasRenderingContext2D.cpp %TT_PATCH_DIR%\CanvasRenderingContext2D.cpp.patch




REM plug-in:
patch --dry-run -N %TT_MOZ_SRC_DIR%\dom\plugins\base\nsPluginNativeWindowWin.cpp %TT_PATCH_DIR%\nsPluginNativeWindowWin.cpp.patch




REM make NS_ASSERT() the same behavior as official builds:
patch --dry-run -N %TT_MOZ_SRC_DIR%\toolkit\content\debug.js %TT_PATCH_DIR%\debug.js.patch




REM avoid an error LNK1120 for tete009 builds:
patch --dry-run -N %TT_MOZ_SRC_DIR%\security\nss\lib\freebl\Makefile %TT_PATCH_DIR%\freebl_Makefile.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\security\nss\lib\freebl\manifest.mn %TT_PATCH_DIR%\freebl_manifest.mn.patch




REM embeds OpenMP manifest for tete009 builds:
patch --dry-run -N %TT_MOZ_SRC_DIR%\toolkit\xre\nsWindowsWMain.cpp %TT_PATCH_DIR%\nsWindowsWMain.cpp.patch




REM apply the patch of bug 526038 to avoid crash:
patch --dry-run -N %TT_MOZ_SRC_DIR%\image\decoders\icon\win\nsIconChannel.cpp %TT_PATCH_DIR%\nsIconChannel.cpp.patch




REM jemalloc:
patch --dry-run -N %TT_MOZ_SRC_DIR%\mozglue\build\mozglue.def.in %TT_PATCH_DIR%\mozglue.def.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\memory\mozjemalloc\jemalloc.c %TT_PATCH_DIR%\jemalloc.c.patch




REM nsWindow:
patch --dry-run -N %TT_MOZ_SRC_DIR%\widget\windows\nsWindow.cpp %TT_PATCH_DIR%\nsWindow.cpp.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\widget\windows\WinUtils.cpp %TT_PATCH_DIR%\WinUtils.cpp.patch




REM set MOZ_UA_BUILDID=20100101:
patch --dry-run -N %TT_MOZ_SRC_DIR%\browser\branding\aurora\configure.sh %TT_PATCH_DIR%\branding_aurora_configure.sh.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\browser\branding\nightly\configure.sh %TT_PATCH_DIR%\branding_nightly_configure.sh.patch




REM installer:
patch --dry-run -N %TT_MOZ_SRC_DIR%\browser\installer\Makefile.in %TT_PATCH_DIR%\browser_installer_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\..\mail\installer\Makefile.in %TT_PATCH_DIR%\mail_installer_Makefile.in.patch




REM avoid the fatal error C1001 of compiler when applying PGO to Thunderbird:
patch --dry-run -N %TT_MOZ_SRC_DIR%\ipc\ipdl\Makefile.in %TT_PATCH_DIR%\ipc_ipdl_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\dom\plugins\ipc\Makefile.in %TT_PATCH_DIR%\dom_plugins_ipc_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\dom\ipc\Makefile.in %TT_PATCH_DIR%\dom_ipc_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\netwerk\base\src\Makefile.in %TT_PATCH_DIR%\netwerk_base_src_Makefile.in.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\security\manager\ssl\src\Makefile.in %TT_PATCH_DIR%\security_manager_ssl_src_Makefile.in.patch




REM patches of Bug 775939:
patch --dry-run -N %TT_MOZ_SRC_DIR%\media\webrtc\trunk\tools\gyp\pylib\gyp\generator\mozmake.py %TT_PATCH_DIR%\mozmake.py.patch
patch --dry-run -N %TT_MOZ_SRC_DIR%\client.mk %TT_PATCH_DIR%\client.mk.patch




