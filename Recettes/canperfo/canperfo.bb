SUMMARY = "ACTIA CAN Perfo"
DESCRIPTION = "Diagnostic trames "
HOMEPAGE = "http://automotive.actia.com"
SECTION = "diagnostic"
#LICENSE = "Proprietary"
LICENSE = "CLOSED"


PR = "r0"

# In case the repo is not set
#PAMELA_REPO ?= "https://github.com/Michouu/Projet_1"

SRC_URI = "git://github.com/Michouu/Projet_1;branch=master;protocol=http\
"

SRCREV = "521dfdccb4340a6be1c077a0f90a8eee0616b407"
#SRC_URI[md5sum] = "7f347591f8541d1fcaa796d1eda3df01"


S = "${WORKDIR}/git"
EXTRA_OEMAKE = 'all -C ${S}'

#do not ship *.so in -dev.ipk
FILES_SOLIBSDEV = ""
#FILES_${PN} += "${libdir}/*.so"


PACKAGES =+ "${PN}-common ${PN}-test ${PN}-update"

FILES_${PN}+= "${bindir}/canCheck"
FILES_${PN}+= "${bindir}/canSend"


do_compile () {
	oe_runmake -C ./can_check
	oe_runmake -C ./can_send
}


do_install () {

	oe_runmake -C ./can_check install DESTDIR=${D}
	oe_runmake -C ./can_send install DESTDIR=${D}
	
	  
}