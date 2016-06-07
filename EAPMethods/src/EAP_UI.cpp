/*
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

#include <StdAfx.h>


//////////////////////////////////////////////////////////////////////
// wxEAPBannerPanel
//////////////////////////////////////////////////////////////////////

wxEAPBannerPanel::wxEAPBannerPanel(wxWindow* parent) : wxEAPBannerPanelBase(parent)
{
    m_product_name->SetLabelText(wxT(PRODUCT_NAME_STR));
}


//////////////////////////////////////////////////////////////////////
// wxCredentialsConfigPanel
//////////////////////////////////////////////////////////////////////

wxCredentialsConfigPanel::wxCredentialsConfigPanel(eap::config_method &cfg, wxWindow* parent) :
    m_cfg(cfg),
    wxCredentialsConfigPanelBase(parent)
{
    // Load and set icon.
    if (m_shell32.load(_T("shell32.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))
        wxSetIconFromResource(m_credentials_icon, m_icon, m_shell32, MAKEINTRESOURCE(48));
}


bool wxCredentialsConfigPanel::TransferDataToWindow()
{
    wxCHECK(wxCredentialsConfigPanelBase::TransferDataToWindow(), false);

    if (m_cfg.m_allow_save) {
        m_allow_save->SetValue(true);
        m_cred_store_set->Enable(true);
        m_cred_store_clear->Enable(false); // TODO: Set according to the Credential Store state.
    } else {
        m_allow_save->SetValue(false);
        m_cred_store_set->Enable(false);
        m_cred_store_clear->Enable(false);
    }

    return true;
}


bool wxCredentialsConfigPanel::TransferDataFromWindow()
{
    m_cfg.m_allow_save = m_allow_save->GetValue();

    return wxCredentialsConfigPanelBase::TransferDataFromWindow();
}


void wxCredentialsConfigPanel::OnAllowSave(wxCommandEvent& event)
{
    if (event.IsChecked()) {
        m_cred_store_set->Enable(true);
        m_cred_store_clear->Enable(false); // TODO: Set according to the Credential Store state.
    } else {
        m_cred_store_set->Enable(false);
        m_cred_store_clear->Enable(false);
    }
}


void wxCredentialsConfigPanel::OnCredentialsSet(wxCommandEvent& event)
{
    event.Skip();
}


void wxCredentialsConfigPanel::OnCredentialsClear(wxCommandEvent& event)
{
    event.Skip();
}


//////////////////////////////////////////////////////////////////////
// wxPasswordCredentialsPanel
//////////////////////////////////////////////////////////////////////

wxPasswordCredentialsPanel::wxPasswordCredentialsPanel(eap::config_pass &cfg, wxWindow* parent) :
    m_cfg(cfg),
    wxPasswordCredentialsPanelBase(parent)
{
    // Load and set icon.
    if (m_shell32.load(_T("shell32.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE))
        wxSetIconFromResource(m_credentials_icon, m_icon, m_shell32, MAKEINTRESOURCE(269));
}


bool wxPasswordCredentialsPanel::TransferDataToWindow()
{
    wxCHECK(wxPasswordCredentialsPanelBase::TransferDataToWindow(), false);

    if (m_cfg.m_allow_save) {
        m_remember->Enable(true);
    } else {
        m_remember->SetValue(false);
        m_remember->Enable(false);
    }

    return true;
}
