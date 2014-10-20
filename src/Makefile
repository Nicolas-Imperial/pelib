# Enable GNU make secondary expansion feature
.SECONDEXPANSION:

## Export all variables to sub-makefiles
export

## Make this folder as a pkgconfig search path so locally compiled libraries can be resolved before installation
export PKG_CONFIG_PATH:=.:$(PKG_CONFIG_PATH)

## Default target
DEFAULT=all

## The default target is the first target in the makefile
$(DEFAULT):
include Makefile.in

## Targets asked for running, or default target if none
TARGETS=$(if $(MAKECMDGOALS),$(MAKECMDGOALS),$(DEFAULT))

## Make all targets called dependent on some action to be run beforehand
$(TARGETS): $(FIRST)

####################################################################
##### Here begin the region you really don't want to mess with #####
####################################################################

## Gives the longest prefix in the prefix list (all elements must have the same character at the same index, or no character at that index)
# $(if $1,$(word $(words $1),$(sort $1)),)
# $(firstword $(shell echo $(sort $(shell echo $1|rev))|rev))
define longest
$(if $1,$(word $(words $1),$(sort $1)),)
endef

## Gives the source file extension of language $1
define ext
$(if $(filter cc,$1),c,$(if $(filter cxx,$1),cpp,))
endef

## Finds the longest suffix of $1 in the list $2
define suffixin
$(firstword $(shell echo $(sort $(shell echo $(foreach var,$2,$(if $(filter %$(var),$1),$(var),))|rev))|rev))
endef

## Get pkg-config output for the operation $2 (cflags|libs) on package list $1, with the corresponding options from variable $3$<package_name>
define pkg-config
$(if $(strip $1),$(if $(shell pkg-config --$2 --static $(foreach pkg,$1,"$(pkg) $(call $3$(pkg)_version)")),$(shell pkg-config --$2 --static $(foreach pkg,$1,"$(pkg) $(call $3$(pkg)_version)")),$(error Missing library)),)
endef

## Gives all source files of projects $1 of type $2 (bin|lib|test) with language $3 (cc|cxx)
define src
$(foreach var,$1,$(call $2$3_$(var)_src))
endef

## Gives source files of projects $1 of type $2 (bin|lib|test) with language $3 (cc|cxx), prefixed with their file variant
define src_file_var
$(foreach var,$1,$(foreach src,$(call $2$3_$(var)_src),$(call VARIANT_$(src))$(src)))
endef

## Gives source files of projects $1 of type $2 (bin|lib|test) with language $3 (cc|cxx), prefixed with their project and file variant
#$(foreach var,$1,$(foreach src,$(call src_file_var,$(var),$2,$3),$(call VARIANT_$1_$(src))$(src)))
$(foreach var,$1,$(foreach src,$(call src_file_var,$(var),$2,$3),$(call VARIANT_$1_$(call source,$(src:%.c=%.o),$1,$2,$3))$(src)))
define src_project_file_var
$(foreach var,$(call src_file_var,$1,$2,$3),$(call VARIANT_$1_$(call source,$(patsubst %.$(call typeof,$1),%.o,$(var)),$1,$2,$3))$(var))
endef

## Gives object files of projects in list $1, of type $2 (bin|lib|test) with language $3 (cc|cxx), prefixed with their project name, project variant and source file variant
define object
$(foreach proj,$1,$(patsubst %.$(call ext,$3),$2$3_$(proj)_%.o,$(call src_project_file_var,$(proj),$2,$3)))
endef

## Gives object files of projects in list $1, of type $2 (bin|lib|test) with language $3 (cc|cxx), prefixed with their project name, project variant and source file variant
define embed
$(foreach proj,$1,$(patsubst %,%.o,$(call $2$3_$(proj)_embed)))
endef

## Gives the source files of the embedded object files in $1
define embed_src
$(foreach var,$1,$(patsubst %.o,%,$(var)))
endef

## Gives object files of projects $1 of type $2 (bin|lib|test) with language $3 (cc|cxx), prefixed with their project name, but no project or source file variant
define object_noprefix
$(foreach proj,$1,$(patsubst %.$(call ext,$3),$2_$(proj)_%.o,$(call src,$(proj),$2,$3)))
endef

