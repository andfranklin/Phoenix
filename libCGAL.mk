# A minimal makefile that mimics the cmake process in CGAL
MAJOR_VERSION  := 13
MINOR_VERSION  := 0.1

VERSION        := $(MAJOR_VERSION).$(MINOR_VERSION)
COMPAT_VERSION := $(MAJOR_VERSION).0.0

PHOENIX_DIR    ?= $(shell pwd | sed "s/\(phoenix\).*/\1/g")
CGAL_DIR       := $(PHOENIX_DIR)/contrib/cgal

# ----------------------------------------------------------------------
# Build miniconda info

MINICONDA_LOC     := $(shell conda info --root)
MINICONDA_LIBS    := $(MINICONDA_LOC)/lib
MINICONDA_INCLUDE := $(MINICONDA_LOC)/include

# ----------------------------------------------------------------------
# Build boost lib locations

BOOST_ROOT ?= $(MINICONDA_LOC)
BOOST_INCLUDE := $(BOOST_ROOT)/include
BOOST_LIB := $(BOOST_ROOT)/lib

# ----------------------------------------------------------------------
# Library names

CGAL_DYLIB := $(CGAL_DIR)/libCGAL-$(METHOD).la
CGAL_CORE_DYLIB := $(CGAL_DIR)/libCGAL_Core-$(METHOD).la

# ----------------------------------------------------------------------
# Find CGAL source files

CGAL_SRC_FILES := $(shell find $(CGAL_DIR)/Installation/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Intersections_2/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Kernel_23/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Kinetic_data_structures/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Number_types/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Profiling_tools/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Random_numbers/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/STL_Extension/src/CGAL -name "*.cpp")
CGAL_SRC_FILES += $(shell find $(CGAL_DIR)/Stream_support/src/CGAL -name "*.cpp")
CGAL_OBJ_FILES := $(patsubst %.cpp, %.$(obj-suffix), $(CGAL_SRC_FILES))
CGAL_deps := $(patsubst %.$(obj-suffix), %.$(obj-suffix).d, $(CGAL_OBJ_FILES))

# ----------------------------------------------------------------------
# Find CGAL_Core source files

CGAL_CORE_SRC_FILES := $(shell find $(CGAL_DIR)/CGAL_Core/src/CGAL_Core -name "*.cpp")
CGAL_CORE_OBJ_FILES := $(patsubst %.cpp, %.$(obj-suffix), $(CGAL_CORE_SRC_FILES))
CGAL_CORE_deps := $(patsubst %.$(obj-suffix), %.$(obj-suffix).d, $(CGAL_CORE_OBJ_FILES))

# Additional includes needed
app_INCLUDES += -I$(MINICONDA_INCLUDE)
app_INCLUDES += -I$(BOOST_INCLUDE)
app_INCLUDES += -I$(CGAL_DIR)/include
app_INCLUDES += -I$(CGAL_DIR)/Algebraic_foundations/include
app_INCLUDES += -I$(CGAL_DIR)/Arithmetic_kernel/include
app_INCLUDES += -I$(CGAL_DIR)/Cartesian_kernel/include
app_INCLUDES += -I$(CGAL_DIR)/CGAL_Core/include
app_INCLUDES += -I$(CGAL_DIR)/Distance_2/include
app_INCLUDES += -I$(CGAL_DIR)/Distance_3/include
app_INCLUDES += -I$(CGAL_DIR)/Installation/include
app_INCLUDES += -I$(CGAL_DIR)/Intersections_2/include
app_INCLUDES += -I$(CGAL_DIR)/Intersections_3/include
app_INCLUDES += -I$(CGAL_DIR)/Interval_support/include
app_INCLUDES += -I$(CGAL_DIR)/Kernel_23/include
app_INCLUDES += -I$(CGAL_DIR)/Kinetic_data_structures/include
app_INCLUDES += -I$(CGAL_DIR)/Modular_arithmetic/include
app_INCLUDES += -I$(CGAL_DIR)/Number_types/include
app_INCLUDES += -I$(CGAL_DIR)/Profiling_tools/include
app_INCLUDES += -I$(CGAL_DIR)/Random_numbers/include
app_INCLUDES += -I$(CGAL_DIR)/STL_Extension/include
app_INCLUDES += -I$(CGAL_DIR)/Stream_support/include


