/*
 * Copyright (C) 2000,2001,2002,2003 Nikos Mavroyanopoulos
 *
 * This file is part of GNUTLS.
 *
 *  The GNUTLS library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public   
 *  License as published by the Free Software Foundation; either 
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */

/* Functions that are record layer specific, are included in this file.
 */

#include "gnutls_int.h"
#include "gnutls_errors.h"
#include "debug.h"
#include "gnutls_compress.h"
#include "gnutls_cipher.h"
#include "gnutls_buffers.h"
#include "gnutls_handshake.h"
#include "gnutls_hash_int.h"
#include "gnutls_cipher_int.h"
#include "gnutls_priority.h"
#include "gnutls_algorithms.h"
#include "gnutls_db.h"
#include "gnutls_auth_int.h"
#include "gnutls_num.h"
#include "gnutls_record.h"
#include "gnutls_datum.h"
#include "ext_max_record.h"
#include <gnutls_state.h>
#include <gnutls_alert.h>
#include <gnutls_dh.h>

/**
  * gnutls_protocol_get_version - Returns the version of the currently used protocol
  * @session: is a &gnutls_session structure.
  *
  * Returns the version of the currently used protocol. 
  *
  **/
gnutls_protocol_version gnutls_protocol_get_version(gnutls_session session) {
	return session->security_parameters.version;
}

void _gnutls_set_current_version(gnutls_session session, gnutls_protocol_version version) {
	session->security_parameters.version = version;
}

/**
  * gnutls_transport_set_lowat - Used to set the lowat value in order for select to check for pending data.
  * @session: is a &gnutls_session structure.
  * @num: is the low water value.
  *
  * Used to set the lowat value in order for select to check
  * if there are pending data to socket buffer. Used only   
  * if you have changed the default low water value (default is 1).
  * Normally you will not need that function. 
  * This function is only useful if using berkeley style sockets.
  * Otherwise it must be called and set lowat to zero.
  *
  **/
void gnutls_transport_set_lowat(gnutls_session session, int num) {
	session->internals.lowat = num;
}

/**
  * gnutls_transport_set_ptr - Used to set first argument of the transport functions
  * @session: is a &gnutls_session structure.
  * @ptr: is the value.
  *
  * Used to set the first argument of the transport function (like PUSH and
  * PULL). In berkeley style sockets this function will set the connection
  * handle.
  *
  **/
void gnutls_transport_set_ptr(gnutls_session session, gnutls_transport_ptr ptr) 
{
	session->internals.transport_recv_ptr = ptr;
	session->internals.transport_send_ptr = ptr;
}


/**
  * gnutls_transport_set_ptr2 - Used to set first argument of the transport functions
  * @session: is a &gnutls_session structure.
  * @recv_ptr: is the value for the pull function
  * @send_ptr: is the value for the push function
  *
  * Used to set the first argument of the transport function (like PUSH and
  * PULL). In berkeley style sockets this function will set the connection
  * handle. With this function you can use two different pointers for
  * receiving and sending.
  *
  **/
void gnutls_transport_set_ptr2(gnutls_session session, gnutls_transport_ptr recv_ptr,
	gnutls_transport_ptr send_ptr) 
{
	session->internals.transport_send_ptr = send_ptr;
	session->internals.transport_recv_ptr = recv_ptr;
}

/**
  * gnutls_transport_get_ptr - Used to return the first argument of the transport functions
  * @session: is a &gnutls_session structure.
  *
  * Used to get the first argument of the transport function (like PUSH and
  * PULL). This must have been set using gnutls_transport_set_ptr().
  *
  **/
gnutls_transport_ptr gnutls_transport_get_ptr(gnutls_session session) 
{
	return session->internals.transport_recv_ptr;
}

/**
  * gnutls_transport_get_ptr2 - Used to return the first argument of the transport functions
  * @session: is a &gnutls_session structure.
  * @recv_ptr: will hold the value for the pull function
  * @send_ptr: will hold the value for the push function
  *
  * Used to get the arguments of the transport functions (like PUSH and
  * PULL). These should have been set using gnutls_transport_set_ptr2().
  *
  **/