## Gives the project of the project-prefixed object file $1 of type $2 (bin|lib|test) and language $3 (cc|cxx)
$(foreach proj,$(call $2$3_package),$(if $(call suffixin,$(call object_novar,$1,$2,$3),$(proj)),$(proj),))
define projectof
$(call longest,$(foreach proj,$(call $2$3_package),$(if $(filter $2$3_$(proj)%,$1),$(proj),)))
endef

## Gives the type of project the object file $1 is part of
define typeof
$(if $(filter bin%,$1),bin,$(if $(filter lib%,$1),lib,$(if $(filter test%,$1),test,)))
endef

## Gives the source file of object $1 project $2 of $3 (bin|lib|test) in language $4 (cc_cxx)
define source
$(call suffixin,$(patsubst %.o,%.$(call ext,$4),$1),$(call src,$2,$3,$4))
endef

## Gives all makefile-wise cflags
## Overridable global CFLAG variable
## Base CFLAGS from MAKEFILE
## pkg-config CFLAGS
## Global DEBUG/RELEASE CFLAGS
## No need of a function here
GLOBAL_CFLAGS = $(CFLAGS) $(CFLAGS_BASE) $(call pkg-config,$(CC_PKG),cflags,CC_PKG_) $(if $(DEBUG:0=),$(CFLAGS_DEBUG),$(CFLAGS_RELEASE))
GLOBAL_CXXFLAGS = $(CXXFLAGS) $(CXXFLAGS_BASE) $(call pkg-config,$(CXX_PKG),cflags,CXX_PKG_) $(if $(DEBUG:0=),$(CXXFLAGS_DEBUG),$(CXXFLAGS_RELEASE))

## Gives cflags for project $1 of type $2 (bin|lib|test) in language $3 (cc|cxx)
## Project cflags
## project pkg-config cflags
define cflags_project
$(call $2$3_$1_cflags) $(call pkg-config,$(call $2$3_$1_pkg),cflags,$2$3_$1_pkg_)
endef

## Same as above for CC++ projects
define cxxflags_project
$(call $2$3_$1_cxxflags) $(call pkg-config,$(call $2$3_$1_pkg),cflags,$2$3_$1_pkg_)
endef

GLOBAL_CC_LDFLAGS=$(LDFLAGS) $(CC_LDFLAGS) $(call pkg-config,$(CC_PKG),libs-only-L,CC_PKG_)
GLOBAL_CXX_LDFLAGS=$(LDFLAGS) $(CXX_LDFLAGS) $(call pkg-config,$(CXX_PKG),libs-only-L,CXX_PKG_)

define ldflags_project
$(call $2$3_$1_ldflags) $(call pkg-config,$(call $2$3_$1_pkg),libs-only-L,$2$3_$1_pkg_)
endef

## Gives all cflags the object file $1 is ought to receive for this project of $2 (bin|lib|test) in language "cc"
## The iglobal cflag for this object file
## The project-defined cflag for this object file
define cflags_file
$(call CFLAGS_$(call source,$1,$(call projectof,$1,$2,cc),$2,cc)) $(call $2cc_$(call projectof,$1,$2,cc)_cflags_$(call source,$1,$(call projectof,$1,$2,cc),$2,cc))
endef

## Same as above for C++ projects
define cxxflags_file
$(call CXXFLAGS_$(call source,$1,$(call projectof,$1,$2,cxx),$2,cxx)) $(call $2cxx_$(call projectof,$1,$2,cxx)_cxxflags_$(call source,$1,$(call projectof,$1,$2,cxx),$2,cxx))
endef

