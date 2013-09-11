sources = 
tests = 
attach = *.hpp

subdirs =

all clean: 

dist:
	mkdir -p $(distdir)
	cp Makefile $(distdir)
	cp $(attach) $(distdir)
	
install:
	install -d $(DESTDIR)$(includedir)
	install -m 644 $(attach) $(DESTDIR)$(includedir)
	
uninstall:
	
check:
        
.PHONY: all dist install check
