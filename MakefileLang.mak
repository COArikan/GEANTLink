#
#    Copyright 1991-2018 Amebis
#    Copyright 2016 GÉANT
#
#    This file is part of GÉANTLink.
#
#    GÉANTLink is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    GÉANTLink is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with GÉANTLink. If not, see <http://www.gnu.org/licenses/>.
#

All ::

Clean ::
	-if exist "$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo" del /f /q "$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo"


######################################################################
# Localization
######################################################################

Localization :: \
	"$(OUTPUT_DIR)\locale\$(LANG)" \
	"$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo"

LocalizationImport : \
	"EAPMethods\locale\$(LANG).po" \
	"EventMonitor\locale\$(LANG).po" \
	"lib\wxExtend\locale\$(LANG).po" \
	"MSI\Base\locale\$(LANG).po" \
	"MSI\MSIBuild\Core\locale\$(LANG).po" \
	"WLANManager\locale\$(LANG).po"


######################################################################
# Folder creation
######################################################################

"$(OUTPUT_DIR)\locale\$(LANG)" :
	if not exist $@ md $@

"$(OUTPUT_DIR)\locale\$(LANG)" : "$(OUTPUT_DIR)\locale"


######################################################################
# Localization import from Transifex
######################################################################

"EAPMethods\locale\$(LANG).po" : \
!IF EXISTS("EAPMethods\locale\eapmethods_$(LANG).po")
	"EAPMethods\locale\eapmethods_$(LANG).po"
!ELSEIF EXISTS("EAPMethods\locale\eapmethods_$(LANG_BASE).po")
	"EAPMethods\locale\eapmethods_$(LANG_BASE).po"
!ENDIF
	copy /y $** $@ > NUL

"EventMonitor\locale\$(LANG).po" : \
!IF EXISTS("EventMonitor\locale\eventmonitor_$(LANG).po")
	"EventMonitor\locale\eventmonitor_$(LANG).po"
!ELSEIF EXISTS("EventMonitor\locale\eventmonitor_$(LANG_BASE).po")
	"EventMonitor\locale\eventmonitor_$(LANG_BASE).po"
!ENDIF
	copy /y $** $@ > NUL

"lib\wxExtend\locale\$(LANG).po" : \
!IF EXISTS("lib\wxExtend\locale\wxextend_$(LANG).po")
	"lib\wxExtend\locale\wxextend_$(LANG).po"
!ELSEIF EXISTS("lib\wxExtend\locale\wxextend_$(LANG_BASE).po")
	"lib\wxExtend\locale\wxextend_$(LANG_BASE).po"
!ENDIF
	copy /y $** $@ > NUL

"MSI\Base\locale\$(LANG).po" : \
!IF EXISTS("MSI\Base\locale\msibase_$(LANG).po")
	"MSI\Base\locale\msibase_$(LANG).po"
!ELSEIF EXISTS("MSI\Base\locale\msibase_$(LANG_BASE).po")
	"MSI\Base\locale\msibase_$(LANG_BASE).po"
!ENDIF
	copy /y $** $@ > NUL

"MSI\MSIBuild\Core\locale\$(LANG).po" : \
!IF EXISTS("MSI\MSIBuild\Core\locale\msibuildcore_$(LANG).po")
	"MSI\MSIBuild\Core\locale\msibuildcore_$(LANG).po"
!ELSEIF EXISTS("MSI\MSIBuild\Core\locale\msibuildcore_$(LANG_BASE).po")
	"MSI\MSIBuild\Core\locale\msibuildcore_$(LANG_BASE).po"
!ENDIF
	copy /y $** $@ > NUL

"WLANManager\locale\$(LANG).po" : \
!IF EXISTS("WLANManager\locale\wlanmanager_$(LANG).po")
	"WLANManager\locale\wlanmanager_$(LANG).po"
!ELSEIF EXISTS("WLANManager\locale\wlanmanager_$(LANG_BASE).po")
	"WLANManager\locale\wlanmanager_$(LANG_BASE).po"
!ENDIF
	copy /y $** $@ > NUL


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\locale\$(LANG)\wxstd.mo" : \
!IF EXISTS("$(WXWIN)\locale\$(LANG).po")
	"$(WXWIN)\locale\$(LANG).po"
!ELSEIF EXISTS("$(WXWIN)\locale\$(LANG_BASE).po")
	"$(WXWIN)\locale\$(LANG_BASE).po"
!ELSE
	"$(WXWIN)\locale\wxstd.pot"
!ENDIF
	msgfmt.exe --output-file=$@ --alignment=1 --endianness=little $**


######################################################################
# Platform-configuration Specific
######################################################################

PLAT=Win32
PLAT_SUFFIX=-x86
PLAT_SLN=x86

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefileLangPlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefileLangPlatCfg.mak"

PLAT=x64
PLAT_SUFFIX=-x64
PLAT_SLN=x64

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefileLangPlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefileLangPlatCfg.mak"

PLAT=ARM64
PLAT_SUFFIX=-ARM64
PLAT_SLN=ARM64

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefileLangPlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefileLangPlatCfg.mak"