## Gives all library switches brought by the makefile and its options
## Global pkg-config packages
## Global manual LIBS variable
GLOBAL_CC_LIBS=$(LIBS) $(CC_LIBS)
GLOBAL_CXX_LIBS=$(LIBS) $(CXX_LIBS)
GLOBAL_CC_TEST_LIBS=$(LIBS) $(CC_TEST_LIBS)
GLOBAL_CXX_TEST_LIBS=$(LIBS) $(CXX_TEST_LIBS)
GLOBAL_CC_PKG=$(call pkg-config,$(CC_PKG),libs-only-l,CC_PKG_)
GLOBAL_CXX_PKG=$(call pkg-config,$(CXX_PKG),libs-only-l,CXX_PKG_)
GLOBAL_CC_TEST_PKG=$(call pkg-config,$(CC_PKG_TEST),libs-only-l,CC_TEST_PKG_)
GLOBAL_CXX_TEST_PKG=$(call pkg-config,$(CXX_PKG_TEST),libs-only-l,CXX_TEST_PKG_)

## Gives all library switches of project $1 of type $2 (bin|lib|test) in language $3
## pkg-config provided switches from _pkg variable
## manual library switches from _libs variable
define libs_project
$($2$3_$(call suffixin,$1,$($2$3_package))_libs)
endef

define pkg_project
$(call pkg-config,$(call $2$3_$1_pkg),libs-only-l,$2$3_$1_pkg_)
endef

## All C and C++ objects, eliminating duplicates
cc_object = $(sort $(call object,$(bincc_package),bin,cc) $(call object,$(libcc_package),lib,cc) $(call object,$(testcc_package),test,cc))
cxx_object = $(sort $(call object,$(bincxx_package),bin,cxx) $(call object,$(libcxx_package),lib,cxx) $(call object,$(testcxx_package),test,cxx))
cc_pkgconfig = $(foreach var,$(libcc_package),$(patsubst %,%.pc,$(var)))
cxx_pkgconfig = $(foreach var,$(libcxx_package),$(patsubst %,%.pc,$(var)))
embed_object = $(sort $(call embed,$(bincc_package),bin,cc) $(call embed,$(libcc_package),lib,cc) $(call embed,$(testcc_package),test,cc)) $(sort $(call embed,$(bincxx_package),bin,cxx) $(call embed,$(libcxx_package),lib,cxx) $(call embed,$(testcxx_package),test,cxx))

-include $(foreach file,$(cc_object) $(cxx_object),$(patsubst %.o,%.d,$(file)))

## Build the list of targets this makefile can build
## The list of all c and c++ packages, prefixed all these packages with a specific variant
## variable that may or may not have been defined
bincc_list = $(foreach var,$(bincc_package),$(call VARIANT_$(var))$(var))
bincxx_list = $(foreach var,$(bincxx_package),$(call VARIANT_$(var))$(var))
bin_list = $(bincc_list) $(bincxx_list)

libcc_target = $(libcc_package:%=lib%.a)
libcxx_target = $(libcxx_package:%=lib%.a)
lib_target = $(libcc_package:%=lib%.a) $(libcxx_package:%=lib%.a)
libcc_list = $(foreach var,$(libcc_package),lib$(call VARIANT_lib$(var).a)$(var).a)
libcxx_list = $(foreach var,$(libcxx_package),lib$(call VARIANT_lib$(var).a)$(var).a)
lib_list = $(libcc_list) $(libcxx_list)

testcc_target = $(testcc_package:%=test_%)
testcxx_target = $(testcxx_package:%=test_%)
test_target = $(testcc_package:%=test_%) $(testcxx_package:%=test_%)
testcc_list = $(foreach var,$(testcc_package),test_$(call VARIANT_test_$(var))$(var))
testcxx_list = $(foreach var,$(testcxx_package),test_$(call VARIANT_test_$(var))$(var))
test_list = $(testcc_list) $(testcxx_list)

## Disable implicit rules
.SUFFIXES:

all: submake $(bin_list) $(lib_list)

## Force rebuild upon Makefile update
$(cc_object) $(cxx_object) $(embed_object) $(cc_pkgconfig) $(cxx_pkgconfig): Makefile Makefile.in

$(embed_object): $$(call embed_src,$$@)
	$(LD) -r -b binary -o $@ $(patsubst %.o,%,$@)