void gnutls_transport_get_ptr2(gnutls_session session,
	gnutls_transport_ptr *recv_ptr,
	gnutls_transport_ptr *send_ptr) 
{
	
	*recv_ptr = session->internals.transport_recv_ptr;
	*send_ptr = session->internals.transport_send_ptr;
}

/**
  * gnutls_bye - This function terminates the current TLS/SSL connection.
  * @session: is a &gnutls_session structure.
  * @how: is an integer
  *
  * Terminates the current TLS/SSL connection. The connection should
  * have been initiated using gnutls_handshake().
  * 'how' should be one of GNUTLS_SHUT_RDWR, GNUTLS_SHUT_WR.
  *
  * In case of GNUTLS_SHUT_RDWR then the TLS connection gets terminated and
  * further receives and sends will be disallowed. If the return
  * value is zero you may continue using the connection.
  * GNUTLS_SHUT_RDWR actually sends an alert containing a close request
  * and waits for the peer to reply with the same message.
  *
  * In case of GNUTLS_SHUT_WR then the TLS connection gets terminated and
  * further sends will be disallowed. In order to reuse the connection
  * you should wait for an EOF from the peer.
  * GNUTLS_SHUT_WR sends an alert containing a close request.
  *
  * This function may also return GNUTLS_E_AGAIN or GNUTLS_E_INTERRUPTED; cf.
  * gnutls_record_get_direction().
  *
  **/
int gnutls_bye( gnutls_session session, gnutls_close_request how)
{
	int ret = 0, ret2 = 0;

	switch (STATE) {
		case STATE0:
		case STATE60:
			if (STATE==STATE60) {
				ret = _gnutls_io_write_flush( session);
			} else {
				ret = gnutls_alert_send( session, GNUTLS_AL_WARNING, GNUTLS_A_CLOSE_NOTIFY);
				STATE = STATE60;
			}

			if (ret < 0)
				return ret;
		case STATE61:
			if ( how == GNUTLS_SHUT_RDWR && ret >= 0) {
				ret2 = _gnutls_recv_int( session, GNUTLS_ALERT, -1, NULL, 0); 
				if (ret2 >= 0) session->internals.may_read = 1;
			}
			STATE = STATE61;

			if (ret2 < 0)
				return ret2;
			break;
		default:
			gnutls_assert();
			return GNUTLS_E_INTERNAL_ERROR;
	}

	STATE = STATE0;
	
	session->internals.may_write = 1;
	return 0;
}

inline
static void _gnutls_session_invalidate( gnutls_session session) {
	session->internals.valid_connection = VALID_FALSE;
}


inline
static void _gnutls_session_unresumable( gnutls_session session) {
	session->internals.resumable = RESUME_FALSE;
}

/* returns 0 if session is valid
 */
inline
static int _gnutls_session_is_valid( gnutls_session session) {
	if (session->internals.valid_connection==VALID_FALSE)
		return GNUTLS_E_INVALID_SESSION;
	
	return 0;
}

/* Copies the record version into the headers. The 
 * version must have 2 bytes at least.
 */
inline static void copy_record_version( gnutls_session session, HandshakeType htype, 
	opaque version[2])
{
gnutls_protocol_version lver;

	if (htype != GNUTLS_CLIENT_HELLO || session->internals.default_record_version[0] == 0) {
		lver = gnutls_protocol_get_version( session);

		version[0] = _gnutls_version_get_major( lver);
		version[1] = _gnutls_version_get_minor( lver);
	} else {
		version[0] = session->internals.default_record_version[0];
		version[1] = session->internals.default_record_version[1];
	}
}

