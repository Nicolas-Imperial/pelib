# http://www.freesoftwaremagazine.com/articles/gnu_coding_standards_applied_to_autotools

include Makefile.in
tarname = $(package)
distdir = $(tarname)-$(version)

all check install uninstall:
	$(shell echo for i in "$(foreach var,$(subdirs),$(var))"\; do $(MAKE) -C \$$i $@ \; done)
	
$(abspath $(distdir)).tar.gz: FORCE $(abspath $(distdir))
	tar -ch -C $(abspath $(distdir)) -O .| gzip -9 -c > $(abspath $(distdir)).tar.gz
	
$(abspath $(distdir)): FORCE clean-dist
	$(shell echo for i in "$(foreach var,$(subdirs),$(var))"\; do $(MAKE) -C \$$i dist distdir=$(abspath $(distdir))/\$$i\; done)
	cp Makefile $(abspath $(distdir))
	cp Makefile.in $(abspath $(distdir))
	
clean: clean-tree clean-dist

clean-tree:
	$(shell echo for i in "$(foreach var,$(subdirs),$(var))"\; do $(MAKE) -C \$$i clean\; done)
	
clean-dist:
	$(RM) -r $(abspath $(distdir))
	$(RM) $(abspath $(distdir)).tar.gz

dist: $(abspath $(distdir)).tar.gz

distcheck: checkdist clean-dist

checkdist: $(abspath $(distdir)).tar.gz
	gzip -cd $+ | tar xvf -
	$(MAKE) -C $(abspath $(distdir)) check
	$(MAKE) -C $(abspath $(distdir)) DESTDIR=$(abspath $(distdir))/_inst install uninstall
	$(MAKE) -C $(abspath $(distdir)) clean

FORCE:
.PHONY: FORCE all clean dist distcheck copy clean-dist clean-tree
.PHONY: install uninstall
