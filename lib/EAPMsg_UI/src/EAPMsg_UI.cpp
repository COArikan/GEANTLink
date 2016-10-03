/*
    Copyright 2015-2016 Amebis
    Copyright 2016 G�ANT

    This file is part of G�ANTLink.

    G�ANTLink is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    G�ANTLink is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with G�ANTLink. If not, see <http://www.gnu.org/licenses/>.
*/

#include "StdAfx.h"

#pragma comment(lib, "Eappcfg.lib")


//////////////////////////////////////////////////////////////////////
// wxEAPMsgMethodConfigPanel
//////////////////////////////////////////////////////////////////////

wxEAPMethodTypeClientData::wxEAPMethodTypeClientData(const EAP_METHOD_TYPE &type, DWORD properties) :
    m_type(type),
    m_properties(properties)
{
}


//////////////////////////////////////////////////////////////////////
// wxEAPMsgMethodConfigPanel
//////////////////////////////////////////////////////////////////////

wxEAPMsgMethodConfigPanel::wxEAPMsgMethodConfigPanel(const eap::config_provider &prov, eap::config_method_eapmsg &cfg, wxWindow *parent) :
    m_cfg(cfg),
    wxEAPMsgMethodConfigPanelBase(parent)
{
    UNREFERENCED_PARAMETER(prov);

    // Load and set icon.
    winstd::library lib_shell32;
    if (lib_shell32.load(_T("shell32.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))
        m_method_icon->SetIcon(wxLoadIconFromResource(lib_shell32, MAKEINTRESOURCE(175)));

    winstd::eap_method_info_array methods;
    winstd::eap_error error;
    DWORD dwResult = EapHostPeerGetMethods(&methods, &error._Myptr);
    if (dwResult == ERROR_SUCCESS) {
        for (DWORD i = 0; i < methods.dwNumberOfMethods; i++)
            m_method->Append(methods.pEapMethods[i].pwszFriendlyName, new wxEAPMethodTypeClientData(methods.pEapMethods[i].eaptype, methods.pEapMethods[i].eapProperties));
    } else if (error)
        wxLogError(_("Enumerating EAP methods failed (error %u, %s, %s)."), error->dwWinError, error->pRootCauseString, error->pRepairString);
    else
        wxLogError(_("Enumerating EAP methods failed (error %u)."), dwResult);
}


bool wxEAPMsgMethodConfigPanel::TransferDataToWindow()
{
    if (m_method->HasClientObjectData()) {
        // Find configured method and set its selection and configuration BLOB.
        for (unsigned int i = 0, n = m_method->GetCount(); i < n; i++) {
            wxEAPMethodTypeClientData *data = dynamic_cast<wxEAPMethodTypeClientData*>(m_method->GetClientObject(i));
            if (data->m_type == m_cfg.m_type) {
                m_method->SetSelection(i);
                data->m_cfg_blob = m_cfg.m_cfg_blob;
            }
        }
    }

    return wxEAPMsgMethodConfigPanelBase::TransferDataToWindow();
}


bool wxEAPMsgMethodConfigPanel::TransferDataFromWindow()
{
    wxCHECK(wxEAPMsgMethodConfigPanelBase::TransferDataFromWindow(), false);

    int sel = m_method->GetSelection();
    const wxEAPMethodTypeClientData *data =
        sel != wxNOT_FOUND && m_method->HasClientObjectData() ?
            dynamic_cast<const wxEAPMethodTypeClientData*>(m_method->GetClientObject(sel)) :
            NULL;
    if (data) {
        // Save method selection and configuration.
        m_cfg.m_type     = data->m_type;
        m_cfg.m_cfg_blob = data->m_cfg_blob;
    }

    return true;
}


void wxEAPMsgMethodConfigPanel::OnUpdateUI(wxUpdateUIEvent& event)
{
    wxEAPMsgMethodConfigPanelBase::OnUpdateUI(event);

    int sel = m_method->GetSelection();
    const wxEAPMethodTypeClientData *data =
        sel != wxNOT_FOUND && m_method->HasClientObjectData() ?
            dynamic_cast<const wxEAPMethodTypeClientData*>(m_method->GetClientObject(sel)) :
            NULL;
    m_settings->Enable(data && (data->m_properties & eapPropSupportsConfig));
}


void wxEAPMsgMethodConfigPanel::OnSettings(wxCommandEvent& event)
{
    wxEAPMsgMethodConfigPanelBase::OnSettings(event);

    int sel = m_method->GetSelection();
    wxEAPMethodTypeClientData *data =
        sel != wxNOT_FOUND && m_method->HasClientObjectData() ?
            dynamic_cast<wxEAPMethodTypeClientData*>(m_method->GetClientObject(sel)) :
            NULL;
    if (data && (data->m_properties & eapPropSupportsConfig)) {
        DWORD cfg_data_size = 0;
        winstd::eap_blob cfg_data;
        winstd::eap_error error;
        DWORD dwResult = EapHostPeerInvokeConfigUI(GetHWND(), 0, data->m_type, (DWORD)data->m_cfg_blob.size(), data->m_cfg_blob.data(), &cfg_data_size, &cfg_data._Myptr, &error._Myptr);
        if (dwResult == ERROR_SUCCESS) {
            const BYTE *_cfg_data = cfg_data.get();
            data->m_cfg_blob.assign(_cfg_data, _cfg_data + cfg_data_size);
        } else if (dwResult == ERROR_CANCELLED) {
            // Not really an error.
        } else if (error)
            wxLogError(_("Configuring EAP method failed (error %u, %s, %s)."), error->dwWinError, error->pRootCauseString, error->pRepairString);
        else
            wxLogError(_("Configuring EAP method failed (error %u)."), dwResult);
    }
}


//////////////////////////////////////////////////////////////////////
// wxEAPMsgConfigPanel
//////////////////////////////////////////////////////////////////////

wxEAPMsgConfigPanel::wxEAPMsgConfigPanel(const eap::config_provider &prov, eap::config_method_eapmsg &cfg, wxWindow* parent) : wxPanel(parent)
{
    wxBoxSizer* sb_content;
    sb_content = new wxBoxSizer( wxVERTICAL );

    m_method = new wxEAPMsgMethodConfigPanel(prov, cfg, this);
    sb_content->Add(m_method, 0, wxEXPAND, 5);

    this->SetSizer(sb_content);
    this->Layout();

    // Connect Events
    this->Connect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(wxEAPMsgConfigPanel::OnInitDialog));
}


wxEAPMsgConfigPanel::~wxEAPMsgConfigPanel()
{
    // Disconnect Events
    this->Disconnect(wxEVT_INIT_DIALOG, wxInitDialogEventHandler(wxEAPMsgConfigPanel::OnInitDialog));
}


void wxEAPMsgConfigPanel::OnInitDialog(wxInitDialogEvent& event)
{
    // Forward the event to child panels.
    if (m_method)
        m_method->GetEventHandler()->ProcessEvent(event);
}