inline static
ssize_t _gnutls_create_empty_record( gnutls_session session, ContentType type,
	opaque* erecord, unsigned int erecord_size)
{
	int cipher_size;
	int retval;
	int data2send;
	uint8 headers[5];

	if (type!=GNUTLS_APPLICATION_DATA ||
		_gnutls_cipher_is_block( gnutls_cipher_get(session))!=CIPHER_BLOCK) 
		/* alert messages and stream ciphers
		 * do not need this protection 
		 */
		return 0;

	headers[0] = type;
	
	copy_record_version( session, (HandshakeType)(-1), &headers[1]);

	data2send = 0;

	cipher_size = _gnutls_encrypt( session, headers, RECORD_HEADER_SIZE, NULL, 0, erecord, erecord_size, type, 0);
	if (cipher_size <= 0) {
		gnutls_assert();
		if (cipher_size==0) cipher_size = GNUTLS_E_ENCRYPTION_FAILED;
		return cipher_size; /* error */
	}

	retval = cipher_size;

	/* increase sequence number
	 */
	if (_gnutls_uint64pp( &session->connection_state.write_sequence_number) != 0) {
		_gnutls_session_invalidate( session);
		gnutls_assert();
		return GNUTLS_E_RECORD_LIMIT_REACHED;
	}

	return retval;
}

/* This function behaves exactly like write(). The only difference is
 * that it accepts, the gnutls_session and the ContentType of data to
 * send (if called by the user the Content is specific)
 * It is intended to transfer data, under the current session.    
 *
 * Oct 30 2001: Removed capability to send data more than MAX_RECORD_SIZE.
 * This makes the function much easier to read, and more error resistant
 * (there were cases were the old function could mess everything up).
 * --nmav
 *
 * This function may accept a NULL pointer for data, and 0 for size, if
 * and only if the previous send was interrupted for some reason.
 *
 */
ssize_t _gnutls_send_int( gnutls_session session, ContentType type, HandshakeType htype, const void *_data, size_t sizeofdata)
{
	uint8 *cipher;
	int cipher_size;
	int retval, ret;
	int data2send_size;
	uint8 headers[5];
	const uint8 *data=_data;
	int erecord_size = 0;
	opaque* erecord = NULL;

	/* Do not allow null pointer if the send buffer is empty.
	 * If the previous send was interrupted then a null pointer is
	 * ok, and means to resume.
	 */
	if (session->internals.record_send_buffer.length == 0 &&
	  (sizeofdata == 0 || _data==NULL)) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	if (type!=GNUTLS_ALERT) /* alert messages are sent anyway */
		if ( _gnutls_session_is_valid( session) || session->internals.may_write != 0) {
			gnutls_assert();
			return GNUTLS_E_INVALID_SESSION;
		}



	headers[0] = type;
	
	/* Use the default record version, if it is
	 * set.
	 */
	copy_record_version( session, htype, &headers[1]);


	_gnutls_record_log( "REC[%x]: Sending Packet[%d] %s(%d) with length: %d\n",
		session, (int) _gnutls_uint64touint32(&session->connection_state.write_sequence_number), _gnutls_packet2str(type), type, sizeofdata);

	if ( sizeofdata > MAX_RECORD_SEND_SIZE)
		data2send_size = MAX_RECORD_SEND_SIZE;
	else 
		data2send_size = sizeofdata;

	/* Only encrypt if we don't have data to send 
	 * from the previous run. - probably interrupted.
	 */
	if (session->internals.record_send_buffer.length > 0) {
		ret = _gnutls_io_write_flush( session);
		if (ret > 0) cipher_size = ret;
		else cipher_size = 0;
		
		cipher = NULL;

		retval = session->internals.record_send_buffer_user_size;
	} else {

		/* Prepend our packet with an empty record. This is to
		 * avoid the recent CBC attacks.
		 */
		/* if this protection has been disabled
		 */
		if (session->internals.cbc_protection_hack!=0) {
			erecord_size = MAX_RECORD_OVERHEAD;
			erecord = gnutls_alloca( erecord_size);
			if (erecord==NULL) {
				gnutls_assert();
				return GNUTLS_E_MEMORY_ERROR;
			}

			erecord_size = 
				_gnutls_create_empty_record( session, type, erecord, erecord_size);
			if (erecord_size < 0) {
				gnutls_assert();
				return erecord_size;
			}
		}

		/* now proceed to packet encryption
		 */
		cipher_size = data2send_size + MAX_RECORD_OVERHEAD;
		cipher = gnutls_malloc( cipher_size);
		if (cipher==NULL) {
			gnutls_assert();
			return GNUTLS_E_MEMORY_ERROR;
		}

		cipher_size = _gnutls_encrypt( session, headers, RECORD_HEADER_SIZE, data, data2send_size, cipher, 
			cipher_size, type, 1);
		if (cipher_size <= 0) {
			gnutls_assert();
			if (cipher_size==0) cipher_size = GNUTLS_E_ENCRYPTION_FAILED;
			gnutls_afree( erecord);
			gnutls_free( cipher);
			return cipher_size; /* error */
		}

		retval = data2send_size;
		session->internals.record_send_buffer_user_size =	data2send_size;

		/* increase sequence number
		 */
		if (_gnutls_uint64pp( &session->connection_state.write_sequence_number) != 0) {
			_gnutls_session_invalidate( session);
			gnutls_assert();
			gnutls_afree( erecord);
			gnutls_free( cipher);
			return GNUTLS_E_RECORD_LIMIT_REACHED;
		}

		ret = _gnutls_io_write_buffered2( session, erecord, erecord_size, cipher, cipher_size);
		gnutls_afree( erecord);
		gnutls_free( cipher);
	}

	if ( ret != cipher_size + erecord_size) {
		if ( ret < 0 && gnutls_error_is_fatal(ret)==0) {
			/* If we have sent any data then just return
			 * the error value. Do not invalidate the session.
			 */
			gnutls_assert();
			return ret;
		}
		
		if (ret > 0) {
			gnutls_assert();
			ret = GNUTLS_E_INTERNAL_ERROR;
		}

		_gnutls_session_unresumable( session);
		_gnutls_session_invalidate( session);
		gnutls_assert();
		return ret;
	}

	session->internals.record_send_buffer_user_size = 0;

	_gnutls_record_log( "REC[%x]: Sent Packet[%d] %s(%d) with length: %d\n",
	session, (int) _gnutls_uint64touint32(&session->connection_state.write_sequence_number), _gnutls_packet2str(type), type, cipher_size);

	return retval;
}