$(cc_object): $$(call source,$$@,$$(call projectof,$$@,$$(call typeof,$$@),cc),$$(call typeof,$$@),cc)
	$(CC) -c $(GLOBAL_CFLAGS) $(call cflags_project,$(call projectof,$@,$(call typeof,$@),cc),$(call typeof,$@),cc) $(call cflags_file,$@,$(call typeof,$@)) $(CPPFLAGS) -o $@ $<
	$(CC) -M $(GLOBAL_CFLAGS) $(call cflags_project,$(call projectof,$@,$(call typeof,$@),cc),$(call typeof,$@),cc) $(call cflags_file,$@,$(call typeof,$@)) $(CPPFLAGS) $< -MF $(patsubst %.o,%.d,$@)
	@# Generate dependencies to header files
	@# More tricks to manage header renaming stored in different folders (http://scottmcpeak.com/autodepend/autodepend.html)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %o,%d,$@).tmp
	@sed -e 's|.*:|$@:|' < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

$(cxx_object): $$(call source,$$@,$$(call projectof,$$@,$$(call typeof,$$@),cxx),$$(call typeof,$$@),cxx)
	$(CXX) -c $(GLOBAL_CXXFLAGS) $(call cxxflags_project,$(call projectof,$@,$(call typeof,$@),cxx),$(call typeof,$@),cxx) $(call cxxflags_file,$@,$(call typeof,$@)) $(CPPFLAGS) -o $@ $<
	$(CXX) -M $(GLOBAL_CXXFLAGS) $(call cxxflags_project,$(call projectof,$@,$(call typeof,$@),cxx),$(call typeof,$@),cxx) $(call cxxflags_file,$@,$(call typeof,$@)) $(CPPFLAGS) $< -MF $(patsubst %.o,%.d,$@)
	@# Generate dependencies to header files
	@# More tricks to manage header renaming stored in different folders (http://scottmcpeak.com/autodepend/autodepend.html)
	@mv -f $(patsubst %.o,%.d,$@) $(patsubst %o,%d,$@).tmp
	@sed -e 's|.*:|$@:|' < $(patsubst %.o,%.d,$@).tmp > $(patsubst %.o,%.d,$@)
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst %.o,%.d,$@).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst %.o,%.d,$@)
	@rm -f $(patsubst %.o,%.d,$@).tmp

$(bincc_list): $$(call object,$$(call suffixin,$$@,$$(bincc_package)),bin,cc) $$(call $$(patsubst %,bincc_%_deps,$$(call suffixin,$$@,$$(bincc_package)))) $$(call embed,$$(call suffixin,$$@,$$(bincc_package)),bin,cc)
	$(CC) -o $@ $(GLOBAL_CC_LDFLAGS) $(call ldflags_project,$(call suffixin,$@,$(bincc_package),bin,cc),bin,cc) $(call object,$(call suffixin,$@,$(bincc_package)),bin,cc) $(call embed,$(call suffixin,$@,$(bincc_package)),bin,cc) $(GLOBAL_CC_LIBS) $(call libs_project,$@,bin,cc) $(GLOBAL_CC_PKG) $(call pkg_project,$@,bin,cc)

$(bincxx_list): $$(call object,$$(call suffixin,$$@,$$(bincxx_package)),bin,cxx) $$(call $$(patsubst %,bincxx_%_deps,$$(call suffixin,$$@,$$(bincxx_package)))) $$(call embed,$$(call suffixin,$$@,$$(bincxx_package)),bin,cxx)
	$(CXX) -o $@ $(GLOBAL_CXX_LDFLAGS) $(call ldflags_project,$(call suffixin,$@,$(bincxx_package),bin,cxx),bin,cxx) $(call object,$(call suffixin,$@,$(bincxx_package)),bin,cxx) $(call embed,$(call suffixin,$@,$(bincxx_package)),bin,cxx) $(GLOBAL_CXX_LIBS) $(call libs_project,$@,bin,cxx) $(GLOBAL_CXX_PKG) $(call pkg_project,$@,bin,cxx)

bin_package: $(bin_list)

