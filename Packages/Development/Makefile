PREFIX            = $(DESTDIR)/usr
INSTALL           = install -D
CP                = cp
MKDIR             = mkdir -p

all:

clean:
	#rm -f ./Files/libConsor.so
	#rm -rf ./Files/Include

files:
	rm -rf Files
	mkdir -p Files
	cp -r ../../Include ./Files/Include
	cp ../../Source/consor.pc ./Files/consor.pc

install:
	$(MKDIR) $(PREFIX)/include/Consor
	$(CP) -r ./Files/Include/Consor/* $(PREFIX)/include/Consor/
	$(INSTALL) ./Files/consor.pc $(PREFIX)/share/pkgconfig/consor.pc

deb: files
	echo Prefix is $(PREFIX)
	@echo generating deb
	./gitlog-to-changelog > debian/changelog
	dpkg-buildpackage -b -rfakeroot; \
	cat debian/control | sed "s/^Version: \(.*\)/Version: auto/g" > debian/control.tmp && mv debian/control.tmp debian/control; \
	mkdir -p ./build/ && mv ../libconsor-dev_* ./build/
	$(RM) build/*.changes
	@echo ".deb is located at build/"

rpm: deb
	fakeroot alien --keep-version --to-rpm build/*.deb && mv *.rpm build/

packages: deb rpm
