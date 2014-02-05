# Enable GNU make secondary expansion feature
.SECONDEXPANSION:

## Make sure the first target is "all"
all:

include Makefile.in

## Include all dependencies for objects in binaries, libraries and test
-include $(foreach var,$(lib_package:%=lib_%_src),$(patsubst %.cpp,%.d,$(call $(var))) )
-include $(foreach var,$(bin_package:%=lib_%_src),$(patsubst %.cpp,%.d,$(call $(var))) )
-include $(foreach var,$(test_package:%=lib_%_src),$(patsubst %.cpp,%.d,$(call $(var))) )

## Additional settings
clean = *.o *.txt *.fig *.bin *.e *.d *.obj

## Here begin the region you don't want to mess with ##
CFLAGS_ = $(CFLAGS) $(if $(DEBUG:0=),$(DEBUG_CFLAGS),$(RELEASE_CFLAGS))
CXXFLAGS_ = $(CXXFLAGS) $(if $(DEBUG:0=),$(DEBUG_CXXFLAGS),$(RELEASE_CXXFLAGS))
subdirs = 

lib_target = $(lib_package:%=lib%.a)
test_target = $(test_package:%=test_%)

all: $(bin_package) $(lib_target)

%.o: %.cpp 
	$(CXX) -c $(CXXFLAGS_) $(CPPFLAGS) -o $@ $<
# Generate dependencies to header files
	$(CXX) -M $(CXXFLAGS_) $< -MF $(patsubst %.o,%.d,$@)
# More tricks to manage header renaming stored in different folders (http://scottmcpeak.com/autodepend/autodepend.html)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %o,%d,$@).tmp
	@sed -e 's|.*:|$@:|' < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

$(bin_package): $$(patsubst %.cpp,%.o,$$(call $$(patsubst %,bin_%_src,$$@))) $$(call $$(patsubst %,bin_%_deps,$$@))
	$(CXX) -o $@ $(LDFLAGS) $(bin_$@_ldflags) $(bin_$@_src:.cpp=.o) $(call $(@:%=bin_$@_deps)) $(LIBS) $(bin_$@_libs)

$(lib_target): $$(patsubst %.cpp,%.o,$$(call $$(patsubst %,lib_%_src,$$(patsubst lib%.a,%,$$@))))
	ar rs $@ $(lib_$(@:lib%.a=%)_src:%.cpp=%.o) 2>&1

$(test_target): $$(patsubst %.cpp,%.o,$$(call $$(patsubst %,test_%_src,$$(patsubst test_%,%,$$@)))) $$(call $$(patsubst %,test_%_deps,$$(patsubst test_%,%,$$@)))
	$(CXX) -o $@ $(LDFLAGS_TEST) $(call $(patsubst %,test_%_ldflags, $(patsubst test_%,%,$@))) $(patsubst %.cpp,%.o, $(call $(patsubst %,test_%_src, $(patsubst test_%,%,$@)))) $(call $(patsubst %,test_%_deps, $(patsubst test_%,%,$@))) $(LIBS_TEST) $(call $(patsubst %,test_%_libs, $(patsubst test_%,%,$@)))
	
dist:
	mkdir -p $(distdir)
	cp Makefile $(distdir)
	cp Makefile.in $(distdir)
	cp $(sort $(foreach var,$(bin_package:%=bin_%_export),$(call $(var)))) $(distdir)
	cp $(sort $(foreach var,$(lib_package:%=lib_%_export),$(call $(var)))) $(distdir)
	cp $(sort $(foreach var,$(test_package:%=test_%_export),$(call $(var)))) $(distdir)
        
clean:
	$(RM) $(test_package:%=test_%)
	$(RM) $(bin_package)
	$(RM) $(patsubst %,lib%.a, $(lib_package))
	$(RM) $(clean)

check: $(test_target)
	$(shell echo for i in "$(foreach var,$(test_package),$(var))"\; do ./test_\$$i\; done)
        
install: $(bin_package) $(lib_target)
	install -d $(DESTDIR)$(libdir)
	install -m 644 $(lib_target) $(DESTDIR)$(libdir)
	install -d $(DESTDIR)$(bindir)
	install -m 755 $(bin_package) $(DESTDIR)$(bindir)

uninstall:
	$(RM) $(foreach var,$(bin_package), $(DESTDIR)$(bindir)/$(var))
	$(RM) $(foreach var,$(lib_target), $(DESTDIR)$(libdir)/$(var))
	
FORCE:
.PHONY: all clean check install uninstall dist FORCE
