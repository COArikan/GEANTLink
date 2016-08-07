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

using namespace std;
using namespace winstd;


//////////////////////////////////////////////////////////////////////
// eap::method_tls
//////////////////////////////////////////////////////////////////////

eap::method_tls::method_tls(_In_ module &module, _In_ config_method_tls &cfg, _In_ credentials_tls &cred) :
    m_phase(phase_client_hello),
    m_seq_num(0),
    method(module, cfg, cred)
{
    m_packet_req.m_code  = (EapCode)0;
    m_packet_req.m_id    = 0;
    m_packet_req.m_flags = 0;

    m_packet_res.m_code  = (EapCode)0;
    m_packet_res.m_id    = 0;
    m_packet_res.m_flags = 0;

    memset(&m_random_client, 0, sizeof(tls_random_t));
    memset(&m_random_server, 0, sizeof(tls_random_t));
}


eap::method_tls::method_tls(_In_ const method_tls &other) :
    m_phase(other.m_phase),
    m_session_id(other.m_session_id),
    m_hash_handshake_msgs_md5(other.m_hash_handshake_msgs_md5),
    m_hash_handshake_msgs_sha1(other.m_hash_handshake_msgs_sha1),
    m_seq_num(other.m_seq_num),
    method(other)
{
    m_packet_req.m_code  = other.m_packet_req.m_code ;
    m_packet_req.m_id    = other.m_packet_req.m_id   ;
    m_packet_req.m_flags = other.m_packet_req.m_flags;
    m_packet_req.m_data  = other.m_packet_req.m_data ;

    m_packet_res.m_code  = other.m_packet_res.m_code ;
    m_packet_res.m_id    = other.m_packet_res.m_id   ;
    m_packet_res.m_flags = other.m_packet_res.m_flags;
    m_packet_res.m_data  = other.m_packet_res.m_data ;

    memcpy(&m_random_client, &other.m_random_client, sizeof(tls_random_t));
    memcpy(&m_random_server, &other.m_random_server, sizeof(tls_random_t));
}


eap::method_tls::method_tls(_Inout_ method_tls &&other) :
    m_phase(std::move(other.m_phase)),
    m_session_id(std::move(other.m_session_id)),
    m_hash_handshake_msgs_md5(std::move(other.m_hash_handshake_msgs_md5)),
    m_hash_handshake_msgs_sha1(std::move(other.m_hash_handshake_msgs_sha1)),
    m_seq_num(std::move(other.m_seq_num)),
    method(std::move(other))
{
    m_packet_req.m_code  = std::move(other.m_packet_req.m_code );
    m_packet_req.m_id    = std::move(other.m_packet_req.m_id   );
    m_packet_req.m_flags = std::move(other.m_packet_req.m_flags);
    m_packet_req.m_data  = std::move(other.m_packet_req.m_data );

    m_packet_res.m_code  = std::move(other.m_packet_res.m_code );
    m_packet_res.m_id    = std::move(other.m_packet_res.m_id   );
    m_packet_res.m_flags = std::move(other.m_packet_res.m_flags);
    m_packet_res.m_data  = std::move(other.m_packet_res.m_data );

    memcpy(&m_random_client, &other.m_random_client, sizeof(tls_random_t));
    memcpy(&m_random_server, &other.m_random_server, sizeof(tls_random_t));
}


eap::method_tls::~method_tls()
{
    SecureZeroMemory(&m_random_client, sizeof(tls_random_t));
    SecureZeroMemory(&m_random_server, sizeof(tls_random_t));
}


eap::method_tls& eap::method_tls::operator=(_In_ const method_tls &other)
{
    if (this != std::addressof(other)) {
        (method&)*this             = other;

        m_phase                    = other.m_phase;

        m_packet_req.m_code        = other.m_packet_req.m_code ;
        m_packet_req.m_id          = other.m_packet_req.m_id   ;
        m_packet_req.m_flags       = other.m_packet_req.m_flags;
        m_packet_req.m_data        = other.m_packet_req.m_data ;

        m_packet_res.m_code        = other.m_packet_res.m_code ;
        m_packet_res.m_id          = other.m_packet_res.m_id   ;
        m_packet_res.m_flags       = other.m_packet_res.m_flags;
        m_packet_res.m_data        = other.m_packet_res.m_data ;

        memcpy(&m_random_client, &other.m_random_client, sizeof(tls_random_t));
        memcpy(&m_random_server, &other.m_random_server, sizeof(tls_random_t));

        m_session_id               = other.m_session_id;

        m_hash_handshake_msgs_md5  = other.m_hash_handshake_msgs_md5;
        m_hash_handshake_msgs_sha1 = other.m_hash_handshake_msgs_sha1;

        m_seq_num                  = other.m_seq_num;
    }

    return *this;
}


