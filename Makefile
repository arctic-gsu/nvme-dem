# SPDX-License-Identifier: GPL-2.0/BSD

.SILENT:

COMMON_DIR=src/common
CLI_DIR=src/cli
HAC_DIR=src/host_auto_connect
DC_DIR=src/discovery_ctrl
SC_DIR=src/supervisory_ctrl
INCL_DIR=src/incl
BIN_DIR=.bin

DEM_CFLAGS = -DMG_ENABLE_THREADS -DMG_ENABLE_HTTP_WEBSOCKET=0

CFLAGS = -W -Wall -Werror -Wno-unused-function
CFLAGS += -Imongoose -Ijansson/src -I${INCL_DIR}

# ALT_CFLAGS used for sparse since mongoose has too many errors
# a modified version of mongoose.h can be created and stored in /files
# for sparse testing but is not valid for executable
ALT_CFLAGS = -W -Wall -Werror -Wno-unused-function
ALT_CFLAGS += -Ifiles -Ijansson/src -I${INCL_DIR}

SPARSE_OPTS = ${DEM_CFLAGS} ${ALT_CFLAGS} -DCS_PLATFORM=0

VALGRIND_OPTS = --leak-check=full --show-leak-kinds=all -v --track-origins=yes
VALGRIND_OPTS += --suppressions=files/valgrind_suppress

DC_LIBS = -lpthread -lrdmacm -libverbs -luuid -lcurl jansson/libjansson.a
SC_LIBS = -lpthread -lrdmacm -libverbs -luuid jansson/libjansson.a
HAC_LIBS = -lpthread -lrdmacm -libverbs -luuid jansson/libjansson.a

CLI_LIBS = -lcurl jansson/libjansson.a

GDB_OPTS = -g -O0

LINUX_INCL = ${INCL_DIR}/linux/nvme.h ${INCL_DIR}/linux/kernel.h \
	     ${INCL_DIR}/linux/list.h ${INCL_DIR}/linux/nvme-rdma.h

CLI_SRC = ${CLI_DIR}/cli.c ${COMMON_DIR}/curl.c ${CLI_DIR}/show.c
CLI_INC = ${INCL_DIR}/dem.h ${INCL_DIR}/curl.h ${CLI_DIR}/show.h \
	  ${INCL_DIR}/tags.h

HAC_SRC = ${HAC_DIR}/daemon.c ${COMMON_DIR}/nvmeof.c ${COMMON_DIR}/rdma.c \
	  ${COMMON_DIR}/logpages.c ${COMMON_DIR}/parse.c
HAC_INC = ${INCL_DIR}/dem.h ${HAC_DIR}/common.h ${INCL_DIR}/ops.h ${LINUX_INCL}

DC_SRC = ${DC_DIR}/daemon.c ${DC_DIR}/json.c ${DC_DIR}/restful.c \
	 ${DC_DIR}/interfaces.c ${DC_DIR}/pseudo_target.c ${DC_DIR}/config.c \
	 ${COMMON_DIR}/nvmeof.c ${COMMON_DIR}/curl.c ${COMMON_DIR}/rdma.c \
	 ${COMMON_DIR}/logpages.c ${COMMON_DIR}/parse.c mongoose/mongoose.c
DC_INC = ${INCL_DIR}/dem.h ${DC_DIR}/json.h ${DC_DIR}/common.h \
	 ${INCL_DIR}/ops.h ${INCL_DIR}/curl.h ${INCL_DIR}/tags.h \
	 mongoose/mongoose.h ${LINUX_INCL}

SC_SRC = ${SC_DIR}/daemon.c ${SC_DIR}/restful.c ${SC_DIR}/configfs.c \
	 ${SC_DIR}/pseudo_target.c ${COMMON_DIR}/rdma.c \
	 ${COMMON_DIR}/nvmeof.c ${COMMON_DIR}/parse.c mongoose/mongoose.c
