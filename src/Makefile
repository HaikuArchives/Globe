NAME= Globe
TYPE= APP
SRCS= RHTML_app.cpp RHTML_win.cpp RHTML_numview.cpp RHTML_savewin.cpp RHTML_aboutwin.cpp RHTML_prefwin.cpp RHTML_fmenu.cpp RHTML_buttonmenu.cpp RHTML_bmenu.cpp RHTML_parammenu.cpp RHTML_colorwin.cpp RHTML_amenu.cpp RHTML_textview.cpp RHTML_tabbutton.cpp RHTML_tagmenu.cpp RHTML_filetypes.cpp RHTML_options.cpp ToolTip/TToolTip.cpp Timer/RTimer.cpp URLView/URLView.cpp Tool/RadioBar.cpp Tool/ToolBar.cpp Tool/ToolItem.cpp Tool/ToolView.cpp
RSRCS= Globe.rsrc
LIBS= /boot/system/lib/libbe.so /boot/system/lib/libtextencoding.so /boot/system/lib/libtracker.so /boot/system/lib/libroot.so /boot/system/lib/libtranslation.so /boot/system/lib/libgame.so /boot/system/lib/libstdc++.so
LIBPATHS=
SYSTEM_INCLUDE_PATHS= /boot/system/develop/headers/be /boot/system/develop/headers/cpp /boot/system/develop/headers/posix /boot/system/develop/headers/os/storage /boot/system/develop/lib /boot/system/beos/system/lib
LOCAL_INCLUDE_PATHS= ./../../../config/lib ./Genesis ToolTip Timer URLView Tool
OPTIMIZE=FULL
#	specify any preprocessor symbols to be defined.  The symbols will not
#	have their values set automatically; you must supply the value (if any)
#	to use.  For example, setting DEFINES to "DEBUG=1" will cause the
#	compiler option "-DDEBUG=1" to be used.  Setting DEFINES to "DEBUG"
#	would pass "-DDEBUG" on the compiler's command line.
DEFINES=
#	specify special warning levels
#	if unspecified default warnings will be used
#	NONE = supress all warnings
#	ALL = enable all warnings
WARNINGS =
# Build with debugging symbols if set to TRUE
SYMBOLS=
COMPILER_FLAGS=-Woverloaded-virtual -funsigned-bitfields -D_HAIKU_
LINKER_FLAGS=

## include the makefile-engine
include /boot/system/develop/etc/makefile-engine