eap::method_tls& eap::method_tls::operator=(_Inout_ method_tls &&other)
{
    if (this != std::addressof(other)) {
        (method&)*this             = std::move(other);

        m_phase                    = std::move(other.m_phase);

        m_packet_req.m_code        = std::move(other.m_packet_req.m_code );
        m_packet_req.m_id          = std::move(other.m_packet_req.m_id   );
        m_packet_req.m_flags       = std::move(other.m_packet_req.m_flags);
        m_packet_req.m_data        = std::move(other.m_packet_req.m_data );

        m_packet_res.m_code        = std::move(other.m_packet_res.m_code );
        m_packet_res.m_id          = std::move(other.m_packet_res.m_id   );
        m_packet_res.m_flags       = std::move(other.m_packet_res.m_flags);
        m_packet_res.m_data        = std::move(other.m_packet_res.m_data );

        memcpy(&m_random_client, &other.m_random_client, sizeof(tls_random_t));
        memcpy(&m_random_server, &other.m_random_server, sizeof(tls_random_t));

        m_session_id               = std::move(other.m_session_id);

        m_hash_handshake_msgs_md5  = std::move(other.m_hash_handshake_msgs_md5);
        m_hash_handshake_msgs_sha1 = std::move(other.m_hash_handshake_msgs_sha1);

        m_seq_num                  = std::move(other.m_seq_num);
    }

    return *this;
}