$(libcc_list): $$(call object,$$(patsubst lib%.a,%,$$@),lib,cc) $$(call embed,$$(patsubst lib%.a,%,$$@),lib,cc) $$(call $$(patsubst %,libcc_%_deps,$$(call suffixin,$$(patsubst lib%.a,%,$$@),$$(libcc_package))))
	ar rs $@ $(call object,$(patsubst lib%.a,%,$@),lib,cc) $(call embed,$(patsubst lib%.a,%,$@),lib,cc)

$(libcxx_list): $$(call object,$$(patsubst lib%.a,%,$$@),lib,cxx) $$(call embed,$$(patsubst lib%.a,%,$$@),lib,cxx) $$(call $$(patsubst %,libcxx_%_deps,$$(call suffixin,$$(patsubst lib%.a,%,$$@),$$(libcxx_package))))
	ar rs $@ $(call object,$(patsubst lib%.a,%,$@),lib,cxx) $(call embed,$(patsubst lib%.a,%,$@),lib,cxx)

lib_target: $(lib_list)

$(testcc_list): $$(call object,$$(patsubst test_%,%,$$@),test,cc) $$(call $$(patsubst %,testcc_%_deps,$$(call suffixin,$$(patsubst lib%.a,%,$$@),$$(testcc_package)))) $$(call embed,$$(call suffixin,$$@,$$(testcc_package)),test,cc)
	$(CC) -o $@ $(GLOBAL_CC_TEST_LDFLAGS) $(call ldflags_project,$(call suffixin,$@,$(testcc_package),test,cc),test,cc) $(call object,$(call suffixin,$@,$(testcc_package)),test,cc) $(call embed,$(call suffixin,$@,$(testcc_package)),test,cc) $(GLOBAL_CC_LIBS) $(call libs_project,$(call suffixin,$@,$(testcc_package)),test,cc) $(GLOBAL_CC_TEST_PKG) $(call pkg_project,$(call suffixin,$@,$(testcc_package)),test,cc)

$(testcxx_list): $$(call object,$$(patsubst test_%,%,$$@),test,cxx) $$(call $$(patsubst %,testcxx_%_deps,$$(call suffixin,$$(patsubst lib%.a,%,$$@),$$(testcxx_package)))) $$(call embed,$$(call suffixin,$$@,$$(testcxx_package)),test,cxx)
	$(CXX) -o $@ $(GLOBAL_CXX_TEST_LDFLAGS) $(call ldflags_project,$(call suffixin,$@,$(testcxx_package),test,cxx),test,cxx) $(call object,$(call suffixin,$@,$(testcxx_package)),test,cxx) $(call embed,$(call suffixin,$@,$(testcxx_package)),test,cxx) $(GLOBAL_CXX_LIBS) $(call libs_project,$(call suffixin,$@,$(testcxx_package)),test,cxx) $(GLOBAL_CXX_TEST_PKG) $(call pkg_project,$(call suffixin,$@,$(testcxx_package)),test,cxx)

test_target: $(test_list)

