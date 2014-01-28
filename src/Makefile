CFLAGS=-isystem ../include
CXXFLAGS=-isystem ../include

RELEASE_CFLAGS = -O3 -DNDEBUG
DEBUG_CFLAGS = -g -O0 -DDEBUG=1

RELEASE_CXXFLAGS = -O3 -DNDEBUG
DEBUG_CXXFLAGS = -g -O0 -DDEBUG=1

# TODO: implement bin_mystuff_dep = ... that lists the objects on which this linking or packaging need to be complete, and $(MAKE) -C $(shell dirname $(dep)) $(shell basename $(dep)) them
# to be used in binaries, libraries and linking tests to their tested objects  

LDFLAGS_TEST = 
LDFLAGS =

LIBS =
LIBS_TEST = 

## Make sure the first target is "all"
all:

## static library packages and their sources
bin_package = demo ampl_i2i ampl_i2o ampl_o2i ampl_o2o

bin_demo_src = demo.cpp
bin_demo_export = $(bin_demo_src) 
bin_demo_deps = libcppelib.a
bin_demo_libs = -lboost_regex
-include $(bin_demo_src:.cpp=.d)

bin_ampl_i2i_src = ampl_i2i.cpp
bin_ampl_i2i_export = $(bin_ampl_i2i_src) 
bin_ampl_i2i_deps = libcppelib.a
bin_ampl_i2i_libs = -lboost_regex
-include $(bin_ampl_i2i_src:.cpp=.d)

bin_ampl_i2o_src = ampl_i2o.cpp
bin_ampl_i2o_export = $(bin_ampl_i2o_src) 
bin_ampl_i2o_deps = libcppelib.a
bin_ampl_i2o_libs = -lboost_regex
-include $(bin_ampl_i2o_src:.cpp=.d)

bin_ampl_o2i_src = ampl_o2i.cpp
bin_ampl_o2i_export = $(bin_ampl_o2i_src) 
bin_ampl_o2i_deps = libcppelib.a
bin_ampl_o2i_libs = -lboost_regex
-include $(bin_ampl_o2i_src:.cpp=.d)

bin_ampl_o2o_src = ampl_o2o.cpp
bin_ampl_o2o_export = $(bin_ampl_o2o_src) 
bin_ampl_o2o_deps = libcppelib.a
bin_ampl_o2o_libs = -lboost_regex
-include $(bin_ampl_o2o_src:.cpp=.d)

## static library packages and their sources
lib_package = cppelib

lib_cppelib_src = ParseException.cpp \
CastException.cpp NoDecimalFloatException.cpp RecordParser.cpp RecordOutput.cpp AmplInput.cpp AmplOutput.cpp \
Data.cpp Record.cpp Scalar.cpp Vector.cpp Matrix.cpp DataParser.cpp DataOutput.cpp \
AmplInputData.cpp AmplOutputData.cpp AmplInputScalar.cpp AmplInputVector.cpp AmplInputSet.cpp \
AmplInputMatrix.cpp AmplOutputScalar.cpp AmplOutputVector.cpp AmplOutputSet.cpp AmplOutputMatrix.cpp \
AmplDataOutput.cpp AmplDataParser.cpp
lib_cppelib_export = $(lib_cppelib_src)
lib_cppelib_deps =
-include $(lib_cppelib_src:.cpp=.d)

## test packages and their sources
test_package = 

test__src = 
test__export = $(test__src) 
test__deps = 
test__ldflags = 

## Additional settings
clean = *.o *.txt *.fig *.bin *.e *.d *.obj

## Here begin the region you don't want to mess with ##
CFLAGS_ = $(CFLAGS) $(if $(DEBUG:0=),$(DEBUG_CFLAGS),$(RELEASE_CFLAGS))
CXXFLAGS_ = $(CXXFLAGS) $(if $(DEBUG:0=),$(DEBUG_CXXFLAGS),$(RELEASE_CXXFLAGS))
subdirs = 

lib_target = $(lib_package:%=lib%.a)

all: $(bin_package) $(lib_target) $(test_package)

%.o: %.cpp 
	$(CXX) -c $(CXXFLAGS_) $(CPPFLAGS) -o $@ $<
# Generate dependencies to header files
	$(CXX) -M $(CXXFLAGS_) $< -MF $(patsubst %.o,%.d,$@)
# More tricks to manage header renaming stored in different folders (http://scottmcpeak.com/autodepend/autodepend.html)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %o,%d,$@).tmp
	@sed -e 's|.*:|$@:|' < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

$(bin_package): $(foreach var,$(bin_package:%=bin_%_src),$(patsubst %.cpp,%.o,$(call $(var)))) $(foreach var,$(bin_package:%=bin_%_deps),$(call $(var)))
#	@if [ "$(strip $(call $(@:%=bin_%_deps)))" != "" ]; then $(MAKE) -C $(dir $(call $(@:%=bin_%_deps))) $(notdir $(call $(@:%=bin_%_deps))); fi
	$(CXX) -o $@ $(LDFLAGS) $(bin_$@_ldflags) $(bin_$@_src:.cpp=.o) $(call $(@:%=bin_%_deps)) $(LIBS) $(bin_$@_libs)
#	objdump -DCl $@ > $@.obj

$(lib_target): $(foreach var,$(lib_package:%=lib_%_src),$(patsubst %.cpp, %.o, $(call $(var))))
	ar rs $@ $(foreach var,$(lib_package:%=lib_%_src),$(patsubst %.cpp, %.o, $(call $(var)))) 2>&1
	
$(test_package): $(foreach var,$(test_package:%=test_%_src),$(patsubst %.cpp, %.o, $(call $(var))))
	@if [ "$(strip $(call $(@:%=test_%_deps)))" != "" ]; then $(MAKE) -C $(dir $(call $(@:%=test_%_deps))) $(notdir $(call $(@:%=test_%_deps))); fi
	$(CXX) -o $@ $(LDFLAGS_TEST) $(test_$@_ldflags) $(test_$@_src:.cpp=.o) $(call $(@:%=test_%_deps)) $(LIBS_TEST) $(bin_$@_libs_test)
	
dist:
	mkdir -p $(distdir)
	cp Makefile $(distdir)
	cp $(sort $(foreach var,$(bin_package:%=bin_%_export),$(call $(var)))) $(distdir)
	cp $(sort $(foreach var,$(lib_package:%=lib_%_export),$(call $(var)))) $(distdir)
	cp $(sort $(foreach var,$(test_package:%=test_%_export),$(call $(var)))) $(distdir)
        
clean:
	$(RM) $(test_package)
	$(RM) $(bin_package)
	$(RM) $(patsubst %,lib%.a, $(lib_package))
	$(RM) $(clean)

check: $(test_package)
	$(shell echo for i in "$(foreach var,$(test_package),$(var))"\; do ./\$$i\; done)
        
install: $(bin_package) $(lib_target)
	install -d $(DESTDIR)$(libdir)
	install -m 644 $(lib_target) $(DESTDIR)$(libdir)
	install -d $(DESTDIR)$(bindir)
	install -m 755 $(bin_package) $(DESTDIR)$(bindir)

uninstall:
	$(RM) $(DESTDIR)$(libdir)/$(lib_target)
	$(RM) $(DESTDIR)$(bindir)/$(bin_package)
	
FORCE:
.PHONY: all clean check install uninstall dist FORCE
