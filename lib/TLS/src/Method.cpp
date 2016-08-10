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
// eap::method_tls::packet
//////////////////////////////////////////////////////////////////////

eap::method_tls::packet::packet() :
    m_code((EapCode)0),
    m_id(0),
    m_flags(0)
{
}


eap::method_tls::packet::packet(_In_ const packet &other) :
    m_code (other.m_code ),
    m_id   (other.m_id   ),
    m_flags(other.m_flags),
    m_data (other.m_data )
{
}


eap::method_tls::packet::packet(_Inout_ packet &&other) :
    m_code (std::move(other.m_code )),
    m_id   (std::move(other.m_id   )),
    m_flags(std::move(other.m_flags)),
    m_data (std::move(other.m_data ))
{
}


eap::method_tls::packet& eap::method_tls::packet::operator=(_In_ const packet &other)
{
    if (this != std::addressof(other)) {
        m_code  = other.m_code ;
        m_id    = other.m_id   ;
        m_flags = other.m_flags;
        m_data  = other.m_data ;
    }

    return *this;
}


eap::method_tls::packet& eap::method_tls::packet::operator=(_Inout_ packet &&other)
{
    if (this != std::addressof(other)) {
        m_code  = std::move(other.m_code );
        m_id    = std::move(other.m_id   );
        m_flags = std::move(other.m_flags);
        m_data  = std::move(other.m_data );
    }

    return *this;
}


void eap::method_tls::packet::clear()
{
    m_code  = (EapCode)0;
    m_id    = 0;
    m_flags = 0;
    m_data.clear();
}


//////////////////////////////////////////////////////////////////////
// eap::method_tls::random
//////////////////////////////////////////////////////////////////////

eap::method_tls::random::random() :
    time(0)
{
    memset(data, 0, sizeof(data));
}


eap::method_tls::random::random(_In_ const random &other) :
    time(other.time)
{
    memcpy(data, other.data, sizeof(data));
}


eap::method_tls::random::~random()
{
    SecureZeroMemory(data, sizeof(data));
}


eap::method_tls::random& eap::method_tls::random::operator=(_In_ const random &other)
{
    if (this != std::addressof(other)) {
        time = other.time;
        memcpy(data, other.data, sizeof(data));
    }

    return *this;
}


void eap::method_tls::random::clear()
{
    time = 0;
    memset(data, 0, sizeof(data));
}


//////////////////////////////////////////////////////////////////////
// eap::method_tls::master_secret
//////////////////////////////////////////////////////////////////////

eap::method_tls::master_secret::master_secret()
{
    memset(data, 0, sizeof(data));
}


eap::method_tls::master_secret::master_secret(_In_ HCRYPTPROV cp)
{
    data[0] = 3;
    data[1] = 1;

    if (!CryptGenRandom(cp, sizeof(data) - 2, data + 2))
        throw win_runtime_error(__FUNCTION__ " Error creating PMS randomness.");
}


eap::method_tls::master_secret::master_secret(_In_ const master_secret &other)
{
    memcpy(data, other.data, sizeof(data));
}


eap::method_tls::master_secret::~master_secret()
{
    SecureZeroMemory(data, sizeof(data));
}


eap::method_tls::master_secret& eap::method_tls::master_secret::operator=(_In_ const master_secret &other)
{
    if (this != std::addressof(other))
        memcpy(data, other.data, sizeof(data));

    return *this;
}


void eap::method_tls::master_secret::clear()
{
    memset(data, 0, sizeof(data));
}


//////////////////////////////////////////////////////////////////////
// eap::method_tls
//////////////////////////////////////////////////////////////////////

eap::method_tls::method_tls(_In_ module &module, _In_ config_method_tls &cfg, _In_ credentials_tls &cred) :
    m_cfg(cfg),
    m_cred(cred),
    m_phase(phase_unknown),
    m_send_client_cert(false),
    //m_server_hello_done(false),
    m_server_finished(false),
    m_cipher_spec(false),
    m_seq_num(0),
    method(module, cfg, cred)
{
}


eap::method_tls::method_tls(_In_ const method_tls &other) :
    m_cfg(other.m_cfg),
    m_cred(other.m_cred),
    m_phase(other.m_phase),
    m_packet_req(other.m_packet_req),
    m_packet_res(other.m_packet_res),
    m_random_client(other.m_random_client),
    m_random_server(other.m_random_server),
    m_session_id(other.m_session_id),
    m_server_cert_chain(other.m_server_cert_chain),
    m_hash_handshake_msgs_md5(other.m_hash_handshake_msgs_md5),
    m_hash_handshake_msgs_sha1(other.m_hash_handshake_msgs_sha1),
    m_master_secret(other.m_master_secret),
    m_send_client_cert(other.m_send_client_cert),
    //m_server_hello_done(other.m_server_hello_done),
    m_server_finished(other.m_server_finished),
    m_cipher_spec(other.m_cipher_spec),
    m_seq_num(other.m_seq_num),
    method(other)
{
}


eap::method_tls::method_tls(_Inout_ method_tls &&other) :
    m_cfg(other.m_cfg),
    m_cred(other.m_cred),
    m_phase(std::move(other.m_phase)),
    m_packet_req(std::move(other.m_packet_req)),
    m_packet_res(std::move(other.m_packet_res)),
    m_random_client(std::move(other.m_random_client)),
    m_random_server(std::move(other.m_random_server)),
    m_session_id(std::move(other.m_session_id)),
    m_server_cert_chain(std::move(other.m_server_cert_chain)),
    m_hash_handshake_msgs_md5(std::move(other.m_hash_handshake_msgs_md5)),
    m_hash_handshake_msgs_sha1(std::move(other.m_hash_handshake_msgs_sha1)),
    m_master_secret(std::move(other.m_master_secret)),
    m_send_client_cert(std::move(other.m_send_client_cert)),
    //m_server_hello_done(std::move(other.m_server_hello_done)),
    m_server_finished(std::move(other.m_server_finished)),
    m_cipher_spec(std::move(other.m_cipher_spec)),
    m_seq_num(std::move(other.m_seq_num)),
    method(std::move(other))
{
}