/* This function is to be called if the handshake was successfully 
 * completed. This sends a Change Cipher Spec packet to the peer.
 */
ssize_t _gnutls_send_change_cipher_spec( gnutls_session session, int again)
{
	static const opaque data[1] = { GNUTLS_TYPE_CHANGE_CIPHER_SPEC };

	_gnutls_handshake_log( "REC[%x]: Sent ChangeCipherSpec\n", session);

	if (again==0)
		return _gnutls_send_int( session, GNUTLS_CHANGE_CIPHER_SPEC, -1, data, 1);
	else {
		return _gnutls_io_write_flush( session);
	}
}

static int _gnutls_check_recv_type( ContentType recv_type) {
	switch( recv_type) {
	case GNUTLS_CHANGE_CIPHER_SPEC:
	case GNUTLS_ALERT:
	case GNUTLS_HANDSHAKE:
	case GNUTLS_APPLICATION_DATA:
		return 0;
	default:
		gnutls_assert();
		return GNUTLS_E_UNSUPPORTED_VERSION_PACKET;
	}

}


/* Checks if there are pending data in the record buffers. If there are
 * then it copies the data.
 */
static int _gnutls_check_buffers( gnutls_session session, ContentType type, opaque* data, int sizeofdata) {
	if ( (type == GNUTLS_APPLICATION_DATA || type == GNUTLS_HANDSHAKE) && _gnutls_record_buffer_get_size(type, session) > 0) {
		int ret, ret2;
		ret = _gnutls_record_buffer_get(type, session, data, sizeofdata);
		if (ret < 0) {
			gnutls_assert();
			return ret;
		}
		
		/* if the buffer just got empty */
		if (_gnutls_record_buffer_get_size(type, session)==0) {
			if ( (ret2=_gnutls_io_clear_peeked_data( session)) < 0) {
				gnutls_assert();
				return ret2;
			}
		}

		return ret;
	}
	
	return 0;
}


#define CHECK_RECORD_VERSION

/* Checks the record headers and returns the length, version and
 * content type.
 */
