//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: String Tools
//
//===========================================================================//

// These are redefined in the project settings to prevent anyone from using them.
// We in this module are of a higher caste and thus are privileged in their use.
#ifdef strncpy
#undef strncpy
#endif

#ifdef _snprintf
#undef _snprintf
#endif

#if defined( sprintf )
#undef sprintf
#endif

#if defined( vsprintf )
#undef vsprintf
#endif

#ifdef _vsnprintf
#ifdef _WIN32
#undef _vsnprintf
#endif
#endif

#ifdef vsnprintf
#ifndef _WIN32
#undef vsnprintf
#endif
#endif

#if defined( strcat )
#undef strcat
#endif

#ifdef strncat
#undef strncat
#endif

// NOTE: I have to include stdio + stdarg first so vsnprintf gets compiled in
#include <stdio.h>
#include <stdarg.h>

#ifdef POSIX
#include <iconv.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#define _getcwd getcwd
#elif _WIN32
#include <direct.h>
#if !defined( _X360 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#endif

#ifdef _WIN32
#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
#endif
#include "strtools.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>


int V_vsnprintf(char* pDest, int maxLen, char const* pFormat, va_list params)
{
	//Assert(maxLen > 0);
	//AssertValidWritePtr(pDest, maxLen);
	//AssertValidStringPtr(pFormat);

	int len = _vsnprintf(pDest, maxLen, pFormat, params);

	// Len > maxLen represents an overflow on POSIX, < 0 is an overflow on windows
	if (len < 0 || len >= maxLen)
	{
		len = maxLen;
		pDest[maxLen - 1] = 0;
	}

	return len;
}

const char* V_strnchr(const char* pStr, char c, int n)
{
	char const* pLetter = pStr;
	char const* pLast = pStr + n;

	// Check the entire string
	while ((pLetter < pLast) && (*pLetter != 0))
	{
		if (*pLetter == c)
			return pLetter;
		++pLetter;
	}
	return NULL;
}

void V_strncpy(char* pDest, char const* pSrc, int maxLen)
{
	//Assert(maxLen >= sizeof(*pDest));
	//AssertValidWritePtr(pDest, maxLen);
	//AssertValidStringPtr(pSrc);

	strncpy(pDest, pSrc, maxLen);
	if (maxLen > 0)
	{
		pDest[maxLen - 1] = 0;
	}
}

static int FastToLower(char c)
{
	int i = (unsigned char)c;
	if (i < 0x80)
	{
		// Brutally fast branchless ASCII tolower():
		i += (((('A' - 1) - i) & (i - ('Z' + 1))) >> 26) & 0x20;
	}
	else
	{
		i += isupper(i) ? 0x20 : 0;
	}
	return i;
}

char const* V_strnistr(char const* pStr, char const* pSearch, int n)
{
	//AssertValidStringPtr(pStr);
	//AssertValidStringPtr(pSearch);

	if (!pStr || !pSearch)
		return 0;

	char const* pLetter = pStr;

	// Check the entire string
	while (*pLetter != 0)
	{
		if (n <= 0)
			return 0;

		// Skip over non-matches
		if (FastToLower(*pLetter) == FastToLower(*pSearch))
		{
			int n1 = n - 1;

			// Check for match
			char const* pMatch = pLetter + 1;
			char const* pTest = pSearch + 1;
			while (*pTest != 0)
			{
				if (n1 <= 0)
					return 0;

				// We've run off the end; don't bother.
				if (*pMatch == 0)
					return 0;

				if (FastToLower(*pMatch) != FastToLower(*pTest))
					break;

				++pMatch;
				++pTest;
				--n1;
			}

			// Found a match!
			if (*pTest == 0)
				return pLetter;
		}

		++pLetter;
		--n;
	}

	return 0;
}

int V_strncmp(const char* s1, const char* s2, int count)
{
	//Assert(count >= 0);
	//AssertValidStringPtr(s1, count);
	//AssertValidStringPtr(s2, count);

	while (count > 0)
	{
		if (*s1 != *s2)
			return (unsigned char)*s1 < (unsigned char)*s2 ? -1 : 1; // string different
		if (*s1 == '\0')
			return 0; // null terminator hit - strings the same
		s1++;
		s2++;
		count--;
	}

	return 0; // count characters compared the same
}

int _V_strcmp(const char* file, int line, const char* s1, const char* s2)
{
	//AssertValidStringPtr(s1);
	//AssertValidStringPtr(s2);

	return strcmp(s1, s2);
}

int	_V_strlen(const char* file, int line, const char* str)
{
	//AssertValidStringPtr(str);
	return strlen(str);
}