SC_INC = ${INCL_DIR}/dem.h ${SC_DIR}/common.h ${INCL_DIR}/tags.h \
	 ${INCL_DIR}/ops.h mongoose/mongoose.h ${LINUX_INCL}

all: ${BIN_DIR} mongoose/ jansson/libjansson.a \
     ${BIN_DIR}/dem ${BIN_DIR}/dem-hac ${BIN_DIR}/dem-dc ${BIN_DIR}/dem-sc
	echo Done.

${BIN_DIR}:
	mkdir ${BIN_DIR}

dem: ${BIN_DIR}/dem
dem-hac: ${BIN_DIR}/dem-hac
dem-dc: ${BIN_DIR}/dem-dc
dem-sc: ${BIN_DIR}/dem-sc

${BIN_DIR}/dem: ${CLI_SRC} ${CLI_INC} Makefile jansson/libjansson.a
	echo CC dem
	gcc ${CLI_SRC} -o $@ ${CFLAGS} ${GDB_OPTS} ${CLI_LIBS} -I${CLI_DIR} \
		-DDEM_CLI

${BIN_DIR}/dem-hac: ${HAC_SRC} ${HAC_INC} Makefile
	echo CC dem-hac
	gcc ${HAC_SRC} -o $@ ${DEM_CFLAGS} ${CFLAGS} ${GDB_OPTS} ${HAC_LIBS} -I${HAC_DIR}

${BIN_DIR}/dem-dc: ${DC_SRC} ${DC_INC} Makefile jansson/libjansson.a
	echo CC dem-dc
	gcc ${DC_SRC} -o $@ ${DEM_CFLAGS} ${CFLAGS} ${GDB_OPTS} ${DC_LIBS} -I${DC_DIR}

${BIN_DIR}/dem-sc: ${SC_SRC} ${SC_INC} Makefile jansson/libjansson.a
	echo CC dem-sc
	gcc ${SC_SRC} -o $@ ${DEM_CFLAGS} ${CFLAGS} ${GDB_OPTS} ${SC_LIBS} -I${SC_DIR}

clean:
	rm -rf ${BIN_DIR}/ config.json *.vglog
	echo Done.

must_be_root:
	[ `whoami` == "root" ]