static int _gnutls_check_record_headers( gnutls_session session, uint8 headers[RECORD_HEADER_SIZE], ContentType type, 
	HandshakeType htype, /*output*/ ContentType *recv_type, opaque version[2], uint16 *length, uint16* header_size) {

	/* Read the first two bytes to determine if this is a 
	 * version 2 message 
	 */

	if ( htype == GNUTLS_CLIENT_HELLO && type==GNUTLS_HANDSHAKE && headers[0] > 127) { 

		/* if msb set and expecting handshake message
		 * it should be SSL 2 hello 
		 */
		version[0] = 3; /* assume SSL 3.0 */
		version[1] = 0;

		*length = (((headers[0] & 0x7f) << 8)) | headers[1];

		/* SSL 2.0 headers */
		*header_size = 2;
		*recv_type = GNUTLS_HANDSHAKE; /* we accept only v2 client hello
					       */
		
		/* in order to assist the handshake protocol.
		 * V2 compatibility is a mess.
		 */
		session->internals.v2_hello = *length;

		_gnutls_record_log( "REC[%x]: V2 packet received. Length: %d\n", session, *length);

	} else {
		/* version 3.x 
		 */
		*recv_type = headers[0];
		version[0] = headers[1];
		version[1] = headers[2];

		/* No DECR_LEN, since headers has enough size. 
		 */
		*length = _gnutls_read_uint16( &headers[3]);
	}

	return 0;
}

/* Here we check if the advertized version is the one we
 * negotiated in the handshake.
 */
inline
static int _gnutls_check_record_version( gnutls_session session, HandshakeType htype, opaque version[2]) 
{
#ifdef CHECK_RECORD_VERSION
	if ( (htype!=GNUTLS_CLIENT_HELLO && htype!=GNUTLS_SERVER_HELLO) && 
		gnutls_protocol_get_version(session) != _gnutls_version_get( version[0], version[1])) {

		gnutls_assert();
		_gnutls_record_log( "REC[%x]: INVALID VERSION PACKET: (%d) %d.%d\n", 
			session, htype, version[0], version[1]);

		return GNUTLS_E_UNSUPPORTED_VERSION_PACKET;
	}
#endif

	return 0;
}

/* This function will check if the received record type is
 * the one we actually expect.
 */
static int _gnutls_record_check_type( gnutls_session session, ContentType recv_type,
	ContentType type, HandshakeType htype, opaque* data, int data_size) {
	
	int ret;

	if ( (recv_type == type) && (type == GNUTLS_APPLICATION_DATA || type == GNUTLS_HANDSHAKE)) {
		_gnutls_record_buffer_put(type, session, (void *) data, data_size);
	} else {
		switch (recv_type) {
		case GNUTLS_ALERT:

			_gnutls_record_log( "REC[%x]: Alert[%d|%d] - %s - was received\n", 
				session, data[0], data[1], gnutls_alert_get_name((int)data[1]));

			session->internals.last_alert = data[1];

			/* if close notify is received and
			 * the alert is not fatal
			 */
			if (data[1] == GNUTLS_A_CLOSE_NOTIFY && data[0] != GNUTLS_AL_FATAL) {
				/* If we have been expecting for an alert do 
				 */

				return GNUTLS_E_INT_RET_0; /* EOF */
			} else {
			
				/* if the alert is FATAL or WARNING
				 * return the apropriate message
				 */
				
				gnutls_assert();		
				ret = GNUTLS_E_WARNING_ALERT_RECEIVED;
				if (data[0] == GNUTLS_AL_FATAL) {
					_gnutls_session_unresumable( session);
					_gnutls_session_invalidate( session);

					ret = GNUTLS_E_FATAL_ALERT_RECEIVED;
				}

				return ret;
			}
			break;

		case GNUTLS_CHANGE_CIPHER_SPEC:
			/* this packet is now handled in the recv_int()
			 * function
			 */
			gnutls_assert();
	
			return GNUTLS_E_UNEXPECTED_PACKET;

		case GNUTLS_APPLICATION_DATA:
			/* even if data is unexpected put it into the buffer */
			if ( (ret=_gnutls_record_buffer_put(recv_type, session, (void *) data, data_size)) < 0) {
				gnutls_assert();
				return ret;
			}

			gnutls_assert();
			
			/* the got_application data is only returned
			 * if expecting client hello (for rehandshake
			 * reasons). Otherwise it is an unexpected packet
			 */
			if (htype == GNUTLS_CLIENT_HELLO && type==GNUTLS_HANDSHAKE)
				return GNUTLS_E_GOT_APPLICATION_DATA;
			else return GNUTLS_E_UNEXPECTED_PACKET;
			
			break;
		case GNUTLS_HANDSHAKE:
			/* This is only legal if HELLO_REQUEST is received - and we are a client 
			 */
			if ( session->security_parameters.entity==GNUTLS_SERVER) {
				gnutls_assert();
				return GNUTLS_E_UNEXPECTED_PACKET;
			}
			
			/* If we are already in a handshake then a Hello
			 * Request is illegal. But here we don't really care
			 * since this message will never make it up here.
			 */

			/* So we accept it */
			return _gnutls_recv_hello_request( session, data, data_size);

			break;
		default:

			_gnutls_record_log( "REC[%x]: Received Unknown packet %d expecting %d\n", 
				session, recv_type, type);

			gnutls_assert();
			return GNUTLS_E_INTERNAL_ERROR;
		}
	}
	
	return 0;

}

