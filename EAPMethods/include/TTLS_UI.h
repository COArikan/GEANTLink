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

#include "TTLS.h"
#include "EAP_UI.h"
#include "../res/wxEAP_UI.h"
#include "../res/wxTTLS_UI.h"
#include "../res/wxTLS_UI.h"

namespace eap
{
    class peer_ttls_ui;
}

class wxEAPTTLSConfigPanel;
class wxEAPTTLSConfig;

#pragma once

#include <wx/icon.h>
#include <wx/scrolwin.h>
#include <wx/choicebk.h>


namespace eap
{
    ///
    /// TTLS UI peer
    ///
    class peer_ttls_ui : public peer_ui<eap::config_ttls, eap::credentials_ttls, int, int>
    {
    public:
        ///
        /// Constructor
        ///
        peer_ttls_ui();

        ///
        /// Raises the EAP method's specific connection configuration user interface dialog on the client.
        ///
        /// \sa [EapPeerInvokeConfigUI function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363614.aspx)
        ///
        /// \param[in]    hwndParent  Parent window
        /// \param[inout] cfg         Configuration to edit
        /// \param[out]   ppEapError  Pointer to error descriptor in case of failure. Free using `module::free_error_memory()`.
        ///
        /// \returns
        /// - \c ERROR_SUCCESS if succeeded
        /// - error code otherwise
        ///
        virtual DWORD invoke_config_ui(
            _In_    HWND        hwndParent,
            _Inout_ config_type &cfg,
            _Out_   EAP_ERROR   **ppEapError);

        ///
        /// Raises a custom interactive user interface dialog to obtain user identity information for the EAP method on the client.
        ///
        /// \sa [EapPeerInvokeIdentityUI function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363615.aspx)
        ///
        /// \param[in]    hwndParent     Parent window
        /// \param[in]    dwFlags        Flags passed to `EapPeerInvokeIdentityUI()` call
        /// \param[inout] cfg            Configuration
        /// \param[inout] usr            User data to edit
        /// \param[out]   ppwszIdentity  Pointer to user identity. Free using `module::free_memory()`.
        /// \param[out]   ppEapError     Pointer to error descriptor in case of failure. Free using `module::free_error_memory()`.
        ///
        /// \returns
        /// - \c ERROR_SUCCESS if succeeded
        /// - error code otherwise
        ///
        virtual DWORD invoke_identity_ui(
            _In_    HWND          hwndParent,
            _In_    DWORD         dwFlags,
            _Inout_ config_type   &cfg,
            _Inout_ identity_type &usr,
            _Out_   LPWSTR        *ppwszIdentity,
            _Out_   EAP_ERROR     **ppEapError);

        ///
        /// Raises a custom interactive user interface dialog for the EAP method on the client.
        ///
        /// \sa [EapPeerInvokeInteractiveUI function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363616.aspx)
        ///
        /// \param[in]  hwndParent     Parent window
        /// \param[in]  req            Interactive request
        /// \param[out] res            Interactive response
        /// \param[out] ppEapError     Pointer to error descriptor in case of failure. Free using `module::free_error_memory()`.
        ///
        /// \returns
        /// - \c ERROR_SUCCESS if succeeded
        /// - error code otherwise
        ///
        virtual DWORD invoke_interactive_ui(
            _In_        HWND                      hwndParent,
            _In_  const interactive_request_type  &req,
            _Out_       interactive_response_type &res,
            _Out_       EAP_ERROR                 **ppEapError);
    };
}


///
/// EAPTTLS configuration panel
///
class wxEAPTTLSConfigPanel : public wxEAPTTLSConfigPanelBase
{
public:
    ///
    /// Constructs a configuration panel
    ///
    wxEAPTTLSConfigPanel(eap::config_ttls &cfg, wxWindow* parent);

protected:
    /// \cond internal
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
    virtual void OnOuterIdentityCustom(wxCommandEvent& event);
    /// \endcond

protected:
    eap::config_ttls &m_cfg;    ///< TLS configuration
    winstd::library m_shell32;  ///< shell32.dll resource library reference
    wxIcon m_icon;              ///< Panel icon
};


///
/// EAPTTLS configuration
///
class wxEAPTTLSConfig : public wxScrolledWindow
{
public:
    ///
    /// Constructs a configuration panel
    ///
    /// \param[inout] cfg            Configuration data
    /// \param[in]    pszCredTarget  Target name of credentials in Windows Credential Manager. Can be further decorated to create final target name.
    /// \param[in]    parent         Parent window
    ///
    wxEAPTTLSConfig(eap::config_ttls &cfg, LPCTSTR pszCredTarget, wxWindow* parent);

    ///
    /// Destructs the configuration panel
    ///
    virtual ~wxEAPTTLSConfig();

protected:
    /// \cond internal
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
    virtual void OnInitDialog(wxInitDialogEvent& event);
    /// \endcond

protected:
    eap::config_ttls &m_cfg;                        ///< TTLS configuration
    wxStaticText *m_outer_title;                    ///< Outer authentication title
    wxEAPTTLSConfigPanel *m_outer_identity;         ///< Outer identity configuration panel
    wxEAPTLSConfigPanel *m_tls;                     ///< TLS configuration panel
    wxStaticText *m_inner_title;                    ///< Inner authentication title
    wxChoicebook *m_inner_type;                     ///< Inner authentication type

    eap::config_pap m_cfg_pap;                      ///< Temporary PAP configuration
};