###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################
ACLOCAL_AMFLAGS                                             =   -I m4

# The list of header files that belong to the library (to be installed later)
HEADER_FILES                                                =   ../Includes/libARMavlink/libARMavlink.h                   \
                                                                ../Includes/libARMavlink/ARMAVLINK_FileGenerator.h        \
                                                                ../Includes/libARMavlink/ARMAVLINK_FileParser.h           \
                                                                ../Includes/libARMavlink/ARMAVLINK_Manager.h              \
                                                                ../Includes/libARMavlink/ARMAVLINK_ListUtils.h            \
                                                                ../Includes/libARMavlink/ARMAVLINK_MissionItemUtils.h     \
                                                                ../Includes/libARMavlink/ARMAVLINK_Error.h

# The sources to add to the library and to add to the source distribution
SOURCE_FILES                                                =   $(HEADER_FILES)                                           \
                                                                ../Sources/ARMAVLINK_FileGenerator.c                      \
                                                                ../Sources/ARMAVLINK_FileGenerator.h                      \
                                                                ../Sources/ARMAVLINK_FileParser.c                         \
                                                                ../Sources/ARMAVLINK_FileParser.h                         \
                                                                ../Sources/ARMAVLINK_Manager.c                            \
                                                                ../Sources/ARMAVLINK_Manager.h                            \
                                                                ../Sources/ARMAVLINK_ListUtils.c                          \
                                                                ../Sources/ARMAVLINK_ListUtils.h                          \
                                                                ../Sources/ARMAVLINK_MissionItemUtils.c                   \
                                                                ../Sources/ARMAVLINK_MissionItemUtils.h                   \
                                                                ../gen/Sources/ARMAVLINK_Error.c

# The library names to build (note we are building static and shared libs)
if DEBUG_MODE
lib_LTLIBRARIES                                             =   libarmavlink_dbg.la
libarmavlink_dbg_ladir                                       =   $(includedir)/libARMavlink
libarmavlink_dbg_la_HEADERS                                  =   $(HEADER_FILES)
libarmavlink_dbg_la_SOURCES                                  =   $(SOURCE_FILES)
else
lib_LTLIBRARIES                                             =   libarmavlink.la
libarmavlink_ladir                                           =   $(includedir)/libARMavlink
libarmavlink_la_HEADERS                                      =   $(HEADER_FILES)
libarmavlink_la_SOURCES                                      =   $(SOURCE_FILES)
endif

check_PROGRAMS                                              =   libarmavlink_autoTest

libarmavlink_autoTest_SOURCES                            =   ../TestBench/autoTest/autotest.c

if DEBUG_MODE
libarmavlink_autoTest_LDADD                              =   -larsal_dbg                 \
                                                                libarmavlink_dbg.la
else
libarmavlink_autoTest_LDADD                              =   -larsal                     \
                                                                libarmavlink.la
endif

CLEAN_FILES                                                 =   libarmavlink.la          \
                                                                libarmavlink_dbg.la

EXTRA_DIST                                                  =   bootstrap

# Ensure the distribution of the doxygen configuration file
# Doxygen support
if HAVE_DOXYGEN
include doxygen.am
EXTRA_DIST                                                  +=  doxygen.ac  \
                                                                doxygen.am  \
                                                                $(DX_CONFIG)
endif