#define MAX_EMPTY_PACKETS_SEQUENCE 4

/* This function behaves exactly like read(). The only difference is
 * that it accepts the gnutls_session and the ContentType of data to
 * receive (if called by the user the Content is Userdata only)
 * It is intended to receive data, under the current session.
 */
ssize_t _gnutls_recv_int( gnutls_session session, ContentType type, HandshakeType htype, 
	opaque *data, size_t sizeofdata)
{
	uint8 *tmpdata;
	int tmplen;
	opaque version[2];
	uint8 *headers;
	ContentType recv_type;
	uint16 length;
	uint8 *ciphertext;
	uint8 *recv_data;
	int ret, ret2;
	uint16 header_size;
	int empty_packet = 0;

	if (sizeofdata == 0 || data == NULL) {
		return GNUTLS_E_INVALID_REQUEST;
	}

	begin:
	
	if (empty_packet > MAX_EMPTY_PACKETS_SEQUENCE) {
		gnutls_assert();
		return GNUTLS_E_TOO_MANY_EMPTY_PACKETS;
	}
	
	if ( _gnutls_session_is_valid(session)!=0 || session->internals.may_read!=0) {
		gnutls_assert();
		return GNUTLS_E_INVALID_SESSION;
	}
	
	/* If we have enough data in the cache do not bother receiving
	 * a new packet. (in order to flush the cache)
	 */
	ret = _gnutls_check_buffers( session, type, data, sizeofdata);
	if (ret != 0)
		return ret;


	/* default headers for TLS 1.0
	 */
	header_size = RECORD_HEADER_SIZE;

	if ( (ret = _gnutls_io_read_buffered( session, &headers, header_size, -1)) != header_size) {
		if (ret < 0 && gnutls_error_is_fatal(ret)==0) return ret;

		_gnutls_session_invalidate( session);
		if (type==GNUTLS_ALERT) {
			gnutls_assert();
			return 0; /* we were expecting close notify */
		}
		_gnutls_session_unresumable( session);
		gnutls_assert();
		return GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
	}

	if ( (ret=_gnutls_check_record_headers( session, headers, type, htype, &recv_type, version, &length, &header_size)) < 0) {
		gnutls_assert();
		return ret;
	}

	/* Here we check if the Type of the received packet is
	 * ok. 
	 */
	if ( (ret = _gnutls_check_recv_type( recv_type)) < 0) {
		
		gnutls_assert();
		return ret;
	}

	/* Here we check if the advertized version is the one we
	 * negotiated in the handshake.
	 */
	if ( (ret=_gnutls_check_record_version( session, htype, version)) < 0) {
		gnutls_assert();
		_gnutls_session_invalidate( session);
		return ret;
	}

	_gnutls_record_log( "REC[%x]: Expected Packet[%d] %s(%d) with length: %d\n",
		session, (int) _gnutls_uint64touint32(&session->connection_state.read_sequence_number), _gnutls_packet2str(type), type, sizeofdata);
	_gnutls_record_log( "REC[%x]: Received Packet[%d] %s(%d) with length: %d\n",
		session, (int) _gnutls_uint64touint32(&session->connection_state.read_sequence_number), _gnutls_packet2str(recv_type), recv_type, length);

	if (length > MAX_RECV_SIZE) {

		_gnutls_record_log( "REC[%x]: FATAL ERROR: Received packet with length: %d\n", session, length);

		_gnutls_session_unresumable( session);
		_gnutls_session_invalidate( session);
		gnutls_assert();
		return GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
	}

	/* check if we have that data into buffer. 
 	 */
	if ( (ret = _gnutls_io_read_buffered( session, &recv_data, header_size+length, recv_type)) != header_size+length) {
		if (ret<0 && gnutls_error_is_fatal(ret)==0) return ret;

		_gnutls_session_unresumable( session);
		_gnutls_session_invalidate( session);
		gnutls_assert();
		return GNUTLS_E_UNEXPECTED_PACKET_LENGTH;		
	}
	
/* ok now we are sure that we can read all the data - so
 * move on !
 */
	_gnutls_io_clear_read_buffer( session);
	ciphertext = &recv_data[header_size];
	
	/* decrypt the data we got. We allocate MAX_RECORD_RECV_SIZE
	 * because we cannot predict the output data by the record
	 * packet length (due to compression).
	 */
	tmplen = MAX_RECORD_RECV_SIZE;
	tmpdata = gnutls_malloc( tmplen);
	if (tmpdata==NULL) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	tmplen = _gnutls_decrypt( session, ciphertext, length, tmpdata, tmplen, recv_type);
	if (tmplen < 0) {
		_gnutls_session_unresumable( session);
		_gnutls_session_invalidate( session);
		gnutls_free(tmpdata);
		gnutls_assert();
		return tmplen;
	}

	/* Check if this is a CHANGE_CIPHER_SPEC
	 */
	if (type == GNUTLS_CHANGE_CIPHER_SPEC && recv_type == GNUTLS_CHANGE_CIPHER_SPEC) {

		_gnutls_record_log( "REC[%x]: ChangeCipherSpec Packet was received\n", session);

		if ((size_t)tmplen!=sizeofdata) { /* sizeofdata should be 1 */
			gnutls_assert();
			gnutls_free(tmpdata);
			return GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
		}
		memcpy( data, tmpdata, sizeofdata);
		gnutls_free(tmpdata);

		return tmplen;
	}

	_gnutls_record_log( "REC[%x]: Decrypted Packet[%d] %s(%d) with length: %d\n",
		session, (int) _gnutls_uint64touint32(&session->connection_state.read_sequence_number), _gnutls_packet2str(recv_type), recv_type, tmplen);

	/* increase sequence number */
	if (_gnutls_uint64pp( &session->connection_state.read_sequence_number)!=0) {
		_gnutls_session_invalidate( session);
		gnutls_free(tmpdata);
		gnutls_assert();
		return GNUTLS_E_RECORD_LIMIT_REACHED;
	}

	if ( (ret=_gnutls_record_check_type( session, recv_type, type, htype, tmpdata, tmplen)) < 0) {
		gnutls_free(tmpdata);

		if (ret==GNUTLS_E_INT_RET_0) return 0;
		gnutls_assert();
		return ret;
	}
	gnutls_free(tmpdata);

	/* Get Application data from buffer 
	 */
	if ((type == GNUTLS_APPLICATION_DATA || type == GNUTLS_HANDSHAKE) && (recv_type == type)) {

		ret = _gnutls_record_buffer_get(type, session, data, sizeofdata);
		if (ret < 0) {
			gnutls_assert();
			return ret;
		}

		/* if the buffer just got empty */
		if (_gnutls_record_buffer_get_size(type, session)==0) {
			if ( (ret2 = _gnutls_io_clear_peeked_data( session)) < 0) {
				gnutls_assert();
				return ret2;
			}
		}
	} else {
		gnutls_assert();
		return GNUTLS_E_UNEXPECTED_PACKET;
		/* we didn't get what we wanted to 
		 */
	}

	/* TLS 1.0 CBC protection. 
	 * Actually this code is called if we just received
	 * an empty packet. An empty TLS packet is usually
	 * sent to protect some vulnerabilities in the CBC mode.
	 * In that case we go to the beginning and start reading
	 * the next packet.
	 */
	if (ret==0) {
		empty_packet++;
		goto begin;
	}
	
	return ret;
}


