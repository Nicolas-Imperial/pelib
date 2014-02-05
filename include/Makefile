include Makefile.in

all clean: 

dist:
	mkdir -p $(distdir)
	cp Makefile $(distdir)
	cp Makefile.in $(distdir)
	cp $(attach) $(distdir)
	
install:
	install -d $(DESTDIR)$(includedir)
	install -m 644 $(attach) $(DESTDIR)$(includedir)
	
uninstall:
	$(RM) $(foreach var,$(attach), $(DESTDIR)$(includedir)/$(var))
	
check:
        
.PHONY: all dist install check
