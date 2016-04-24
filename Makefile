


PACKAGE_TARNAME	= libdebug
PACKAGE_VERSION	= 1.0

srcdir		= .


prefix		= /usr/local
exec_prefix	= ${prefix}
libdir		= ${exec_prefix}/lib

SFDC		= SFDC

CC		= m68k-amigaos-gcc -noixemul
AR		= m68k-amigaos-ar
RANLIB		= m68k-amigaos-ranlib

CFLAGS		= -O2 -fomit-frame-pointer
CPPFLAGS	= -W -Wall -Wstrict-prototypes
LDFLAGS		= 
LIBS		= 
ARFLAGS		= cr

INSTALL		= /opt/local/bin/ginstall -c
INSTALL_DATA	= ${INSTALL} -m 644
INSTALL_PROGRAM	= ${INSTALL}

SOURCES		= debug.c
SER_OBJECTS	= kdebug.o
PAR_OBJECTS	= pdebug.o
OBJECTS		= $(SER_OBJECTS) $(PAR_OBJECTS)
TARGETS		= libdebug.a #libddebug.a

k%.o:		%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DUSE_SERIAL -c $^ $(OUTPUT_OPTION)

p%.o:		%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DUSE_PARALELL -c $^ $(OUTPUT_OPTION)

all:		$(TARGETS)

clean:
	$(RM) $(OBJECTS) $(TARGETS) config.log 

distclean:	clean
	$(RM) -r autom4te.cache config.h config.status stamp-h Makefile \
		 exec_dbg_inline.h \
		 $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz

libdebug.a:	$(SER_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

libddebug.a:	$(PAR_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

test:		test.c libdebug.a

install:	all
	$(INSTALL) -d $(libdir)
	$(INSTALL_DATA) $(TARGETS) $(libdir)

tgz:	distclean
	cp -r $(srcdir) /tmp/$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)
	(cd /tmp && tar cfvz - --exclude=CVS \
		$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)) \
		> $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz
	$(RM) -r /tmp/$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)

#
# Auto-remake autoconf stuff
########################################

${srcdir}/configure: configure.ac
	cd ${srcdir} && autoconf

# autoheader might not change config.h.in, so touch a stamp file.
${srcdir}/config.h.in: stamp-h.in
${srcdir}/stamp-h.in: configure.ac
	cd ${srcdir} && autoheader
	echo timestamp > ${srcdir}/stamp-h.in

config.h: stamp-h
stamp-h: config.h.in config.status
	./config.status

Makefile: Makefile.in config.status
	./config.status

config.status: configure
	./config.status --recheck