/**
  * gnutls_record_send - sends to the peer the specified data
  * @session: is a &gnutls_session structure.
  * @data: contains the data to send
  * @sizeofdata: is the length of the data
  *
  * This function has the similar semantics to write(). The only
  * difference is that is accepts a GNUTLS session, and uses different
  * error codes.
  *
  * If the EINTR is returned by the internal push function (write())
  * then GNUTLS_E_INTERRUPTED will be returned. If GNUTLS_E_INTERRUPTED or
  * GNUTLS_E_AGAIN is returned, you must call this function again, with the
  * same parameters; cf. gnutls_record_get_direction(). Otherwise the write
  * operation will be corrupted and the connection will be terminated.
  *
  * This function may accept a NULL pointer for data, and 0 for size, if
  * and only if the previous send was interrupted for some reason.
  *
  * Returns the number of bytes sent, or a negative error code.
  *
  **/
ssize_t gnutls_record_send( gnutls_session session, const void *data, size_t sizeofdata) {
	return _gnutls_send_int( session, GNUTLS_APPLICATION_DATA, -1, data, sizeofdata);
}

/**
  * gnutls_record_recv - reads data from the TLS record protocol
  * @session: is a &gnutls_session structure.
  * @data: contains the data to send
  * @sizeofdata: is the length of the data
  *
  * This function has the similar semantics to read(). The only
  * difference is that is accepts a GNUTLS session.
  * Also returns the number of bytes received, zero on EOF, but
  * a negative error code in case of an error.
  *
  * If this function returns GNUTLS_E_REHANDSHAKE, then you may
  * ignore this message, send an alert containing NO_RENEGOTIATION, 
  * or perform a handshake again. (only a client may receive this message)
  *
  **/