bool eap::method_tls::begin_session(
    _In_        DWORD         dwFlags,
    _In_  const EapAttributes *pAttributeArray,
    _In_        HANDLE        hTokenImpersonateUser,
    _In_        DWORD         dwMaxSendPacketSize,
    _Out_       EAP_ERROR     **ppEapError)
{
    if (!eap::method::begin_session(dwFlags, pAttributeArray, hTokenImpersonateUser, dwMaxSendPacketSize, ppEapError))
        return false;

    // Create cryptographics provider.
    if (!m_cp.create(NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error creating cryptographics provider."));
        return false;
    }

    // Generate client randomness.
    m_random_client.time = (unsigned long)time(NULL);
    if (!CryptGenRandom(m_cp, sizeof(m_random_client.data), m_random_client.data)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error creating client randomness."));
        return false;
    }

    if (!m_hash_handshake_msgs_md5.create(m_cp, CALG_MD5, NULL, 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error creating MD5 hashing object."));
        return false;
    }

    if (!m_hash_handshake_msgs_sha1.create(m_cp, CALG_SHA1, NULL, 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error creating SHA-1 hashing object."));
        return false;
    }

    // HMAC symmetric key generation sample. To be used later...
    //crypt_hash hash_key;
    //hash_key.create(m_cp, CALG_SHA1, 0, 0);
    //CryptHashData(hash_key, Data1, sizeof(Data1), 0);
    //m_key_hmac.derive(m_cp, CALG_RC4, hash_key, 0);

    return true;
}


bool eap::method_tls::process_request_packet(
    _In_bytecount_(dwReceivedPacketSize) const EapPacket           *pReceivedPacket,
    _In_                                       DWORD               dwReceivedPacketSize,
    _Out_                                      EapPeerMethodOutput *pEapOutput,
    _Out_                                      EAP_ERROR           **ppEapError)
{
    assert(pReceivedPacket && dwReceivedPacketSize >= 4);
    assert(pEapOutput);
    assert(ppEapError);

    // Is this a valid EAP-TLS packet?
    if (dwReceivedPacketSize < 6) {
        *ppEapError = m_module.make_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, _T(__FUNCTION__) _T(" Packet is too small. EAP-%s packets should be at least 6B."));
        return false;
    }/* else if (pReceivedPacket->Data[0] != eap_type_tls) { // Skip method check, to allow TTLS extension.
        *ppEapError = m_module.make_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, wstring_printf(_T(__FUNCTION__) _T(" Packet is not EAP-TLS (expected: %u, received: %u)."), eap_type_tls, pReceivedPacket->Data[0]).c_str());
        return false;
    }*/

    if (pReceivedPacket->Data[1] & tls_req_flags_more_frag) {
        if (pReceivedPacket->Data[1] & tls_req_flags_length_incl) {
            // First fragment received.
            if (dwReceivedPacketSize < 10) {
                *ppEapError = m_module.make_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, _T(__FUNCTION__) _T(" Packet is too small. EAP-TLS first fragmented packet should be at least 10B."));
                return false;
            }

            // Start a new packet.
            m_packet_req.m_code  = (EapCode)pReceivedPacket->Code;
            m_packet_req.m_id    = pReceivedPacket->Id;
            m_packet_req.m_flags = pReceivedPacket->Data[1];
            m_packet_req.m_data.reserve(*(unsigned long*)(pReceivedPacket->Data + 2));
            m_packet_req.m_data.assign(pReceivedPacket->Data + 6, pReceivedPacket->Data + dwReceivedPacketSize - 4);
        } else {
            // Mid fragment received. Append data.
            m_packet_req.m_data.insert(m_packet_req.m_data.end(), pReceivedPacket->Data + 2, pReceivedPacket->Data + dwReceivedPacketSize - 4);
        }

        // Reply with ACK packet.
        m_packet_res.m_code  = EapCodeResponse;
        m_packet_res.m_id    = m_packet_req.m_id;
        m_packet_res.m_flags = 0;
        m_packet_res.m_data.clear();
        pEapOutput->fAllowNotifications = FALSE;
        pEapOutput->action = EapPeerMethodResponseActionSend;
        return true;
    } else if (!m_packet_req.m_data.empty()) {
        // Last fragment received. Append data.
        m_packet_req.m_data.insert(m_packet_req.m_data.end(),
            pReceivedPacket->Data + (!(pReceivedPacket->Data[1] & tls_req_flags_length_incl) ? 2 : 6), // Should not include "Length" field (by RFC 5281: https://tools.ietf.org/html/rfc5281#section-9.2.2). Tolerate.
            pReceivedPacket->Data + dwReceivedPacketSize - 4);
    } else {
        // This is a complete non-fragmented packet.
        m_packet_req.m_code  = (EapCode)pReceivedPacket->Code;
        m_packet_req.m_id    = pReceivedPacket->Id;
        m_packet_req.m_flags = pReceivedPacket->Data[1];
        m_packet_req.m_data.assign(
            pReceivedPacket->Data + (!(pReceivedPacket->Data[1] & tls_req_flags_length_incl) ? 2 : 6),
            pReceivedPacket->Data + dwReceivedPacketSize - 4);
    }

    if (  m_packet_req.m_code == EapCodeRequest                                                               &&
          m_packet_req.m_id   == m_packet_res.m_id                                                            &&
          m_packet_req.m_data.empty()                                                                         &&
        !(m_packet_req.m_flags & (tls_req_flags_length_incl | tls_req_flags_more_frag | tls_req_flags_start)) &&
         (m_packet_res.m_flags &                              tls_res_flags_more_frag                      ))
    {
        // This is an ACK of our fragmented packet response. Send the next fragment.
        m_packet_res.m_id++;
        pEapOutput->fAllowNotifications = FALSE;
        pEapOutput->action = EapPeerMethodResponseActionSend;
        return true;
    }

    switch (m_phase) {
        case phase_client_hello: {
            // Is this an EAP-TLS Start packet?
            if (m_packet_req.m_code != EapCodeRequest) {
                *ppEapError = m_module.make_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, wstring_printf(_T(__FUNCTION__) _T(" Packet is not a request (expected: %x, received: %x)."), EapCodeRequest, m_packet_req.m_code).c_str());
                return false;
            } else if (!(m_packet_req.m_flags & tls_req_flags_start)) {
                *ppEapError = m_module.make_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, wstring_printf(_T(__FUNCTION__) _T(" Packet is not EAP-TLS Start (expected: %x, received: %x)."), tls_req_flags_start, m_packet_req.m_flags).c_str());
                return false;
            }

            // Build response packet.
            m_packet_res.m_code  = EapCodeResponse;
            m_packet_res.m_id    = m_packet_req.m_id;
            m_packet_res.m_flags = 0;
            sanitizing_blob hello(make_client_hello());
            sanitizing_blob handshake;
            if (!make_handshake(hello, false, handshake, ppEapError)) return false;
            m_packet_res.m_data.assign(handshake.begin(), handshake.end());
            pEapOutput->fAllowNotifications = FALSE;
            pEapOutput->action = EapPeerMethodResponseActionSend;

            // Save the client_hello message.
            CryptHashData(m_hash_handshake_msgs_md5 , hello.data(), (DWORD)hello.size(), 0);
            CryptHashData(m_hash_handshake_msgs_sha1, hello.data(), (DWORD)hello.size(), 0);

            m_phase = phase_server_hello;
            break;
        }

        case phase_server_hello:

        default:
            *ppEapError = m_module.make_error(ERROR_NOT_SUPPORTED, _T(__FUNCTION__) _T(" Not supported."));
            return false;
    }

    return true;
}


