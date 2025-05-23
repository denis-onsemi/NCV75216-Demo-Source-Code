#############################################################################
# Makefile for building: P215
# Generated by qmake (2.01a) (Qt 4.8.6) on: Mon May 12 15:48:20 2025
# Project:  project_app.pro
# Template: app
# Command: c:\Qt\4.8.6\bin\qmake.exe -spec c:\Qt\4.8.6\mkspecs\win32-msvc2010 CONFIG+=release -o Makefile project_app.pro
#############################################################################

first: release
install: release-install
uninstall: release-uninstall
MAKEFILE      = Makefile
QMAKE         = c:\Qt\4.8.6\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = $(COPY)
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = del
SYMLINK       = 
DEL_DIR       = rmdir
MOVE          = move
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
SUBTARGETS    =  \
		release \
		debug

release: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release
release-make_default: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release 
release-make_first: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release first
release-all: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_default: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug 
debug-make_first: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug first
debug-all: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: project_app.pro  c:\Qt\4.8.6\mkspecs\win32-msvc2010\qmake.conf c:\Qt\4.8.6\mkspecs\qconfig.pri \
		c:\Qt\4.8.6\mkspecs\modules\qt_webkit_version.pri \
		c:\Qt\4.8.6\mkspecs\features\qt_functions.prf \
		c:\Qt\4.8.6\mkspecs\features\qt_config.prf \
		c:\Qt\4.8.6\mkspecs\features\exclusive_builds.prf \
		c:\Qt\4.8.6\mkspecs\features\default_pre.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\default_pre.prf \
		bench\generic\controlboard\controlboard.pri \
		bench\generic\controls\customcontrols.pri \
		bench\generic\generic.pri \
		bench\bench.pri \
		c:\Qt\4.8.6\mkspecs\features\release.prf \
		c:\Qt\4.8.6\mkspecs\features\debug_and_release.prf \
		c:\Qt\4.8.6\mkspecs\features\default_post.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\default_post.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\rtti.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\exceptions.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\stl.prf \
		c:\Qt\4.8.6\mkspecs\features\shared.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\embed_manifest_exe.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\embed_manifest_dll.prf \
		c:\Qt\4.8.6\mkspecs\features\warn_on.prf \
		c:\Qt\4.8.6\mkspecs\features\qt.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\thread.prf \
		c:\Qt\4.8.6\mkspecs\features\moc.prf \
		c:\Qt\4.8.6\mkspecs\features\win32\windows.prf \
		c:\Qt\4.8.6\mkspecs\features\resources.prf \
		c:\Qt\4.8.6\mkspecs\features\uic.prf \
		c:\Qt\4.8.6\mkspecs\features\yacc.prf \
		c:\Qt\4.8.6\mkspecs\features\lex.prf \
		c:\Qt\4.8.6\mkspecs\features\include_source_dir.prf \
		c:\Qt\4.8.6\lib\qtmain.prl \
		c:\Qt\4.8.6\lib\qaxcontainer.prl
	$(QMAKE) -spec c:\Qt\4.8.6\mkspecs\win32-msvc2010 CONFIG+=release -o Makefile project_app.pro
c:\Qt\4.8.6\mkspecs\qconfig.pri:
c:\Qt\4.8.6\mkspecs\modules\qt_webkit_version.pri:
c:\Qt\4.8.6\mkspecs\features\qt_functions.prf:
c:\Qt\4.8.6\mkspecs\features\qt_config.prf:
c:\Qt\4.8.6\mkspecs\features\exclusive_builds.prf:
c:\Qt\4.8.6\mkspecs\features\default_pre.prf:
c:\Qt\4.8.6\mkspecs\features\win32\default_pre.prf:
bench\generic\controlboard\controlboard.pri:
bench\generic\controls\customcontrols.pri:
bench\generic\generic.pri:
bench\bench.pri:
c:\Qt\4.8.6\mkspecs\features\release.prf:
c:\Qt\4.8.6\mkspecs\features\debug_and_release.prf:
c:\Qt\4.8.6\mkspecs\features\default_post.prf:
c:\Qt\4.8.6\mkspecs\features\win32\default_post.prf:
c:\Qt\4.8.6\mkspecs\features\win32\rtti.prf:
c:\Qt\4.8.6\mkspecs\features\win32\exceptions.prf:
c:\Qt\4.8.6\mkspecs\features\win32\stl.prf:
c:\Qt\4.8.6\mkspecs\features\shared.prf:
c:\Qt\4.8.6\mkspecs\features\win32\embed_manifest_exe.prf:
c:\Qt\4.8.6\mkspecs\features\win32\embed_manifest_dll.prf:
c:\Qt\4.8.6\mkspecs\features\warn_on.prf:
c:\Qt\4.8.6\mkspecs\features\qt.prf:
c:\Qt\4.8.6\mkspecs\features\win32\thread.prf:
c:\Qt\4.8.6\mkspecs\features\moc.prf:
c:\Qt\4.8.6\mkspecs\features\win32\windows.prf:
c:\Qt\4.8.6\mkspecs\features\resources.prf:
c:\Qt\4.8.6\mkspecs\features\uic.prf:
c:\Qt\4.8.6\mkspecs\features\yacc.prf:
c:\Qt\4.8.6\mkspecs\features\lex.prf:
c:\Qt\4.8.6\mkspecs\features\include_source_dir.prf:
c:\Qt\4.8.6\lib\qtmain.prl:
c:\Qt\4.8.6\lib\qaxcontainer.prl:
qmake: qmake_all FORCE
	@$(QMAKE) -spec c:\Qt\4.8.6\mkspecs\win32-msvc2010 CONFIG+=release -o Makefile project_app.pro

qmake_all: FORCE

make_default: release-make_default debug-make_default FORCE
make_first: release-make_first debug-make_first FORCE
all: release-all debug-all FORCE
clean: release-clean debug-clean FORCE
	-$(DEL_FILE) ".\P215.intermediate.manifest"
	-$(DEL_FILE) bin\P215.exp
distclean: release-distclean debug-distclean FORCE
	-$(DEL_FILE) Makefile

check: first

release-mocclean: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
