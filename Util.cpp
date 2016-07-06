#include "Util.h"
#include <stdio.h>
#include <fstream>

#include <iostream>

#include "Log.h"

NSP_UTIL_BEG
	NSP_MD5_BEG
		static void md5_process(md5_state_t *pms, const unsigned char *data /*[64]*/)
		{
			unsigned int
				a = pms->abcd[0], b = pms->abcd[1],
				c = pms->abcd[2], d = pms->abcd[3];
			unsigned int t;
		#if BYTE_ORDER > 0
			/* Define storage only for big-endian CPUs. */
			unsigned int X[16];
		#else
			/* Define storage for little-endian or both types of CPUs. */
			unsigned int xbuf[16];
			const unsigned int *X;
		#endif

			{
		#if BYTE_ORDER == 0
				/*
				* Determine dynamically whether this is a big-endian or
				* little-endian machine, since we can use a more efficient
				* algorithm on the latter.
				*/
				static const int w = 1;

				if (*((const unsigned char *)&w)) /* dynamic little-endian */
		#endif
		#if BYTE_ORDER <= 0		/* little-endian */
				{
					/*
					* On little-endian machines, we can process properly aligned
					* data without copying it.
					*/
					if (!((data - (const unsigned char *)0) & 3)) {
						/* data are properly aligned */
						X = (const unsigned int *)data;
					}
					else {
						/* not aligned */
						memcpy(xbuf, data, 64);
						X = xbuf;
					}
				}
		#endif
		#if BYTE_ORDER == 0
				else			/* dynamic big-endian */
		#endif
		#if BYTE_ORDER >= 0		/* big-endian */
				{
					/*
					* On big-endian machines, we must arrange the bytes in the
					* right order.
					*/
					const unsigned char *xp = data;
					int i;

		#  if BYTE_ORDER == 0
					X = xbuf;		/* (dynamic only) */
		#  else
		#    define xbuf X		/* (static only) */
		#  endif
					for (i = 0; i < 16; ++i, xp += 4)
						xbuf[i] = xp[0] + (xp[1] << 8) + (xp[2] << 16) + (xp[3] << 24);
				}
		#endif
			}

		#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

			/* Round 1. */
			/* Let [abcd k s i] denote the operation
			a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s). */
		#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
		#define SET(a, b, c, d, k, s, Ti)\
			  t = a + F(b,c,d) + X[k] + Ti;\
			  a = ROTATE_LEFT(t, s) + b
			/* Do the following 16 operations. */
			SET(a, b, c, d, 0, 7, T1);
			SET(d, a, b, c, 1, 12, T2);
			SET(c, d, a, b, 2, 17, T3);
			SET(b, c, d, a, 3, 22, T4);
			SET(a, b, c, d, 4, 7, T5);
			SET(d, a, b, c, 5, 12, T6);
			SET(c, d, a, b, 6, 17, T7);
			SET(b, c, d, a, 7, 22, T8);
			SET(a, b, c, d, 8, 7, T9);
			SET(d, a, b, c, 9, 12, T10);
			SET(c, d, a, b, 10, 17, T11);
			SET(b, c, d, a, 11, 22, T12);
			SET(a, b, c, d, 12, 7, T13);
			SET(d, a, b, c, 13, 12, T14);
			SET(c, d, a, b, 14, 17, T15);
			SET(b, c, d, a, 15, 22, T16);
		#undef SET

			/* Round 2. */
			/* Let [abcd k s i] denote the operation
			a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s). */
		#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
		#define SET(a, b, c, d, k, s, Ti)\
			  t = a + G(b,c,d) + X[k] + Ti;\
			  a = ROTATE_LEFT(t, s) + b
			/* Do the following 16 operations. */
			SET(a, b, c, d, 1, 5, T17);
			SET(d, a, b, c, 6, 9, T18);
			SET(c, d, a, b, 11, 14, T19);
			SET(b, c, d, a, 0, 20, T20);
			SET(a, b, c, d, 5, 5, T21);
			SET(d, a, b, c, 10, 9, T22);
			SET(c, d, a, b, 15, 14, T23);
			SET(b, c, d, a, 4, 20, T24);
			SET(a, b, c, d, 9, 5, T25);
			SET(d, a, b, c, 14, 9, T26);
			SET(c, d, a, b, 3, 14, T27);
			SET(b, c, d, a, 8, 20, T28);
			SET(a, b, c, d, 13, 5, T29);
			SET(d, a, b, c, 2, 9, T30);
			SET(c, d, a, b, 7, 14, T31);
			SET(b, c, d, a, 12, 20, T32);
		#undef SET

			/* Round 3. */
			/* Let [abcd k s t] denote the operation
			a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s). */
		#define H(x, y, z) ((x) ^ (y) ^ (z))
		#define SET(a, b, c, d, k, s, Ti)\
			  t = a + H(b,c,d) + X[k] + Ti;\
			  a = ROTATE_LEFT(t, s) + b
			/* Do the following 16 operations. */
			SET(a, b, c, d, 5, 4, T33);
			SET(d, a, b, c, 8, 11, T34);
			SET(c, d, a, b, 11, 16, T35);
			SET(b, c, d, a, 14, 23, T36);
			SET(a, b, c, d, 1, 4, T37);
			SET(d, a, b, c, 4, 11, T38);
			SET(c, d, a, b, 7, 16, T39);
			SET(b, c, d, a, 10, 23, T40);
			SET(a, b, c, d, 13, 4, T41);
			SET(d, a, b, c, 0, 11, T42);
			SET(c, d, a, b, 3, 16, T43);
			SET(b, c, d, a, 6, 23, T44);
			SET(a, b, c, d, 9, 4, T45);
			SET(d, a, b, c, 12, 11, T46);
			SET(c, d, a, b, 15, 16, T47);
			SET(b, c, d, a, 2, 23, T48);
		#undef SET

			/* Round 4. */
			/* Let [abcd k s t] denote the operation
			a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s). */
		#define I(x, y, z) ((y) ^ ((x) | ~(z)))
		#define SET(a, b, c, d, k, s, Ti)\
			  t = a + I(b,c,d) + X[k] + Ti;\
			  a = ROTATE_LEFT(t, s) + b
			/* Do the following 16 operations. */
			SET(a, b, c, d, 0, 6, T49);
			SET(d, a, b, c, 7, 10, T50);
			SET(c, d, a, b, 14, 15, T51);
			SET(b, c, d, a, 5, 21, T52);
			SET(a, b, c, d, 12, 6, T53);
			SET(d, a, b, c, 3, 10, T54);
			SET(c, d, a, b, 10, 15, T55);
			SET(b, c, d, a, 1, 21, T56);
			SET(a, b, c, d, 8, 6, T57);
			SET(d, a, b, c, 15, 10, T58);
			SET(c, d, a, b, 6, 15, T59);
			SET(b, c, d, a, 13, 21, T60);
			SET(a, b, c, d, 4, 6, T61);
			SET(d, a, b, c, 11, 10, T62);
			SET(c, d, a, b, 2, 15, T63);
			SET(b, c, d, a, 9, 21, T64);
		#undef SET

			/* Then perform the following additions. (That is increment each
			of the four registers by the value it had before this block
			was started.) */
			pms->abcd[0] += a;
			pms->abcd[1] += b;
			pms->abcd[2] += c;
			pms->abcd[3] += d;
		}

		void
		md5_init(md5_state_t *pms)
		{
			pms->count[0] = pms->count[1] = 0;
			pms->abcd[0] = 0x67452301;
			pms->abcd[1] = /*0xefcdab89*/ T_MASK ^ 0x10325476;
			pms->abcd[2] = /*0x98badcfe*/ T_MASK ^ 0x67452301;
			pms->abcd[3] = 0x10325476;
		}

		void
		md5_append(md5_state_t *pms, const unsigned char *data, int nbytes)
		{
			const unsigned char *p = data;
			int left = nbytes;
			int offset = (pms->count[0] >> 3) & 63;
			unsigned int nbits = (unsigned int)(nbytes << 3);

			if (nbytes <= 0)
				return;

			/* Update the message length. */
			pms->count[1] += nbytes >> 29;
			pms->count[0] += nbits;
			if (pms->count[0] < nbits)
				pms->count[1]++;

			/* Process an initial partial block. */
			if (offset) {
				int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);

				memcpy(pms->buf + offset, p, copy);
				if (offset + copy < 64)
					return;
				p += copy;
				left -= copy;
				md5_process(pms, pms->buf);
			}

			/* Process full blocks. */
			for (; left >= 64; p += 64, left -= 64)
				md5_process(pms, p);

			/* Process a final partial block. */
			if (left)
				memcpy(pms->buf, p, left);
		}

		void
		md5_finish(md5_state_t *pms, unsigned char digest[16])
		{
			static const unsigned char pad[64] = {
				0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};
			unsigned char data[8];
			int i;

			/* Save the length before padding. */
			for (i = 0; i < 8; ++i)
				data[i] = (unsigned char)(pms->count[i >> 2] >> ((i & 3) << 3));
			/* Pad to 56 bytes mod 64. */
			md5_append(pms, pad, ((55 - (pms->count[0] >> 3)) & 63) + 1);
			/* Append the length. */
			md5_append(pms, data, 8);
			for (i = 0; i < 16; ++i)
				digest[i] = (unsigned char)(pms->abcd[i >> 2] >> ((i & 3) << 3));
		}
	};
	NSP_IO_BEG

		bool strsep(vector<string> &vec, string &s)
		{
			size_t begin = 0, end = 0;
			vec.clear();
			while (begin < s.length())
			{
				end = begin + (s.substr(begin)).find_first_of(' ');
				if ((end >= begin) && (end != 0))
				{
					vec.push_back(s.substr(begin, end - begin));
				}
				else
				{
					vec.push_back(s.substr(begin));
					break;
				}
				begin = end + 1;
			}
			return true;
		}
		bool strsep(vector<string> &vec, string &s, char c)
		{
			unsigned int begin = 0;
			unsigned int end = 0;
			vec.clear();
			while (begin < s.size())
			{
				if (s.substr(begin).find_first_of(c) <= s.substr(begin).size())
					end = begin + (s.substr(begin).find_first_of(c));
				if ((end >= begin) && (end != 0))
				{
					vec.push_back(s.substr(begin, end - begin));
				}
				else
				{
					vec.push_back(s.substr(begin));
					break;
				}
				begin = end + 1;
			}
			return true;
		}

		//Speichern
		bool save(vector<string>  &vec, string filename)
		{
			unsigned int i = 0;
			ofstream Save(filename.c_str());
			while (Save.good() && (i < vec.size()));
			{
				Save << vec[i++] << "\n";
			}
			Save.close();
			vec.clear();
			return true;
			//Fehler abfangen???
		}
		bool save(vector<unsigned char>&vec, string filename)
		{
			char * buffer;
			ofstream Save(filename.c_str(), ios::binary | ios::trunc);
			if (Save.good())
			{
				buffer = new char[vec.size()]; //char Pointer auf die Länge der Datei einstellen
											   //vec nach buffer kopieren
				for (unsigned int i = 0; i < vec.size(); i++)
				{
					buffer[i] = vec[i];
				}
				Save.write(buffer, vec.size()); //Datei schreiben
				delete[] buffer; //Speicherplatz freigeben
				vec.clear();
			}
			if (Save.good())
			{
				Save.close();
				return true;
			}
			else
			{
				LOG << "IO Thread " << "Fehler beim Schreiben der Datei " + filename << "\n";
				Save.close();
				return false;
			}
		}
	};
};