ssize_t gnutls_record_recv( gnutls_session session, void *data, size_t sizeofdata) {
	return _gnutls_recv_int( session, GNUTLS_APPLICATION_DATA, -1, data, sizeofdata);
}

/**
  * gnutls_record_get_max_size - returns the maximum record size
  * @session: is a &gnutls_session structure.
  *
  * This function returns the maximum record packet size in this connection.
  * The maximum record size is negotiated by the client after the
  * first handshake message.
  *
  **/
size_t gnutls_record_get_max_size( gnutls_session session) {
	/* Recv will hold the negotiated max record size
	 * always.
	 */
	return session->security_parameters.max_record_recv_size;
}


/**
  * gnutls_record_set_max_size - sets the maximum record size
  * @session: is a &gnutls_session structure.
  * @size: is the new size
  *
  * This function sets the maximum record packet size in this connection.
  * This property can only be set to clients. The server may
  * choose not to accept the requested size.
  *
  * Acceptable values are 512(=2^9), 1024(=2^10), 2048(=2^11) and 4096(=2^12).
  * Returns 0 on success. The requested record size does
  * get in effect immediately only while sending data. The receive
  * part will take effect after a successful handshake.
  *
  * This function uses a TLS extension called 'max record size'.
  * Not all TLS implementations use or even understand this extension.
  *
  **/
ssize_t gnutls_record_set_max_size( gnutls_session session, size_t size) {
ssize_t new_size;

	if (session->security_parameters.entity==GNUTLS_SERVER)
		return GNUTLS_E_INVALID_REQUEST;

	new_size = _gnutls_mre_record2num( size);

	if (new_size < 0) {
		gnutls_assert();
		return new_size;
	}
	
	session->security_parameters.max_record_send_size = size;

	session->internals.proposed_record_size = size;

	return 0;
}
