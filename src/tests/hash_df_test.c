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
 * Hash-based derivation function (Hash_df) tests
 *
 * These test vectors are provided by NIST as part of the
 * Cryptographic Toolkit Examples, downloadable from:
 *
 *    http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/Hash_DRBG.pdf
 *
 */

/* Forcibly enable assertions */
#undef NDEBUG

#include <assert.h>
#include <string.h>
#include <ipxe/hash_df.h>
#include <ipxe/test.h>

/** Define inline input data */
#define INPUT(...) { __VA_ARGS__ }

/** Define inline expected data */
#define EXPECT(...) { __VA_ARGS__ }

/** A Hash_df test */
struct hash_df_test {
	/** Input data */
	const void *input;
	/** Length of input data */
	size_t input_len;
	/** Expected output data */
	const void *expected;
	/** Length of expected output data */
	size_t expected_len;
};

/**
 * Define a Hash_df test
 *
 * @v name		Test name
 * @v input_array	Input data
 * @v expected_array	Expected output data
 * @ret test		Hash_df test
 */
#define HASH_DF_TEST( name, input_array, expected_array )		\
	static const uint8_t name ## _input [] = input_array;		\
	static const uint8_t name ## _expected [] = expected_array;	\
	static const struct hash_df_test name = {			\
		.input = name ## _input,				\
		.input_len = sizeof ( name ## _input ),			\
		.expected = name ## _expected,				\
		.expected_len = sizeof ( name ## _expected ),		\
	}

/** Test 1 */
HASH_DF_TEST ( test_1,
	INPUT ( 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
		0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
		0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
		0x32, 0x33, 0x34, 0x35, 0x36, 0x20, 0x21, 0x22, 0x23, 0x24 ),
	EXPECT ( 0xd0, 0x8f, 0xb4, 0x41, 0xf2, 0xf4, 0xcb, 0x37, 0xcf, 0x6c,
		 0x24, 0x20, 0xa8, 0x2c, 0x74, 0x27, 0xac, 0xf7, 0xfc, 0xfd,
		 0x79, 0x90, 0x14, 0x38, 0x34, 0xa5, 0xc2, 0x56, 0xab, 0x28,
		 0x39, 0x36, 0x6d, 0x96, 0x34, 0x8c, 0xfe, 0x8c, 0x97, 0xab,
		 0x67, 0x67, 0xb0, 0x5e, 0x83, 0xa9, 0x80, 0x40, 0x6d, 0x94,
		 0xbe, 0xe3, 0x3c, 0xbb, 0x89 ) );

/** Test 2 */
HASH_DF_TEST ( test_2,
	INPUT ( 0x00, 0xd0, 0x8f, 0xb4, 0x41, 0xf2, 0xf4, 0xcb, 0x37, 0xcf,
		0x6c, 0x24, 0x20, 0xa8, 0x2c, 0x74, 0x27, 0xac, 0xf7, 0xfc,
		0xfd, 0x79, 0x90, 0x14, 0x38, 0x34, 0xa5, 0xc2, 0x56, 0xab,
		0x28, 0x39, 0x36, 0x6d, 0x96, 0x34, 0x8c, 0xfe, 0x8c, 0x97,
		0xab, 0x67, 0x67, 0xb0, 0x5e, 0x83, 0xa9, 0x80, 0x40, 0x6d,
		0x94, 0xbe, 0xe3, 0x3c, 0xbb, 0x89 ),
	EXPECT ( 0x54, 0xc5, 0x21, 0x7b, 0x51, 0x02, 0xd8, 0xda, 0x8b, 0xf1,
		 0x68, 0x6e, 0xdb, 0xab, 0x2b, 0xbc, 0x0c, 0x11, 0xb0, 0xcc,
		 0xb0, 0xf0, 0xaf, 0x23, 0x4c, 0x24, 0xcf, 0x15, 0xec, 0xc8,
		 0xcb, 0x39, 0xc2, 0x33, 0xaa, 0xca, 0x48, 0xfc, 0xce, 0xee,
		 0x86, 0x3d, 0xa8, 0x81, 0xff, 0xcb, 0xb4, 0x34, 0xa6, 0xcc,
		 0xb7, 0xda, 0x2f, 0xb2, 0x10 ) );

/** Test 3 */
HASH_DF_TEST ( test_3,
	INPUT ( 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
		0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
		0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
		0x32, 0x33, 0x34, 0x35, 0x36, 0x20, 0x21, 0x22, 0x23, 0x24,
		0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
		0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
		0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d,
		0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
		0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
		0x72, 0x73, 0x74, 0x75, 0x76 ),
	EXPECT ( 0x99, 0xb9, 0x53, 0x7b, 0x84, 0x27, 0xb8, 0xce, 0x23, 0x21,
		 0x9a, 0x61, 0x1c, 0xbe, 0x61, 0x06, 0x44, 0xcf, 0x85, 0x03,
		 0xee, 0xc5, 0xba, 0x22, 0xde, 0x1a, 0xb2, 0x12, 0xc3, 0xd0,
		 0x85, 0x8e, 0x9e, 0x3b, 0x90, 0x26, 0xd4, 0xe7, 0x7d, 0x58,
		 0xe0, 0x2e, 0x85, 0xa2, 0x31, 0x4c, 0xe3, 0xd7, 0x4a, 0x93,
		 0x32, 0x4b, 0x27, 0xbd, 0xe8 ) );

/** Test 4 */
HASH_DF_TEST ( test_4,
	INPUT ( 0x00, 0x99, 0xb9, 0x53, 0x7b, 0x84, 0x27, 0xb8, 0xce, 0x23,
		0x21, 0x9a, 0x61, 0x1c, 0xbe, 0x61, 0x06, 0x44, 0xcf, 0x85,
		0x03, 0xee, 0xc5, 0xba, 0x22, 0xde, 0x1a, 0xb2, 0x12, 0xc3,
		0xd0, 0x85, 0x8e, 0x9e, 0x3b, 0x90, 0x26, 0xd4, 0xe7, 0x7d,
		0x58, 0xe0, 0x2e, 0x85, 0xa2, 0x31, 0x4c, 0xe3, 0xd7, 0x4a,
		0x93, 0x32, 0x4b, 0x27, 0xbd, 0xe8 ),
	EXPECT ( 0xa7, 0x02, 0x66, 0xf7, 0xf9, 0x1e, 0xc4, 0xd2, 0x88, 0x73,
		 0x14, 0x79, 0x34, 0xce, 0xaf, 0x2a, 0x2c, 0xc3, 0x5a, 0x0f,
		 0xd5, 0xe0, 0x0a, 0xba, 0xe7, 0x9d, 0xc6, 0x60, 0x5f, 0xab,
		 0xd6, 0xf5, 0xf9, 0x28, 0xe1, 0x8c, 0x63, 0x26, 0x8e, 0x1a,
		 0xf4, 0x85, 0xda, 0x6c, 0xbf, 0x04, 0x16, 0xdc, 0xdc, 0x5f,
		 0xb8, 0xbc, 0x9c, 0x94, 0xb6 ) );

/** Test 5 */
HASH_DF_TEST ( test_5,
	INPUT ( 0x01, 0xd0, 0x8f, 0xb4, 0x41, 0xf2, 0xf4, 0xcb, 0x37, 0xcf,
		0x6c, 0x24, 0x20, 0xa8, 0x2c, 0x74, 0x27, 0xac, 0xf7, 0xfc,
		0xfd, 0x79, 0x90, 0x14, 0x38, 0x34, 0xa5, 0xc2, 0x56, 0xab,
		0x28, 0x39, 0x36, 0x6d, 0x96, 0x34, 0x8c, 0xfe, 0x8c, 0x97,
		0xab, 0x67, 0x67, 0xb0, 0x5e, 0x83, 0xa9, 0x80, 0x40, 0x6d,
		0x94, 0xbe, 0xe3, 0x3c, 0xbb, 0x89, 0x80, 0x81, 0x82, 0x83,
		0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d,
		0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1,
		0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
		0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
		0xb6 ),
	EXPECT ( 0x0a, 0x04, 0x41, 0xa5, 0x2b, 0xed, 0xf7, 0x94, 0xf5, 0xaa,
		 0x62, 0x7b, 0xcb, 0xd8, 0x1f, 0x93, 0xe0, 0x11, 0xd5, 0x1f,
		 0x34, 0x74, 0x80, 0x2c, 0x37, 0x50, 0x76, 0x75, 0x51, 0xb4,
		 0x5b, 0x69, 0xf3, 0xd3, 0x59, 0x39, 0xc9, 0x32, 0xae, 0x1c,
		 0xb7, 0xc9, 0x89, 0x4f, 0xb8, 0x84, 0x65, 0xe0, 0xcf, 0xd1,
		 0xcc, 0x26, 0x1e, 0x22, 0xc5 ) );

/** Test 6 */
HASH_DF_TEST ( test_6,
	INPUT ( 0x00, 0x0a, 0x04, 0x41, 0xa5, 0x2b, 0xed, 0xf7, 0x94, 0xf5,
		0xaa, 0x62, 0x7b, 0xcb, 0xd8, 0x1f, 0x93, 0xe0, 0x11, 0xd5,
		0x1f, 0x34, 0x74, 0x80, 0x2c, 0x37, 0x50, 0x76, 0x75, 0x51,
		0xb4, 0x5b, 0x69, 0xf3, 0xd3, 0x59, 0x39, 0xc9, 0x32, 0xae,
		0x1c, 0xb7, 0xc9, 0x89, 0x4f, 0xb8, 0x84, 0x65, 0xe0, 0xcf,
		0xd1, 0xcc, 0x26, 0x1e, 0x22, 0xc5 ),
	EXPECT ( 0x04, 0x11, 0xc8, 0xb0, 0xdb, 0xa7, 0x56, 0xe8, 0x84, 0x2b,
		 0x3f, 0xb0, 0x2d, 0x2f, 0xeb, 0x7c, 0xee, 0xa5, 0x67, 0x42,
		 0xee, 0x93, 0x79, 0xc9, 0x0e, 0x6d, 0x3b, 0x2f, 0x10, 0x10,
		 0xd4, 0x0f, 0x4f, 0x4d, 0xca, 0xda, 0x61, 0xcf, 0xdf, 0xb4,
		 0x8a, 0xf8, 0x47, 0xca, 0xcc, 0x4c, 0x92, 0xc6, 0x14, 0x44,
		 0x85, 0xc2, 0x27, 0xca, 0x05 ) );

/** Test 7 */
HASH_DF_TEST ( test_7,
	INPUT ( 0x01, 0x0e, 0x16, 0x0a, 0x56, 0x07, 0x95, 0x4e, 0x7d, 0x79,
		0xd5, 0xa2, 0x2b, 0xf9, 0x08, 0x0b, 0x10, 0xce, 0xb7, 0x3c,
		0x62, 0x23, 0x07, 0xf9, 0xf5, 0x45, 0xbd, 0xb1, 0xa4, 0x61,
		0xc5, 0x2f, 0x79, 0x43, 0x21, 0x24, 0x3a, 0xac, 0xe2, 0x3f,
		0x36, 0x3f, 0xef, 0xb3, 0x5d, 0xc5, 0xbe, 0xa7, 0xe7, 0x31,
		0x44, 0x14, 0xcf, 0x78, 0xb3, 0xf9, 0xc0, 0xc1, 0xc2, 0xc3,
		0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
		0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
		0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1,
		0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
		0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
		0xf6 ),
	EXPECT ( 0xdc, 0x24, 0xdf, 0x10, 0x2f, 0xa9, 0xf9, 0x6c, 0xc1, 0xcf,
		 0xf8, 0xc1, 0x16, 0xc7, 0x9d, 0x14, 0x97, 0xd7, 0xc2, 0x7b,
		 0xba, 0x5b, 0xa8, 0x01, 0xe1, 0x56, 0x21, 0x93, 0x35, 0x3f,
		 0x31, 0xe3, 0x22, 0x39, 0x57, 0x84, 0x69, 0xb8, 0x0f, 0x2f,
		 0x51, 0x64, 0x54, 0x37, 0x28, 0x71, 0x7f, 0x17, 0x1f, 0xdb,
		 0x02, 0xb2, 0xad, 0x57, 0x95 ) );

/** Test 8 */
HASH_DF_TEST ( test_8,
	INPUT ( 0x00, 0xdc, 0x24, 0xdf, 0x10, 0x2f, 0xa9, 0xf9, 0x6c, 0xc1,
		0xcf, 0xf8, 0xc1, 0x16, 0xc7, 0x9d, 0x14, 0x97, 0xd7, 0xc2,
		0x7b, 0xba, 0x5b, 0xa8, 0x01, 0xe1, 0x56, 0x21, 0x93, 0x35,
		0x3f, 0x31, 0xe3, 0x22, 0x39, 0x57, 0x84, 0x69, 0xb8, 0x0f,
		0x2f, 0x51, 0x64, 0x54, 0x37, 0x28, 0x71, 0x7f, 0x17, 0x1f,
		0xdb, 0x02, 0xb2, 0xad, 0x57, 0x95 ),
	EXPECT ( 0xff, 0xaf, 0x45, 0x66, 0x5b, 0x11, 0x0c, 0xa1, 0x33, 0x5a,
		 0x3f, 0xce, 0x73, 0xa7, 0x98, 0x1d, 0x0f, 0xd5, 0xc8, 0xd9,
		 0x03, 0xf6, 0x5f, 0xaa, 0x46, 0xa3, 0xd5, 0x97, 0xbf, 0x34,
		 0xc4, 0xe0, 0xcc, 0x16, 0x75, 0x60, 0xab, 0x94, 0xec, 0x10,
		 0xd6, 0x41, 0x5f, 0x37, 0x83, 0xb0, 0x15, 0x67, 0x89, 0x1b,
		 0x57, 0x66, 0x2a, 0xbb, 0x39 ) );

/** Test 9 */
HASH_DF_TEST ( test_9,
	INPUT ( 0x01, 0xd0, 0x8f, 0xb4, 0x41, 0xf2, 0xf4, 0xcb, 0x37, 0xcf,
		0x6c, 0x24, 0x20, 0xa8, 0x2c, 0x74, 0x27, 0xac, 0xf7, 0xfc,
		0xfd, 0x79, 0x90, 0x14, 0x38, 0x34, 0xa5, 0xc2, 0x56, 0xab,
		0x28, 0x39, 0x36, 0x6d, 0x96, 0x34, 0x8c, 0xfe, 0x8c, 0x97,
		0xab, 0x67, 0x67, 0xb0, 0x5e, 0x83, 0xa9, 0x80, 0x40, 0x6d,
		0x94, 0xbe, 0xe3, 0x3c, 0xbb, 0x89, 0x80, 0x81, 0x82, 0x83,
		0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d,
		0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1,
		0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
		0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
		0xb6, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72,
		0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
		0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86,
		0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
		0x91, 0x92, 0x93, 0x94, 0x95, 0x96 ),
	EXPECT ( 0x8f, 0xde, 0xc9, 0xe6, 0x18, 0x96, 0x36, 0xf0, 0xa5, 0xce,
		 0x53, 0xe8, 0x1c, 0x13, 0xac, 0x93, 0x84, 0xfa, 0xfb, 0xa0,
		 0xee, 0x50, 0xc1, 0xe2, 0xc8, 0xa0, 0x99, 0xde, 0x41, 0xd8,
		 0xcc, 0x7a, 0x31, 0x42, 0x9e, 0x8c, 0x8c, 0x88, 0x80, 0xe3,
		 0xb4, 0x5d, 0x89, 0xdb, 0x61, 0x2c, 0xd9, 0xd2, 0x8a, 0x55,
		 0xc0, 0xf0, 0xd1, 0xf8, 0xf9 ) );

/** Test 10 */
HASH_DF_TEST ( test_10,
	INPUT ( 0x00, 0x8f, 0xde, 0xc9, 0xe6, 0x18, 0x96, 0x36, 0xf0, 0xa5,
		0xce, 0x53, 0xe8, 0x1c, 0x13, 0xac, 0x93, 0x84, 0xfa, 0xfb,
		0xa0, 0xee, 0x50, 0xc1, 0xe2, 0xc8, 0xa0, 0x99, 0xde, 0x41,
		0xd8, 0xcc, 0x7a, 0x31, 0x42, 0x9e, 0x8c, 0x8c, 0x88, 0x80,
		0xe3, 0xb4, 0x5d, 0x89, 0xdb, 0x61, 0x2c, 0xd9, 0xd2, 0x8a,
		0x55, 0xc0, 0xf0, 0xd1, 0xf8, 0xf9 ),
	EXPECT ( 0x97, 0xd0, 0x76, 0x31, 0xb2, 0x2f, 0x7c, 0x95, 0x7f, 0x19,
		 0xf8, 0x44, 0xf4, 0xdc, 0x2a, 0xfa, 0x6f, 0xf9, 0x7c, 0x35,
		 0x66, 0x18, 0x98, 0x21, 0x69, 0x91, 0xd1, 0x5b, 0xda, 0x75,
		 0xbb, 0xd0, 0x5e, 0xdf, 0x8a, 0x0f, 0xa8, 0x0c, 0xca, 0xb9,
		 0x51, 0x95, 0xf4, 0x79, 0xcd, 0x76, 0x20, 0x22, 0x35, 0x10,
		 0x2e, 0xf6, 0x27, 0x29, 0x19 ) );

/** Test 11 */
HASH_DF_TEST ( test_11,
	INPUT ( 0x01, 0x27, 0xaf, 0x40, 0x17, 0xca, 0xc5, 0xb3, 0x86, 0x24,
		0xe8, 0x4c, 0x2d, 0x10, 0xef, 0xd7, 0x8d, 0xf4, 0xf4, 0x77,
		0xd6, 0x54, 0x69, 0x5a, 0x04, 0x32, 0x32, 0x6b, 0x3a, 0x1c,
		0x4e, 0x88, 0x4a, 0x90, 0x22, 0x28, 0xe8, 0x9e, 0xaa, 0x90,
		0x36, 0xcd, 0x2a, 0xf7, 0x05, 0x66, 0x81, 0x26, 0x23, 0x72,
		0xc7, 0x13, 0x71, 0xd4, 0x53, 0x3d, 0xc0, 0xc1, 0xc2, 0xc3,
		0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
		0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
		0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1,
		0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
		0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
		0xf6, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
		0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2,
		0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc,
		0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6,
		0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
		0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6 ),
	EXPECT ( 0x2c, 0x9c, 0x0d, 0x80, 0x03, 0xe3, 0x40, 0x23, 0xbe, 0x5b,
		 0x63, 0xfd, 0xb9, 0xd2, 0x24, 0xb4, 0x25, 0x0c, 0xc8, 0x15,
		 0x5b, 0xd1, 0xee, 0xd8, 0xe5, 0x5d, 0x91, 0x06, 0x2f, 0xdd,
		 0x27, 0x64, 0xb8, 0xae, 0xa9, 0xc8, 0x2f, 0x84, 0x7e, 0x09,
		 0xa3, 0xfe, 0xa1, 0xc7, 0x11, 0x7d, 0x6f, 0x7d, 0xd2, 0xef,
		 0x77, 0x7d, 0x7c, 0xf3, 0xeb ) );

/** Test 12 */
HASH_DF_TEST ( test_12,
	INPUT ( 0x00, 0x2c, 0x9c, 0x0d, 0x80, 0x03, 0xe3, 0x40, 0x23, 0xbe,
		0x5b, 0x63, 0xfd, 0xb9, 0xd2, 0x24, 0xb4, 0x25, 0x0c, 0xc8,
		0x15, 0x5b, 0xd1, 0xee, 0xd8, 0xe5, 0x5d, 0x91, 0x06, 0x2f,
		0xdd, 0x27, 0x64, 0xb8, 0xae, 0xa9, 0xc8, 0x2f, 0x84, 0x7e,
		0x09, 0xa3, 0xfe, 0xa1, 0xc7, 0x11, 0x7d, 0x6f, 0x7d, 0xd2,
		0xef, 0x77, 0x7d, 0x7c, 0xf3, 0xeb ),
	EXPECT ( 0x7e, 0x8a, 0xa4, 0x93, 0x42, 0x72, 0xf2, 0xa2, 0x8b, 0xbf,
		 0xd7, 0xaf, 0xcc, 0x88, 0xce, 0x1c, 0x80, 0x6a, 0x38, 0xea,
		 0x7b, 0x89, 0x45, 0xc8, 0xd1, 0xb6, 0xf1, 0x75, 0x03, 0x78,
		 0x54, 0x6a, 0xb1, 0xa2, 0x96, 0x00, 0xd6, 0x44, 0xec, 0x52,
		 0x0e, 0x8b, 0xff, 0xf6, 0x0c, 0xb7, 0x7f, 0xa5, 0x4b, 0xb1,
		 0x1a, 0x83, 0x31, 0xcb, 0x24 ) );

/** Test 13 */
HASH_DF_TEST ( test_13,
	INPUT ( 0x01, 0x99, 0xb9, 0x53, 0x7b, 0x84, 0x27, 0xb8, 0xce, 0x23,
		0x21, 0x9a, 0x61, 0x1c, 0xbe, 0x61, 0x06, 0x44, 0xcf, 0x85,
		0x03, 0xee, 0xc5, 0xba, 0x22, 0xde, 0x1a, 0xb2, 0x12, 0xc3,
		0xd0, 0x85, 0x8e, 0x9e, 0x3b, 0x90, 0x26, 0xd4, 0xe7, 0x7d,
		0x58, 0xe0, 0x2e, 0x85, 0xa2, 0x31, 0x4c, 0xe3, 0xd7, 0x4a,
		0x93, 0x32, 0x4b, 0x27, 0xbd, 0xe8, 0x80, 0x81, 0x82, 0x83,
		0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d,
		0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1,
		0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
		0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
		0xb6 ),
	EXPECT ( 0xe5, 0x04, 0x3d, 0x1b, 0x95, 0x4b, 0x34, 0xba, 0x60, 0xd2,
		 0x48, 0xe8, 0x83, 0xef, 0x49, 0x8c, 0x5c, 0x52, 0x36, 0xb8,
		 0x26, 0x0e, 0x23, 0x8e, 0x02, 0xc8, 0xd4, 0xfc, 0x5f, 0xfe,
		 0x90, 0xfa, 0x40, 0x13, 0x44, 0x70, 0x75, 0xbb, 0x54, 0x3e,
		 0xf0, 0x0c, 0x3b, 0xda, 0x59, 0x6b, 0x10, 0x88, 0x61, 0xf0,
		 0x6b, 0xf9, 0x1b, 0x45, 0xd6 ) );

/** Test 14 */
HASH_DF_TEST ( test_14,
	INPUT ( 0x00, 0xe5, 0x04, 0x3d, 0x1b, 0x95, 0x4b, 0x34, 0xba, 0x60,
		0xd2, 0x48, 0xe8, 0x83, 0xef, 0x49, 0x8c, 0x5c, 0x52, 0x36,
		0xb8, 0x26, 0x0e, 0x23, 0x8e, 0x02, 0xc8, 0xd4, 0xfc, 0x5f,
		0xfe, 0x90, 0xfa, 0x40, 0x13, 0x44, 0x70, 0x75, 0xbb, 0x54,
		0x3e, 0xf0, 0x0c, 0x3b, 0xda, 0x59, 0x6b, 0x10, 0x88, 0x61,
		0xf0, 0x6b, 0xf9, 0x1b, 0x45, 0xd6 ),
	EXPECT ( 0x1f, 0x3f, 0x63, 0x10, 0xed, 0x10, 0xfc, 0x9f, 0x93, 0x8c,
		 0x43, 0x22, 0x61, 0xaf, 0x42, 0xe9, 0xe9, 0x17, 0x5f, 0x08,
		 0x0f, 0x32, 0x22, 0xdc, 0x11, 0x8b, 0xa7, 0xcf, 0x88, 0x8c,
		 0xdc, 0x3f, 0x36, 0x0d, 0xd2, 0x8f, 0x5e, 0xcb, 0x7c, 0x80,
		 0xa6, 0xbc, 0xfc, 0xfc, 0x0f, 0x51, 0xfe, 0x2f, 0x77, 0xc1,
		 0xc9, 0x9d, 0xf0, 0xa2, 0x09 ) );

/** Test 15 */
HASH_DF_TEST ( test_15,
	INPUT ( 0x01, 0x04, 0x43, 0xa0, 0x2c, 0x82, 0x5c, 0x31, 0x59, 0xf4,
		0x5e, 0x8c, 0x0a, 0xe5, 0x9e, 0x8c, 0x76, 0x45, 0x69, 0x95,
		0xc0, 0x35, 0x40, 0x46, 0x6a, 0x14, 0x54, 0x7c, 0xcb, 0xe8,
		0x8b, 0x6d, 0x39, 0x76, 0x21, 0x17, 0x32, 0x84, 0x72, 0xf5,
		0x2b, 0x84, 0x57, 0x5a, 0xaf, 0xe8, 0x8b, 0x2d, 0x1e, 0x50,
		0x4f, 0x21, 0xec, 0x4e, 0x31, 0x35, 0xc0, 0xc1, 0xc2, 0xc3,
		0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
		0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
		0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1,
		0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
		0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
		0xf6 ),
	EXPECT ( 0x9d, 0xc3, 0x52, 0x08, 0xee, 0x2b, 0x8c, 0x58, 0x1e, 0xa3,
		 0x0b, 0xaa, 0xcb, 0x5d, 0x74, 0x31, 0x7a, 0x87, 0x94, 0x54,
		 0x10, 0x71, 0x7e, 0x58, 0xd3, 0x70, 0x5f, 0xbd, 0xc7, 0x60,
		 0xbe, 0x0c, 0xc9, 0x0e, 0xd1, 0xcc, 0xbb, 0x89, 0x7d, 0x47,
		 0xd2, 0x7e, 0x2b, 0x2e, 0x42, 0x2b, 0x32, 0xb9, 0x7f, 0x05,
		 0x0d, 0x1b, 0xd2, 0xb4, 0x90 ) );

/** Test 16 */
HASH_DF_TEST ( test_16,
	INPUT ( 0x00, 0x9d, 0xc3, 0x52, 0x08, 0xee, 0x2b, 0x8c, 0x58, 0x1e,
		0xa3, 0x0b, 0xaa, 0xcb, 0x5d, 0x74, 0x31, 0x7a, 0x87, 0x94,
		0x54, 0x10, 0x71, 0x7e, 0x58, 0xd3, 0x70, 0x5f, 0xbd, 0xc7,
		0x60, 0xbe, 0x0c, 0xc9, 0x0e, 0xd1, 0xcc, 0xbb, 0x89, 0x7d,
		0x47, 0xd2, 0x7e, 0x2b, 0x2e, 0x42, 0x2b, 0x32, 0xb9, 0x7f,
		0x05, 0x0d, 0x1b, 0xd2, 0xb4, 0x90 ),
	EXPECT ( 0x1a, 0x5a, 0xd6, 0xce, 0xa3, 0xd1, 0x5d, 0xa5, 0xfb, 0x47,
		 0x42, 0x13, 0x13, 0x09, 0xf0, 0xed, 0x88, 0xcf, 0x4c, 0x90,
		 0xa6, 0xc1, 0xcc, 0xee, 0x35, 0xa8, 0x76, 0xeb, 0xfc, 0xcc,
		 0x82, 0x67, 0x29, 0xb6, 0x63, 0x9f, 0x81, 0x19, 0x65, 0xb0,
		 0xef, 0x85, 0x76, 0xe7, 0x5c, 0xb3, 0xcf, 0xe8, 0x22, 0x07,
		 0x68, 0xb2, 0x6c, 0xe7, 0x7a ) );

/** Test 17 */
HASH_DF_TEST ( test_17,
	INPUT ( 0x01, 0x99, 0xb9, 0x53, 0x7b, 0x84, 0x27, 0xb8, 0xce, 0x23,
		0x21, 0x9a, 0x61, 0x1c, 0xbe, 0x61, 0x06, 0x44, 0xcf, 0x85,
		0x03, 0xee, 0xc5, 0xba, 0x22, 0xde, 0x1a, 0xb2, 0x12, 0xc3,
		0xd0, 0x85, 0x8e, 0x9e, 0x3b, 0x90, 0x26, 0xd4, 0xe7, 0x7d,
		0x58, 0xe0, 0x2e, 0x85, 0xa2, 0x31, 0x4c, 0xe3, 0xd7, 0x4a,
		0x93, 0x32, 0x4b, 0x27, 0xbd, 0xe8, 0x80, 0x81, 0x82, 0x83,
		0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d,
		0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1,
		0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
		0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5,
		0xb6, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72,
		0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
		0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86,
		0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
		0x91, 0x92, 0x93, 0x94, 0x95, 0x96 ),
	EXPECT ( 0x56, 0x3a, 0x5d, 0x20, 0x7d, 0x37, 0x70, 0x7b, 0xf5, 0xf2,
		 0x4d, 0x0b, 0xd4, 0x93, 0x5d, 0xc3, 0x8d, 0xbe, 0x04, 0x36,
		 0x37, 0xb3, 0xff, 0x8a, 0xb6, 0x8c, 0xfc, 0xe2, 0xf2, 0x90,
		 0xd1, 0x69, 0x95, 0x20, 0x55, 0x24, 0x19, 0x0f, 0xd2, 0x91,
		 0xaa, 0x8a, 0x6e, 0x6b, 0x8e, 0x6d, 0x56, 0xa4, 0x31, 0x33,
		 0x3b, 0x40, 0x8e, 0x6f, 0xa8 ) );

/** Test 18 */
HASH_DF_TEST ( test_18,
	INPUT ( 0x00, 0x56, 0x3a, 0x5d, 0x20, 0x7d, 0x37, 0x70, 0x7b, 0xf5,
		0xf2, 0x4d, 0x0b, 0xd4, 0x93, 0x5d, 0xc3, 0x8d, 0xbe, 0x04,
		0x36, 0x37, 0xb3, 0xff, 0x8a, 0xb6, 0x8c, 0xfc, 0xe2, 0xf2,
		0x90, 0xd1, 0x69, 0x95, 0x20, 0x55, 0x24, 0x19, 0x0f, 0xd2,
		0x91, 0xaa, 0x8a, 0x6e, 0x6b, 0x8e, 0x6d, 0x56, 0xa4, 0x31,
		0x33, 0x3b, 0x40, 0x8e, 0x6f, 0xa8 ),
	EXPECT ( 0xc5, 0xd3, 0xe9, 0x55, 0x1e, 0x00, 0xe4, 0xee, 0x32, 0xb2,
		 0x11, 0x6f, 0xaf, 0x4d, 0xef, 0xf4, 0xd4, 0xcf, 0xad, 0x2b,
		 0xdc, 0x2d, 0xba, 0xa2, 0xe0, 0xe7, 0xf9, 0xdd, 0xb9, 0xd8,
		 0x1e, 0xed, 0x45, 0xe0, 0xa5, 0x0d, 0xa5, 0xaf, 0xd5, 0xc1,
		 0xf6, 0xbc, 0xda, 0xf8, 0x1d, 0x28, 0x9c, 0xf4, 0xbd, 0x3c,
		 0x91, 0xb7, 0x00, 0x5c, 0x18 ) );

/** Test 19 */
HASH_DF_TEST ( test_19,
	INPUT ( 0x01, 0x1c, 0x0e, 0x46, 0x75, 0x9b, 0x38, 0x55, 0x6a, 0x28,
		0xa4, 0x5e, 0x7b, 0x83, 0xe1, 0x4d, 0xb8, 0x62, 0x8d, 0xb1,
		0x62, 0x13, 0xe1, 0xba, 0x2d, 0x97, 0x74, 0xf6, 0xc0, 0xac,
		0x68, 0xf0, 0x56, 0xdb, 0x00, 0xfb, 0x12, 0xe1, 0x5b, 0xf4,
		0xde, 0x95, 0x50, 0xb7, 0x33, 0x1e, 0x2d, 0xbd, 0x66, 0x4c,
		0x3a, 0xb7, 0x76, 0xe8, 0x25, 0x51, 0xc0, 0xc1, 0xc2, 0xc3,
		0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
		0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
		0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1,
		0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
		0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
		0xf6, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
		0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2,
		0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc,
		0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6,
		0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
		0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6 ),
	EXPECT ( 0x60, 0x01, 0x93, 0xc8, 0xf6, 0x03, 0x1a, 0x2d, 0x49, 0x37,
		 0x2a, 0x8b, 0x0f, 0x60, 0xf6, 0x8c, 0x1d, 0xfd, 0xac, 0xd4,
		 0xf8, 0xea, 0x01, 0x37, 0x47, 0xd7, 0x14, 0x82, 0x33, 0x3d,
		 0xf5, 0x25, 0x2e, 0x95, 0xb8, 0x22, 0x57, 0x39, 0x1b, 0xf1,
		 0x0a, 0xb0, 0x7d, 0x12, 0x08, 0xb6, 0xbd, 0x66, 0x5b, 0x30,
		 0x0a, 0xa4, 0xdb, 0x9c, 0x3e ) );

/** Test 20 */
HASH_DF_TEST ( test_20,
	INPUT ( 0x00, 0x60, 0x01, 0x93, 0xc8, 0xf6, 0x03, 0x1a, 0x2d, 0x49,
		0x37, 0x2a, 0x8b, 0x0f, 0x60, 0xf6, 0x8c, 0x1d, 0xfd, 0xac,
		0xd4, 0xf8, 0xea, 0x01, 0x37, 0x47, 0xd7, 0x14, 0x82, 0x33,
		0x3d, 0xf5, 0x25, 0x2e, 0x95, 0xb8, 0x22, 0x57, 0x39, 0x1b,
		0xf1, 0x0a, 0xb0, 0x7d, 0x12, 0x08, 0xb6, 0xbd, 0x66, 0x5b,
		0x30, 0x0a, 0xa4, 0xdb, 0x9c, 0x3e ),
	EXPECT ( 0x6b, 0x71, 0x82, 0x3b, 0x18, 0x20, 0x07, 0x71, 0xca, 0xae,
		 0x5d, 0x12, 0x55, 0xc1, 0x40, 0x3e, 0xdf, 0xe3, 0x8b, 0x4d,
		 0x18, 0xc7, 0x87, 0xbb, 0x44, 0xcd, 0x17, 0x18, 0x61, 0x52,
		 0xef, 0xea, 0xd6, 0xfd, 0xc4, 0xb8, 0x94, 0xf9, 0x20, 0x02,
		 0xc0, 0x72, 0x09, 0x55, 0x5d, 0x7e, 0x35, 0x54, 0xf9, 0xd1,
		 0x2f, 0xc5, 0x59, 0x7f, 0x22 ) );

/**
 * Report Hash_df test result
 *
 * @v test		Hash_df test
 */
#define hash_df_ok( test ) do {						\
	uint8_t output[ (test)->expected_len ];				\
	hash_df ( (test)->input, (test)->input_len, output,		\
		  sizeof ( output ) );					\
	ok ( memcmp ( (test)->expected, output,				\
		      sizeof ( output ) ) == 0 );			\
	} while ( 0 )

/**
 * Perform Hash_df self-test
 *
 */
static void hash_df_test_exec ( void ) {

	hash_df_ok ( &test_1 );
	hash_df_ok ( &test_2 );
	hash_df_ok ( &test_3 );
	hash_df_ok ( &test_4 );
	hash_df_ok ( &test_5 );
	hash_df_ok ( &test_6 );
	hash_df_ok ( &test_7 );
	hash_df_ok ( &test_8 );
	hash_df_ok ( &test_9 );
	hash_df_ok ( &test_10 );
	hash_df_ok ( &test_11 );
	hash_df_ok ( &test_12 );
	hash_df_ok ( &test_13 );
	hash_df_ok ( &test_14 );
	hash_df_ok ( &test_15 );
	hash_df_ok ( &test_16 );
	hash_df_ok ( &test_17 );
	hash_df_ok ( &test_18 );
	hash_df_ok ( &test_19 );
	hash_df_ok ( &test_20 );
}

/** Hash_df self-test */
struct self_test hash_df_test __self_test = {
	.name = "hash_df",
	.exec = hash_df_test_exec,
};
