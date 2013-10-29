sources = 
tests = 
attach = AmplDataOutput.hpp AmplDataParser.hpp AmplInputData.hpp AmplInput.hpp AmplInputMatrix.hpp AmplInputScalar.hpp AmplInputVector.hpp AmplInputSet.hpp AmplOutputData.hpp AmplOutput.hpp AmplOutputMatrix.hpp AmplOutputScalar.hpp AmplOutputVector.hpp AmplOutputSet.hpp CastException.hpp Data.hpp DataOutput.hpp DataParser.hpp Matrix.hpp ParseException.hpp Record.hpp RecordOutput.hpp RecordParser.hpp Scalar.hpp Vector.hpp Set.hpp

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
