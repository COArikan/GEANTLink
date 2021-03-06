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

#include <StdAfx.h>

using namespace std;
using namespace winstd;

#pragma comment(lib, "Ws2_32.lib")

#if EAPMETHOD_TYPE==21
#define _EAPMETHOD_PEER    eap::peer_ttls
#else
#error Unknown EAP Method type.
#endif

_EAPMETHOD_PEER g_peer;


///
/// DLL main entry point
///
/// \sa [DllMain entry point](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx)
///
BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH) {
#ifdef _DEBUG
        //Sleep(10000);
#endif
        g_peer.m_instance = hinstDLL;
    } else if (fdwReason == DLL_PROCESS_DETACH)
        assert(!_CrtDumpMemoryLeaks());

    return TRUE;
}


///
/// Releases all memory associated with an opaque user interface context data buffer.
///
/// \sa [EapPeerFreeMemory function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363606.aspx)
///
VOID WINAPI EapPeerFreeMemory(_In_ void *pUIContextData)
{
    event_fn_auto event_auto(g_peer.get_event_fn_auto(__FUNCTION__));

    if (pUIContextData)
        g_peer.free_memory((BYTE*)pUIContextData);
}


///
/// Releases error-specific memory allocated by the EAP peer method.
///
/// \sa [EapPeerFreeErrorMemory function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363605.aspx)
///
VOID WINAPI EapPeerFreeErrorMemory(_In_ EAP_ERROR *ppEapError)
{
    event_fn_auto event_auto(g_peer.get_event_fn_auto(__FUNCTION__));

    if (ppEapError)
        g_peer.free_error_memory(ppEapError);
}


///
/// Obtains a set of function pointers for an implementation of the EAP peer method currently loaded on the EapHost service.
///
/// \sa [EapPeerGetInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363608.aspx)
///
DWORD WINAPI EapPeerGetInfo(_In_ EAP_TYPE* pEapType, _Out_ EAP_PEER_METHOD_ROUTINES* pEapPeerMethodRoutines, _Out_ EAP_ERROR **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!pEapType)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapType is NULL.")));
    else if (pEapType->type != EAPMETHOD_TYPE)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" Input EAP type (%d) does not match the supported EAP type (%d)."), (int)pEapType->type, (int)EAPMETHOD_TYPE).c_str()));
    else if (!pEapPeerMethodRoutines)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapPeerMethodRoutines is NULL.")));
    else {
        pEapPeerMethodRoutines->dwVersion                    = PRODUCT_VERSION;
        pEapPeerMethodRoutines->pEapType                     = NULL;

        pEapPeerMethodRoutines->EapPeerInitialize            = EapPeerInitialize;
        pEapPeerMethodRoutines->EapPeerShutdown              = EapPeerShutdown;
        pEapPeerMethodRoutines->EapPeerBeginSession          = EapPeerBeginSession;
        pEapPeerMethodRoutines->EapPeerEndSession            = EapPeerEndSession;
        pEapPeerMethodRoutines->EapPeerSetCredentials        = NULL;    // Always NULL unless we want to use generic credential UI
        pEapPeerMethodRoutines->EapPeerGetIdentity           = EapPeerGetIdentity;
        pEapPeerMethodRoutines->EapPeerProcessRequestPacket  = EapPeerProcessRequestPacket;
        pEapPeerMethodRoutines->EapPeerGetResponsePacket     = EapPeerGetResponsePacket;
        pEapPeerMethodRoutines->EapPeerGetResult             = EapPeerGetResult;
        pEapPeerMethodRoutines->EapPeerGetUIContext          = EapPeerGetUIContext;
        pEapPeerMethodRoutines->EapPeerSetUIContext          = EapPeerSetUIContext;
        pEapPeerMethodRoutines->EapPeerGetResponseAttributes = EapPeerGetResponseAttributes;
        pEapPeerMethodRoutines->EapPeerSetResponseAttributes = EapPeerSetResponseAttributes;
    }

    return dwResult;
}


#pragma warning(push)
#pragma warning(disable: 4702) // Compiler is smart enough to find out the initialize() method is empty => never throws an exception.

