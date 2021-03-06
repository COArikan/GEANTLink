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
	msbuild.exe $(MSBUILDFLAGS) "GEANTLink.sln" /t:Clean /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)

CleanSetup ::
	-if exist "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf"       del /f /q "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf"
	-if exist "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).cab"       del /f /q "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).cab"
	-if exist "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).inf"       del /f /q "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).inf"
	-if exist "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).rpt"       del /f /q "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).rpt"
	-if exist "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).sign"      del /f /q "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).sign"
	-if exist "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"       del /f /q "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
	-if exist "$(OUTPUT_DIR)\Setup\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" del /f /q "$(OUTPUT_DIR)\Setup\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"


######################################################################
# Building
######################################################################

"$(OUTPUT_DIR)\$(PLAT).$(CFG)\CredWrite.exe" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\MsiUseFeature.exe" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\WLANManager.exe" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\Events.dll" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\EAPTTLS.dll" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\EAPTTLSUI.dll" :: Localization

"$(OUTPUT_DIR)\$(PLAT).$(CFG)\CredWrite.exe" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\MsiUseFeature.exe" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\WLANManager.exe" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\Events.dll" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\EAPTTLS.dll" \
"$(OUTPUT_DIR)\$(PLAT).$(CFG)\EAPTTLSUI.dll" ::
	msbuild.exe $(MSBUILDFLAGS) "GEANTLink.sln" /t:Build /p:Configuration=$(CFG) /p:Platform=$(PLAT_SLN)

"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf" : \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-bg_BG$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-ca_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-cs_CZ$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-cy_UK$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-de_DE$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-el_GR$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-es_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-et_EE$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-eu_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-fi_FI$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-fr_FR$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-fr_CA$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-gl_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-hr_HR$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-hu_HU$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-is_IS$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-it_IT$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-ko_KR$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-lt_LT$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-nb_NO$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-nl_NL$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-pl_PL$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-pt_PT$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-ru_RU$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sk_SK$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sl_SI$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sr_RS$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sv_SE$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-tr_TR$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-vi_VN$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-en_US$(PLAT_SUFFIX)$(CFG_SUFFIX).2.msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:MakeDDF //Nologo "$(@:"=).tmp" $** /O:"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX)" /C:LZX
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).cab" \
"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).inf" \
"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).rpt" : "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).ddf"
	makecab.exe /F $**

!IFDEF MANIFESTCERTIFICATETHUMBPRINT

"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).sign" : "$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).cab"

!ENDIF

"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" : \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-bg_BG$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-ca_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-cs_CZ$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-cy_UK$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-de_DE$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-el_GR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-es_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-et_EE$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-eu_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-fi_FI$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-fr_FR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-fr_CA$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-gl_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-hr_HR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-hu_HU$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-is_IS$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-it_IT$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-ko_KR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-lt_LT$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-nb_NO$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-nl_NL$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-pl_PL$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-pt_PT$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-ru_RU$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sk_SK$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sl_SI$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sr_RS$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-sv_SE$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-tr_TR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
#	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-vi_VN$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)-en_US$(PLAT_SUFFIX)$(CFG_SUFFIX).msi"
	-if exist $@ del /f /q $@
	-if exist "$(@:"=).tmp" del /f /q "$(@:"=).tmp"
	copy /y "$(OUTPUT_DIR)\$(PRODUCT_NAME)-en_US$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" "$(@:"=).tmp" > NUL
	attrib.exe -r "$(@:"=).tmp"
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-bg_BG$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1026 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-ca_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1027 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-cs_CZ$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1029 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-cy_UK$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1106 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-de_DE$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1031 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-el_GR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1032 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-es_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1034 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-et_EE$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1061 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-eu_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1069 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-fi_FI$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1035 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-fr_FR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 3084 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-fr_CA$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1036 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-gl_ES$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1110 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-hr_HR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1050 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-hu_HU$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1038 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-is_IS$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1039 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-it_IT$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1040 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-ko_KR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1042 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-lt_LT$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1063 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-nb_NO$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1044 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-nl_NL$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1043 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-pl_PL$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1045 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-pt_PT$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 2070 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-ru_RU$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1049 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-sk_SK$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1051 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-sl_SI$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1060 /L
	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-sr_RS$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 2074 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-sv_SE$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1053 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-tr_TR$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1055 /L
#	cscript.exe "MSI\MSIBuild\MSI.wsf" //Job:AddStorage //Nologo "$(@:"=).tmp" "$(OUTPUT_DIR)\$(PRODUCT_NAME)-vi_VN$(PLAT_SUFFIX)$(CFG_SUFFIX).mst" 1066 /L
	move /y "$(@:"=).tmp" $@ > NUL

"$(OUTPUT_DIR)\Setup\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" : \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).cab" \
	"$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).inf"
	$(MAKE) /f "MSI\MSIBuild\CAB.mak" /$(MAKEFLAGS) MSIBUILD_ROOT="MSI\MSIBuild" MSIBUILD_TARGET_MSI=$@ MSIBUILD_SOURCE_MSI="$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).msi" MSIBUILD_INF="$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).inf" MSIBUILD_CAB="$(OUTPUT_DIR)\$(PRODUCT_NAME)$(PLAT_SUFFIX)$(CFG_SUFFIX).cab" MSIBUILD_PRODUCT_NAME="$(MSIBUILD_PRODUCT_NAME)"