# ----------------------------------------------------------------------
# Build compiler flags for both object files that are used in the cgal
# code

CGAL_LICENSE_FLAG = -DCGAL_LICENSE_WARNING
# CGAL_LICENSE_FLAG = -DCGAL_LICENSE_ERROR

CGAL_DEFINES := CGAL_EXPORTS
CGAL_DEFINES += CGAL_USE_GMP
CGAL_DEFINES += CGAL_USE_MPFR
CXX_DEFINES := $(addprefix -D, $(CGAL_DEFINES))

# ----------------------------------------------------------------------
# Build rules

.PHONY: cleanCGAL

# build CGAL dylib
$(CGAL_DYLIB) : $(CGAL_OBJ_FILES)
	@echo "Linking Library "$@"..."
	@$(libmesh_LIBTOOL) --tag=CC $(LIBTOOLFLAGS) --mode=link --quiet \
	  $(libmesh_CC) $(libmesh_CFLAGS) -o $@ $(CGAL_OBJ_FILES) $(libmesh_LDFLAGS) $(EXTERNAL_FLAGS) -rpath $(CGAL_DIR)
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(CGAL_DYLIB) $(CGAL_DIR)

# build CGAL_Core dylib
$(CGAL_CORE_DYLIB) : $(CGAL_DYLIB) $(CGAL_CORE_OBJ_FILES)
	@echo "Linking Library "$@"..."
	@$(libmesh_LIBTOOL) --tag=CC $(LIBTOOLFLAGS) --mode=link --quiet \
	  $(libmesh_CC) $(libmesh_CFLAGS) -o $@ $(CGAL_CORE_OBJ_FILES) $(libmesh_LDFLAGS) $(EXTERNAL_FLAGS) $(CGAL_DYLIB) -rpath $(CGAL_DIR)
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(CGAL_CORE_DYLIB) $(CGAL_DIR)

app_LIBS += $(CGAL_CORE_DYLIB)
app_LIBS += $(CGAL_DYLIB)

ADDITIONAL_CPPFLAGS += $(CGAL_LICENSE_FLAG)
ADDITIONAL_CPPFLAGS += $(CXX_DEFINES)
ADDITIONAL_CPPFLAGS += -Wno-parentheses-equality

ADDITIONAL_LIBS += $(CGAL_CORE_DYLIB)
ADDITIONAL_LIBS += $(CGAL_DYLIB)

# gmp, mpfr
ADDITIONAL_LIBS += -Wl,-rpath,$(MINICONDA_LIBS) -L$(MINICONDA_LIBS) -lgmp -lmpfr
# boost
ADDITIONAL_LIBS += -Wl,-rpath,$(BOOST_LIB) -L$(BOOST_LIB) -lboost_thread -lboost_system -lboost_chrono -lboost_date_time -lboost_atomic


$(app_EXEC): $(CGAL_CORE_DYLIB) $(CGAL_DYLIB)

-include $(CGAL_CORE_deps)
-include $(CGAL_deps)

cleanCGAL:
	@echo "Cleaning CGAL"
	@rm -f $(CGAL_OBJ_FILES)
	@rm -f $(CGAL_deps)
	@rm -f $(CGAL_DYLIB)
	@rm -f $(CGAL_CORE_OBJ_FILES)
	@rm -f $(CGAL_deps)
	@rm -f $(CGAL_CORE_DYLIB)
	@rm -f $(CGAL_DIR)/libCGAL*-$(METHOD)*.dylib
	@rm -f $(CGAL_DIR)/libCGAL*-$(METHOD)*.so*
	@rm -f $(CGAL_DIR)/libCGAL*-$(METHOD)*.a