///
/// Initializes an EAP peer method for EapHost.
///
/// \sa [EapPeerGetInfo function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363613.aspx)
///
DWORD APIENTRY EapPeerInitialize(_Out_ EAP_ERROR **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    try {
        g_peer.initialize();
    } catch (std::exception &err) {
        g_peer.log_error(*ppEapError = g_peer.make_error(err));
        dwResult = (*ppEapError)->dwWinError;
    } catch (...) {
        dwResult = ERROR_INVALID_DATA;
    }

    return dwResult;
}

#pragma warning(pop)


#pragma warning(push)
#pragma warning(disable: 4702) // Compiler is smart enough to find out the shutdown() method is empty => never throws an exception.

///
/// Shuts down the EAP method and prepares to unload its corresponding DLL.
///
/// \sa [EapPeerShutdown function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363627.aspx)
///
DWORD APIENTRY EapPeerShutdown(_Out_ EAP_ERROR **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    try {
        g_peer.shutdown();
    } catch (std::exception &err) {
        g_peer.log_error(*ppEapError = g_peer.make_error(err));
        dwResult = (*ppEapError)->dwWinError;
    } catch (...) {
        dwResult = ERROR_INVALID_DATA;
    }

    return dwResult;
}

#pragma warning(pop)


///
/// Returns the user data and user identity after being called by EapHost.
///
/// \sa [EapPeerGetIdentity function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363607.aspx)
///
DWORD APIENTRY EapPeerGetIdentity(
    _In_                                   DWORD     dwFlags,
    _In_                                   DWORD     dwConnectionDataSize,
    _In_count_(dwConnectionDataSize) const BYTE      *pConnectionData,
    _In_                                   DWORD     dwUserDataSize,
    _In_count_(dwUserDataSize)       const BYTE      *pUserData,
    _In_                                   HANDLE    hTokenImpersonateUser,
    _Out_                                  BOOL      *pfInvokeUI,
    _Out_                                  DWORD     *pdwUserDataOutSize,
    _Out_                                  BYTE      **ppUserDataOut,
    _Out_                                  WCHAR     **ppwszIdentity,
    _Out_                                  EAP_ERROR **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!pConnectionData && dwConnectionDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pConnectionData is NULL.")));
    else if (!pUserData && dwUserDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pUserData is NULL.")));
    else if (!pfInvokeUI)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pfInvokeUI is NULL.")));
    else if (!pdwUserDataOutSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pdwUserDataOutSize is NULL.")));
    else if (!ppUserDataOut)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" ppUserDataOut is NULL.")));
    else if (!ppwszIdentity)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" ppwszIdentity is NULL.")));
    else {
        try {
            g_peer.get_identity(dwFlags, pConnectionData, dwConnectionDataSize, pUserData, dwUserDataSize, ppUserDataOut, pdwUserDataOutSize, hTokenImpersonateUser, pfInvokeUI, ppwszIdentity);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Starts an EAP authentication session on the peer EapHost using the EAP method.
///
/// \sa [EapPeerBeginSession function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363600.aspx)
///
DWORD APIENTRY EapPeerBeginSession(
    _In_                                       DWORD              dwFlags,
    _In_                               const   EapAttributes      *pAttributeArray,
    _In_                                       HANDLE             hTokenImpersonateUser,
    _In_                                       DWORD              dwConnectionDataSize,
    _In_count_(dwConnectionDataSize) /*const*/ BYTE               *pConnectionData,
    _In_                                       DWORD              dwUserDataSize,
    _In_count_(dwUserDataSize)       /*const*/ BYTE               *pUserData,
    _In_                                       DWORD              dwMaxSendPacketSize,
    _Out_                                      EAP_SESSION_HANDLE *phSession,
    _Out_                                      EAP_ERROR          **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!pConnectionData && dwConnectionDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pConnectionData is NULL.")));
    else if (!pUserData && dwUserDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pUserData is NULL.")));
    else if (!phSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" phSession is NULL.")));
    else {
        try {
            *phSession = g_peer.begin_session(dwFlags, pAttributeArray, hTokenImpersonateUser, pConnectionData, dwConnectionDataSize, pUserData, dwUserDataSize, dwMaxSendPacketSize);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Ends an EAP authentication session for the EAP method.
///
/// \sa [EapPeerEndSession function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363604.aspx)
///
DWORD APIENTRY EapPeerEndSession(
    _In_  EAP_SESSION_HANDLE hSession,
    _Out_ EAP_ERROR          **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else {
        try {
            g_peer.end_session(hSession);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Processes a packet received by EapHost from a supplicant.
///
/// \sa [EapPeerProcessRequestPacket function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363621.aspx)
///
DWORD APIENTRY EapPeerProcessRequestPacket(
    _In_                                           EAP_SESSION_HANDLE  hSession,
    _In_                                           DWORD               dwReceivedPacketSize,
    _In_bytecount_(dwReceivedPacketSize) /*const*/ EapPacket           *pReceivedPacket,
    _Out_                                          EapPeerMethodOutput *pEapOutput,
    _Out_                                          EAP_ERROR           **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pReceivedPacket || dwReceivedPacketSize < 6)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pReceivedPacket is NULL or too short.")));
    else if (pReceivedPacket->Data[0] != EAPMETHOD_TYPE)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, wstring_printf(_T(__FUNCTION__) _T(" Packet EAP type (%d) does not match the supported EAP type (%d)."), (int)pReceivedPacket->Data[0], (int)EAPMETHOD_TYPE).c_str()));
    else if (!pEapOutput)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapOutput is NULL.")));
    else {
        try {
            g_peer.process_request_packet(hSession, pReceivedPacket, dwReceivedPacketSize, pEapOutput);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Obtains a response packet from the EAP method.
///
/// \sa [EapPeerGetResponsePacket function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363610.aspx)
///
DWORD APIENTRY EapPeerGetResponsePacket(
    _In_                               EAP_SESSION_HANDLE hSession,
    _Inout_                            DWORD              *pdwSendPacketSize,
    _Inout_bytecap_(*dwSendPacketSize) EapPacket          *pSendPacket,
    _Out_                              EAP_ERROR          **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pdwSendPacketSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pdwSendPacketSize is NULL.")));
    else if (!pSendPacket && *pdwSendPacketSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pSendPacket is NULL.")));
    else {
        try {
            g_peer.get_response_packet(hSession, pSendPacket, pdwSendPacketSize);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Obtains the result of an authentication session from the EAP method.
///
/// \sa [EapPeerGetResult function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363611.aspx)
///
DWORD APIENTRY EapPeerGetResult(
    _In_  EAP_SESSION_HANDLE        hSession,
    _In_  EapPeerMethodResultReason reason,
    _Out_ EapPeerMethodResult       *pResult,
    _Out_ EAP_ERROR                 **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pResult)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pResult is NULL.")));
    else {
        try {
            g_peer.get_result(hSession, reason, pResult);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Obtains the user interface context from the EAP method.
///
/// \note This function is always followed by the `EapPeerInvokeInteractiveUI()` function, which is followed by the `EapPeerSetUIContext()` function.
///
/// \sa [EapPeerGetUIContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363612.aspx)
///
DWORD APIENTRY EapPeerGetUIContext(
    _In_  EAP_SESSION_HANDLE hSession,
    _Out_ DWORD              *pdwUIContextDataSize,
    _Out_ BYTE               **ppUIContextData,
    _Out_ EAP_ERROR          **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pdwUIContextDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pdwUIContextDataSize is NULL.")));
    else if (!ppUIContextData)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" ppUIContextData is NULL.")));
    else {
        try {
            g_peer.get_ui_context(hSession, ppUIContextData, pdwUIContextDataSize);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Provides a user interface context to the EAP method.
///
/// \note This function is called after the UI has been raised through the `EapPeerGetUIContext()` function.
///
/// \sa [EapPeerSetUIContext function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363626.aspx)
///
DWORD APIENTRY EapPeerSetUIContext(
    _In_                                  EAP_SESSION_HANDLE  hSession,
    _In_                                  DWORD               dwUIContextDataSize,
    _In_count_(dwUIContextDataSize) const BYTE                *pUIContextData,
    _Out_                                 EapPeerMethodOutput *pEapOutput,
    _Out_                                 EAP_ERROR           **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pUIContextData && dwUIContextDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pUIContextData is NULL.")));
    else if (!pEapOutput)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapOutput is NULL.")));
    else {
        try {
            g_peer.set_ui_context(hSession, pUIContextData, dwUIContextDataSize, pEapOutput);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Obtains an array of EAP response attributes from the EAP method.
///
/// \sa [EapPeerGetResponseAttributes function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363609.aspx)
///
DWORD APIENTRY EapPeerGetResponseAttributes(
    _In_  EAP_SESSION_HANDLE hSession,
    _Out_ EapAttributes      *pAttribs,
    _Out_ EAP_ERROR          **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pAttribs)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pAttribs is NULL.")));
    else {
        try {
            g_peer.get_response_attributes(hSession, pAttribs);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Provides an updated array of EAP response attributes to the EAP method.
///
/// \sa [EapPeerSetResponseAttributes function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363625.aspx)
///
DWORD APIENTRY EapPeerSetResponseAttributes(
    _In_            EAP_SESSION_HANDLE  hSession,
    _In_  /*const*/ EapAttributes       *pAttribs,
    _Out_           EapPeerMethodOutput *pEapOutput,
    _Out_           EAP_ERROR           **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!hSession)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" hSession is NULL.")));
    else if (!pEapOutput)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapOutput is NULL.")));
    else {
        try {
            g_peer.set_response_attributes(hSession, pAttribs, pEapOutput);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Defines the implementation of an EAP method-specific function that retrieves the properties of an EAP method given the connection and user data.
///
/// \sa [EapPeerGetMethodProperties function](https://msdn.microsoft.com/en-us/library/windows/desktop/hh706636.aspx)
///
DWORD WINAPI EapPeerGetMethodProperties(
    _In_                                   DWORD                     dwVersion,
    _In_                                   DWORD                     dwFlags,
    _In_                                   EAP_METHOD_TYPE           eapMethodType,
    _In_                                   HANDLE                    hUserImpersonationToken,
    _In_                                   DWORD                     dwConnectionDataSize,
    _In_count_(dwConnectionDataSize) const BYTE                      *pConnectionData,
    _In_                                   DWORD                     dwUserDataSize,
    _In_count_(dwUserDataSize)       const BYTE                      *pUserData,
    _Out_                                  EAP_METHOD_PROPERTY_ARRAY *pMethodPropertyArray,
    _Out_                                  EAP_ERROR                 **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (eapMethodType.eapType.type != EAPMETHOD_TYPE)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" Input EAP type (%d) does not match the supported EAP type (%d)."), (int)eapMethodType.eapType.type, (int)EAPMETHOD_TYPE).c_str()));
    else if (eapMethodType.dwAuthorId != 67532)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" EAP author (%d) does not match the supported author (%d)."), (int)eapMethodType.dwAuthorId, (int)67532).c_str()));
    else if (!pConnectionData && dwConnectionDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pConnectionData is NULL.")));
    else if (!pUserData && dwUserDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pUserData is NULL.")));
    else if (!pMethodPropertyArray)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pMethodPropertyArray is NULL.")));
    else {
        try {
            g_peer.get_method_properties(dwVersion, dwFlags, hUserImpersonationToken, pConnectionData, dwConnectionDataSize, pUserData, dwUserDataSize, pMethodPropertyArray);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Converts XML into the configuration BLOB. The XML based credentials can come from group policy or from a system administrator.
///
/// \sa [EapPeerCredentialsXml2Blob function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363603.aspx)
///
DWORD WINAPI EapPeerCredentialsXml2Blob(
    _In_                                   DWORD            dwFlags,
    _In_                                   EAP_METHOD_TYPE  eapMethodType,
    _In_                                   IXMLDOMDocument2 *pCredentialsDoc,
    _In_count_(dwConnectionDataSize) const BYTE             *pConnectionData,
    _In_                                   DWORD            dwConnectionDataSize,
    _Out_                                  BYTE             **ppCredentialsOut,
    _Out_                                  DWORD            *pdwCredentialsOutSize,
    _Out_                                  EAP_ERROR        **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (eapMethodType.eapType.type != EAPMETHOD_TYPE)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" Input EAP type (%d) does not match the supported EAP type (%d)."), (int)eapMethodType.eapType.type, (int)EAPMETHOD_TYPE).c_str()));
    else if (eapMethodType.dwAuthorId != 67532)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" EAP author (%d) does not match the supported author (%d)."), (int)eapMethodType.dwAuthorId, (int)67532).c_str()));
    else if (!pCredentialsDoc)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pCredentialsDoc is NULL.")));
    else if (!pConnectionData && dwConnectionDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pConnectionData is NULL.")));
    else if (!ppCredentialsOut)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" ppCredentialsOut is NULL.")));
    else if (!pdwCredentialsOutSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pdwCredentialsOutSize is NULL.")));
    else {
        // <Credentials>
        com_obj<IXMLDOMNode> pXmlElCredentials;
        if (FAILED(eapxml::select_node(pCredentialsDoc, bstr(L"//EapHostUserCredentials/Credentials"), pXmlElCredentials))) {
            g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_FOUND, _T(__FUNCTION__) _T(" Error selecting <EapHostUserCredentials><Credentials> element.")));
            return dwResult;
        }

        // Load credentials.
        pCredentialsDoc->setProperty(bstr(L"SelectionNamespaces"), variant(L"xmlns:eap-metadata=\"urn:ietf:params:xml:ns:yang:ietf-eap-metadata\""));
        try {
            g_peer.credentials_xml2blob(dwFlags, pXmlElCredentials, pConnectionData, dwConnectionDataSize, ppCredentialsOut, pdwCredentialsOutSize);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Defines the implementation of an EAP method-specific function that obtains the EAP Single-Sign-On (SSO) credential input fields for an EAP method.
///
/// \sa [EapPeerQueryCredentialInputFields function](https://msdn.microsoft.com/en-us/library/windows/desktop/aa363622.aspx)
///
DWORD WINAPI EapPeerQueryCredentialInputFields(
    _In_                                   HANDLE                       hUserImpersonationToken,
    _In_                                   EAP_METHOD_TYPE              eapMethodType,
    _In_                                   DWORD                        dwFlags,
    _In_                                   DWORD                        dwConnectionDataSize,
    _In_count_(dwConnectionDataSize) const BYTE                         *pConnectionData,
    _Out_                                  EAP_CONFIG_INPUT_FIELD_ARRAY *pEapConfigInputFieldsArray,
    _Out_                                  EAP_ERROR                    **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (eapMethodType.eapType.type != EAPMETHOD_TYPE)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" Input EAP type (%d) does not match the supported EAP type (%d)."), (int)eapMethodType.eapType.type, (int)EAPMETHOD_TYPE).c_str()));
    else if (eapMethodType.dwAuthorId != 67532)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" EAP author (%d) does not match the supported author (%d)."), (int)eapMethodType.dwAuthorId, (int)67532).c_str()));
    else if (!pConnectionData && dwConnectionDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pConnectionData is NULL.")));
    else if (!pEapConfigInputFieldsArray)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapConfigInputFieldsArray is NULL.")));
    else {
        try {
            g_peer.query_credential_input_fields(hUserImpersonationToken, dwFlags, dwConnectionDataSize, pConnectionData, pEapConfigInputFieldsArray);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Defines the implementation of an EAP method function that obtains the user BLOB data provided in an interactive Single-Sign-On (SSO) UI raised on the supplicant.
///
/// \sa [EapPeerQueryUserBlobFromCredentialInputFields function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb204697.aspx)
///
DWORD WINAPI EapPeerQueryUserBlobFromCredentialInputFields(
    _In_                                   HANDLE                       hUserImpersonationToken,
    _In_                                   EAP_METHOD_TYPE              eapMethodType,
    _In_                                   DWORD                        dwFlags,
    _In_                                   DWORD                        dwConnectionDataSize,
    _In_count_(dwConnectionDataSize) const BYTE                         *pConnectionData,
    _In_                             const EAP_CONFIG_INPUT_FIELD_ARRAY *pEapConfigInputFieldArray,
    _Inout_                                DWORD                        *pdwUsersBlobSize,
    _Inout_                                BYTE                         **ppUserBlob,
    _Out_                                  EAP_ERROR                    **ppEapError)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (eapMethodType.eapType.type != EAPMETHOD_TYPE)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" Input EAP type (%d) does not match the supported EAP type (%d)."), (int)eapMethodType.eapType.type, (int)EAPMETHOD_TYPE).c_str()));
    else if (eapMethodType.dwAuthorId != 67532)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_NOT_SUPPORTED, wstring_printf(_T(__FUNCTION__) _T(" EAP author (%d) does not match the supported author (%d)."), (int)eapMethodType.dwAuthorId, (int)67532).c_str()));
    else if (!pConnectionData && dwConnectionDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pConnectionData is NULL.")));
    else if (!pEapConfigInputFieldArray)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapConfigInputFieldArray is NULL.")));
    else if (!pdwUsersBlobSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pdwUsersBlobSize is NULL.")));
    else if (!ppUserBlob)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" ppUserBlob is NULL.")));
    else {
        try {
            g_peer.query_user_blob_from_credential_input_fields(hUserImpersonationToken, dwFlags, dwConnectionDataSize, pConnectionData, pEapConfigInputFieldArray, pdwUsersBlobSize, ppUserBlob);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Defines the implementation of an EAP method API that provides the input fields for interactive UI components to be raised on the supplicant.
///
/// \sa [EapPeerQueryInteractiveUIInputFields function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb204695.aspx)
///
DWORD WINAPI EapPeerQueryInteractiveUIInputFields(
    _In_                                  DWORD                   dwVersion,
    _In_                                  DWORD                   dwFlags,
    _In_                                  DWORD                   dwUIContextDataSize,
    _In_count_(dwUIContextDataSize) const BYTE                    *pUIContextData,
    _Out_                                 EAP_INTERACTIVE_UI_DATA *pEapInteractiveUIData,
    _Out_                                 EAP_ERROR               **ppEapError,
    _Inout_                               LPVOID                  *ppvReserved)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!pUIContextData && dwUIContextDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pUIContextData is NULL.")));
    else if (!pUIContextData && dwUIContextDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pUIContextData is NULL.")));
    else if (!pEapInteractiveUIData)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapInteractiveUIData is NULL.")));
    else {
        UNREFERENCED_PARAMETER(ppvReserved);

        try {
            g_peer.query_interactive_ui_input_fields(dwVersion, dwFlags, dwUIContextDataSize, pUIContextData, pEapInteractiveUIData);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}


///
/// Converts user information into a user BLOB that can be consumed by EapHost run-time functions.
///
/// \sa [EapPeerQueryUIBlobFromInteractiveUIInputFields function](https://msdn.microsoft.com/en-us/library/windows/desktop/bb204696.aspx)
///
DWORD WINAPI EapPeerQueryUIBlobFromInteractiveUIInputFields(
    _In_                                  DWORD                   dwVersion,
    _In_                                  DWORD                   dwFlags,
    _In_                                  DWORD                   dwUIContextDataSize,
    _In_count_(dwUIContextDataSize) const BYTE                    *pUIContextData,
    _In_                            const EAP_INTERACTIVE_UI_DATA *pEapInteractiveUIData,
    _Out_                                 DWORD                   *pdwDataFromInteractiveUISize,
    _Out_                                 BYTE                    **ppDataFromInteractiveUI,
    _Out_                                 EAP_ERROR               **ppEapError,
    _Inout_                               LPVOID                  *ppvReserved)
{
    DWORD dwResult = ERROR_SUCCESS;
    event_fn_auto_ret<DWORD> event_auto(g_peer.get_event_fn_auto(__FUNCTION__, dwResult));
#ifdef _DEBUG
    //Sleep(10000);
#endif

    // Parameter check
    if (!ppEapError)
        return dwResult = ERROR_INVALID_PARAMETER;

    assert(!*ppEapError);

    if (!pUIContextData && dwUIContextDataSize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapConfigInputFieldArray is NULL.")));
    else if (!pEapInteractiveUIData)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pEapInteractiveUIData is NULL.")));
    else if (!pdwDataFromInteractiveUISize)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" pdwDataFromInteractiveUISize is NULL.")));
    else if (!ppDataFromInteractiveUI)
        g_peer.log_error(*ppEapError = g_peer.make_error(dwResult = ERROR_INVALID_PARAMETER, _T(__FUNCTION__) _T(" ppDataFromInteractiveUI is NULL.")));
    else {
        UNREFERENCED_PARAMETER(ppvReserved);

        try {
            g_peer.query_ui_blob_from_interactive_ui_input_fields(dwVersion, dwFlags, dwUIContextDataSize, pUIContextData, pEapInteractiveUIData, pdwDataFromInteractiveUISize, ppDataFromInteractiveUI);
        } catch (std::exception &err) {
            g_peer.log_error(*ppEapError = g_peer.make_error(err));
            dwResult = (*ppEapError)->dwWinError;
        } catch (...) {
            dwResult = ERROR_INVALID_DATA;
        }
    }

    return dwResult;
}
