/*
 * Copyright (C) 2012 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

FILE_LICENCE ( GPL2_OR_LATER );

/** @file
 *
 * HMAC_DRBG tests
 *
 * These test vectors are provided by NIST as part of the
 * Cryptographic Toolkit Examples, downloadable from:
 *
 *    http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/HMAC_DRBG.pdf
 *
 */

/* Forcibly enable assertions */
#undef NDEBUG

#include <assert.h>
#include <string.h>
#include <ipxe/hmac_drbg.h>
#include <ipxe/test.h>

/** Define inline expected data */
#define EXPECT(...) { __VA_ARGS__ }

/** An HMAC_DRBG instantiation test */
struct hmac_drbg_test_instantiate {
	/** Entropy (including nonce) */
	const void *entropy;
	/** Length of entropy (including nonce) */
	size_t entropy_len;
	/** Personalisation string */
	const void *personal;
	/** Length of personalisation string */
	size_t personal_len;
	/** Expected key */
	const void *expected_key;
	/** Length of expected key */
	size_t expected_key_len;
	/** Expected value */
	const void *expected_value;
	/** Length of expected value */
	size_t expected_value_len;
};

/**
 * Define an HMAC_DRBG instantiation test
 *
 * @v name		Test name
 * @v entropy_array	Entropy input (including nonce)
 * @v personal_array	Personalisation string
 * @v key		Expected key
 * @v value		Expected value
 * @ret test		Instantiation test
 */