eap::method_tls& eap::method_tls::operator=(_In_ const method_tls &other)
{
    if (this != std::addressof(other)) {
        assert(std::addressof(m_cfg ) == std::addressof(other.m_cfg )); // Copy method with same configuration only!
        assert(std::addressof(m_cred) == std::addressof(other.m_cred)); // Copy method with same credentials only!
        (method&)*this             = other;
        m_phase                    = other.m_phase;
        m_packet_req               = other.m_packet_req;
        m_packet_res               = other.m_packet_res;
        m_random_client            = other.m_random_client;
        m_random_server            = other.m_random_server;
        m_session_id               = other.m_session_id;
        m_server_cert_chain        = other.m_server_cert_chain;
        m_hash_handshake_msgs_md5  = other.m_hash_handshake_msgs_md5;
        m_hash_handshake_msgs_sha1 = other.m_hash_handshake_msgs_sha1;
        m_master_secret            = other.m_master_secret;
        m_send_client_cert         = other.m_send_client_cert;
        //m_server_hello_done        = other.m_server_hello_done;
        m_server_finished          = other.m_server_finished;
        m_cipher_spec              = other.m_cipher_spec;
        m_seq_num                  = other.m_seq_num;
    }

    return *this;
}


eap::method_tls& eap::method_tls::operator=(_Inout_ method_tls &&other)
{
    if (this != std::addressof(other)) {
        assert(std::addressof(m_cfg ) == std::addressof(other.m_cfg )); // Move method with same configuration only!
        assert(std::addressof(m_cred) == std::addressof(other.m_cred)); // Move method with same credentials only!
        (method&)*this             = std::move(other);
        m_phase                    = std::move(other.m_phase);
        m_packet_req               = std::move(other.m_packet_req);
        m_packet_res               = std::move(other.m_packet_res);
        m_random_client            = std::move(other.m_random_client);
        m_random_server            = std::move(other.m_random_server);
        m_session_id               = std::move(other.m_session_id);
        m_server_cert_chain        = std::move(other.m_server_cert_chain);
        m_hash_handshake_msgs_md5  = std::move(other.m_hash_handshake_msgs_md5);
        m_hash_handshake_msgs_sha1 = std::move(other.m_hash_handshake_msgs_sha1);
        m_master_secret            = std::move(other.m_master_secret);
        m_send_client_cert         = std::move(other.m_send_client_cert);
        //m_server_hello_done        = std::move(other.m_server_hello_done);
        m_server_finished          = std::move(other.m_server_finished);
        m_cipher_spec              = std::move(other.m_cipher_spec);
        m_seq_num                  = std::move(other.m_seq_num);
    }

    return *this;
}


void eap::method_tls::begin_session(
    _In_        DWORD         dwFlags,
    _In_  const EapAttributes *pAttributeArray,
    _In_        HANDLE        hTokenImpersonateUser,
    _In_        DWORD         dwMaxSendPacketSize)
{
    eap::method::begin_session(dwFlags, pAttributeArray, hTokenImpersonateUser, dwMaxSendPacketSize);

    // Create cryptographics provider.
    if (!m_cp.create(NULL, MS_ENHANCED_PROV, PROV_RSA_FULL))
        throw win_runtime_error(__FUNCTION__ " Error creating cryptographics provider.");
}


