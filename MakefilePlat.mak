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

CleanSetup ::
	-if exist "$(OUTPUT_DIR)\Setup\WLANManager$(PLAT_SUFFIX).exe" del /f /q "$(OUTPUT_DIR)\Setup\WLANManager$(PLAT_SUFFIX).exe"


######################################################################
# Setup
######################################################################

Setup :: \
	"$(OUTPUT_DIR)\Setup\$(PRODUCT_NAME)$(PLAT_SUFFIX).msi" \
	"$(OUTPUT_DIR)\Setup\WLANManager$(PLAT_SUFFIX).exe"

SetupDebug :: \
	"$(OUTPUT_DIR)\Setup\$(PRODUCT_NAME)$(PLAT_SUFFIX)D.msi"


######################################################################
# Publishing
######################################################################

Publish :: \
	"$(PUBLISH_PACKAGE_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX).msi" \
	"$(PUBLISH_PACKAGE_DIR)\WLANManager$(PLAT_SUFFIX).exe"


######################################################################
# File copy
######################################################################

"$(PUBLISH_PACKAGE_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX).msi" : "$(OUTPUT_DIR)\Setup\$(PRODUCT_NAME)$(PLAT_SUFFIX).msi"
	copy /y $** $@ > NUL

"$(OUTPUT_DIR)\Setup\WLANManager$(PLAT_SUFFIX).exe" \
"$(PUBLISH_PACKAGE_DIR)\WLANManager$(PLAT_SUFFIX).exe" : "$(OUTPUT_DIR)\$(PLAT).Release\WLANManager.exe"
	copy /y $** $@ > NUL


######################################################################
# Configuration Specific
######################################################################

CFG=Release
CFG_SUFFIX=
!INCLUDE "MakefilePlatCfg.mak"

CFG=Debug
CFG_SUFFIX=D
!INCLUDE "MakefilePlatCfg.mak"