install: must_be_root
	#cp ${BIN_DIR}/* /bin
	ln -sf `pwd`/${BIN_DIR}/dem /bin/dem
	ln -sf `pwd`/${BIN_DIR}/dem-hac /bin/dem-hac
	ln -sf `pwd`/${BIN_DIR}/dem-dc /bin/dem-dc
	ln -sf `pwd`/${BIN_DIR}/dem-sc /bin/dem-sc
	cp usr/nvmeof.conf /etc/nvme
	[ -e /etc/nvme/nvmeof-dem ] || mkdir /etc/nvme/nvmeof-dem
	cp usr/nvmeof /usr/libexec/
	cp usr/nvmeof.service /usr/lib/systemd/system/nvmeof.service
	cp usr/bash_completion.d/* /etc/bash_completion.d/
	cp usr/man/*.1 /usr/share/man/man1/
	cp usr/man/*.8 /usr/share/man/man8/
	echo Done.

uninstall: must_be_root
	[ `whoami` == "root" ]
	rm -f /etc/bash_completion.d/dem
	rm -f /bin/dem /bin/dem-dc /bin/dem-sc
	rm -f /etc/nvme/nvmeof.conf
	rm -rf /etc/nvme/nvmeof-dem
	rm -f /usr/libexec/nvmeof
	rm -f /usr/lib/systemd/system/nvmeof.service
	mr -f /usr/share/man/man1/dem.1 /usr/share/man/man8/dem-[ade]c.8
	echo Done.

mongoose/:
	echo cloning github.com/cesanta/mongoose.git
	git clone https://github.com/cesanta/mongoose.git
	cd mongoose ; patch -p 1 < ../files/mongoose.patch

jansson/Makefile.am:
	echo cloning github.com/akheron/jansson.git
	git clone https://github.com/akheron/jansson.git >/dev/null
jansson/configure: jansson/Makefile.am
	echo configuring jansson
	cd jansson ; autoreconf -i >/dev/null 2>&1
jansson/Makefile: jansson/configure
	cd jansson ; ./configure >/dev/null
jansson/src/.libs/libjansson.a: jansson/Makefile
	echo building libjansson
	cd jansson/src ; make libjansson.la >/dev/null
jansson/libjansson.a: jansson/src/.libs/libjansson.a
	cp jansson/src/.libs/libjansson.a jansson

archive: clean
	[ -d archive ] || mkdir archive
	tar cz -f archive/`date +"%y%m%d_%H%M"`.tgz .gitignore .git/config \
		files/ html/ Makefile src/ usr/ 

test_cli: dem
	./dem config
	./dem list ctrl
	./dem set ctrl ctrl1
	./dem set portid 1 rdma ipv4 1.1.1.2 2332
	./dem show ctrl ctrl1
	./dem rename ctrl ctrl1 ctrl2
	./dem add ss ctrl2 ss21 ss22
	./dem delete ss ctrl2 ss21
	./dem delete ctrl ctrl2
	./dem list host
	./dem set host host01
	./dem rename host host01 host02
	./dem add acl host01 ss11 ss21
	./dem show host host01
	./dem delete acl host02 ss21
	./dem delete host host02
	./dem shutdown

# show format for raw: fmt=-r make show_hosts
# show format for pretty json: fmt=-j make show_hosts
show_hosts:
	for i in `./dem lis h |grep -v ^http: | grep -v "^No .* defined"`; \
		do ./dem $$fmt get h $$i ; done

show_targets:
	for i in `./dem lis t |grep -v ^http: | grep -v "^No .* defined"`; \
		do ./dem $$fmt get t $$i ; done

del_hosts:
	for i in `./dem lis h |grep -v ^http: | grep -v "^No .* defined"`; \
		do ./dem -f del h $$i ; done

del_targets:
	for i in `./dem lis t |grep -v ^http: | grep -v "^No .* defined"`; \
		do ./dem -f del t $$i ; done

put:
	echo PUT Commands
	curl -X PUT -d '' http://127.0.0.1:22345/host/host01
	echo
	curl -X PUT -d '' http://127.0.0.1:22345/host/host02
	echo
	curl -X PUT -d '' http://127.0.0.1:22345/target/ctrl1
	echo
	curl -X PUT -d '' http://127.0.0.1:22345/target/ctrl2
	echo
	curl -X PUT -d '' http://127.0.0.1:22345/target/ctrl1/subsys/subsys1
	echo
	echo
get:
	echo GET Commands
	curl http://127.0.0.1:22345/target
	echo
	curl http://127.0.0.1:22345/target/ctrl1
	echo
	curl http://127.0.0.1:22345/host
	echo
	curl http://127.0.0.1:22345/host/host01
	echo
	echo

del: delhost01 delhost02 delctrl1 delctrl2
	echo

delctrl2:
	curl -X DELETE  http://127.0.0.1:22345/target/ctrl2
	echo
delctrl1:
	curl -X DELETE  http://127.0.0.1:22345/target/ctrl1
	echo
delhost01:
	echo DELETE Commands
	curl -X DELETE  http://127.0.0.1:22345/host/host01
	echo
delhost02:
	curl -X DELETE  http://127.0.0.1:22345/host/host02
	echo
post:
	echo POST Commands
	curl -d '{"HOSTNQN":"host02"}' http://127.0.0.1:22345/host/host01
	echo
	echo

get16:
	for i in `seq 1 16` ; do make get ; done

test: put get16 post get del

memcheck: dem-dc
	reset
	valgrind ${VALGRIND_OPTS} --log-file=dem-dc.vglog dem-dc
	echo "valgrind output in 'dem-dc.vglog'"

sparse:
	echo running sparse of each .c file with options
	echo "${SPARSE_OPTS}"
	for i in src/*/*.c ; do sparse $$i ${SPARSE_OPTS} ; done
	echo Done.