#define HMAC_DRBG_TEST_INSTANTIATE( name, entropy_array,		\
				    personal_array, key, value )	\
	static const uint8_t name ## _key [] = key;			\
	static const uint8_t name ## _value [] = value;			\
	static const struct hmac_drbg_test_instantiate name = {		\
		.entropy = entropy_array,				\
		.entropy_len = sizeof ( entropy_array ),		\
		.personal = personal_array,				\
		.personal_len = sizeof ( personal_array ),		\
		.expected_key = name ## _key,				\
		.expected_key_len = sizeof ( name ## _key ),		\
		.expected_value = name ## _value,			\
		.expected_value_len = sizeof ( name ## _value ),	\
	}

/**
 * Report instantiation test result
 *
 * @v state		HMAC_DRBG internal state
 * @v test		Instantiation test
 */
#define instantiate_ok( state, test ) do {				\
	assert ( (test)->expected_key_len == HMAC_DRBG_OUTLEN_BYTES );	\
	assert ( (test)->expected_value_len == HMAC_DRBG_OUTLEN_BYTES ); \
	hmac_drbg_instantiate ( (state), (test)->entropy,		\
				(test)->entropy_len, (test)->personal,	\
				(test)->personal_len );			\
	ok ( memcmp ( (state)->key, (test)->expected_key,		\
		      sizeof ( (state)->key ) ) == 0 );			\
	ok ( memcmp ( (state)->value, (test)->expected_value,		\
		      sizeof ( (state)->value ) ) == 0 );		\
	} while ( 0 )

/** An HMAC_DRBG reseed test */
struct hmac_drbg_test_reseed {
	/** Entropy */
	const void *entropy;
	/** Length of entropy */
	size_t entropy_len;
	/** Additional input */
	const void *additional;
	/** Length of additional_input */
	size_t additional_len;
	/** Expected key */
	const void *expected_key;
	/** Length of expected key */
	size_t expected_key_len;
	/** Expected value */
	const void *expected_value;
	/** Length of expected value */
	size_t expected_value_len;
};

/**
 * Define an HMAC_DRBG reseed test
 *
 * @v name		Test name
 * @v entropy_array	Entropy input
 * @v additional_array	Additional input
 * @v key		Expected key
 * @v value		Expected value
 * @ret test		Reseed test
 */
#define HMAC_DRBG_TEST_RESEED( name, entropy_array, additional_array,	\
			       key, value )		\
	static const uint8_t name ## _key [] = key;			\
	static const uint8_t name ## _value [] = value;			\
	static const struct hmac_drbg_test_reseed name = {		\
		.entropy = entropy_array,				\
		.entropy_len = sizeof ( entropy_array ),		\
		.additional = additional_array,				\
		.additional_len = sizeof ( additional_array ),		\
		.expected_key = name ## _key,				\
		.expected_key_len = sizeof ( name ## _key ),		\
		.expected_value = name ## _value,			\
		.expected_value_len = sizeof ( name ## _value ),	\
	}

/**
 * Report reseed test result
 *
 * @v state		HMAC_DRBG internal state
 * @v test		Reseed test
 */
#define reseed_ok( state, test ) do {					\
	assert ( (test)->expected_key_len == HMAC_DRBG_OUTLEN_BYTES );	\
	assert ( (test)->expected_value_len == HMAC_DRBG_OUTLEN_BYTES ); \
	hmac_drbg_reseed ( (state), (test)->entropy,			\
			   (test)->entropy_len, (test)->additional,	\
			   (test)->additional_len );			\
	ok ( memcmp ( (state)->key, (test)->expected_key,		\
		      sizeof ( (state)->key ) ) == 0 );			\
	ok ( memcmp ( (state)->value, (test)->expected_value,		\
		      sizeof ( (state)->value ) ) == 0 );		\
	} while ( 0 )

/** An HMAC_DRBG generation test */
struct hmac_drbg_test_generate {
	/** Additional input */
	const void *additional;
	/** Length of additional_input */
	size_t additional_len;
	/** Expected key */
	const void *expected_key;
	/** Length of expected key */
	size_t expected_key_len;
	/** Expected value */
	const void *expected_value;
	/** Length of expected value */
	size_t expected_value_len;
	/** Expected pseudorandom data */
	const void *expected_data;
	/** Length of data */
	size_t len;
};

/**
 * Define an HMAC_DRBG generation test
 *
 * @v name		Test name
 * @v additional_array	Additional input
 * @v key		Expected key
 * @v value		Expected value
 * @v data		Expected pseudorandom data
 * @ret test		Generation test
 */
#define HMAC_DRBG_TEST_GENERATE( name, additional_array, key, value,	\
				 data )					\
	static const uint8_t name ## _key [] = key;			\
	static const uint8_t name ## _value [] = value;			\
	static const uint8_t name ## _data [] = data;			\
	static const struct hmac_drbg_test_generate name = {		\
		.additional = additional_array,				\
		.additional_len = sizeof ( additional_array ),		\
		.expected_key = name ## _key,				\
		.expected_key_len = sizeof ( name ## _key ),		\
		.expected_value = name ## _value,			\
		.expected_value_len = sizeof ( name ## _value ),	\
		.expected_data = name ## _data,				\
		.len = sizeof ( name ## _data ),			\
	}

/**
 * Report generation test result
 *
 * @v state		HMAC_DRBG internal state
 * @v test		Generation test
 */
#define generate_ok( state, test ) do {					\
	uint8_t data[ (test)->len ];					\
	int rc;								\
									\
	assert ( (test)->expected_key_len == HMAC_DRBG_OUTLEN_BYTES );	\
	assert ( (test)->expected_value_len == HMAC_DRBG_OUTLEN_BYTES );\
	rc = hmac_drbg_generate ( (state), (test)->additional,		\
				  (test)->additional_len, data,		\
				  sizeof ( data ) );			\
	ok ( rc == 0 );							\
	ok ( memcmp ( (state)->key, (test)->expected_key,		\
		      sizeof ( (state)->key ) ) == 0 );			\
	ok ( memcmp ( (state)->value, (test)->expected_value,		\
		      sizeof ( (state)->value ) ) == 0 );		\
	ok ( memcmp ( data, (test)->expected_data,			\
		      sizeof ( data ) ) == 0 );				\
	} while ( 0 )

/** An HMAC_DRBG generation failure test */
struct hmac_drbg_test_generate_fail {
	/** Additional input */
	const void *additional;
	/** Length of additional_input */
	size_t additional_len;
	/** Length of requested data */
	size_t requested_len;
};

/**
 * Define an HMAC_DRBG generation failure test
 *
 * @v name		Test name
 * @v additional_array	Additional input
 * @ret test		Generation failure test
 */
#define HMAC_DRBG_TEST_GENERATE_FAIL( name, additional_array, len )	\
	static const struct hmac_drbg_test_generate_fail name = {	\
		.additional = additional_array,				\
		.additional_len = sizeof ( additional_array ),		\
		.requested_len = len,					\
	}

/**
 * Report generation failure test result
 *
 * @v state		HMAC_DRBG internal state
 * @v test		Generation failure test
 */
#define generate_fail_ok( state, test ) do {				\
	uint8_t data[ (test)->requested_len ];				\
	int rc;								\
									\
	rc = hmac_drbg_generate ( (state), (test)->additional,		\
				  (test)->additional_len, data,		\
				  sizeof ( data ) );			\
	ok ( rc != 0 );							\
	} while ( 0 )

/** "EntropyInput" and "Nonce"
 *
 * These are pre-concatenated since our implementation expects to
 * receive the nonce in the form of additional entropy.
 */
static const uint8_t entropy_input[] = {
	/* "EntropyInput" */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
	/* "Nonce" */
	0x20, 0x21, 0x22, 0x23, 0x24
};

/** "EntropyInput1 (for Reseed1) */
static const uint8_t entropy_input_1[] = {
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
	0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
	0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
	0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6
};

/** "EntropyInput2 (for Reseed2) */
static const uint8_t entropy_input_2[] = {
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
	0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
	0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
	0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6
};

/** "PersonalizationString = <empty>" */
static const uint8_t personalisation_string_empty[] = {};

/** "PersonalizationString" */
static const uint8_t personalisation_string[] = {
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
	0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76
};

/** "AdditionalInput = <empty>" */
static const uint8_t additional_input_empty[] = {};

/** "AdditionalInput1" */
static const uint8_t additional_input_1[] = {
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
	0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
	0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96
};

/** "AdditionalInput2" */
static const uint8_t additional_input_2[] = {
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
	0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
	0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6
};

/** Test 1 : Instantiation */
HMAC_DRBG_TEST_INSTANTIATE ( instantiate_1,
	entropy_input, personalisation_string_empty,
	EXPECT ( 0xab, 0x16, 0x0d, 0xd2, 0x1c, 0x30, 0x98, 0x0c, 0xa3, 0xca,
		 0x5a, 0x9c, 0x77, 0xb7, 0xbd, 0xf0, 0x50, 0xe6, 0x4e, 0xe9 ),
	EXPECT ( 0x61, 0x44, 0x99, 0xea, 0x98, 0x0c, 0xfb, 0x3d, 0xaa, 0x2c,
		 0xa8, 0x6d, 0x65, 0xa4, 0x6b, 0xf4, 0x48, 0x8d, 0x8c, 0xc5 ) );

/** Test 1.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_1_1,
	additional_input_empty,
	EXPECT ( 0x7b, 0xb1, 0x80, 0x28, 0xe0, 0x1d, 0x03, 0x42, 0xdf, 0x4f,
		 0x54, 0xda, 0x51, 0x22, 0xfa, 0x5f, 0x2c, 0x3a, 0x05, 0xe4 ),
	EXPECT ( 0x2f, 0x89, 0x4f, 0x28, 0xcc, 0x2f, 0x53, 0x82, 0x96, 0x40,
		 0x64, 0x3a, 0xd1, 0x7b, 0x84, 0xb0, 0xcd, 0x3c, 0x79, 0x79 ),
	EXPECT ( 0x5a, 0x7d, 0x3b, 0x44, 0x9f, 0x48, 0x1c, 0xb3, 0x8d, 0xf7,
		 0x9a, 0xd2, 0xb1, 0xfc, 0xc0, 0x1e, 0x57, 0xf8, 0x13, 0x5e,
		 0x8c, 0x0b, 0x22, 0xcd, 0x06, 0x30, 0xbf, 0xb0, 0x12, 0x7f,
		 0xb5, 0x40, 0x8c, 0x8e, 0xfc, 0x17, 0xa9, 0x29, 0x89, 0x6e ) );

/** Test 1.2 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_1_2,
	additional_input_empty,
	EXPECT ( 0x3d, 0x4d, 0x73, 0x77, 0xe9, 0x17, 0x2a, 0xaf, 0xa7, 0x76,
		 0xb0, 0xdd, 0xcb, 0x89, 0x42, 0x00, 0x4a, 0x44, 0xb7, 0xfd ),
	EXPECT ( 0x1a, 0x26, 0xbd, 0x9b, 0xfc, 0x97, 0x44, 0xbd, 0x29, 0xf6,
		 0xae, 0xbe, 0x24, 0x37, 0xe2, 0x09, 0xf1, 0xf7, 0x16, 0x25 ),
	EXPECT ( 0x82, 0xcf, 0x77, 0x2e, 0xc3, 0xe8, 0x4b, 0x00, 0xfc, 0x74,
		 0xf5, 0xdf, 0x10, 0x4e, 0xfb, 0xfb, 0x24, 0x28, 0x55, 0x4e,
		 0x9c, 0xe3, 0x67, 0xd0, 0x3a, 0xea, 0xde, 0x37, 0x82, 0x7f,
		 0xa8, 0xe9, 0xcb, 0x6a, 0x08, 0x19, 0x61, 0x15, 0xd9, 0x48 ) );

/** Test 2 : Instantiation */
#define instantiate_2 instantiate_1

/** Test 2.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_2_1,
	additional_input_1,
	EXPECT ( 0x3a, 0x06, 0x2e, 0x6b, 0x79, 0xfe, 0x70, 0xdb, 0xff, 0xeb,
		 0x3a, 0x2b, 0x6b, 0xe8, 0x03, 0x23, 0xf7, 0xd6, 0x74, 0xc5 ),
	EXPECT ( 0xbd, 0x36, 0x31, 0x28, 0xbf, 0x58, 0x0d, 0x7a, 0x54, 0x42,
		 0x9d, 0xdd, 0x58, 0xe8, 0x19, 0x3b, 0x98, 0x43, 0xbd, 0x2b ),
	EXPECT ( 0xc7, 0xaa, 0xac, 0x58, 0x3c, 0x6e, 0xf6, 0x30, 0x07, 0x14,
		 0xc2, 0xcc, 0x5d, 0x06, 0xc1, 0x48, 0xcf, 0xfb, 0x40, 0x44,
		 0x9a, 0xd0, 0xbb, 0x26, 0xfa, 0xc0, 0x49, 0x7b, 0x5c, 0x57,
		 0xe1, 0x61, 0xe3, 0x66, 0x81, 0xbc, 0xc9, 0x30, 0xce, 0x80 ) );

/** Test 2.2 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_2_2,
	additional_input_2,
	EXPECT ( 0x8a, 0xd7, 0xe3, 0x47, 0x72, 0xb5, 0xfc, 0x7c, 0x3b, 0x3b,
		 0x27, 0x62, 0x4f, 0x0b, 0x91, 0x77, 0x6a, 0x8a, 0x71, 0x12 ),
	EXPECT ( 0xd7, 0x13, 0x76, 0xa4, 0x6d, 0x76, 0x4b, 0x17, 0xc3, 0xb7,
		 0x39, 0x34, 0x7b, 0x38, 0x4e, 0x51, 0x51, 0xe8, 0x7e, 0x88 ),
	EXPECT ( 0x6e, 0xbd, 0x2b, 0x7b, 0x5e, 0x0a, 0x2a, 0xd7, 0xa2, 0x4b,
		 0x1b, 0xf9, 0xa1, 0xdb, 0xa4, 0x7d, 0x43, 0x27, 0x17, 0x19,
		 0xb9, 0xc3, 0x7b, 0x7f, 0xe8, 0x1b, 0xa9, 0x40, 0x45, 0xa1,
		 0x4a, 0x7c, 0xb5, 0x14, 0xb4, 0x46, 0x66, 0x6e, 0xa5, 0xa7 ) );

/** Test 3 : Instantiation */
HMAC_DRBG_TEST_INSTANTIATE ( instantiate_3,
	entropy_input, personalisation_string,
	EXPECT ( 0xb7, 0xd9, 0x66, 0xd7, 0x0d, 0x4e, 0x27, 0xa7, 0xfa, 0x83,
		 0x8f, 0x7d, 0x61, 0x12, 0x6c, 0x0e, 0xdc, 0x84, 0x76, 0x1c ),
	EXPECT ( 0xda, 0xb2, 0xa7, 0x18, 0x83, 0xf1, 0x00, 0x5c, 0x5d, 0xd0,
		 0x39, 0x32, 0x4d, 0x3c, 0x36, 0x4d, 0x6e, 0x18, 0xf9, 0x54 ) );

/** Test 3.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_3_1,
	additional_input_empty,
	EXPECT ( 0x87, 0xd3, 0x82, 0x8b, 0xe0, 0x3a, 0x80, 0x7d, 0xd3, 0x40,
		 0x29, 0x41, 0xbe, 0xd6, 0xde, 0x98, 0x6e, 0xe7, 0xa2, 0x86 ),
	EXPECT ( 0x6a, 0xe1, 0xd0, 0x08, 0x6f, 0x53, 0xb1, 0xb7, 0x63, 0xa4,
		 0x51, 0x5b, 0x19, 0x06, 0xfe, 0xe4, 0x76, 0x61, 0xfd, 0x47 ),
	EXPECT ( 0xb3, 0xbd, 0x05, 0x24, 0x6c, 0xba, 0x12, 0xa6, 0x47, 0x35,
		 0xa4, 0xe3, 0xfd, 0xe5, 0x99, 0xbc, 0x1b, 0xe3, 0x0f, 0x43,
		 0x9b, 0xd0, 0x60, 0x20, 0x8e, 0xea, 0x7d, 0x71, 0xf9, 0xd1,
		 0x23, 0xdf, 0x47, 0xb3, 0xce, 0x06, 0x9d, 0x98, 0xed, 0xe6 ) );

/** Test 3.2 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_3_2,
	additional_input_empty,
	EXPECT ( 0x26, 0xab, 0xbf, 0x54, 0xb2, 0x8b, 0x93, 0xff, 0x90, 0x08,
		 0x67, 0x0e, 0xbf, 0xee, 0x86, 0xcd, 0xd7, 0x22, 0x8e, 0xd5 ),
	EXPECT ( 0xe9, 0x25, 0x47, 0x29, 0xe0, 0x02, 0x04, 0xa1, 0xb6, 0xc0,
		 0x21, 0x58, 0xa6, 0xc7, 0x27, 0x86, 0x47, 0x14, 0xf1, 0xf7 ),
	EXPECT ( 0xb5, 0xda, 0xda, 0x38, 0x0e, 0x28, 0x72, 0xdf, 0x93, 0x5b,
		 0xca, 0x55, 0xb8, 0x82, 0xc8, 0xc9, 0x37, 0x69, 0x02, 0xab,
		 0x63, 0x97, 0x65, 0x47, 0x2b, 0x71, 0xac, 0xeb, 0xe2, 0xea,
		 0x8b, 0x1b, 0x6b, 0x49, 0x62, 0x9c, 0xb6, 0x73, 0x17, 0xe0 ) );

/** Test 4 : Instantiation */
#define instantiate_4 instantiate_3

/** Test 4.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_4_1,
	additional_input_1,
	EXPECT ( 0x17, 0xa5, 0xd7, 0x9f, 0x07, 0x67, 0x87, 0x6f, 0x3a, 0x45,
		 0xe0, 0xc9, 0xc3, 0x3e, 0xc8, 0x8b, 0x03, 0xce, 0xea, 0x13 ),
	EXPECT ( 0x4d, 0x2f, 0x3b, 0xc7, 0x77, 0x50, 0x5c, 0x45, 0xf7, 0xe1,
		 0x7d, 0xcd, 0x3d, 0x86, 0xbf, 0x37, 0x9c, 0xb6, 0x02, 0x5e ),
	EXPECT ( 0x1f, 0x8f, 0xec, 0x7b, 0xc7, 0xcf, 0xa9, 0xa8, 0x80, 0x34,
		 0x5d, 0x28, 0x0b, 0x13, 0xc6, 0x32, 0xb8, 0x52, 0x77, 0x0a,
		 0x6d, 0xfc, 0x30, 0x2e, 0xad, 0x4c, 0xe3, 0xf5, 0x54, 0xc7,
		 0x9b, 0x0d, 0x44, 0x23, 0x9e, 0xba, 0x56, 0xa7, 0xea, 0x2d ) );

/** Test 4.2 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_4_2,
	additional_input_2,
	EXPECT ( 0x07, 0x9b, 0x57, 0xd9, 0x40, 0x6e, 0x11, 0xc2, 0xf8, 0x7c,
		 0x8c, 0x82, 0x8c, 0x8c, 0x6f, 0xa7, 0x6e, 0x40, 0xea, 0x01 ),
	EXPECT ( 0xa6, 0x54, 0xfe, 0x72, 0xf8, 0xa7, 0x7b, 0xb8, 0xf0, 0x3d,
		 0xff, 0x07, 0xc7, 0x9a, 0x51, 0x53, 0x00, 0x9e, 0xdd, 0xda ),
	EXPECT ( 0xaf, 0x97, 0xcd, 0xe1, 0xe8, 0xab, 0x32, 0x2a, 0x2e, 0xac,
		 0xa8, 0xe6, 0xf4, 0xe5, 0xbf, 0x78, 0xa1, 0x1b, 0xde, 0xf7,
		 0xdc, 0x91, 0x21, 0x5d, 0x44, 0xb1, 0x07, 0xb4, 0xd5, 0xa7,
		 0x79, 0x01, 0x59, 0x25, 0x09, 0x76, 0x52, 0x80, 0xf9, 0x69 ) );

/** Test 5 : Instantiation */
#define instantiate_5 instantiate_1

/** Test 5.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_5_1,
	additional_input_empty, ( 320 / 8 ) );

/** Test 5.2 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_5_2,
	entropy_input_1, additional_input_empty,
	EXPECT ( 0xcd, 0x4c, 0xab, 0x38, 0xc8, 0xad, 0x65, 0x71, 0x22, 0xbf,
		 0x5d, 0x3d, 0x00, 0xd0, 0xac, 0x9b, 0x13, 0xd6, 0x29, 0xbb ),
	EXPECT ( 0xf6, 0x60, 0xe2, 0x3e, 0x91, 0x00, 0x6b, 0x62, 0xc6, 0x54,
		 0x3a, 0xb1, 0x34, 0x4d, 0x23, 0xa3, 0x1a, 0xb4, 0xcf, 0x2c ) );

/** Test 5.3 : Retried first call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_5_3,
	additional_input_empty,
	EXPECT ( 0x58, 0x7f, 0xd8, 0x21, 0xef, 0x6c, 0x9d, 0xa4, 0xa8, 0x3c,
		 0x19, 0x21, 0x1f, 0x10, 0x56, 0xca, 0xcd, 0x23, 0xfc, 0x1a ),
	EXPECT ( 0x84, 0x8f, 0xd1, 0x4c, 0x13, 0xb7, 0xea, 0x93, 0x72, 0x0c,
		 0xcf, 0xde, 0x71, 0xf2, 0xf6, 0x44, 0x39, 0xdb, 0x79, 0x5d ),
	EXPECT ( 0xfe, 0xc4, 0x59, 0x7f, 0x06, 0xa3, 0xa8, 0xcc, 0x85, 0x29,
		 0xd5, 0x95, 0x57, 0xb9, 0xe6, 0x61, 0x05, 0x38, 0x09, 0xc0,
		 0xbc, 0x0e, 0xfc, 0x28, 0x2a, 0xbd, 0x87, 0x60, 0x5c, 0xc9,
		 0x0c, 0xba, 0x9b, 0x86, 0x33, 0xdc, 0xb1, 0xda, 0xe0, 0x2e ) );

/** Test 5.4 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_5_4,
	additional_input_empty, ( 320 / 8 ) );

/** Test 5.5 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_5_5,
	entropy_input_2, additional_input_empty,
	EXPECT ( 0xdb, 0xa1, 0xcf, 0xf4, 0x87, 0x95, 0x46, 0xa0, 0x38, 0xa5,
		 0x59, 0xb2, 0xa2, 0x4d, 0xf2, 0xc0, 0x30, 0x08, 0x9a, 0x41 ),
	EXPECT ( 0x2f, 0x88, 0x3c, 0x46, 0x48, 0xe1, 0x31, 0xe8, 0x6d, 0xdf,
		 0x9d, 0xca, 0x0d, 0x74, 0xf3, 0x0c, 0xa1, 0xce, 0x6e, 0xfb ) );

/** Test 5.6 : Retried second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_5_6,
	additional_input_empty,
	EXPECT ( 0xf9, 0x39, 0xa5, 0xab, 0x08, 0xa3, 0x9f, 0x23, 0x10, 0x70,
		 0xb0, 0xd4, 0xc9, 0x6d, 0xc2, 0x37, 0x90, 0xba, 0x01, 0x53 ),
	EXPECT ( 0xce, 0x6d, 0x08, 0xb4, 0xae, 0x2c, 0xe3, 0x83, 0xfd, 0xab,
		 0xb0, 0x1e, 0xaa, 0xfc, 0x9c, 0x8e, 0x76, 0xa0, 0xd4, 0x72 ),
	EXPECT ( 0x84, 0xad, 0xd5, 0xe2, 0xd2, 0x04, 0x1c, 0x01, 0x72, 0x3a,
		 0x4d, 0xe4, 0x33, 0x5b, 0x13, 0xef, 0xdf, 0x16, 0xb0, 0xe5,
		 0x1a, 0x0a, 0xd3, 0x9b, 0xd1, 0x5e, 0x86, 0x2e, 0x64, 0x4f,
		 0x31, 0xe4, 0xa2, 0xd7, 0xd8, 0x43, 0xe5, 0x7c, 0x59, 0x68 ) );

/** Test 6 : Instantiate */
#define instantiate_6 instantiate_1

/** Test 6.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_6_1,
	additional_input_1, ( 320 / 8 ) );

/** Test 6.2 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_6_2,
	entropy_input_1, additional_input_1,
	EXPECT ( 0x52, 0x28, 0xa4, 0xb6, 0xa4, 0x46, 0x92, 0x90, 0x5e, 0xc0,
		 0x44, 0xbf, 0xf0, 0xbb, 0x4e, 0x25, 0xa3, 0x87, 0xca, 0xc1 ),
	EXPECT ( 0x24, 0x77, 0x32, 0xd0, 0x4c, 0xb8, 0x4e, 0xd4, 0x1a, 0xdd,
		 0x95, 0xa4, 0xb7, 0x8b, 0x50, 0xcd, 0x9b, 0x3d, 0x3f, 0x32 ) );

/** Test 6.3 : Retried first call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_6_3,
	additional_input_empty,
	EXPECT ( 0xab, 0x3d, 0xd4, 0x89, 0x5b, 0xc8, 0xcd, 0x22, 0x71, 0xde,
		 0xba, 0x5f, 0x3c, 0x13, 0x63, 0x52, 0x6b, 0x8b, 0x74, 0x52 ),
	EXPECT ( 0xa8, 0x66, 0xc5, 0xef, 0xf2, 0xaf, 0x04, 0x2b, 0x11, 0x86,
		 0x44, 0x94, 0x45, 0x23, 0x7f, 0x9c, 0x02, 0x44, 0x98, 0x64 ),
	EXPECT ( 0xa1, 0xba, 0x8f, 0xa5, 0x8b, 0xb5, 0x01, 0x3f, 0x43, 0xf7,
		 0xb6, 0xed, 0x52, 0xb4, 0x53, 0x9f, 0xa1, 0x6d, 0xc7, 0x79,
		 0x57, 0xae, 0xe8, 0x15, 0xb9, 0xc0, 0x70, 0x04, 0xc7, 0xe9,
		 0x92, 0xeb, 0x8c, 0x7e, 0x59, 0x19, 0x64, 0xaf, 0xee, 0xa2 ) );

/** Test 6.4 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_6_4,
	additional_input_2, ( 320 / 8 ) );

/** Test 6.5 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_6_5,
	entropy_input_2, additional_input_2,
	EXPECT ( 0xe5, 0x73, 0x9f, 0x9c, 0xf7, 0xff, 0x43, 0x84, 0xd1, 0x27,
		 0x3e, 0x02, 0x6b, 0x45, 0x31, 0x21, 0x36, 0x49, 0x4f, 0x41 ),
	EXPECT ( 0x30, 0xc3, 0x43, 0x05, 0xc2, 0xc6, 0x48, 0xb0, 0x57, 0xa6,
		 0x40, 0x22, 0x1b, 0x5c, 0x56, 0x57, 0x26, 0xcd, 0x32, 0xb2 ) );

/** Test 6.6 : Retried second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_6_6,
	additional_input_empty,
	EXPECT ( 0x61, 0x91, 0xca, 0x9b, 0xf0, 0x00, 0xd1, 0x0a, 0x71, 0x69,
		 0x0a, 0xc1, 0x0e, 0x09, 0xff, 0xc8, 0x92, 0xab, 0xde, 0x9a ),
	EXPECT ( 0x1e, 0xc0, 0x49, 0x0f, 0xa0, 0xb7, 0x65, 0x52, 0x7e, 0x5e,
		 0xa1, 0x8b, 0x53, 0x22, 0xb2, 0x8b, 0xdd, 0x0e, 0x7b, 0xc0 ),
	EXPECT ( 0x84, 0x26, 0x4a, 0x73, 0xa8, 0x18, 0xc9, 0x5c, 0x2f, 0x42,
		 0x4b, 0x37, 0xd3, 0xcc, 0x99, 0x0b, 0x04, 0x6f, 0xb5, 0x0c,
		 0x2d, 0xc6, 0x4a, 0x16, 0x42, 0x11, 0x88, 0x9a, 0x01, 0x0f,
		 0x24, 0x71, 0xa0, 0x91, 0x2f, 0xfe, 0xa1, 0xbf, 0x01, 0x95 ) );

/** Test 7 : Instantiation */
#define instantiate_7 instantiate_3

/** Test 7.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_7_1,
	additional_input_empty, ( 320 / 8 ) );

/** Test 7.2 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_7_2,
	entropy_input_1, additional_input_empty,
	EXPECT ( 0xb9, 0x25, 0x4d, 0x8a, 0xac, 0xba, 0x43, 0xfb, 0xda, 0xe6,
		 0x39, 0x4f, 0x2b, 0x3a, 0xfc, 0x5d, 0x58, 0x08, 0x00, 0xbf ),
	EXPECT ( 0x28, 0x40, 0x3b, 0x60, 0x36, 0x38, 0xd0, 0x7d, 0x79, 0x66,
		 0x66, 0x1e, 0xf6, 0x7b, 0x9d, 0x39, 0x05, 0xf4, 0x6d, 0xb9 ) );

/** Test 7.3 : Retried first call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_7_3,
	additional_input_empty,
	EXPECT ( 0x64, 0xfe, 0x07, 0x4a, 0x6e, 0x77, 0x97, 0xd1, 0xa4, 0x35,
		 0xda, 0x89, 0x64, 0x48, 0x4d, 0x6c, 0xf8, 0xbd, 0xc0, 0x1b ),
	EXPECT ( 0x43, 0xe0, 0xc0, 0x52, 0x15, 0x86, 0xe9, 0x47, 0x3b, 0x06,
		 0x0d, 0x87, 0xd0, 0x8a, 0x23, 0x25, 0xfa, 0xe1, 0x49, 0xd1 ),
	EXPECT ( 0x6c, 0x37, 0xfd, 0xd7, 0x29, 0xaa, 0x40, 0xf8, 0x0b, 0xc6,
		 0xab, 0x08, 0xca, 0x7c, 0xc6, 0x49, 0x79, 0x4f, 0x69, 0x98,
		 0xb5, 0x70, 0x81, 0xe4, 0x22, 0x0f, 0x22, 0xc5, 0xc2, 0x83,
		 0xe2, 0xc9, 0x1b, 0x8e, 0x30, 0x5a, 0xb8, 0x69, 0xc6, 0x25 ) );

/** Test 7.4 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_7_4,
	additional_input_empty, ( 320 / 8 ) );

/** Test 7.5 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_7_5,
	entropy_input_2, additional_input_empty,
	EXPECT ( 0x02, 0xbc, 0x57, 0x7f, 0xd1, 0x0e, 0xf7, 0x19, 0x3c, 0x1d,
		 0xb0, 0x98, 0xbd, 0x5b, 0x75, 0xc7, 0xc4, 0xb6, 0x79, 0x59 ),
	EXPECT ( 0xbc, 0xbd, 0xf0, 0x52, 0xe0, 0xe0, 0x2a, 0xe8, 0x9a, 0x77,
		 0x67, 0x94, 0x3f, 0x98, 0x65, 0xb8, 0xb7, 0x22, 0x90, 0x2d ) );

/** Test 7.6 : Retried second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_7_6,
	additional_input_empty,
	EXPECT ( 0x1a, 0xa4, 0x24, 0x1c, 0x69, 0x5e, 0x29, 0xc0, 0xa5, 0x9a,
		 0xd1, 0x8a, 0x60, 0x70, 0xe3, 0x38, 0xa5, 0x48, 0xbe, 0x92 ),
	EXPECT ( 0x03, 0x47, 0x35, 0x9b, 0xc9, 0xc7, 0xf8, 0x8c, 0xc8, 0x33,
		 0x0d, 0x4f, 0x59, 0xfb, 0xc7, 0x70, 0xb0, 0xb7, 0x7b, 0x03 ),
	EXPECT ( 0xca, 0xf5, 0x7d, 0xcf, 0xea, 0x39, 0x3b, 0x92, 0x36, 0xbf,
		 0x69, 0x1f, 0xa4, 0x56, 0xfe, 0xa7, 0xfd, 0xf1, 0xdf, 0x83,
		 0x61, 0x48, 0x2c, 0xa5, 0x4d, 0x5f, 0xa7, 0x23, 0xf4, 0xc8,
		 0x8b, 0x4f, 0xa5, 0x04, 0xbf, 0x03, 0x27, 0x7f, 0xa7, 0x83 ) );

/** Test 8 : Instantiate */
#define instantiate_8 instantiate_3

/** Test 8.1 : First call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_8_1,
	additional_input_1, ( 320 / 8 ) );

/** Test 8.2 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_8_2,
	entropy_input_1, additional_input_1,
	EXPECT ( 0xc0, 0x95, 0x48, 0xc0, 0xd3, 0xc8, 0x61, 0xd7, 0x40, 0xf2,
		 0x83, 0x7d, 0x72, 0xb5, 0x07, 0x23, 0x5c, 0x26, 0xdb, 0x82 ),
	EXPECT ( 0x17, 0x4b, 0x3f, 0x84, 0xc3, 0x53, 0x1f, 0x7c, 0x0a, 0x2e,
		 0x54, 0x21, 0x23, 0x4e, 0xa1, 0x6b, 0x70, 0x8d, 0xdf, 0x0d ) );

/** Test 8.3 : Retried first call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_8_3,
	additional_input_empty,
	EXPECT ( 0x60, 0x3f, 0x09, 0x49, 0x27, 0x9c, 0x70, 0xe8, 0xc6, 0x6c,
		 0x0f, 0x56, 0x37, 0xc0, 0xf3, 0x75, 0x60, 0x07, 0xe5, 0xac ),
	EXPECT ( 0xf2, 0xb3, 0x3b, 0x21, 0x15, 0x1f, 0xaf, 0x61, 0x20, 0x01,
		 0x83, 0x10, 0xf4, 0x4e, 0x4c, 0xd0, 0xbf, 0xe3, 0x68, 0xea ),
	EXPECT ( 0xbd, 0x07, 0xc2, 0x5c, 0xfd, 0x7c, 0x5e, 0x3a, 0x4e, 0xaa,
		 0x6e, 0x2e, 0xdc, 0x5a, 0xb7, 0xea, 0x49, 0x42, 0xa0, 0x91,
		 0x34, 0x71, 0xfd, 0xa5, 0x5c, 0x6d, 0xdd, 0x2c, 0x03, 0xef,
		 0xa3, 0xb9, 0x64, 0x3a, 0xb3, 0xbb, 0x22, 0xf6, 0xc9, 0xf2 ) );

/** Test 8.4 : Second call to Generate */
HMAC_DRBG_TEST_GENERATE_FAIL ( generate_fail_8_4,
	additional_input_2, ( 320 / 8 ) );

/** Test 8.5 : Reseed */
HMAC_DRBG_TEST_RESEED ( reseed_8_5,
	entropy_input_2, additional_input_2,
	EXPECT ( 0x89, 0x42, 0xa5, 0x4f, 0x34, 0x9e, 0x28, 0x1b, 0x84, 0xaa,
		 0x46, 0x95, 0x87, 0xfb, 0xdd, 0xaf, 0x9d, 0x11, 0x40, 0x82 ),
	EXPECT ( 0x07, 0x73, 0x0e, 0x3c, 0xbf, 0xfd, 0x3c, 0xaf, 0xd7, 0xa8,
		 0xaa, 0xe2, 0xbf, 0x01, 0xd6, 0x01, 0x43, 0x01, 0xe2, 0x4d ) );

/** Test 8.6 : Retried second call to Generate */
HMAC_DRBG_TEST_GENERATE ( generate_8_6,
	additional_input_empty,
	EXPECT ( 0xbd, 0xe1, 0xb4, 0x6c, 0xdc, 0x54, 0x13, 0xb3, 0xd9, 0xf7,
		 0x35, 0xac, 0xdb, 0x80, 0xb1, 0x3c, 0x57, 0xbf, 0xe4, 0x73 ),
	EXPECT ( 0x72, 0x5a, 0x3c, 0x78, 0x20, 0xde, 0x1a, 0x06, 0xd0, 0x95,
		 0x81, 0x9c, 0xcf, 0x6f, 0x2c, 0x9b, 0x3a, 0x67, 0xf2, 0xce ),
	EXPECT ( 0xd1, 0xa9, 0xc1, 0xa2, 0x2c, 0x84, 0xfc, 0x23, 0xff, 0x22,
		 0x27, 0xef, 0x98, 0xec, 0x8b, 0xa9, 0xdf, 0x2a, 0x20, 0x9b,
		 0xa1, 0xdb, 0x09, 0x80, 0x9f, 0x57, 0xbf, 0xea, 0xe5, 0xb3,
		 0xe5, 0xf1, 0x46, 0xc7, 0x5f, 0x2d, 0x8d, 0xbb, 0x5e, 0x4a ) );

/**
 * Force a "reseed required" state
 *
 * @v state		HMAC_DRBG internal state
 */
static inline void force_reseed_required ( struct hmac_drbg_state *state ) {
	state->reseed_counter = ( HMAC_DRBG_RESEED_INTERVAL + 1 );
}

/**
 * Perform HMAC_DRBG self-test
 *
 */
static void hmac_drbg_test_exec ( void ) {
	struct hmac_drbg_state state;

	/*
	 * IMPORTANT NOTE
	 *
	 * The NIST test vector set includes several calls to
	 * HMAC_DRBG_Generate() that are expected to fail with a
	 * status of "Reseed required".  The pattern seems to be that
	 * when prediction resistance is requested, any call to
	 * HMAC_DRBG_Generate() is at first expected to fail.  After
	 * an explicit reseeding, the call to HMAC_DRBG_Generate() is
	 * retried, and on this second time it is expected to succeed.
	 *
	 * This pattern does not match the specifications for
	 * HMAC_DRBG_Generate(): neither HMAC_DRBG_Generate_algorithm
	 * (defined in ANS X9.82 Part 3-2007 Section 10.2.2.2.5 (NIST
	 * SP 800-90 Section 10.1.2.5)) nor the higher-level wrapper
	 * Generate_function defined in ANS X9.82 Part 3-2007 Section
	 * 9.4 (NIST SP 800-90 Section 9.3)) can possibly exhibit this
	 * behaviour:
	 *
	 * a) HMAC_DRBG_Generate_algorithm can return a "reseed
	 *    required" status only as a result of the test
	 *
	 *      "1. If reseed_counter > reseed_interval, then return
	 *       an indication that a reseed is required."
	 *
	 *    Since the reseed interval is independent of any request
	 *    for prediction resistance, and since the reseed interval
	 *    is not specified as part of the NIST test vector set,
	 *    then this cannot be the source of the "Reseed required"
	 *    failure expected by the NIST test vector set.
	 *
	 * b) Generate_function cannot return a "reseed required"
	 *    status under any circumstances.  If the underlying
	 *    HMAC_DRBG_Generate_algorithm call returns "reseed
	 *    required", then Generate_function will automatically
	 *    reseed and try again.
	 *
	 * To produce the behaviour expected by the NIST test vector
	 * set, we therefore contrive to produce a "reseed required"
	 * state where necessary by setting the reseed_counter to
	 * greater than the reseed_interval.
	 */

	/* Test 1 */
	instantiate_ok ( &state, &instantiate_1 );
	generate_ok ( &state, &generate_1_1 );
	generate_ok ( &state, &generate_1_2 );

	/* Test 2 */
	instantiate_ok ( &state, &instantiate_2 );
	generate_ok ( &state, &generate_2_1 );
	generate_ok ( &state, &generate_2_2 );

	/* Test 3 */
	instantiate_ok ( &state, &instantiate_3 );
	generate_ok ( &state, &generate_3_1 );
	generate_ok ( &state, &generate_3_2 );

	/* Test 4 */
	instantiate_ok ( &state, &instantiate_4 );
	generate_ok ( &state, &generate_4_1 );
	generate_ok ( &state, &generate_4_2 );

	/* Test 5 */
	instantiate_ok ( &state, &instantiate_5 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_5_1 );
	reseed_ok ( &state, &reseed_5_2 );
	generate_ok ( &state, &generate_5_3 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_5_4 );
	reseed_ok ( &state, &reseed_5_5 );
	generate_ok ( &state, &generate_5_6 );

	/* Test 6 */
	instantiate_ok ( &state, &instantiate_6 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_6_1 );
	reseed_ok ( &state, &reseed_6_2 );
	generate_ok ( &state, &generate_6_3 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_6_4 );
	reseed_ok ( &state, &reseed_6_5 );
	generate_ok ( &state, &generate_6_6 );

	/* Test 7 */
	instantiate_ok ( &state, &instantiate_7 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_7_1 );
	reseed_ok ( &state, &reseed_7_2 );
	generate_ok ( &state, &generate_7_3 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_7_4 );
	reseed_ok ( &state, &reseed_7_5 );
	generate_ok ( &state, &generate_7_6 );

	/* Test 8 */
	instantiate_ok ( &state, &instantiate_8 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_8_1 );
	reseed_ok ( &state, &reseed_8_2 );
	generate_ok ( &state, &generate_8_3 );
	force_reseed_required ( &state ); /* See above comments */
	generate_fail_ok ( &state, &generate_fail_8_4 );
	reseed_ok ( &state, &reseed_8_5 );
	generate_ok ( &state, &generate_8_6 );
}

/** HMAC_DRBG self-test */
struct self_test hmac_drbg_test __self_test = {
	.name = "hmac_drbg",
	.exec = hmac_drbg_test_exec,
};