void eap::method_tls::process_request_packet(
    _In_bytecount_(dwReceivedPacketSize) const EapPacket           *pReceivedPacket,
    _In_                                       DWORD               dwReceivedPacketSize,
    _Inout_                                    EapPeerMethodOutput *pEapOutput)
{
    assert(pReceivedPacket && dwReceivedPacketSize >= 4);
    assert(pEapOutput);

    // Is this a valid EAP-TLS packet?
    if (dwReceivedPacketSize < 6)
        throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Packet is too small. EAP-%s packets should be at least 6B.");
    //else if (pReceivedPacket->Data[0] != eap_type_tls) // Skip method check, to allow TTLS extension.
    //    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, string_printf(__FUNCTION__ " Packet is not EAP-TLS (expected: %u, received: %u).", eap_type_tls, pReceivedPacket->Data[0]));

    // Get packet data pointer and size for more readable code later on.
    const unsigned char *packet_data_ptr;
    size_t packet_data_size;
    if (pReceivedPacket->Data[1] & flags_req_length_incl) {
        // Length field is included.
        packet_data_ptr  = pReceivedPacket->Data + 6;
        packet_data_size = dwReceivedPacketSize - 10;
    } else {
        // Length field not included.
        packet_data_ptr  = pReceivedPacket->Data + 2;
        packet_data_size = dwReceivedPacketSize - 6;
    }

    // Do the TLS defragmentation.
    if (pReceivedPacket->Data[1] & flags_req_more_frag) {
        if (m_packet_req.m_data.empty()) {
            // Start a new packet.
            if (pReceivedPacket->Data[1] & flags_req_length_incl) {
                // Preallocate data according to the Length field.
                size_t size_tot  = ntohl(*(unsigned int*)(pReceivedPacket->Data + 2));
                m_packet_req.m_data.reserve(size_tot);
                m_module.log_event(&EAPMETHOD_PACKET_RECV_FRAG_FIRST, event_data((unsigned int)eap_type_tls), event_data((unsigned int)packet_data_size), event_data((unsigned int)size_tot), event_data::blank);
            } else {
                // The Length field was not included. Odd. Nevermind, no pre-allocation then.
                m_module.log_event(&EAPMETHOD_PACKET_RECV_FRAG_FIRST1, event_data((unsigned int)eap_type_tls), event_data((unsigned int)packet_data_size), event_data::blank);
            }
        } else {
            // Mid fragment received.
            m_module.log_event(&EAPMETHOD_PACKET_RECV_FRAG_MID, event_data((unsigned int)eap_type_tls), event_data((unsigned int)packet_data_size), event_data((unsigned int)m_packet_req.m_data.size()), event_data::blank);
        }
        m_packet_req.m_data.insert(m_packet_req.m_data.end(), packet_data_ptr, packet_data_ptr + packet_data_size);

        // Reply with ACK packet.
        m_packet_res.m_code  = EapCodeResponse;
        m_packet_res.m_id    = pReceivedPacket->Id;
        m_packet_res.m_flags = 0;
        m_packet_res.m_data.clear();
        pEapOutput->fAllowNotifications = FALSE;
        pEapOutput->action = EapPeerMethodResponseActionSend;
        return;
    } else if (!m_packet_req.m_data.empty()) {
        // Last fragment received. Append data.
        m_packet_req.m_data.insert(m_packet_req.m_data.end(), packet_data_ptr, packet_data_ptr + packet_data_size);
        m_module.log_event(&EAPMETHOD_PACKET_RECV_FRAG_LAST, event_data((unsigned int)eap_type_tls), event_data((unsigned int)packet_data_size), event_data((unsigned int)m_packet_req.m_data.size()), event_data::blank);
    } else {
        // This is a complete non-fragmented packet.
        m_packet_req.m_data.assign(packet_data_ptr, packet_data_ptr + packet_data_size);
        m_module.log_event(&EAPMETHOD_PACKET_RECV, event_data((unsigned int)eap_type_tls), event_data((unsigned int)packet_data_size), event_data::blank);
    }

    if (pReceivedPacket->Code == EapCodeRequest && pReceivedPacket->Data[1] & flags_req_start) {
        // This is the TLS start message: initialize method.
        m_phase = phase_client_hello;
        m_packet_res.clear();
        m_key_hmac.free();
        m_key_encrypt.free();
        m_key_decrypt.free();

        // Generate client randomness.
        _time32(&m_random_client.time);
        if (!CryptGenRandom(m_cp, sizeof(m_random_client.data), m_random_client.data))
            throw win_runtime_error(__FUNCTION__ " Error creating client randomness.");
        m_random_server.clear();
        m_server_cert_chain.clear();
        m_session_id.clear();

        // Create MD5 hash object.
        if (!m_hash_handshake_msgs_md5.create(m_cp, CALG_MD5))
            throw win_runtime_error(__FUNCTION__ " Error creating MD5 hashing object.");

        // Create SHA-1 hash object.
        if (!m_hash_handshake_msgs_sha1.create(m_cp, CALG_SHA1))
            throw win_runtime_error(__FUNCTION__ " Error creating SHA-1 hashing object.");

        m_master_secret.clear();
        m_send_client_cert = false;
        //m_server_hello_done = false;
        m_server_finished = false;
        m_cipher_spec = false;
        m_seq_num = 0;
    }

    m_packet_req.m_code  = (EapCode)pReceivedPacket->Code;
    m_packet_req.m_id    = pReceivedPacket->Id;
    m_packet_req.m_flags = pReceivedPacket->Data[1];

    if (m_packet_res.m_flags & flags_res_more_frag) {
        // We are sending a fragmented message.
        if (  m_packet_req.m_code == EapCodeRequest    &&
              m_packet_req.m_id   == m_packet_res.m_id &&
              m_packet_req.m_data.empty()              &&
            !(m_packet_req.m_flags & (flags_req_length_incl | flags_req_more_frag | flags_req_start)))
        {
            // This is the ACK of our fragmented message packet. Send the next fragment.
            m_packet_res.m_id++;
            pEapOutput->fAllowNotifications = FALSE;
            pEapOutput->action = EapPeerMethodResponseActionSend;
            return;
        } else
            throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, string_printf(__FUNCTION__ " ACK expected, received %u-%u-%x.", m_packet_req.m_code, m_packet_req.m_id, m_packet_req.m_flags));
    }

    switch (m_phase) {
        case phase_client_hello: {
            m_module.log_event(&EAPMETHOD_HANDSHAKE_START2, event_data((unsigned int)eap_type_tls), event_data::blank);

            // Build response packet.
            m_packet_res.m_code  = EapCodeResponse;
            m_packet_res.m_id    = m_packet_req.m_id;
            m_packet_res.m_flags = 0;
            sanitizing_blob hello(make_client_hello());
            sanitizing_blob handshake(make_handshake(hello, m_cipher_spec));
            m_packet_res.m_data.assign(handshake.begin(), handshake.end());
            CryptHashData(m_hash_handshake_msgs_md5 , hello.data(), (DWORD)hello.size(), 0);
            CryptHashData(m_hash_handshake_msgs_sha1, hello.data(), (DWORD)hello.size(), 0);

            pEapOutput->fAllowNotifications = FALSE;
            pEapOutput->action = EapPeerMethodResponseActionSend;
            m_phase = phase_server_hello;
            break;
        }

        case phase_server_hello: {
            process_packet(m_packet_req.m_data.data(), m_packet_req.m_data.size());

            if (m_server_cert_chain.empty())
                throw win_runtime_error(ERROR_ENCRYPTION_FAILED, __FUNCTION__ " Can not continue without server's certificate.");

            verify_server_trust();

            // Build response packet.
            m_packet_res.m_code  = EapCodeResponse;
            m_packet_res.m_id    = m_packet_req.m_id;
            m_packet_res.m_flags = 0;
            m_packet_res.m_data.clear();

            if (!m_server_finished || !m_cipher_spec) {
                // New session.

                if (m_send_client_cert) {
                    // Client certificate requested, and append to packet.
                    sanitizing_blob client_cert(make_client_cert());
                    sanitizing_blob handshake(make_handshake(client_cert, m_cipher_spec));
                    m_packet_res.m_data.insert(m_packet_res.m_data.end(), handshake.begin(), handshake.end());
                    CryptHashData(m_hash_handshake_msgs_md5 , client_cert.data(), (DWORD)client_cert.size(), 0);
                    CryptHashData(m_hash_handshake_msgs_sha1, client_cert.data(), (DWORD)client_cert.size(), 0);
                }

                // Generate pre-master secret and encrypt it. PMS will get sanitized in its destructor when going out-of-scope.
                master_secret pms(m_cp);
                sanitizing_blob pms_enc((const unsigned char*)&pms, (const unsigned char*)(&pms + 1));
                crypt_key key;
                if (!key.import_public(m_cp, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, &(m_server_cert_chain.front()->pCertInfo->SubjectPublicKeyInfo)))
                    throw win_runtime_error(__FUNCTION__ " Error importing server's public key.");
                if (!CryptEncrypt(key,  NULL, TRUE, 0, pms_enc))
                    throw win_runtime_error(__FUNCTION__ " Error encrypting PMS.");

                // Derive master secret.
                vector<unsigned char> lblseed, hash;
                const unsigned char s_label[] = "master secret";
                lblseed.assign(s_label, s_label + _countof(s_label) - 1);
                lblseed.insert(lblseed.end(), (const unsigned char*)&m_random_client, (const unsigned char*)(&m_random_client + 1));
                lblseed.insert(lblseed.end(), (const unsigned char*)&m_random_server, (const unsigned char*)(&m_random_server + 1));
                memcpy(&m_master_secret, prf(&pms, sizeof(pms), lblseed.data(), lblseed.size(), sizeof(m_master_secret)).data(), sizeof(m_master_secret));

                // Create client key exchange message, and append to packet.
                sanitizing_blob client_key_exchange(make_client_key_exchange(pms_enc));
                sanitizing_blob handshake(make_handshake(client_key_exchange, m_cipher_spec));
                m_packet_res.m_data.insert(m_packet_res.m_data.end(), handshake.begin(), handshake.end());
                CryptHashData(m_hash_handshake_msgs_md5 , client_key_exchange.data(), (DWORD)client_key_exchange.size(), 0);
                CryptHashData(m_hash_handshake_msgs_sha1, client_key_exchange.data(), (DWORD)client_key_exchange.size(), 0);
            }

            // Append change cipher spec to packet.
            sanitizing_blob ccs(make_change_chiper_spec());
            m_packet_res.m_data.insert(m_packet_res.m_data.end(), ccs.begin(), ccs.end());

            if (!m_server_finished || !m_cipher_spec) {
                // Setup encryption.
                derive_keys();
                m_cipher_spec = true;
            }

            sanitizing_blob finished(make_finished());
            sanitizing_blob handshake(make_handshake(finished, m_cipher_spec));
            m_packet_res.m_data.insert(m_packet_res.m_data.end(), handshake.begin(), handshake.end());
            CryptHashData(m_hash_handshake_msgs_md5 , finished.data(), (DWORD)finished.size(), 0);
            CryptHashData(m_hash_handshake_msgs_sha1, finished.data(), (DWORD)finished.size(), 0);

            pEapOutput->fAllowNotifications = FALSE;
            pEapOutput->action = EapPeerMethodResponseActionSend;
            m_phase = phase_resume_session;

            //break; // Leave commented until finished.
        }

        default:
            throw win_runtime_error(ERROR_NOT_SUPPORTED, __FUNCTION__ " Not supported.");
    }

    // Request packet was processed. Clear its data since we use the absence of data to detect first of fragmented message packages.
    m_packet_req.m_data.clear();
}


