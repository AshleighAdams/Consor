PREFIX            = $(DESTDIR)/usr
INSTALL           = install -D
CP                = cp
MKDIR             = mkdir -p

all:

clean:
	#rm -f ./Files/libConsor.so
	#rm -rf ./Files/Include

files:
	mkdir -p Files
	cp ../../Binaries/libConsor.so ./Files/libConsor.so

install:
	$(INSTALL) ./Files/libConsor.so $(PREFIX)/lib/libconsor.so

deb: files
	echo Prefix is $(PREFIX)
	@echo generating deb
	./gitlog-to-changelog > debian/changelog
	dpkg-buildpackage -b -rfakeroot; \
	cat debian/control | sed "s/^Version: \(.*\)/Version: auto/g" > debian/control.tmp && mv debian/control.tmp debian/control; \
	mkdir -p ./build/ && mv ../libconsor_* ./build/
	$(RM) build/*.changes
	@echo ".deb is located at build/"

rpm: deb
	fakeroot alien --keep-version --to-rpm build/*.deb && mv *.rpm build/

packages: deb rpm