bool eap::method_tls::get_response_packet(
    _Inout_bytecap_(*dwSendPacketSize) EapPacket *pSendPacket,
    _Inout_                            DWORD     *pdwSendPacketSize,
    _Out_                              EAP_ERROR **ppEapError)
{
    assert(pdwSendPacketSize);
    assert(pSendPacket);
    UNREFERENCED_PARAMETER(ppEapError);

    unsigned long
        size_data   = (unsigned long)m_packet_res.m_data.size(),
        size_packet = size_data + 6;
    unsigned short size_packet_limit = (unsigned short)std::min<unsigned long>(*pdwSendPacketSize, USHRT_MAX);
    unsigned char *data_dst;

    if (!(m_packet_res.m_flags & tls_res_flags_more_frag)) {
        // Not fragmented.
        if (size_packet <= size_packet_limit) {
            // No need to fragment the packet.
            m_packet_res.m_flags &= ~tls_res_flags_length_incl; // No need to explicitly include the Length field either.
            data_dst = pSendPacket->Data + 2;
        } else {
            // But it should be fragmented.
            m_packet_res.m_flags |= tls_res_flags_length_incl | tls_res_flags_more_frag;
            *(unsigned long*)(pSendPacket->Data + 2) = (unsigned long)size_packet;
            data_dst = pSendPacket->Data + 6;
            size_data   = size_packet_limit - 10;
            size_packet = size_packet_limit;
        }
    } else {
        // Continuing the fragmented packet...
        if (size_packet <= size_packet_limit) {
            // This is the last fragment.
            m_packet_res.m_flags &= ~(tls_res_flags_length_incl | tls_res_flags_more_frag);
        } else {
            // This is a mid fragment.
            m_packet_res.m_flags &= ~tls_res_flags_length_incl;
            size_data   = size_packet_limit - 6;
            size_packet = size_packet_limit;
        }
        data_dst = pSendPacket->Data + 2;
    }

    pSendPacket->Code = (BYTE)m_packet_res.m_code;
    pSendPacket->Id   = m_packet_res.m_id;
    *(unsigned short*)pSendPacket->Length = htons((unsigned short)size_packet);
    pSendPacket->Data[0] = (BYTE)eap_type_tls;
    pSendPacket->Data[1] = m_packet_res.m_flags;
    memcpy(data_dst, m_packet_res.m_data.data(), size_data);
    m_packet_res.m_data.erase(m_packet_res.m_data.begin(), m_packet_res.m_data.begin() + size_data);
    *pdwSendPacketSize = size_packet;
    return true;
}


eap::sanitizing_blob eap::method_tls::make_client_hello() const
{
    size_t size_data;
    sanitizing_blob msg;
    msg.reserve(
        4                    + // SSL header
        (size_data =
        2                    + // SSL version
        sizeof(tls_random_t) + // Client random
        1                    + // Session ID size
        m_session_id.size()) + // Session ID
        2                    + // Length of cypher suite list
        2                    + // Cyper suite list
        1                    + // Length of compression suite
        1);                    // Compression suite

    // SSL header
    assert(size_data <= 0xffffff);
    unsigned long ssl_header = htonl(0x01000000 | (unsigned long)size_data); // client_hello (0x01)
    msg.insert(msg.end(), (unsigned char*)&ssl_header, (unsigned char*)(&ssl_header + 1));

    // SSL version
    msg.push_back(3); // SSL major version
    msg.push_back(3); // SSL minor version

    // Client random
    unsigned long time = htonl(m_random_client.time);
    msg.insert(msg.end(), (unsigned char*)&time, (unsigned char*)(&time + 1));
    msg.insert(msg.end(), m_random_client.data, m_random_client.data + _countof(m_random_client.data)); // TODO: Check if byte order should be changed!

    // Session ID
    assert(m_session_id.size() <= 32);
    msg.push_back((unsigned char)m_session_id.size());
    msg.insert(msg.end(), m_session_id.begin(), m_session_id.end());

    // Cypher suite list
    msg.push_back(0x00); // Length of cypher suite is two (in network-byte-order).
    msg.push_back(0x02); // --^
    msg.push_back(0x00); // TLS_RSA_WITH_3DES_EDE_CBC_SHA (0x00 0x0a)
    msg.push_back(0x0a); // --^

    // Compression
    msg.push_back(0x01); // Length of compression section
    msg.push_back(0x00); // No compression (0)

    return msg;
}