void eap::method_tls::get_response_packet(
    _Inout_bytecap_(*dwSendPacketSize) EapPacket *pSendPacket,
    _Inout_                            DWORD     *pdwSendPacketSize)
{
    assert(pdwSendPacketSize);
    assert(pSendPacket);

    unsigned int
        size_data   = (unsigned int)m_packet_res.m_data.size(),
        size_packet = size_data + 6;
    unsigned short size_packet_limit = (unsigned short)std::min<unsigned int>(*pdwSendPacketSize, USHRT_MAX);
    unsigned char *data_dst;

    if (!(m_packet_res.m_flags & flags_res_more_frag)) {
        // Not fragmented.
        if (size_packet <= size_packet_limit) {
            // No need to fragment the packet.
            m_packet_res.m_flags &= ~flags_res_length_incl; // No need to explicitly include the Length field either.
            data_dst = pSendPacket->Data + 2;
            m_module.log_event(&EAPMETHOD_PACKET_SEND, event_data((unsigned int)eap_type_tls), event_data((unsigned int)size_data), event_data::blank);
        } else {
            // But it should be fragmented.
            m_packet_res.m_flags |= flags_res_length_incl | flags_res_more_frag;
            *(unsigned int*)(pSendPacket->Data + 2) = (unsigned int)size_packet;
            data_dst = pSendPacket->Data + 6;
            size_data   = size_packet_limit - 10;
            size_packet = size_packet_limit;
            m_module.log_event(&EAPMETHOD_PACKET_SEND_FRAG_FIRST, event_data((unsigned int)eap_type_tls), event_data((unsigned int)size_data), event_data((unsigned int)(m_packet_res.m_data.size() - size_data)), event_data::blank);
        }
    } else {
        // Continuing the fragmented packet...
        if (size_packet > size_packet_limit) {
            // This is a mid fragment.
            m_packet_res.m_flags &= ~flags_res_length_incl;
            size_data   = size_packet_limit - 6;
            size_packet = size_packet_limit;
            m_module.log_event(&EAPMETHOD_PACKET_SEND_FRAG_MID, event_data((unsigned int)eap_type_tls), event_data((unsigned int)size_data), event_data((unsigned int)(m_packet_res.m_data.size() - size_data)), event_data::blank);
        } else {
            // This is the last fragment.
            m_packet_res.m_flags &= ~(flags_res_length_incl | flags_res_more_frag);
            m_module.log_event(&EAPMETHOD_PACKET_SEND_FRAG_LAST, event_data((unsigned int)eap_type_tls), event_data((unsigned int)size_data), event_data((unsigned int)(m_packet_res.m_data.size() - size_data)), event_data::blank);
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
}


void eap::method_tls::get_result(
    _In_    EapPeerMethodResultReason reason,
    _Inout_ EapPeerMethodResult       *ppResult)
{
    UNREFERENCED_PARAMETER(reason);
    UNREFERENCED_PARAMETER(ppResult);

    throw win_runtime_error(ERROR_NOT_SUPPORTED, __FUNCTION__ " Not supported.");
}


eap::sanitizing_blob eap::method_tls::make_client_hello() const
{
    size_t size_data;
    sanitizing_blob msg;
    msg.reserve(
        4                    + // SSL header
        (size_data =
        2                    + // SSL version
        sizeof(random)       + // Client random
        1                    + // Session ID size
        m_session_id.size()  + // Session ID
        2                    + // Length of cypher suite list
        2                    + // Cyper suite list
        1                    + // Length of compression suite
        1));                   // Compression suite

    // SSL header
    assert(size_data <= 0xffffff);
    unsigned int ssl_header = htonl(((unsigned int)handshake_type_client_hello << 24) | (unsigned int)size_data);
    msg.insert(msg.end(), (unsigned char*)&ssl_header, (unsigned char*)(&ssl_header + 1));

    // SSL version: TLS 1.0
    msg.push_back(3); // SSL major version
    msg.push_back(1); // SSL minor version

    // Client random
    msg.insert(msg.end(), (unsigned char*)&m_random_client, (unsigned char*)(&m_random_client + 1));

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


eap::sanitizing_blob eap::method_tls::make_client_cert() const
{
    // Select client certificate.
    PCCERT_CONTEXT cert;
    if (m_cfg.m_use_preshared) {
        // Using pre-shared credentials.
        const credentials_tls *preshared = dynamic_cast<credentials_tls*>(m_cfg.m_preshared.get());
        cert = preshared && preshared->m_cert ? preshared->m_cert : NULL;
    } else {
        // Using own credentials.
        cert = m_cred.m_cert ? m_cred.m_cert : NULL;
    }

    size_t size_data, size_list;
    sanitizing_blob msg;
    msg.reserve(
        4                                      + // SSL header
        (size_data =
        3                                      + // Certificate list size
        (size_list =
        (cert ? 3 + cert->cbCertEncoded : 0)))); // Certificate (optional)

    // SSL header
    assert(size_data <= 0xffffff);
    unsigned int ssl_header = htonl(((unsigned int)handshake_type_certificate << 24) | (unsigned int)size_data);
    msg.insert(msg.end(), (unsigned char*)&ssl_header, (unsigned char*)(&ssl_header + 1));

    // List size
    assert(size_list <= 0xffffff);
    msg.push_back((unsigned char)((size_list >> 16) & 0xff));
    msg.push_back((unsigned char)((size_list >>  8) & 0xff));
    msg.push_back((unsigned char)((size_list      ) & 0xff));

    if (cert) {
        // Cert size
        assert(cert->cbCertEncoded <= 0xffffff);
        msg.push_back((unsigned char)((cert->cbCertEncoded >> 16) & 0xff));
        msg.push_back((unsigned char)((cert->cbCertEncoded >>  8) & 0xff));
        msg.push_back((unsigned char)((cert->cbCertEncoded      ) & 0xff));

        msg.insert(msg.end(), cert->pbCertEncoded, cert->pbCertEncoded + cert->cbCertEncoded);
    }

    return msg;
}


eap::sanitizing_blob eap::method_tls::make_client_key_exchange(_In_ const sanitizing_blob &pms_enc) const
{
    size_t size_data, size_pms_enc = pms_enc.size();
    sanitizing_blob msg;
    msg.reserve(
        4             + // SSL header
        (size_data =
        2             + // Encrypted pre master secret size
        size_pms_enc)); // Encrypted pre master secret

    // SSL header
    assert(size_data <= 0xffffff);
    unsigned int ssl_header = htonl(((unsigned int)handshake_type_client_key_exchange << 24) | (unsigned int)size_data);
    msg.insert(msg.end(), (unsigned char*)&ssl_header, (unsigned char*)(&ssl_header + 1));

    // Encrypted pre master secret size
    assert(size_pms_enc <= 0xffff);
    msg.push_back((unsigned char)((size_pms_enc >> 8) & 0xff));
    msg.push_back((unsigned char)((size_pms_enc     ) & 0xff));

    // Encrypted pre master secret
    msg.insert(msg.end(), pms_enc.begin(), pms_enc.end());

    return msg;
}


eap::sanitizing_blob eap::method_tls::make_change_chiper_spec()
{
    static const unsigned char s_msg_css[] = {
        (unsigned char)message_type_change_cipher_spec, // SSL record type
        3,                                              // SSL major version
        1,                                              // SSL minor version
        0,                                              // Message size (high-order byte)
        1,                                              // Message size (low-order byte)
        1,                                              // Message: change_cipher_spec is always "1"
    };
    return eap::sanitizing_blob(s_msg_css, s_msg_css + _countof(s_msg_css));
}


eap::sanitizing_blob eap::method_tls::make_finished()
{
    sanitizing_blob msg;
    msg.reserve(
        4  + // SSL header
        12); // verify_data is 12B

    // SSL header
    unsigned int ssl_header = htonl(((unsigned int)handshake_type_finished << 24) | 12);
    msg.insert(msg.end(), (unsigned char*)&ssl_header, (unsigned char*)(&ssl_header + 1));

    // Create label + hash MD5 + hash SHA-1 seed.
    vector<unsigned char> lblseed, hash;
    const unsigned char s_label[] = "client finished";
    lblseed.assign(s_label, s_label + _countof(s_label) - 1);
    if (!CryptGetHashParam(m_hash_handshake_msgs_md5, HP_HASHVAL, hash, 0))
        throw win_runtime_error(__FUNCTION__ " Error finishing MD5 hash calculation.");
    lblseed.insert(lblseed.end(), hash.begin(), hash.end());
    if (!CryptGetHashParam(m_hash_handshake_msgs_sha1, HP_HASHVAL, hash, 0))
        throw win_runtime_error(__FUNCTION__ " Error finishing SHA-1 hash calculation.");
    lblseed.insert(lblseed.end(), hash.begin(), hash.end());
    vector<unsigned char> verify(prf(&m_master_secret, sizeof(m_master_secret), lblseed.data(), lblseed.size(), 12));
    msg.insert(msg.end(), verify.begin(), verify.end());

    return msg;
}


eap::sanitizing_blob eap::method_tls::make_handshake(_In_ const sanitizing_blob &msg)
{
    size_t size_msg = msg.size();
    eap::sanitizing_blob msg_h;
    msg_h.reserve(
        1        + // SSL record type
        2        + // SSL version
        2        + // Message size
        size_msg); // Message

    // SSL record type
    msg_h.push_back((unsigned char)message_type_handshake);

    // SSL version: TLS 1.0
    msg_h.push_back(3); // SSL major version
    msg_h.push_back(1); // SSL minor version

    // Message
    assert(size_msg <= 0xffff);
    unsigned short size_msg_n = htons((unsigned short)size_msg);
    msg_h.insert(msg_h.end(), (unsigned char*)&size_msg_n, (unsigned char*)(&size_msg_n + 1));
    msg_h.insert(msg_h.end(), msg.begin(), msg.end());

    return msg_h;
}


void eap::method_tls::derive_keys()
{
    vector<unsigned char> lblseed;
    const unsigned char s_label[] = "key expansion";
    lblseed.assign(s_label, s_label + _countof(s_label) - 1);
    lblseed.insert(lblseed.end(), (const unsigned char*)&m_random_server, (const unsigned char*)(&m_random_server + 1));
    lblseed.insert(lblseed.end(), (const unsigned char*)&m_random_client, (const unsigned char*)(&m_random_client + 1));

    vector<unsigned char> key_block(prf(&m_master_secret, sizeof(m_master_secret), lblseed.data(), lblseed.size(),
        2*20 +  // client_write_MAC_secret & server_write_MAC_secret (SHA1)
        2*24 +  // client_write_key        & server_write_key        (3DES)
        2* 8)); // client_write_IV         & server_write_IV

    const unsigned char *data = key_block.data();
    static const BLOBHEADER s_key_struct = {
        OPAQUEKEYBLOB,
        CUR_BLOB_VERSION,
        0,
        CALG_RC4
    };
    vector<unsigned char> key;
    key.assign((const unsigned char*)&s_key_struct, (const unsigned char*)(&s_key_struct + 1));
    key.insert(key.end(), data, data + 20);
    if (!m_key_hmac.import(m_cp, key.data(), (DWORD)key.size(), NULL, 0))
        throw win_runtime_error(__FUNCTION__ " Error importing client_write_MAC_secret key.");


    // TODO: Derive rest of the keys.
}


void eap::method_tls::process_packet(_In_bytecount_(size_pck) const void *_pck, _In_ size_t size_pck)
{
    for (const unsigned char *pck = (const unsigned char*)_pck, *pck_end = pck + size_pck; pck < pck_end; ) {
        if (pck + 5 > pck_end)
            throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Incomplete message header.");
        const message *hdr = (const message*)pck;
        const unsigned char
            *msg     = hdr->data,
            *msg_end = msg + ntohs(*(unsigned short*)hdr->length);
        if (msg_end > pck_end)
            throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Incomplete message data.");

        if (hdr->version.major == 3 && hdr->version.minor == 1) {
            // Process TLS 1.0 message.
            switch (hdr->type) {
            case message_type_handshake:
                if (m_cipher_spec) {
                    sanitizing_blob msg_dec(msg, msg_end);
                    decrypt_message(msg_dec);
                    process_handshake(msg_dec.data(), msg_dec.size());
                } else
                    process_handshake(msg, msg_end - msg);
                break;

            case message_type_change_cipher_spec:
                if (msg + 1 > msg_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Incomplete change cipher spec message.");
                else if (msg[0] != 1)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, string_printf(__FUNCTION__ " Invalid change cipher spec message (expected 1, received %u).", msg[0]));

                if (!m_cipher_spec) {
                    // Resuming previous session.
                    derive_keys();
                    m_cipher_spec = true;
                }
                break;

            case message_type_alert:
                if (m_cipher_spec) {
                    sanitizing_blob msg_dec(msg, msg_end);
                    decrypt_message(msg_dec);
                    process_alert(msg_dec.data(), msg_dec.size());
                } else
                    process_alert(msg, msg_end - msg);
                break;
            }
        }

        pck = msg_end;
    }
}


void eap::method_tls::process_handshake(_In_bytecount_(msg_size) const void *_msg, _In_ size_t msg_size)
{
    for (const unsigned char *msg = (const unsigned char*)_msg, *msg_end = msg + msg_size; msg < msg_end; ) {
        // Parse record header.
        if (msg + sizeof(unsigned int) > msg_end)
            throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Incomplete record header.");
        unsigned int hdr = ntohl(*(unsigned int*)msg);
        const unsigned char
            *rec     = msg + sizeof(unsigned int),
            *rec_end = rec + (hdr & 0xffffff);
        if (rec_end > msg_end)
            throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Incomplete record data.");

        // Process record.
        switch (hdr >> 24) {
            case handshake_type_server_hello:
                // TLS version
                if (rec + 2 > rec_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Server SSL/TLS version missing or incomplete.");
                else if (rec[0] != 3 || rec[1] != 1)
                    throw win_runtime_error(ERROR_NOT_SUPPORTED, __FUNCTION__ " Unsupported SSL/TLS version.");
                rec += 2;

                // Server random
                if (rec + sizeof(m_random_server) > rec_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Server random missing or incomplete.");
                memcpy(&m_random_server, rec, sizeof(m_random_server));
                rec += sizeof(m_random_server);

                // Session ID
                if (rec + 1 > rec_end || rec + 1 + rec[0] > rec_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Session ID missing or incomplete.");
                assert(rec[0] <= 32); // According to RFC 5246 session IDs should not be longer than 32B.
                m_session_id.assign(rec + 1, rec + 1 + rec[0]);
                rec += rec[0] + 1;

                // Cipher
                if (rec + 2 > rec_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Cipher or incomplete.");
                if (rec[0] != 0x00 || rec[1] != 0x0a)
                    throw win_runtime_error(ERROR_NOT_SUPPORTED, string_printf(__FUNCTION__ " Other than requested cipher selected (expected 0x000a, received 0x%02x%02x).", rec[0], rec[1]));

                break;

            case handshake_type_certificate: {
                // Certificate list size
                if (rec + 3 > rec_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Certificate list size missing or incomplete.");
                const unsigned char
                    *list     = rec  + 3,
                    *list_end = list + ((rec[0] << 16) | (rec[1] << 8) | rec[2]);
                if (list_end > rec_end)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Certificate list missing or incomplete.");

                m_server_cert_chain.clear();
                while (list < list_end) {
                    // Certificate size
                    if (list + 3 > list_end)
                        throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Certificate size missing or incomplete.");
                    const unsigned char
                        *cert     = list + 3,
                        *cert_end = cert + ((list[0] << 16) | (list[1] << 8) | list[2]);
                    if (cert_end > list_end)
                        throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Certificate rec missing or incomplete.");

                    // Certificate
                    cert_context c;
                    if (!c.create(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, cert, (DWORD)(cert_end - cert)))
                        throw win_runtime_error(__FUNCTION__ " Error reading certificate.");
                    m_server_cert_chain.push_back(std::move(c));

                    list = cert_end;
                }

                break;
            }

            case handshake_type_certificate_request:
                m_send_client_cert = true;
                break;

            //case handshake_type_server_hello_done:
            //    m_server_hello_done = true;

            case handshake_type_finished: {
                // According to https://tools.ietf.org/html/rfc5246#section-7.4.9 all verify_data is 12B.
                if (rec_end - rec != 12)
                    throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, string_printf(__FUNCTION__ " Finished record size incorrect (expected 12B, received %uB).", rec_end - rec));

                // Create label + hash MD5 + hash SHA-1 seed.
                vector<unsigned char> lblseed, hash;
                const unsigned char s_label[] = "server finished";
                lblseed.assign(s_label, s_label + _countof(s_label) - 1);
                if (!CryptGetHashParam(m_hash_handshake_msgs_md5, HP_HASHVAL, hash, 0))
                    throw win_runtime_error(__FUNCTION__ " Error finishing MD5 hash calculation.");
                lblseed.insert(lblseed.end(), hash.begin(), hash.end());
                if (!CryptGetHashParam(m_hash_handshake_msgs_sha1, HP_HASHVAL, hash, 0))
                    throw win_runtime_error(__FUNCTION__ " Error finishing SHA-1 hash calculation.");
                lblseed.insert(lblseed.end(), hash.begin(), hash.end());

                if (memcmp(prf(&m_master_secret, sizeof(m_master_secret), lblseed.data(), lblseed.size(), 12).data(), rec, 12))
                    throw win_runtime_error(ERROR_ENCRYPTION_FAILED, __FUNCTION__ " Integrity check failed.");

                m_server_finished = true;
                break;
            }
        }

        msg = rec_end;
    }
}


void eap::method_tls::process_alert(_In_bytecount_(msg_size) const void *msg, _In_ size_t msg_size)
{
    UNREFERENCED_PARAMETER(msg);

    if (msg_size < 2)
        throw win_runtime_error(EAP_E_EAPHOST_METHOD_INVALID_PACKET, __FUNCTION__ " Incomplete alert.");

    //if (msg[0] == alert_level_fatal) {
    //    // Clear session ID to avoid reconnection attempts.
    //    m_session_id.clear();
    //}
}


void eap::method_tls::verify_server_trust()
{
    assert(!m_server_cert_chain.empty());
    cert_context &cert = m_server_cert_chain.front();

    if (!m_cfg.m_server_names.empty()) {
        // Check server name.

        string subj;
        if (!CertGetNameStringA(cert, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, subj))
            throw win_runtime_error(__FUNCTION__ " Error retrieving server's certificate subject name.");

        for (list<string>::const_iterator s = m_cfg.m_server_names.cbegin(), s_end = m_cfg.m_server_names.cend();; ++s) {
            if (s != s_end) {
                const char
                    *a = s->c_str(),
                    *b = subj.c_str();
                size_t
                    len_a = s->length(),
                    len_b = subj.length();

                if (_stricmp(a, b) == 0 || // Direct match
                    a[0] == '*' && len_b + 1 >= len_a && _stricmp(a + 1, b + len_b - (len_a - 1)) == 0) // "*..." wildchar match
                {
                    m_module.log_event(&EAPMETHOD_SERVER_NAME_TRUSTED, event_data(subj), event_data::blank);
                    break;
                }
            } else
                throw win_runtime_error(ERROR_INVALID_DOMAINNAME, string_printf(__FUNCTION__ " Server name %s is not on the list of trusted server names.", subj.c_str()).c_str());
        }
    }

    // Create temporary certificate store of our trusted root CAs.
    cert_store store;
    if (!store.create(CERT_STORE_PROV_MEMORY, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, NULL, 0, NULL))
        throw win_runtime_error(ERROR_INVALID_DOMAINNAME, __FUNCTION__ " Error creating temporary certificate store.");
    for (list<cert_context>::const_iterator c = m_cfg.m_trusted_root_ca.cbegin(), c_end = m_cfg.m_trusted_root_ca.cend(); c != c_end; ++c)
        CertAddCertificateContextToStore(store, *c, CERT_STORE_ADD_REPLACE_EXISTING, NULL);

    // Add all certificates from the server's certificate chain, except the first one.
    for (list<cert_context>::const_iterator c = m_server_cert_chain.cbegin(), c_end = m_server_cert_chain.cend(); ++c != c_end;)
        CertAddCertificateContextToStore(store, *c, CERT_STORE_ADD_REPLACE_EXISTING, NULL);

    // Prepare the certificate chain validation, and check.
    CERT_CHAIN_PARA chain_params = {
        sizeof(chain_params),      // cbSize
        {
            USAGE_MATCH_TYPE_AND,  // RequestedUsage.dwType
            {},                    // RequestedUsage.Usage
        },
#ifdef CERT_CHAIN_PARA_HAS_EXTRA_FIELDS
        {},                        // RequestedIssuancePolicy
        1,                         // dwUrlRetrievalTimeout (1ms to speed up checking)
#else
#define _S2(x) #x
#define _S(x) _S2(x)
#pragma message(__FILE__ "(" _S(__LINE__) "): warning X0000: Please define CERT_CHAIN_PARA_HAS_EXTRA_FIELDS constant when compiling this project.")
#endif
    };
    cert_chain_context context;
    if (!context.create(NULL, cert, NULL, store, &chain_params, 0))
        throw win_runtime_error(ERROR_INVALID_DOMAINNAME, __FUNCTION__ " Error creating certificate chain context.");

    // Check chain validation error flags. Ignore CERT_TRUST_IS_UNTRUSTED_ROOT flag when we check root CA explicitly.
    if (context->TrustStatus.dwErrorStatus != CERT_TRUST_NO_ERROR &&
        (m_cfg.m_trusted_root_ca.empty() || (context->TrustStatus.dwErrorStatus & ~CERT_TRUST_IS_UNTRUSTED_ROOT) != CERT_TRUST_NO_ERROR))
        throw win_runtime_error(context->TrustStatus.dwErrorStatus, "Error validating certificate chain.");

    if (!m_cfg.m_trusted_root_ca.empty()) {
        // Verify Root CA against our trusted root CA list
        if (context->cChain != 1)
            throw win_runtime_error(ERROR_NOT_SUPPORTED, __FUNCTION__ " Multiple chain verification not supported.");
        if (context->rgpChain[0]->cElement == 0)
            throw win_runtime_error(ERROR_NOT_SUPPORTED, __FUNCTION__ " Can not verify empty certificate chain.");

        PCCERT_CONTEXT cert_root = context->rgpChain[0]->rgpElement[context->rgpChain[0]->cElement-1]->pCertContext;
        for (list<cert_context>::const_iterator c = m_cfg.m_trusted_root_ca.cbegin(), c_end = m_cfg.m_trusted_root_ca.cend();; ++c) {
            if (c != c_end) {
                if (cert_root->cbCertEncoded == (*c)->cbCertEncoded &&
                    memcmp(cert_root->pbCertEncoded, (*c)->pbCertEncoded, cert_root->cbCertEncoded) == 0)
                {
                    // Trusted root CA found.
                    break;
                }
            } else {
                // Not found.
                throw win_runtime_error(ERROR_FILE_NOT_FOUND, __FUNCTION__ " Server's certificate not issued by one of configured trusted root CAs.");
            }
        }
    }

    m_module.log_event(&EAPMETHOD_SERVER_CERT_TRUSTED, event_data::blank);
}


void eap::method_tls::encrypt_message(_Inout_ sanitizing_blob &msg)
{
    // Create a HMAC hash.
    static const HMAC_INFO s_hmac_info = { CALG_SHA1 };
    crypt_hash hash_hmac(create_hmac_hash(m_key_hmac, s_hmac_info));

    // Hash sequence number and message.
    unsigned __int64 seq_num = htonll(m_seq_num);
    if (!CryptHashData(hash_hmac, (const BYTE*)&seq_num, sizeof(seq_num), 0) ||
        !CryptHashData(hash_hmac, msg.data(), (DWORD)msg.size(), 0))
        throw win_runtime_error(__FUNCTION__ " Error hashing data.");

    // Calculate hash.
    vector<unsigned char> hmac;
    if (!CryptGetHashParam(hash_hmac, HP_HASHVAL, hmac, 0))
        throw win_runtime_error(__FUNCTION__ " Error finishing hash calculation.");

    // Remove SSL/TLS header (record type, version, message size).
    msg.erase(msg.begin(), msg.begin() + 5);

    size_t size =
        msg.size() + // TLS message
        20         + // HMAC hash (SHA-1)
        1;           // Padding length
    unsigned char padding = (8 - size) % 8;
    size += padding;
    msg.reserve(size);

    // Append HMAC hash.
#ifdef _HOST_LOW_ENDIAN
    std::reverse(hmac.begin(), hmac.end());
#endif
    msg.insert(msg.end(), hmac.begin(), hmac.end());

    // Append padding.
    msg.insert(msg.end(), padding + 1, padding);

    // Encrypt.
    assert(size < 0xffffffff);
    DWORD size2 = (DWORD)size;
    if (!CryptEncrypt(m_key_encrypt, NULL, FALSE, 0, msg.data(), &size2, (DWORD)size))
        throw win_runtime_error(__FUNCTION__ " Error encrypting message.");

    // Increment sequence number.
    m_seq_num++;
}


void eap::method_tls::decrypt_message(_Inout_ sanitizing_blob &msg)
{
    // Decrypt.
    DWORD size = (DWORD)msg.size();
    if (!CryptDecrypt(m_key_decrypt, NULL, FALSE, 0, msg.data(), &size))
        throw win_runtime_error(__FUNCTION__ " Error decrypting message.");

    // Remove padding.
    msg.resize(size - msg.back() - 1);
}


vector<unsigned char> eap::method_tls::prf(
    _In_bytecount_(size_secret ) const void   *secret,
    _In_                               size_t size_secret,
    _In_bytecount_(size_lblseed) const void   *lblseed,
    _In_                               size_t size_lblseed,
    _In_                               size_t size)
{
    size_t
        L_S1 = (size_secret + 1) / 2,
        L_S2 = L_S1;

    const void
        *S1 = secret,
        *S2 = (const unsigned char*)secret + (size_secret - L_S2);

    vector<unsigned char>
        p_md5 (p_hash(CALG_MD5 , S1, L_S1, lblseed, size_lblseed, size)),
        p_sha1(p_hash(CALG_SHA1, S2, L_S2, lblseed, size_lblseed, size)),
        p(size);

    for (size_t i = 0; i < size; i++)
        p[i] = p_md5[i] ^ p_sha1[i];

    return p;
}


vector<unsigned char> eap::method_tls::p_hash(
    _In_                              ALG_ID alg,
    _In_bytecount_(size_secret) const void   *secret,
    _In_                              size_t size_secret,
    _In_bytecount_(size_seed)   const void   *seed,
    _In_                              size_t size_seed,
    _In_                              size_t size)
{
    // HMAC symmetric key creation.
    crypt_key key_hmac(create_hmac_key(secret, size_secret));
    vector<unsigned char> block;
    const HMAC_INFO hmac_info = { alg };

    vector<unsigned char> data;
    data.reserve(size);

    // https://tools.ietf.org/html/rfc5246#section-5:
    //
    // P_hash(secret, seed) = HMAC_hash(secret, A(1) + seed) +
    //                        HMAC_hash(secret, A(2) + seed) +
    //                        HMAC_hash(secret, A(3) + seed) + ...
    // 
    // where + indicates concatenation.
    // 
    // A() is defined as:
    // 
    //    A(0) = seed
    //    A(i) = HMAC_hash(secret, A(i-1))

    vector<unsigned char> A((unsigned char*)seed, (unsigned char*)seed + size_seed);
    while (data.size() < size) {
        // Hash A.
        crypt_hash hash_hmac1(create_hmac_hash(key_hmac, hmac_info));
        if (!CryptHashData(hash_hmac1, A.data(), (DWORD)A.size(), 0))
            throw win_runtime_error(__FUNCTION__ " Error hashing A.");
        if (!CryptGetHashParam(hash_hmac1, HP_HASHVAL, A, 0))
            throw win_runtime_error(__FUNCTION__ " Error finishing hash A calculation.");

        // Hash A and seed.
        crypt_hash hash_hmac2(create_hmac_hash(key_hmac, hmac_info));
        if (!CryptHashData(hash_hmac2, A.data(), (DWORD)A.size(), 0) ||
            !CryptHashData(hash_hmac2, (const BYTE*)seed, (DWORD)size_seed, 0))
            throw win_runtime_error(__FUNCTION__ " Error hashing seed.");
        if (!CryptGetHashParam(hash_hmac2, HP_HASHVAL, block, 0))
            throw win_runtime_error(__FUNCTION__ " Error finishing hash A+seed calculation.");

        // Append to output data.
        data.insert(data.end(), block.begin(), block.end());
    }

    return data;
}
