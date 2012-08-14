/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 * I don't know who is author of original code of this code. 
 * If you know author of original code of this code, send email to me.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include <stdio.h>
#include <string.h>

static char base64_table[] =
	{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
	};
static char base64_pad = '=';

/** This function is help function to know how much buffer is needed to 
 *	base64 encoding.
 *
 *	@param	length	length of plain string.
 *	@return	return buffer length to save base64 string. 
 */

int Base64GetEncodeLength( int length )
{
	return ((length + 3 - length % 3) * 4 / 3 + 1);
}

/** This function is help function to know how much buffer is needed to
 *	base64 decoding.
 *
 *	@param	length	length of base64 string
 *	@return	return buffer length to save plain string.
 */

int Base64GetDecodeLength( int length )
{
	return( (length / 4 * 3 + 1) * sizeof(char) );
}

/**	This function encode plain string to base64 string.
 *
 *	@param	string		[in] plain string
 *	@param	length		[in] length of plain string
 *	@param	result		[out] buffer for base64 string.
 *	@param	ret_length	[in] size of buffer to save base64 string.
 *	@return	if it success, it returns encoded character number.
 *			if szResult is NULL, it returns -1.
 *			if size of szResult is smaller than base64 string to save, it returns -1.
 */

int Base64Encode(const char * string, int length, char * szResult, int ret_length )
{
	const unsigned char *current = (const unsigned char *)string;
	unsigned char * result = (unsigned char *)szResult;
	int i = 0;

	if( result == NULL ) return -1;
	if( ret_length < Base64GetEncodeLength( length )) return -1;
	
	while (length > 2) 
	{ 
		result[i++] = base64_table[current[0] >> 2];
		result[i++] = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		result[i++] = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		result[i++] = base64_table[current[2] & 0x3f];

		current += 3;
		length -= 3; 
	}

	if (length != 0) 
	{
		result[i++] = base64_table[current[0] >> 2];
		if (length > 1) 
		{
			result[i++] = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			result[i++] = base64_table[(current[1] & 0x0f) << 2];
			result[i++] = base64_pad;
		}
		else 
		{
			result[i++] = base64_table[(current[0] & 0x03) << 4];
			result[i++] = base64_pad;
			result[i++] = base64_pad;
		}
	}

	result[i] = '\0';
	return i;
}

/**	This function decode base64 string to plain string.
 *
 *	@param	string		[in] base64 string
 *	@param	length		[in] length of base64 string.
 *	@param	result		[out] buffer to save plain string.
 *	@param	ret_length	[in] size of buffer to save plain string.
 *	@return	if it successes, it returns decoded character number.
 *			if result is NULL, it returns -1.
 *			if result buffer is smaller than decoded strng, it returns -1.
 */

int Base64Decode(const char *string, int length, char *szResult, int ret_length)
{
	const unsigned char *current = (const unsigned char *)string;
	unsigned char * result = (unsigned char *)szResult;
	int ch, i = 0, j = 0, k;
	char *chp;

	if( result == NULL ) return -1;
	if( ret_length < Base64GetDecodeLength(length) ) return -1;
	
	while ((ch = *current++) != '\0') 
	{
		if (ch == base64_pad) break;
		chp = strchr(base64_table, ch);
		if (chp == NULL) continue;
		ch = (int)(chp - base64_table);

		switch(i % 4) 
		{
		case 0:
			result[j] = ch << 2;
			break;
		case 1:
			result[j++] |= ch >> 4;
			result[j] = (ch & 0x0f) << 4;
			break;
		case 2:
			result[j++] |= ch >>2;
			result[j] = (ch & 0x03) << 6;
			break;
		case 3:
			result[j++] |= ch;
			break;
		}
		i++;
	}

	k = j;
	if (ch == base64_pad) 
	{
		switch(i % 4) 
		{
		case 0:
		case 1:
			return -1;
		case 2:
			k++;
		case 3:
			result[k++] = 0;
		}
	}

	result[k] = '\0';
	return j;
}
