/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2007, 2008, 2010
 * Free Software Foundation, Inc.
 *
 * Copyright (C) 2011
 * Bardenheuer GmbH, Munich and Bundesdruckerei GmbH, Berlin
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA
 *
 */

#ifndef AUTH_RSA_H
# define AUTH_RSA_H

#include <gnutls_auth.h>

int
_gnutls_get_public_rsa_params (gnutls_session_t session,
			       bigint_t params[MAX_PUBLIC_PARAMS_SIZE],
			       int *params_len);

int
_gnutls_get_private_rsa_params (gnutls_session_t session,
				bigint_t ** params, int *params_size);

#endif /* AUTH_RSA_H */
