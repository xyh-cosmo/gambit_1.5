# Package information
PKGNAME         := FlexibleSUSY
VERSION         := 0.5.3
ABSBASEDIR      := /home/peter/Gambit/GAMBIT/modules/SpecBit/MSSM/

LIBEXT          := .a

MODEL           := MSSM

MODELDIR        := $(ABSBASEDIR)/models/$(MODEL)
CONFIGDIR       := $(ABSBASEDIR)/config
FLEXIDIR        := $(ABSBASEDIR)/src
LEGACYDIR       := $(ABSBASEDIR)/legacy

INCMODEL        := -I$(MODELDIR)
INCCONFIG       := -I$(CONFIGDIR)
INCFLEXI        := -I$(FLEXIDIR)
INCLEGACY       := -I$(LEGACYDIR)
LIBMODEL        := $(MODELDIR)/lib$(MODEL)$(LIBEXT)
LIBFLEXI        := $(FLEXIDIR)/libflexisusy$(LIBEXT)
LIBLEGACY       := $(LEGACYDIR)/liblegacy$(LIBEXT)

# Variables for compilation
CXX             := g++
CPPFLAGS        :=  -I. $(INCCONFIG) $(INCFLEXI) $(INCLEGACY) \
                   $(INCMODEL)
CXXFLAGS        := -std=c++11 -O2
CXX_DEP_GEN     := g++
CXXFLAGS_DEP_GEN:= -std=c++11
FC              := gfortran
FFLAGS          := -O2 -frecursive
FLIBS           := -L/usr/lib/gcc/x86_64-linux-gnu/4.6/ -lgfortran -lm
FOR_DEP_GEN     := gfortran
BOOSTTESTLIBS   := -lboost_unit_test_framework
BOOSTTHREADLIBS := 
BOOSTFLAGS      := 
EIGENFLAGS      := -I/usr/include/eigen3
GSLLIBS         := -L/usr/lib -lgsl -lgslcblas -lm
GSLFLAGS        := -I/usr/include
LAPACKLIBS      := -llapack
LOOPTOOLSFLAGS  := 
LOOPTOOLSLIBS   := 
THREADLIBS      := -lpthread

# Switches
ENABLE_LOOPTOOLS := no
ENABLE_THREADS   := yes

STANDALONE_SRC := \
		standalone.cpp

STANDALONE_OBJ := \
		$(patsubst %.cpp, %.o, $(filter %.cpp, $(STANDALONE_SRC))) \
		$(patsubst %.f, %.o, $(filter %.f, $(STANDALONE_SRC)))

STANDALONE_DEP := \
		$(STANDALONE_OBJ:.o=.d)

STANDALONE_EXE := \
		standalone.x

# returns file name with absolute path, taking whitespace in directory
# names into account
abspathx        = $(foreach name,$(1),\
		$(shell echo '$(abspath $(name))' | sed s/\[\[:space:\]\]/\\\\\&/g))

.PHONY:         all clean clean-dep clean-obj distclean showbuild

all: $(STANDALONE_EXE)

clean-dep:
		-rm -f $(STANDALONE_DEP)

clean-obj:
		-rm -f $(STANDALONE_OBJ)

clean: clean-dep clean-obj
		-rm -f $(STANDALONE_EXE)

distclean: clean
		-rm -f Makefile

$(STANDALONE_DEP) $(STANDALONE_OBJ): CPPFLAGS += $(GSLFLAGS) $(EIGENFLAGS) $(BOOSTFLAGS)

ifeq ($(ENABLE_LOOPTOOLS),yes)
$(STANDALONE_DEP) $(STANDALONE_OBJ): CPPFLAGS += $(LOOPTOOLSFLAGS)
endif

$(STANDALONE_EXE): $(STANDALONE_OBJ) $(LIBMODEL) $(LIBFLEXI) $(LIBLEGACY)
		$(CXX) -o $@ $(call abspathx,$^) $(GSLLIBS) $(BOOSTTHREADLIBS) $(THREADLIBS) $(LAPACKLIBS) $(FLIBS) $(LOOPTOOLSLIBS)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),clean-dep)
ifneq ($(MAKECMDGOALS),clean-obj)
ifneq ($(MAKECMDGOALS),distclean)
ifneq ($(MAKECMDGOALS),showbuild)
-include $(STANDALONE_DEP)
endif
endif
endif
endif
endif

%.d: %.cpp
# -MT '$*.o' ensures that the target contains the full path
	$(CXX_DEP_GEN) $(CPPFLAGS) $(CXXFLAGS_DEP_GEN) -MM -MP -MG -o $@ -MT '$*.o' $^

%.d: %.f
# the sed script ensures that the target contains the full path
	$(FOR_DEP_GEN) $(CPPFLAGS) -cpp -MM -MP -MG $^ -MT '$*.o' | \
	sed 's|.*\.o:|$*.o:|' > $@

showbuild:
	@echo "# package information"
	@echo "PKGNAME            = $(PKGNAME)"
	@echo "VERSION            = $(VERSION)"
	@echo "ABSBASEDIR         = $(ABSBASEDIR)"
	@echo ""
	@echo "# linked FlexibleSUSY libraries"
	@echo "MODEL              = $(MODEL)"
	@echo "MODELDIR           = $(MODELDIR)"
	@echo "FLEXIDIR           = $(FLEXIDIR)"
	@echo "LEGACYDIR          = $(LEGACYDIR)"
	@echo "LIBMODEL           = $(LIBMODEL)"
	@echo "LIBFLEXI           = $(LIBFLEXI)"
	@echo "LIBLEGACY          = $(LIBLEGACY)"
	@echo ""
	@echo "# compilation information"
	@echo "CXX                = $(CXX)"
	@echo "CPPFLAGS           = $(CPPFLAGS)"
	@echo "CXXFLAGS           = $(CXXFLAGS)"
	@echo "CXX_DEP_GEN        = $(CXX_DEP_GEN)"
	@echo "CXXFLAGS_DEP_GEN   = $(CXXFLAGS_DEP_GEN)"
	@echo "FC                 = $(FC)"
	@echo "FFLAGS             = $(FFLAGS)"
	@echo "FLIBS              = $(FLIBS)"
	@echo "FOR_DEP_GEN        = $(FOR_DEP_GEN)"
	@echo "LIBEXT             = $(LIBEXT)"
	@echo "BOOSTTESTLIBS      = $(BOOSTTESTLIBS)"
	@echo "BOOSTTHREADLIBS    = $(BOOSTTHREADLIBS)"
	@echo "BOOSTFLAGS         = $(BOOSTFLAGS)"
	@echo "EIGENFLAGS         = $(EIGENFLAGS)"
	@echo "GSLLIBS            = $(GSLLIBS)"
	@echo "GSLFLAGS           = $(GSLFLAGS)"
	@echo "LAPACKLIBS         = $(LAPACKLIBS)"
	@echo "LOOPTOOLSFLAGS     = $(LOOPTOOLSFLAGS)"
	@echo "LOOPTOOLSLIBS      = $(LOOPTOOLSLIBS)"
	@echo "THREADLIBS         = $(THREADLIBS)"
	@echo ""
	@echo "ENABLE_LOOPTOOLS   = $(ENABLE_LOOPTOOLS)"
	@echo "ENABLE_THREADS     = $(ENABLE_THREADS)"