$(cc_pkgconfig):
	@echo prefix=$(prefix) > $@
	@echo exec_prefix=$(if $(call libcc_$(patsubst %.pc,%,$@)_pkg_exec_prefix),$(call libcc_$(patsubst %.pc,%,$@)_pkg_exec_prefix),'$$'\{prefix\}) >> $@
	@echo libdir=$(if $(call libcc_$(patsubst %.pc,%,$@)_pkg_libdir),$(call libcc_$(patsubst %.pc,%,$@)_pkg_libdir),'$$'\{exec_prefix\}/lib) >> $@
	@echo includedir=$(if $(call libcc_$(patsubst %.pc,%,$@)_pkg_includedir),$(call libcc_$(patsubst %.pc,%,$@)_pkg_includedir),'$$'\{prefix\}/include) >> $@
	@echo >> $@
	@echo Name: $(if $(call libcc_$(patsubst %.pc,%,$@)_pkg_name),$(call libcc_$(patsubst %.pc,%,$@)_pkg_name),$(patsubst %.pc,lib%,$@)) >> $@
	@echo Description: $(call libcc_$(patsubst %.pc,%,$@)_pkg_description) >> $@
	@echo Version: $(version) >> $@
	@echo URL: $(call libcc_$(patsubst %.pc,%,$@)_pkg_url) >> $@
	@echo Libs: -l$(patsubst %.pc,%,$@) $(call libcc_$(patsubst %.pc,%,$@)_pkg_libs) >> $@
	@echo Libs.private: $(GLOBAL_CC_LDFLAGS) $(call $2$3_$1_ldflags) $(GLOBAL_CC_LIBS) $(call libcc_$(patsubst %.pc,%,$@)_libs) $(call libcc_$(patsubst %.pc,%,$@)_pkg_libs_private) >> $@
	@echo Require: $(call libcc_$(patsubst %.pc,%,$@)_pkg_require) >> $@
	@echo Require.private: $(foreach var,$(CC_PKG),"$(var) $(call CC_PKG_$(var)_version)") $(foreach var,$(call libcc_$(patsubst %.pc,%,$@)_pkg),"$(var) $(call libcc_$(patsubst %.pc,%,$@)_pkg_$(var)_version)") $(call libcc_$(patsubst %.pc,%,$@)_pkg_require_private) >> $@
	@echo Conflicts: $(call libcc_$(patsubst %.pc,%,$@)_pkg_conflicts) >> $@
	@echo Cflags: $(GLOBAL_CFLAGS) $(call cflags_project,$(patsubst %.pc,%,$@),lib,cc) $(CPPFLAGS) $(call libcc_$(patsubst %.pc,%,$@)_pkg_ccflags) >> $@

$(cxx_pkgconfig):
	@echo prefix=$(prefix) > $@
	@echo exec_prefix=$(if $(call libcxx_$(patsubst %.pc,%,$@)_pkg_exec_prefix),$(call libcxx_$(patsubst %.pc,%,$@)_pkg_exec_prefix),'$$'\{prefix\}) >> $@
	@echo libdir=$(if $(call libcxx_$(patsubst %.pc,%,$@)_pkg_libdir),$(call libcxx_$(patsubst %.pc,%,$@)_pkg_libdir),'$$'\{exec_prefix\}/lib) >> $@
	@echo includedir=$(if $(call libcxx_$(patsubst %.pc,%,$@)_pkg_includedir),$(call libcxx_$(patsubst %.pc,%,$@)_pkg_includedir),'$$'\{prefix\}/include) >> $@
	@echo >> $@
	@echo Name: $(if $(call libcxx_$(patsubst %.pc,%,$@)_pkg_name),$(call libcxx_$(patsubst %.pc,%,$@)_pkg_name),$(patsubst %.pc,lib%,$@)) >> $@
	@echo Description: $(call libcxx_$(patsubst %.pc,%,$@)_pkg_description) >> $@
	@echo Version: $(version) >> $@
	@echo URL: $(call libcxx_$(patsubst %.pc,%,$@)_pkg_url) >> $@
	@echo Libs: -l$(patsubst %.pc,%,$@) $(call libcxx_$(patsubst %.pc,%,$@)_pkg_libs) >> $@
	@echo Libs.private: $(GLOBAL_CXX_LDFLAGS) $(call ldflags_project,$(call suffixin,$(patsubst %.pc,%,$@),$(libcxx_package),lib,cxx),lib,cxx) $(GLOBAL_CXX_LIBS) $(call libs_project,$(patsubst %.pc,%,$@),lib,cxx) $(GLOBAL_CXX_PKG) $(call pkg_project,$(patsubst %.pc,%,$@),lib,cxx) $(call libcxx_$(patsubst %.pc,%,$@)_pkg_libs_private) >> $@
	@echo Require: $(call libcxx_$(patsubst %.pc,%,$@)_pkg_require) >> $@
	@echo Require.private: $(foreach var,$(CXX_PKG),"$(var) $(call CXX_PKG_$(var)_version)") $(foreach var,$(call libcxx_$(patsubst %.pc,%,$@)_pkg),"$(var) $(call libcxx_$(patsubst %.pc,%,$@)_pkg_$(var)_version)") $(call libcxx_$(patsubst %.pc,%,$@)_pkg_require_private) >> $@
	@echo Conflicts: $(call libcxx_$(patsubst %.pc,%,$@)_pkg_conflicts) >> $@
	@echo Cflags: $(GLOBAL_CXXFLAGS) $(call cxxflags_project,$(patsubst %.pc,%,$@),lib,cxx) $(CPPFLAGS) $(call libcxx_$(patsubst %.pc,%,$@)_pkg_cxxflags) >> $@

