#
#    Copyright 1991-2016 Amebis
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

######################################################################
# MSI General Package Information

MSIBUILD_VENDOR_NAME=GÉANT
MSIBUILD_PRODUCT_NAME=GÉANTLink
!IF "$(LANG)" == "bg_BG"
MSIBUILD_LANGID=1026
MSIBUILD_CODEPAGE=1251
!ELSEIF "$(LANG)" == "ca_ES"
MSIBUILD_LANGID=1027
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "cs_CZ"
MSIBUILD_LANGID=1029
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "cy_UK"
MSIBUILD_LANGID=1106
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "de_DE"
MSIBUILD_LANGID=1031
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "el_GR"
MSIBUILD_LANGID=1032
MSIBUILD_CODEPAGE=1253
!ELSEIF "$(LANG)" == "es_ES"
MSIBUILD_LANGID=1034
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "et_EE"
MSIBUILD_LANGID=1061
MSIBUILD_CODEPAGE=1257
!ELSEIF "$(LANG)" == "eu_ES"
MSIBUILD_LANGID=1069
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "fi_FI"
MSIBUILD_LANGID=1035
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "fr_CA"
MSIBUILD_LANGID=3084
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "fr_FR"
MSIBUILD_LANGID=1036
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "gl_ES"
MSIBUILD_LANGID=1110
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "hr_HR"
MSIBUILD_LANGID=1050
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "hu_HU"
MSIBUILD_LANGID=1038
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "is_IS"
MSIBUILD_LANGID=1039
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "it_IT"
MSIBUILD_LANGID=1040
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "ko_KR"
MSIBUILD_LANGID=1042
MSIBUILD_CODEPAGE=949
!ELSEIF "$(LANG)" == "lt_LT"
MSIBUILD_LANGID=1063
MSIBUILD_CODEPAGE=1257
!ELSEIF "$(LANG)" == "nb_NO"
MSIBUILD_LANGID=1044
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "nl_NL"
MSIBUILD_LANGID=1043
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "pl_PL"
MSIBUILD_LANGID=1045
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "pt_PT"
MSIBUILD_LANGID=2070
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "ru_RU"
MSIBUILD_LANGID=1049
MSIBUILD_CODEPAGE=1251
!ELSEIF "$(LANG)" == "sk_SK"
MSIBUILD_LANGID=1051
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "sl_SI"
MSIBUILD_LANGID=1060
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "sr_RS"
MSIBUILD_LANGID=2074
MSIBUILD_CODEPAGE=1250
!ELSEIF "$(LANG)" == "sv_SE"
MSIBUILD_LANGID=1053
MSIBUILD_CODEPAGE=1252
!ELSEIF "$(LANG)" == "tr_TR"
MSIBUILD_LANGID=1055
MSIBUILD_CODEPAGE=1254
!ELSEIF "$(LANG)" == "vi_VN"
MSIBUILD_LANGID=1066
MSIBUILD_CODEPAGE=1258
!ELSE
LANG=en_US
MSIBUILD_LANGID=1033
MSIBUILD_CODEPAGE=1252
!ENDIF


######################################################################
# Project name variations for directory variables & stuff

MSIBUILD_PRODUCT_NAME_ID=GEANTLink
MSIBUILD_PRODUCT_NAME_UC=GEANTLINK
MSIBUILD_PRODUCT_NAME_8_3=GEANTL~1


######################################################################
# The base name of the output MSI file

MSIBUILD_TARGET=$(MSIBUILD_PRODUCT_NAME_ID)-$(LANG)
!IF "$(PLAT)" == "x64"
MSIBUILD_TARGET=$(MSIBUILD_TARGET)-x64
!ELSEIF "$(PLAT)" == "ARM64"
MSIBUILD_TARGET=$(MSIBUILD_TARGET)-ARM64
!ELSE
PLAT=Win32
MSIBUILD_TARGET=$(MSIBUILD_TARGET)-x86
!ENDIF
!IF "$(CFG)" == "Debug"
MSIBUILD_TARGET=$(MSIBUILD_TARGET)D
!ELSE
CFG=Release
!ENDIF


######################################################################
# Path to version file
# (relative from MSIBuild\Version folder)

MSIBUILD_VERSION_FILE=..\..\..\include\Version.h


######################################################################
# Working/output folder for building MSI file
# (relative from MSIBuild folder)

MSIBUILD_OUTPUT_DIR=..\..\output


######################################################################
# Path to MSIBuild folder
# (relative from Base folder)

MSIBUILD_ROOT=..\MSIBuild


######################################################################
# GUID used to determine MSI upgrade logic

!IF "$(PLAT)" == "x64"
MSIBUILD_UPGRADE_GUID={3B90FD10-2DB4-4729-8081-C5668BA0BD2F}
!ELSEIF "$(PLAT)" == "ARM64"
MSIBUILD_UPGRADE_GUID={094B1885-C53C-4A1C-842F-D66994D60BB5}
!ELSE
MSIBUILD_UPGRADE_GUID={41546260-2E34-40C3-9890-1FF733E71C6D}
!ENDIF


######################################################################
# Minimum MSI version required to install this package

!IF "$(PLAT)" == "ARM64"
MSIBUILD_MSI_VERSION_MIN=500
!ELSE
MSIBUILD_MSI_VERSION_MIN=200
!ENDIF


######################################################################
# Length of ID and help fields in MSI tables (in characters)

MSIBUILD_LENGTH_ID=128
MSIBUILD_LENGTH_HELP=256


######################################################################
# Should MSIBuild compress files into CAB itself?

#MSIBUILD_COMPRESS=1


######################################################################
# Prevent installation of 32-bit MSI on 64-bit Windows

MSIBUILD_HAS_X64=1
MSIBUILD_NO_WOW64=1


######################################################################
# Component and registry settings (platform dependant)

!IF "$(PLAT)" == "x64" || "$(PLAT)" == "ARM64"
MSIBUILD_COMPONENT_ATTRIB_FILE=256
MSIBUILD_COMPONENT_ATTRIB_REGISTRY=260
MSIBUILD_REG32_RELOCATION=\Wow6432Node
!ELSE
MSIBUILD_COMPONENT_ATTRIB_FILE=0
MSIBUILD_COMPONENT_ATTRIB_REGISTRY=4
MSIBUILD_REG32_RELOCATION=
!ENDIF


######################################################################
# List of modules to compile and include in link

MSIBUILD_MODULES=\
	"$(MSIBUILD_ROOT)\Core\$(LANG).$(PLAT).$(CFG).msm" \
	"$(MSIBUILD_ROOT)\Version\$(LANG).$(PLAT).$(CFG).msm" \
	"Main\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\Events\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\lib\wxExtend\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\EAPMethods\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
	"..\..\EventMonitor\MSIBuild\$(LANG).$(PLAT).$(CFG).msm" \
#	"..\..\WLANManager\MSIBuild\$(LANG).$(PLAT).$(CFG).msm"


######################################################################
# wxExtend Module

WXEXTEND_STATIC=1
WXEXTEND_BIN_DIR=$(MSIBUILD_PRODUCT_NAME_UC)BINDIR
WXEXTEND_LOC_DIR=$(MSIBUILD_PRODUCT_NAME_UC)LOCDIR