bool eap::method_tls::make_handshake(_In_ const sanitizing_blob &msg, _In_ bool encrypt, _Out_ eap::sanitizing_blob &msg_h, _Out_ EAP_ERROR **ppEapError)
{
    const unsigned char *msg_ptr;
    size_t size_msg;
    vector<unsigned char> msg_enc;

    if (encrypt) {
        // Create an unencrypted handshake first.
        eap::sanitizing_blob msg_h_unenc;
        if (!make_handshake(msg, false, msg_h_unenc, ppEapError)) return false;

        // Encrypt it.
        if (!encrypt_message(msg_h_unenc, msg_enc, ppEapError)) return false;

        size_msg = msg_enc.size();
        msg_ptr  = msg_enc.data();
    } else {
        size_msg = msg.size();
        msg_ptr  = msg.data();
    }

    // Create a handshake.
    msg_h.clear();
    msg_h.reserve(
        1        + // SSL record type
        2        + // SSL version
        2        + // Message size
        size_msg); // Message

    // SSL record type
    msg_h.push_back(22); // handshake (22)

    // SSL version
    msg_h.push_back(3); // SSL major version
    msg_h.push_back(3); // SSL minor version

    // Message
    assert(size_msg <= 0xffff);
    unsigned short size_msg_n = htons((unsigned short)size_msg);
    msg_h.insert(msg_h.end(), (unsigned char*)&size_msg_n, (unsigned char*)(&size_msg_n + 1));
    msg_h.insert(msg_h.end(), msg_ptr, msg_ptr + size_msg);

    return true;
}


bool eap::method_tls::encrypt_message(_In_ const sanitizing_blob &msg, _Out_ std::vector<unsigned char> &msg_enc, _Out_ EAP_ERROR **ppEapError)
{
    assert(ppEapError);

    // Create a HMAC hash.
    crypt_hash hash_hmac;
    if (!hash_hmac.create(m_cp, CALG_HMAC, m_key_hmac, 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error creating HMAC hash."));
        return false;
    }
    static const HMAC_INFO s_hmac_info = { CALG_SHA1 };
    if (!CryptSetHashParam(hash_hmac, HP_HMAC_INFO, (const BYTE*)&s_hmac_info, 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error setting hash parameter."));
        return false;
    }

    // Hash sequence number.
    unsigned __int64 seq_num = htonll(m_seq_num);
    if (!CryptHashData(hash_hmac, (const BYTE*)&seq_num, sizeof(seq_num), 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error hashing data."));
        return false;
    }

    // Hash message.
    if (!CryptHashData(hash_hmac, msg.data(), (DWORD)msg.size(), 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error hashing data."));
        return false;
    }

    // Calculate hash.
    vector<unsigned char> hmac;
    if (!CryptGetHashParam(hash_hmac, HP_HASHVAL, hmac, 0)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error finishing hash calculation."));
        return false;
    }

    size_t size =
        msg.size() - 5 + // TLS message without SSL header (SSL record type, SSL version, Message size)
        20             + // SHA-1
        1;               // Padding length
    unsigned char padding = (8 - size) % 8;
    size += padding;

    // Copy data.
    sanitizing_blob enc;
    enc.reserve(size);
    enc.assign(msg.begin() + 5, msg.end());

    // Append HMAC hash.
#ifdef _HOST_LOW_ENDIAN
    std::reverse(hmac.begin(), hmac.end());
#endif
    enc.insert(enc.end(), hmac.begin(), hmac.end());

    // Append padding.
    enc.insert(enc.end(), padding + 1, padding);

    // Encrypt.
    assert(size < 0xffffffff);
    DWORD size2 = (DWORD)size;
    if (!CryptEncrypt(m_key_write, NULL, FALSE, 0, enc.data(), &size2, (DWORD)size)) {
        *ppEapError = m_module.make_error(GetLastError(), _T(__FUNCTION__) _T(" Error encrypting message."));
        return false;
    }

    // Increment sequence number.
    m_seq_num++;

    // Copy to output.
    msg_enc.assign(enc.begin(), enc.end());
    return true;
}