dist: submake
	mkdir -p $(distdir)
	cp Makefile $(distdir)
	cp Makefile.in $(distdir)
	$(if $(bincc_package:%=bincc_%_export),cp $(sort $(foreach var,$(bincc_package:%=bincc_%_export),$(call $(var)))) $(distdir),)
	$(if $(bincxx_package:%=bincxx_%_export),cp $(sort $(foreach var,$(bincxx_package:%=bincxx_%_export),$(call $(var)))) $(distdir),)
	$(if $(libcc_package:%=libcc_%_export),cp $(sort $(foreach var,$(libcc_package:%=libcc_%_export),$(call $(var)))) $(distdir),)
	$(if $(libcxx_package:%=libcxx_%_export),cp $(sort $(foreach var,$(libcxx_package:%=libcxx_%_export),$(call $(var)))) $(distdir),)
	$(if $(testcc_package:%=testcc_%_export),cp $(sort $(foreach var,$(testcc_package:%=testcc_%_export),$(call $(var)))) $(distdir),)
	$(if $(testcxx_package:%=testcxx_%_export),cp $(sort $(foreach var,$(testcxx_package:%=testcxx_%_export),$(call $(var)))) $(distdir),)
        
clean: submake
	$(RM) $(cc_object) $(foreach var,$(cc_object),$(var:%.o=%.d))
	$(RM) $(cxx_object) $(foreach var,$(cxx_object),$(var:%.o=%.d))
	$(RM) $(cc_pkgconfig) $(cxx_pkgconfig)
	$(RM) $(embed_object)
	$(RM) $(test_target)
	$(RM) $(bin_list)
	$(RM) $(lib_target)
	$(RM) $(clean)

check: submake $(test_list)
	@$(shell echo for i in "$(foreach var,$(testcc_list),$(var))"\; do eval echo Running \$$i...\; eval ./\$$i\; done)
	@$(shell echo for i in "$(foreach var,$(testcxx_list),$(var))"\; do eval echo Running \$$i...\; eval ./\$$i\; done)

checkdist distcheck: dist
        
install: submake $(bin_list) $(lib_target) $(cc_pkgconfig) $(cxx_pkgconfig)
	$(if $(lib_target),install -d $(DESTDIR)$(libdir))
	$(if $(lib_target),install -m 644 $(lib_target) $(DESTDIR)$(libdir))
	$(if $(bin_list),install -d $(DESTDIR)$(bindir))
	$(if $(bin_list),install -m 755 $(bin_list) $(DESTDIR)$(bindir))
	$(if $(lib_target),install -d $(DESTDIR)$(libdir)/pkgconfig)
	$(if $(cc_pkgconfig) $(cxx_pkgconfig),install -m 644 $(cc_pkgconfig) $(cxx_pkgconfig) $(DESTDIR)$(libdir)/pkgconfig)

uninstall: submake
	$(RM) $(foreach var,$(bin_list), $(DESTDIR)$(bindir)/$(var))
	$(RM) $(foreach var,$(lib_target), $(DESTDIR)$(libdir)/$(var))
	$(RM) $(foreach var,$(cc_pkgconfig) $(cxx_pkgconfig), $(DESTDIR)$(libdir)/pkgconfig/$(var))
	
submake:
	@$(shell echo for i in "$(foreach var,$(subdirs),$(var))"\; do $(MAKE) -C \$$i \|\| exit \$$? \; done)

FORCE:
.PHONY: all clean check install uninstall dist FORCE submake bin_package lib_target test_target
