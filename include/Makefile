sources = 
tests = 
attach = AmplRecord.hpp AmplParse.hpp ParamMatrix.hpp ParamScalar.hpp ParamVector.hpp ParseException.hpp

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
