﻿/*
    Copyright 2015-2016 Amebis
    Copyright 2016 GÉANT

    This file is part of GÉANTLink.

    GÉANTLink is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GÉANTLink is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GÉANTLink. If not, see <http://www.gnu.org/licenses/>.
*/

#include "../../EAPBase_UI/include/EAP_UI.h"
#include "../../EAPMsg/include/Config.h"

///
/// Helper class for auto-destroyable EAP_METHOD_TYPE used in wxWidget's item containers
///
class wxEAPMethodTypeClientData;

///
/// Inner EAP method config panel
///
class wxEAPMsgMethodConfigPanel;

///
/// EAPMsg credential configuration panel
///
typedef wxEAPCredentialsConfigPanel<eap::credentials_pass, wxPasswordCredentialsPanel<eap::credentials_pass, wxPasswordCredentialsPanelBase> > wxEAPMsgCredentialsConfigPanel;

///
/// EAPMsg configuration panel
///
class wxEAPMsgConfigPanel;

#pragma once

#include "../res/wxEAPMsg_UI.h"

#include <wx/panel.h>
#include <wx/stattext.h>

#include <Windows.h>


class wxEAPMethodTypeClientData : public wxClientData
{
public:
    ///
    /// Constructs client data object with existing handle
    ///
    wxEAPMethodTypeClientData(const EAP_METHOD_TYPE &type, DWORD properties);

public:
    EAP_METHOD_TYPE m_type;             ///< EapHost method type
    DWORD m_properties;                 ///< Method properties
    eap::sanitizing_blob m_cfg_blob;    ///< Method configuration BLOB
};


class wxEAPMsgMethodConfigPanel : public wxEAPMsgMethodConfigPanelBase
{
public:
    ///
    /// Constructs an inner EAP method config panel
    ///
    /// \param[in   ] prov    Provider configuration data
    /// \param[inout] cfg     Configuration data
    /// \param[in   ] parent  Parent window
    ///
    wxEAPMsgMethodConfigPanel(const eap::config_provider &prov, eap::config_method_eapmsg &cfg, wxWindow *parent);

protected:
    /// \cond internal
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
    virtual void OnUpdateUI(wxUpdateUIEvent& event);
    virtual void OnSettings(wxCommandEvent& event);
    /// \endcond

protected:
    eap::config_method_eapmsg &m_cfg;   ///< Method configuration
};


class wxEAPMsgConfigPanel : public wxPanel
{
public:
    ///
    /// Constructs a configuration panel
    ///
    /// \param[in   ] prov    Provider configuration data
    /// \param[inout] cfg     Configuration data
    /// \param[in   ] parent  Parent window
    ///
    wxEAPMsgConfigPanel(const eap::config_provider &prov, eap::config_method_eapmsg &cfg, wxWindow* parent);

    ///
    /// Destructs the configuration panel
    ///
    virtual ~wxEAPMsgConfigPanel();

protected:
    /// \cond internal
    virtual void OnInitDialog(wxInitDialogEvent& event);
    /// \endcond

protected:
    wxEAPMsgMethodConfigPanel *m_method; ///< Method configuration panel
};
