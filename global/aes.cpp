#include ".\aes.hpp"

namespace OKps::aes
{
    const unsigned char string_device::Sbox[256] =
    {
        /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, /*0*/
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, /*1*/
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, /*2*/
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, /*3*/
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, /*4*/
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, /*5*/
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, /*6*/
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, /*7*/
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, /*8*/
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, /*9*/
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, /*a*/
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, /*b*/
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, /*c*/
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, /*d*/
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, /*e*/
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  /*f*/
    };

    const unsigned char string_device::InvSbox[256] =
    {
        /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, /*0*/
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, /*1*/
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, /*2*/
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, /*3*/
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, /*4*/
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, /*5*/
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, /*6*/
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, /*7*/
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, /*8*/
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, /*9*/
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, /*a*/
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, /*b*/
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, /*c*/
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, /*d*/
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, /*e*/
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d  /*f*/
    };

    const std::string string_device::key_of_keys()
    {
        std::string result;

        result.push_back(static_cast<char>(17));
        result.push_back(static_cast<char>(39));
        result.push_back(static_cast<char>(58));
        result.push_back(static_cast<char>(122));
        result.push_back(static_cast<char>(45));
        result.push_back(static_cast<char>(62));
        result.push_back(static_cast<char>(79));
        result.push_back(static_cast<char>(238));
        result.push_back(static_cast<char>(152));
        result.push_back(static_cast<char>(232));
        result.push_back(static_cast<char>(176));
        result.push_back(static_cast<char>(73));
        result.push_back(static_cast<char>(96));
        result.push_back(static_cast<char>(45));
        result.push_back(static_cast<char>(196));
        result.push_back(static_cast<char>(87));

        return result;
    }
    string_device::string_device(string_device const & origin)
        :MEMBER_key(origin.MEMBER_key)
    {
        unsigned char buffer[16];

        for (int count = 0; count < 16; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
    }
    void string_device::operator=(string_device const & origin)
    {
        this->MEMBER_key = origin.MEMBER_key;

        unsigned char buffer[16];

        for (int count = 0; count < 16; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        this->KeyExpansion(buffer);

    }

    string_device::string_device(const unsigned char * key) noexcept
    {
        for (auto count = 0;count < 16;count++)
        {
            this->MEMBER_key[count] = key[count];
        }
        this->KeyExpansion(key);
    }

    string_device::string_device(const std::string & key)
    {
        if (key.size() != 16)
        {
            throw std::invalid_argument("密钥长度必须为16");
        }
        for (auto count = 0;count < 16;count++)
        {
            this->MEMBER_key[count] = key[count];
        }
        this->KeyExpansion(reinterpret_cast<const unsigned char *>(key.c_str()));
    }
    string_device::string_device(const string_device::TYPE_key & key)
        :MEMBER_key(key)
    {
        unsigned char buffer[16];

        for (int count = 0; count < 16; count++)
        {
            buffer[count] = key[count];
        }
        this->KeyExpansion(buffer);
    }

    string_device::~string_device()
    {
    }

    void string_device::Cipher(unsigned char * const input)
    {
        unsigned char state[4][4];

        // 算法中很多4×4的循环，展开写并不复杂，却能够大幅度提高运行速度

        state[0][0] = input[0];
        state[0][1] = input[4];
        state[0][2] = input[8];
        state[0][3] = input[12];

        state[1][0] = input[1];
        state[1][1] = input[5];
        state[1][2] = input[9];
        state[1][3] = input[13];

        state[2][0] = input[2];
        state[2][1] = input[6];
        state[2][2] = input[10];
        state[2][3] = input[14];

        state[3][0] = input[3];
        state[3][1] = input[7];
        state[3][2] = input[11];
        state[3][3] = input[15];

        this->AddRoundKey(state, this->w[0]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[1]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[2]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[3]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[4]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[5]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[6]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[7]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[8]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, this->w[9]);

        this->SubBytes(state);
        this->ShiftRows(state);
        this->AddRoundKey(state, this->w[10]);

        input[0] = state[0][0];
        input[4] = state[0][1];
        input[8] = state[0][2];
        input[12] = state[0][3];

        input[1] = state[1][0];
        input[5] = state[1][1];
        input[9] = state[1][2];
        input[13] = state[1][3];

        input[2] = state[2][0];
        input[6] = state[2][1];
        input[10] = state[2][2];
        input[14] = state[2][3];

        input[3] = state[3][0];
        input[7] = state[3][1];
        input[11] = state[3][2];
        input[15] = state[3][3];
    }

    void string_device::InvCipher(unsigned char * const input)
    {
        unsigned char state[4][4];

        state[0][0] = input[0];
        state[0][1] = input[4];
        state[0][2] = input[8];
        state[0][3] = input[12];

        state[1][0] = input[1];
        state[1][1] = input[5];
        state[1][2] = input[9];
        state[1][3] = input[13];

        state[2][0] = input[2];
        state[2][1] = input[6];
        state[2][2] = input[10];
        state[2][3] = input[14];

        state[3][0] = input[3];
        state[3][1] = input[7];
        state[3][2] = input[11];
        state[3][3] = input[15];

        this->AddRoundKey(state, this->w[10]);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[9]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[8]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[7]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[6]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[5]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[4]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[3]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[2]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[1]);
        this->InvMixColumns(state);

        this->InvShiftRows(state);
        this->InvSubBytes(state);
        this->AddRoundKey(state, this->w[0]);

        input[0] = state[0][0];
        input[4] = state[0][1];
        input[8] = state[0][2];
        input[12] = state[0][3];

        input[1] = state[1][0];
        input[5] = state[1][1];
        input[9] = state[1][2];
        input[13] = state[1][3];

        input[2] = state[2][0];
        input[6] = state[2][1];
        input[10] = state[2][2];
        input[14] = state[2][3];

        input[3] = state[3][0];
        input[7] = state[3][1];
        input[11] = state[3][2];
        input[15] = state[3][3];
    }

    /*
    void* OKps::string_device::Cipher(void* input, int length)
    {
        unsigned char* in = (unsigned char*)input;
        int i;
        if (not length)
        {
            while (*(in + length++));
            in = (unsigned char*)input;
        }
        for (i = 0; i < length; i += 16)
        {
            Cipher(in + i);
        }
        return input;
    }

    void* OKps::string_device::InvCipher(void* input, int length)
    {
        unsigned char* in = (unsigned char*)input;
        int i;
        for (i = 0; i < length; i += 16)
        {
            InvCipher(in + i);
        }
        return input;
    }
    */

    void string_device::INNER_KeyExpansion(int const i, unsigned char const rc[]) noexcept
    {
        unsigned char t[4];

        t[0] = 0 ? this->w[i][0][0 - 1] : this->w[i - 1][0][3];
        t[1] = 0 ? this->w[i][1][0 - 1] : this->w[i - 1][1][3];
        t[2] = 0 ? this->w[i][2][0 - 1] : this->w[i - 1][2][3];
        t[3] = 0 ? this->w[i][3][0 - 1] : this->w[i - 1][3][3];

        unsigned char temp = t[0];

        t[0] = string_device::Sbox[t[(1) % 4]];
        t[1] = string_device::Sbox[t[(2) % 4]];
        t[2] = string_device::Sbox[t[(3) % 4]];
        t[3] = string_device::Sbox[t[(4) % 4]];

        t[3] = string_device::Sbox[temp];
        t[0] ^= rc[i - 1];

        this->w[i][0][0] = this->w[i - 1][0][0] ^ t[0];
        this->w[i][1][0] = this->w[i - 1][1][0] ^ t[1];
        this->w[i][2][0] = this->w[i - 1][2][0] ^ t[2];
        this->w[i][3][0] = this->w[i - 1][3][0] ^ t[3];

        t[0] = 1 ? this->w[i][0][0] : this->w[i - 1][0][3];
        t[1] = 1 ? this->w[i][1][0] : this->w[i - 1][1][3];
        t[2] = 1 ? this->w[i][2][0] : this->w[i - 1][2][3];
        t[3] = 1 ? this->w[i][3][0] : this->w[i - 1][3][3];

        this->w[i][0][1] = this->w[i - 1][0][1] ^ t[0];
        this->w[i][1][1] = this->w[i - 1][1][1] ^ t[1];
        this->w[i][2][1] = this->w[i - 1][2][1] ^ t[2];
        this->w[i][3][1] = this->w[i - 1][3][1] ^ t[3];

        t[0] = 2 ? w[i][0][1] : w[i - 1][0][3];
        t[1] = 2 ? w[i][1][1] : w[i - 1][1][3];
        t[2] = 2 ? w[i][2][1] : w[i - 1][2][3];
        t[3] = 2 ? w[i][3][1] : w[i - 1][3][3];

        this->w[i][0][2] = this->w[i - 1][0][2] ^ t[0];
        this->w[i][1][2] = this->w[i - 1][1][2] ^ t[1];
        this->w[i][2][2] = this->w[i - 1][2][2] ^ t[2];
        this->w[i][3][2] = this->w[i - 1][3][2] ^ t[3];

        t[0] = 3 ? this->w[i][0][2] : this->w[i - 1][0][3];
        t[1] = 3 ? this->w[i][1][2] : this->w[i - 1][1][3];
        t[2] = 3 ? this->w[i][2][2] : this->w[i - 1][2][3];
        t[3] = 3 ? this->w[i][3][2] : this->w[i - 1][3][3];

        this->w[i][0][3] = this->w[i - 1][0][3] ^ t[0];
        this->w[i][1][3] = this->w[i - 1][1][3] ^ t[1];
        this->w[i][2][3] = this->w[i - 1][2][3] ^ t[2];
        this->w[i][3][3] = this->w[i - 1][3][3] ^ t[3];
    }

    void string_device::KeyExpansion(const unsigned char * key) noexcept
    {
        unsigned char rc[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

        this->w[0][0][0] = key[0];
        this->w[0][0][1] = key[4];
        this->w[0][0][2] = key[8];
        this->w[0][0][3] = key[12];

        this->w[0][1][0] = key[1];
        this->w[0][1][1] = key[5];
        this->w[0][1][2] = key[9];
        this->w[0][1][3] = key[13];

        this->w[0][2][0] = key[2];
        this->w[0][2][1] = key[6];
        this->w[0][2][2] = key[10];
        this->w[0][2][3] = key[14];

        this->w[0][3][0] = key[3];
        this->w[0][3][1] = key[7];
        this->w[0][3][2] = key[11];
        this->w[0][3][3] = key[15];

        this->INNER_KeyExpansion(1, rc);
        this->INNER_KeyExpansion(2, rc);
        this->INNER_KeyExpansion(3, rc);
        this->INNER_KeyExpansion(4, rc);
        this->INNER_KeyExpansion(5, rc);
        this->INNER_KeyExpansion(6, rc);
        this->INNER_KeyExpansion(7, rc);
        this->INNER_KeyExpansion(8, rc);
        this->INNER_KeyExpansion(9, rc);
        this->INNER_KeyExpansion(10, rc);
    }

    unsigned char string_device::FFmul(const unsigned char & a, const unsigned char & b) noexcept
    {
        unsigned char bw[4];
        unsigned char res = 0;
        bw[0] = b;

        bw[1] = bw[0] << 1;
        if (bw[0] & 0x80)
        {
            bw[1] ^= 0x1b;
        }

        bw[2] = bw[1] << 1;
        if (bw[1] & 0x80)
        {
            bw[2] ^= 0x1b;
        }

        bw[3] = bw[2] << 1;
        if (bw[2] & 0x80)
        {
            bw[3] ^= 0x1b;
        }

        if ((a >> 0) & 0x01)
        {
            res ^= bw[0];
        }

        if ((a >> 1) & 0x01)
        {
            res ^= bw[1];
        }

        if ((a >> 2) & 0x01)
        {
            res ^= bw[2];
        }

        if ((a >> 3) & 0x01)
        {
            res ^= bw[3];
        }

        return res;
    }

    void string_device::SubBytes(unsigned char state[][4]) noexcept
    {

        state[0][0] = string_device::Sbox[state[0][0]];
        state[0][1] = string_device::Sbox[state[0][1]];
        state[0][2] = string_device::Sbox[state[0][2]];
        state[0][3] = string_device::Sbox[state[0][3]];

        state[1][0] = string_device::Sbox[state[1][0]];
        state[1][1] = string_device::Sbox[state[1][1]];
        state[1][2] = string_device::Sbox[state[1][2]];
        state[1][3] = string_device::Sbox[state[1][3]];

        state[2][0] = string_device::Sbox[state[2][0]];
        state[2][1] = string_device::Sbox[state[2][1]];
        state[2][2] = string_device::Sbox[state[2][2]];
        state[2][3] = string_device::Sbox[state[2][3]];

        state[3][0] = string_device::Sbox[state[3][0]];
        state[3][1] = string_device::Sbox[state[3][1]];
        state[3][2] = string_device::Sbox[state[3][2]];
        state[3][3] = string_device::Sbox[state[3][3]];
    }

    void string_device::ShiftRows(unsigned char state[][4]) noexcept
    {
        unsigned char t[4];

        t[0] = state[1][1 % 4];
        t[1] = state[1][2 % 4];
        t[2] = state[1][3 % 4];
        t[3] = state[1][4 % 4];

        state[1][0] = t[0];
        state[1][1] = t[1];
        state[1][2] = t[2];
        state[1][3] = t[3];

        t[0] = state[2][2 % 4];
        t[1] = state[2][3 % 4];
        t[2] = state[2][4 % 4];
        t[3] = state[2][5 % 4];

        state[2][0] = t[0];
        state[2][1] = t[1];
        state[2][2] = t[2];
        state[2][3] = t[3];

        t[0] = state[3][3 % 4];
        t[1] = state[3][4 % 4];
        t[2] = state[3][5 % 4];
        t[3] = state[3][6 % 4];

        state[3][0] = t[0];
        state[3][1] = t[1];
        state[3][2] = t[2];
        state[3][3] = t[3];
    }

    void string_device::MixColumns(unsigned char state[][4]) noexcept
    {
        unsigned char t[4];

        t[0] = state[0][0];
        t[1] = state[1][0];
        t[2] = state[2][0];
        t[3] = state[3][0];

        state[0][0] = this->FFmul(0x02, t[0]) ^ this->FFmul(0x03, t[1 % 4]) ^ this->FFmul(0x01, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]);

        state[1][0] = this->FFmul(0x02, t[1]) ^ this->FFmul(0x03, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]);

        state[2][0] = this->FFmul(0x02, t[2]) ^ this->FFmul(0x03, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]);

        state[3][0] = this->FFmul(0x02, t[3]) ^ this->FFmul(0x03, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]) ^ this->FFmul(0x01, t[6 % 4]);

        t[0] = state[0][1];
        t[1] = state[1][1];
        t[2] = state[2][1];
        t[3] = state[3][1];

        state[0][1] = this->FFmul(0x02, t[0]) ^ this->FFmul(0x03, t[1 % 4]) ^ this->FFmul(0x01, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]);

        state[1][1] = this->FFmul(0x02, t[1]) ^ this->FFmul(0x03, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]);

        state[2][1] = this->FFmul(0x02, t[2]) ^ this->FFmul(0x03, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]);

        state[3][1] = this->FFmul(0x02, t[3]) ^ this->FFmul(0x03, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]) ^ this->FFmul(0x01, t[6 % 4]);

        t[0] = state[0][2];
        t[1] = state[1][2];
        t[2] = state[2][2];
        t[3] = state[3][2];

        state[0][2] = this->FFmul(0x02, t[0]) ^ this->FFmul(0x03, t[1 % 4]) ^ this->FFmul(0x01, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]);
        ;

        state[1][2] = this->FFmul(0x02, t[1]) ^ this->FFmul(0x03, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]);

        state[2][2] = this->FFmul(0x02, t[2]) ^ this->FFmul(0x03, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]);

        state[3][2] = this->FFmul(0x02, t[3]) ^ this->FFmul(0x03, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]) ^ this->FFmul(0x01, t[6 % 4]);

        t[0] = state[0][3];
        t[1] = state[1][3];
        t[2] = state[2][3];
        t[3] = state[3][3];

        state[0][3] = this->FFmul(0x02, t[0]) ^ this->FFmul(0x03, t[1 % 4]) ^ this->FFmul(0x01, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]);

        state[1][3] = this->FFmul(0x02, t[1]) ^ this->FFmul(0x03, t[2 % 4]) ^ this->FFmul(0x01, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]);

        state[2][3] = this->FFmul(0x02, t[2]) ^ this->FFmul(0x03, t[3 % 4]) ^ this->FFmul(0x01, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]);

        state[3][3] = this->FFmul(0x02, t[3]) ^ this->FFmul(0x03, t[4 % 4]) ^ this->FFmul(0x01, t[5 % 4]) ^ this->FFmul(0x01, t[6 % 4]);
    }

    void string_device::AddRoundKey(unsigned char state[][4], const unsigned char k[][4]) noexcept
    {

        state[0][0] ^= k[0][0];
        state[1][0] ^= k[1][0];
        state[2][0] ^= k[2][0];
        state[3][0] ^= k[3][0];

        state[0][1] ^= k[0][1];
        state[1][1] ^= k[1][1];
        state[2][1] ^= k[2][1];
        state[3][1] ^= k[3][1];

        state[0][2] ^= k[0][2];
        state[1][2] ^= k[1][2];
        state[2][2] ^= k[2][2];
        state[3][2] ^= k[3][2];

        state[0][3] ^= k[0][3];
        state[1][3] ^= k[1][3];
        state[2][3] ^= k[2][3];
        state[3][3] ^= k[3][3];
    }

    void string_device::InvSubBytes(unsigned char state[][4]) noexcept
    {

        state[0][0] = string_device::InvSbox[state[0][0]];
        state[0][1] = string_device::InvSbox[state[0][1]];
        state[0][2] = string_device::InvSbox[state[0][2]];
        state[0][3] = string_device::InvSbox[state[0][3]];

        state[1][0] = string_device::InvSbox[state[1][0]];
        state[1][1] = string_device::InvSbox[state[1][1]];
        state[1][2] = string_device::InvSbox[state[1][2]];
        state[1][3] = string_device::InvSbox[state[1][3]];

        state[2][0] = string_device::InvSbox[state[2][0]];
        state[2][1] = string_device::InvSbox[state[2][1]];
        state[2][2] = string_device::InvSbox[state[2][2]];
        state[2][3] = string_device::InvSbox[state[2][3]];

        state[3][0] = string_device::InvSbox[state[3][0]];
        state[3][1] = string_device::InvSbox[state[3][1]];
        state[3][2] = string_device::InvSbox[state[3][2]];
        state[3][3] = string_device::InvSbox[state[3][3]];
    }

    void string_device::InvShiftRows(unsigned char state[][4]) noexcept
    {
        unsigned char t[4];

        t[0] = state[1][3 % 4];
        t[1] = state[1][4 % 4];
        t[2] = state[1][5 % 4];
        t[3] = state[1][6 % 4];

        state[1][0] = t[0];
        state[1][1] = t[1];
        state[1][2] = t[2];
        state[1][3] = t[3];

        t[0] = state[2][2 % 4];
        t[1] = state[2][3 % 4];
        t[2] = state[2][4 % 4];
        t[3] = state[2][5 % 4];

        state[2][0] = t[0];
        state[2][1] = t[1];
        state[2][2] = t[2];
        state[2][3] = t[3];

        t[0] = state[3][1 % 4];
        t[1] = state[3][2 % 4];
        t[2] = state[3][3 % 4];
        t[3] = state[3][4 % 4];
        ;

        state[3][0] = t[0];
        state[3][1] = t[1];
        state[3][2] = t[2];
        state[3][3] = t[3];
    }

    void string_device::InvMixColumns(unsigned char state[][4]) noexcept
    {
        unsigned char t[4];

        t[0] = state[0][0];
        t[1] = state[1][0];
        t[2] = state[2][0];
        t[3] = state[3][0];

        state[0][0] = this->FFmul(0x0e, t[0]) ^ this->FFmul(0x0b, t[1 % 4]) ^ this->FFmul(0x0d, t[2 % 4]) ^ this->FFmul(0x09, t[3 % 4]);

        state[1][0] = this->FFmul(0x0e, t[1]) ^ this->FFmul(0x0b, t[2 % 4]) ^ this->FFmul(0x0d, t[3 % 4]) ^ this->FFmul(0x09, t[4 % 4]);

        state[2][0] = this->FFmul(0x0e, t[2]) ^ this->FFmul(0x0b, t[3 % 4]) ^ this->FFmul(0x0d, t[4 % 4]) ^ this->FFmul(0x09, t[5 % 4]);

        state[3][0] = this->FFmul(0x0e, t[3]) ^ this->FFmul(0x0b, t[4 % 4]) ^ this->FFmul(0x0d, t[5 % 4]) ^ this->FFmul(0x09, t[6 % 4]);

        t[0] = state[0][1];
        t[1] = state[1][1];
        t[2] = state[2][1];
        t[3] = state[3][1];

        state[0][1] = this->FFmul(0x0e, t[0]) ^ this->FFmul(0x0b, t[1 % 4]) ^ this->FFmul(0x0d, t[2 % 4]) ^ this->FFmul(0x09, t[3 % 4]);

        state[1][1] = this->FFmul(0x0e, t[1]) ^ this->FFmul(0x0b, t[2 % 4]) ^ this->FFmul(0x0d, t[3 % 4]) ^ this->FFmul(0x09, t[4 % 4]);

        state[2][1] = this->FFmul(0x0e, t[2]) ^ this->FFmul(0x0b, t[3 % 4]) ^ this->FFmul(0x0d, t[4 % 4]) ^ this->FFmul(0x09, t[5 % 4]);

        state[3][1] = this->FFmul(0x0e, t[3]) ^ this->FFmul(0x0b, t[4 % 4]) ^ this->FFmul(0x0d, t[5 % 4]) ^ this->FFmul(0x09, t[6 % 4]);

        t[0] = state[0][2];
        t[1] = state[1][2];
        t[2] = state[2][2];
        t[3] = state[3][2];

        state[0][2] = this->FFmul(0x0e, t[0]) ^ this->FFmul(0x0b, t[1 % 4]) ^ this->FFmul(0x0d, t[2 % 4]) ^ this->FFmul(0x09, t[3 % 4]);

        state[1][2] = this->FFmul(0x0e, t[1]) ^ this->FFmul(0x0b, t[2 % 4]) ^ this->FFmul(0x0d, t[3 % 4]) ^ this->FFmul(0x09, t[4 % 4]);

        state[2][2] = this->FFmul(0x0e, t[2]) ^ this->FFmul(0x0b, t[3 % 4]) ^ this->FFmul(0x0d, t[4 % 4]) ^ this->FFmul(0x09, t[5 % 4]);

        state[3][2] = this->FFmul(0x0e, t[3]) ^ this->FFmul(0x0b, t[4 % 4]) ^ this->FFmul(0x0d, t[5 % 4]) ^ this->FFmul(0x09, t[6 % 4]);

        t[0] = state[0][3];
        t[1] = state[1][3];
        t[2] = state[2][3];
        t[3] = state[3][3];

        state[0][3] = this->FFmul(0x0e, t[0]) ^ this->FFmul(0x0b, t[1 % 4]) ^ this->FFmul(0x0d, t[2 % 4]) ^ this->FFmul(0x09, t[3 % 4]);

        state[1][3] = this->FFmul(0x0e, t[1]) ^ this->FFmul(0x0b, t[2 % 4]) ^ this->FFmul(0x0d, t[3 % 4]) ^ this->FFmul(0x09, t[4 % 4]);

        state[2][3] = this->FFmul(0x0e, t[2]) ^ this->FFmul(0x0b, t[3 % 4]) ^ this->FFmul(0x0d, t[4 % 4]) ^ this->FFmul(0x09, t[5 % 4]);

        state[3][3] = this->FFmul(0x0e, t[3]) ^ this->FFmul(0x0b, t[4 % 4]) ^ this->FFmul(0x0d, t[5 % 4]) ^ this->FFmul(0x09, t[6 % 4]);
    }

    void string_device::make_align(unsigned char * buffer, const int count)
    {
        if (count < 0 or count > 16)
        {
            throw std::invalid_argument("末尾对齐处理的位数不合法");
        }
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<int> distribution(0, 255); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

        for (int counter = count; counter < 16; counter++)
        {
            buffer[counter] = distribution(random_engine);
        }
    }

    void string_device::align_Cipher(std::string & origin)
    {
        size_t count = 0;
        while (count + 16 <= origin.size())
        {
            count = count + 16;
        }
        count = origin.size() - count; // 末尾对齐的原文位数
        this->Cipher(origin);
        unsigned char buffer[16];

        for (int count1 = 0; count1 < count; count1++)
        {
            buffer[count1] = origin[origin.size() - count + count1];
        }
        this->make_align(buffer, count);
        this->Cipher(buffer);
        for (int count1 = 0; count1 < count; count1++)
        {
            origin[origin.size() - count + count1] = buffer[count1];
        }
        for (int count1 = count; count1 < 16; count1++)
        {
            origin.push_back(buffer[count1]);
        }
        origin.push_back(count);
    }

    void string_device::align_InvCipher(std::string & origin)
    {
        if (origin.size() % 16 != 1)
        {

            throw std::invalid_argument("输入串的格式错误");
        }
        int count = origin[origin.size() - 1];
        origin.pop_back();
        this->InvCipher(origin);
        for (int count1 = 0; count1 < 16 - count; count1++)
        {
            origin.pop_back();
        }
    }

    void file_device::aes_bifile()
    {
        /*
        分配任务
         原文为 16n+end_info 字节，其中end_info字节是需要末尾对齐的部分
         16n 分给 m 个加密器线程处理，它们全部结束以后，end_info 字节由本函数处理
         注意basic_aes_device算法的原文必须以16字节为一组，得到16字节密文，所以要分配的不是 16n 个字节，而是 n 个 16字节块
         则应该分给每个加密器线程 n/m 块，而留下 n-(n/m)*m 块，再加上末尾对齐的 end_info 字节，由本函数处理，处理时应该使用下标[this->cipher_number-1]的文件对象和this->write_size数组
        */
        // 原文的总块数
        unsigned long long int block_count = this->MEMBER_file_size / 16;
        // 末尾对齐处理的位数
        int end_info = static_cast<int>(this->MEMBER_file_size - block_count * 16);
        // 加密器线程的任务起始点
        unsigned long long int begin = 0;
        // 单个加密器线程的任务块数
        const unsigned long long int single_work_block_count = block_count / this->cipher_number;

        std::vector<file_device::TYPE_cipher_ptr> cipher;
        for (int count = 0; count < this->cipher_number; count++)
        {
            cipher.push_back(std::make_shared<file_device::aes_cipher>(this->origin[count], this->result[count], begin, single_work_block_count, file_device::aes_mode::cipher, this->MEMBER_write_size[count], this->key));
            // 下一个加密器线程的起始点
            begin = begin + 16 * single_work_block_count;
        }
        for (int count = 0; count < this->cipher_number; count++)
        {
            cipher[count]->join();
        }

        // 所有加密器线程结束以后，现在begin是本函数需要处理的起始点

        char buffer[16]; // 16字节缓存，一组明文
        string_device f_aes(this->key);
        // 处理原文中剩余的 完整16字节块
        for (int count = 0; count < block_count - single_work_block_count * this->cipher_number; count++)
        {
            this->origin[this->cipher_number - 1]->read(buffer, 16);
            std::string TEMP_buffer = file_device::INNER_buffer_str(buffer, 16);
            f_aes.Cipher(TEMP_buffer);
            this->result[this->cipher_number - 1]->write(TEMP_buffer.c_str(), 16);
            this->MEMBER_write_size[this->cipher_number - 1] = this->MEMBER_write_size[this->cipher_number - 1] + 16;
        }
        // 处理末尾对齐
        this->origin[this->cipher_number - 1]->read(buffer, end_info);
        f_aes.make_align(reinterpret_cast<unsigned char *>(buffer), end_info);
        std::string TEMP_buffer = file_device::INNER_buffer_str(buffer, 16);
        f_aes.Cipher(TEMP_buffer);
        this->result[this->cipher_number - 1]->write(TEMP_buffer.c_str(), 16);
        this->MEMBER_write_size[this->cipher_number - 1] = this->MEMBER_write_size[this->cipher_number - 1] + end_info;
        {
            char info = static_cast<char>(end_info);
            this->result[this->cipher_number - 1]->write(&info, 1);
        }
    }

    void file_device::deaes_bifile()
    {
        /*
        分配任务
        原文为 16n+16+1 字节，其中1是end_info信息，即末尾对齐的位数，16是末尾对齐处理的末尾块，16n是 n 个一般的密文块
        把16n分给 m 个加密器线程，它们全部运行结束以后，再由本函数处理 16+1
        注意basic_aes_device算法的密文必须以16字节为一组，得到16字节明文，所以要分配的不是 16n 个字节，而是 n 个 16字节块
        则应该分给每个加密器线程 n/m 块，而留下 n-(n/m)*m 块，再加上末尾的 16+1 字节，由本函数处理，处理时应该使用下标[this->cipher_number-1]的文件对象和this->write_size数组
        */
        // 一般16字节密文块的块数 n
        unsigned long long int block_count = this->MEMBER_file_size / 16 - 1;
        // 加密器线程的任务起始点
        unsigned long long int begin = 0;
        // 单个加密器线程的任务块数
        const unsigned long long int single_work_block_count = block_count / this->cipher_number;

        std::vector<file_device::TYPE_cipher_ptr> cipher;
        for (int count = 0; count < this->cipher_number; count++)
        {
            cipher.push_back(std::make_shared<file_device::aes_cipher>(this->origin[count], this->result[count], begin, single_work_block_count, file_device::aes_mode::incipher, this->MEMBER_write_size[count], this->key));
            // 下一个加密器线程的起始点
            begin = begin + 16 * single_work_block_count;
        }
        for (int count = 0; count < this->cipher_number; count++)
        {
            cipher[count]->join();
        }

        // 所有加密器线程结束以后，现在begin是本函数需要处理的起始点

        string_device f_aes(this->key);
        {
            char buffer[16];
            // 处理剩余的 n-(n/m)*m 块
            for (int count = 0; count < block_count - single_work_block_count * this->cipher_number; count++)
            {
                this->origin[this->cipher_number - 1]->read(buffer, 16);
                std::string TEMP_buffer = file_device::INNER_buffer_str(buffer, 16);

                f_aes.InvCipher(TEMP_buffer);
                this->result[this->cipher_number - 1]->write(TEMP_buffer.c_str(), 16);
                this->MEMBER_write_size[this->cipher_number - 1] = this->MEMBER_write_size[this->cipher_number - 1] + 16;
            }
        }
        // 处理末尾对齐
        {
            char buffer[17];
            // 读取最后16+1字节，则buf[16]是 end_info ，末尾对齐处理的位数
            this->origin[this->cipher_number - 1]->read(buffer, 17);
            std::string TEMP_buffer = file_device::INNER_buffer_str(buffer, 17);

            f_aes.InvCipher(TEMP_buffer);
            this->result[this->cipher_number - 1]->write(TEMP_buffer.c_str(), static_cast<int>(TEMP_buffer[16]));
            this->MEMBER_write_size[this->cipher_number - 1] = this->MEMBER_write_size[this->cipher_number - 1] + 17;
        }
    }

    std::string string_device::random_aes_key()
    {
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<int> distribution(0, 255); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间
        std::string result;

        for (int counter = 0; counter < 16; counter++)
        {

            result.push_back(distribution(random_engine)); // 生成一个0到255的随机整数，转换为unsigned char，存储到key中
        }
        return result;
    }

    void string_device::random_Cipher(std::string & origin)
    {
        std::string key = string_device::random_aes_key();
        string_device worker1(key);
        worker1.align_Cipher(origin);
        string_device worker2(string_device::key_of_keys());
        worker2.Cipher(key);
        for (int count = 0; count < 16; count++)
        {
            origin.push_back(key[count]);
        }
    }

    void string_device::random_InvCipher(std::string & origin)
    {
        if (origin.size() < 16 + 17)
        {

            throw std::invalid_argument("输入的串长度太短");
        }
        std::string key;
        for (int count = 0; count < 16; count++)
        {
            key.push_back(origin[origin.size() - 16 + count]);
        }
        for (int count = 0; count < 16; count++)
        {
            origin.pop_back();
        }
        string_device worker1(string_device::key_of_keys());
        worker1.InvCipher(key);
        string_device worker2(key);
        worker2.align_InvCipher(origin);
    }

    void string_device::OKps_Cipher(std::string & origin)
    {
        string_device::random_Cipher(origin);
        block<char> length_buffer(sizeof(size_t) / sizeof(char));
        length_buffer.at<size_t>(0) = origin.size();
        std::string length;
        for (size_t count = 0; count < length_buffer.size(); count++)
        {
            length.push_back(length_buffer.at(count));
        }
        origin = length + origin;
    }

    std::string string_device::OKps_InvCipher(file::reader & file, std::streampos & begin_position)
    {
        if (not file.is_open())
        {
            throw std::invalid_argument("输入的文件对象没有打开文件");
        }
        block<char> length_buffer(sizeof(size_t) / sizeof(char));
        file.seekg(begin_position);
        file.read(&length_buffer, sizeof(size_t) / sizeof(char));
        block<char> data_buffer(length_buffer.at<size_t>(0));
        file.read(&data_buffer, length_buffer.at<size_t>(0));
        std::string result;
        for (size_t count = 0; count < data_buffer.size(); count++)
        {
            result.push_back(data_buffer.at(count));
        }
        string_device::random_InvCipher(result);
        begin_position = file.tellg();
        return result;
    }

    bool file_device::is_running() const noexcept
    {
        return this->running;
    }

    bool file_device::is_used() const noexcept
    {
        return this->used;
    }

    std::uintmax_t file_device::file_size() const noexcept
    {
        return this->MEMBER_file_size;
    }

    const std::vector<std::uintmax_t> & file_device::write_size() const noexcept
    {
        return this->MEMBER_write_size;
    }

    std::size_t file_device::cipher_count() const noexcept
    {
        return this->cipher_number;
    }

    std::string file_device::INNER_buffer_str(const char * buffer, std::size_t size)
    {
        std::string result;
        for (std::size_t count = 0; count < size; count++)
        {
            result.push_back(buffer[count]);
        }
        return result;
    }
    file_device::file_device()
        : origin_route(), result_route()
    {
        this->MEMBER_file_size = 0;
        this->running = false;
        this->used = false;
        this->is_start = false;
        this->aes_work_mode = file_device::aes_mode::cipher;
        this->cipher_number = 0;
    }

    file_device::~file_device()
    {

        /* 在任何一个线程对象析构之前，我们一定调用线程的join方法，无论线程函数是否已经自然地执行完毕
        我还没有查到相关资料，以解释这样做的缘由，但是这是必须的
        https://www.bilibili.com/read/cv25541913/
        可以查看这篇文章的描述：运行时明显可以看见子线程已经在主线程结束前自然结束了，但如果程序员不在主线程中调用join方法，程序还是会调用abort异常退出
        也就是说join除了等待线程结束以外还做了其他什么事情，但我们并不知道。
        */
        if (this->is_start)
        {
            /*
            但是，对于空的std::thread对象，又不能调用join方法，否则会产生运行时异常，并使程序调用abort异常退出
            所以这里还要检查线程是否启动过
            因为用户有可能打开了标签页，而没有进行文件操作，就关掉了标签页，此时线程对象是空的
            */
            this->main_thread.join();
        }
    }

    void file_device::change_mode() noexcept
    {
        if (this->aes_work_mode == file_device::aes_mode::cipher)
        {
            this->aes_work_mode = file_device::aes_mode::incipher;
        }
        else
        {
            this->aes_work_mode = file_device::aes_mode::cipher;
        }
    }
    void file_device::change_mode(file_device::aes_mode _mode) noexcept
    {
        this->aes_work_mode = _mode;
    }

    void file_device::aes_start(const std::string & _origin_route, const std::string & _result_route, const file_device::TYPE_key & _key, const std::size_t thread_count)
    {
        if (thread_count <= 0)
        {

            throw std::invalid_argument("输入的线程数不为正数");
        }
        this->cipher_number = thread_count;

        this->MEMBER_write_size.clear();
        this->origin.clear();

        this->result.clear();
        this->key = _key;

        this->origin_route = _origin_route;
        this->result_route = _result_route;
        this->MEMBER_file_size = std::filesystem::file_size(origin_route);

        for (int count = 0; count < this->cipher_number; count++)
        {
            this->MEMBER_write_size.push_back(0);
            this->origin.push_back(std::make_shared<file::reader>(64 * 1024));

            this->result.push_back(std::make_shared<file::writter>(64 * 1024));
        }
        this->is_start = true;
        this->main_thread = std::thread(&file_device::aes_main, this);
    }

    void file_device::aes_start(const std::string & _origin_route, const std::string & _result_route, const std::string & _key, const std::size_t thread_count)
    {
        if (_key.size() == 16)
        {
            file_device::TYPE_key TEMP_key;
            for (int count = 0; count < 16; count++)
            {
                TEMP_key[count] = _key[count];
            }
            this->aes_start(_origin_route, _result_route, TEMP_key, thread_count);
        }
        else
        {

            throw std::invalid_argument("密钥长度错误");
        }
    }

    void file_device::aes_main()
    {

        this->running = true;
        if (this->aes_work_mode == file_device::aes_mode::cipher)
        // 加密
        {
            for (int count = 0; count < this->cipher_number; count++)
            {
                this->origin[count]->open(this->origin_route, std::ios::in | std::ios::binary); // 二进制读方式打开文件
                if (not this->origin[count]->is_open())
                {
                    auto  TEMP_error = std::runtime_error("加密原文件打开失败");

                    this->running = false;
                    this->used = true;
                    throw TEMP_error;
                }
            }
            for (int count = 0; count < this->cipher_number; count++)
            {
                this->result[count]->open(this->result_route, std::ios::out | std::ios::binary);
                if (not this->result[count]->is_open())
                {
                    auto  TEMP_error = std::runtime_error("加密输出文件创建失败");
                    this->running = false;
                    this->used = true;
                    throw TEMP_error;
                }
            }

            // 创建用户指定的aes加密后的新文件成功

            // aes加密
            this->aes_bifile();

            // aes加密完成

            for (int count = 0; count < this->cipher_number; count++)
            {
                this->origin[count]->close();
                this->result[count]->close();
            }
        }
        else
        // 解密
        {
            for (int count = 0; count < this->cipher_number; count++)
            {
                this->origin[count]->open(this->origin_route, std::ios::in | std::ios::binary); // 二进制读方式打开文件
                if (not this->origin[count]->is_open())
                {
                    auto  TEMP_error = std::runtime_error("解密原文件打开失败");

                    this->running = false;
                    this->used = true;
                    throw TEMP_error;
                }
            }
            for (int count = 0; count < this->cipher_number; count++)
            {
                this->result[count]->open(this->result_route, std::ios::out | std::ios::binary);
                if (not this->result[count]->is_open())
                {
                    auto  TEMP_error = std::runtime_error("解密输出文件创建失败");
                    this->running = false;
                    this->used = true;
                    throw TEMP_error;
                }
            }

            // 创建用户指定的aes解密后的新文件成功

            // aes解密
            this->deaes_bifile();
            // aes解密完成

            for (int count = 0; count < this->cipher_number; count++)
            {
                this->origin[count]->close();
                this->result[count]->close();
            }
        }
        this->running = false;
        this->used = true;
    }

    file_device::aes_cipher::aes_cipher(TYPE_reader origin, TYPE_writter result, const unsigned long long int begin, const unsigned long long int block_number, const aes_mode mode, std::uintmax_t & write_size, const TYPE_key & key)
        : begin(begin), block_number(block_number), aes_work_mode(mode), write_size(write_size)
    {
        this->origin = origin;
        this->result = result;
        this->key = key;
        for (int count = 0; count < 16; count++)
        {
            this->key[count] = key[count];
        }
        this->cipher_thread = std::thread(&file_device::aes_cipher::cipher_main, this);
    }

    file_device::aes_cipher::~aes_cipher()
    {
    }

    void file_device::aes_cipher::cipher_main()
    {
        this->origin->seekg(this->begin);
        this->result->seekp(this->begin);
        string_device aes_calculator(this->key);
        char buf[16];
        if (this->aes_work_mode == aes_mode::cipher)
        // 加密
        {
            for (unsigned long long int count = 0; count < this->block_number; count++)
            {
                this->origin->read(buf, 16);
                std::string TEMP_buffer = file_device::INNER_buffer_str(buf, 16);
                aes_calculator.Cipher(TEMP_buffer);
                this->result->write(TEMP_buffer.c_str(), 16);
                this->write_size = this->write_size + 16;
            }
        }
        else
        // 解密
        {
            for (unsigned long long int count = 0; count < this->block_number; count++)
            {
                this->origin->read(buf, 16);
                std::string TEMP_buffer = file_device::INNER_buffer_str(buf, 16);

                aes_calculator.InvCipher(TEMP_buffer);
                this->result->write(TEMP_buffer.c_str(), 16);
                this->write_size = this->write_size + 16;
            }
        }
    }

    void file_device::aes_cipher::join()
    {
        this->cipher_thread.join();
    }

    void string_device::Cipher(std::string & origin)
    {

        unsigned char buffer[16];
        for (size_t count = 0; count + 16 <= origin.size(); count = count + 16)
        {

            /*
            初始化缓冲区
            手动循环展开，因为msvc不会优化
            */
            buffer[0] = origin[count];
            buffer[1] = origin[count + 1];
            buffer[2] = origin[count + 2];
            buffer[3] = origin[count + 3];
            buffer[4] = origin[count + 4];
            buffer[5] = origin[count + 5];
            buffer[6] = origin[count + 6];
            buffer[7] = origin[count + 7];
            buffer[8] = origin[count + 8];
            buffer[9] = origin[count + 9];
            buffer[10] = origin[count + 10];
            buffer[11] = origin[count + 11];
            buffer[12] = origin[count + 12];
            buffer[13] = origin[count + 13];
            buffer[14] = origin[count + 14];
            buffer[15] = origin[count + 15];

            this->Cipher(buffer);

            /*
            密文写入原串
            */
            origin[count] = buffer[0];
            origin[count + 1] = buffer[1];
            origin[count + 2] = buffer[2];
            origin[count + 3] = buffer[3];
            origin[count + 4] = buffer[4];
            origin[count + 5] = buffer[5];
            origin[count + 6] = buffer[6];
            origin[count + 7] = buffer[7];
            origin[count + 8] = buffer[8];
            origin[count + 9] = buffer[9];
            origin[count + 10] = buffer[10];
            origin[count + 11] = buffer[11];
            origin[count + 12] = buffer[12];
            origin[count + 13] = buffer[13];
            origin[count + 14] = buffer[14];
            origin[count + 15] = buffer[15];
        }
    }

    void string_device::InvCipher(std::string & origin)
    {

        unsigned char buffer[16];
        for (size_t count = 0; count + 16 <= origin.size(); count = count + 16)
        {

            /*
            初始化缓冲区
            手动循环展开，因为msvc不会优化
            */
            buffer[0] = origin[count];
            buffer[1] = origin[count + 1];
            buffer[2] = origin[count + 2];
            buffer[3] = origin[count + 3];
            buffer[4] = origin[count + 4];
            buffer[5] = origin[count + 5];
            buffer[6] = origin[count + 6];
            buffer[7] = origin[count + 7];
            buffer[8] = origin[count + 8];
            buffer[9] = origin[count + 9];
            buffer[10] = origin[count + 10];
            buffer[11] = origin[count + 11];
            buffer[12] = origin[count + 12];
            buffer[13] = origin[count + 13];
            buffer[14] = origin[count + 14];
            buffer[15] = origin[count + 15];

            this->InvCipher(buffer);

            /*
            密文写入原串
            */
            origin[count] = buffer[0];
            origin[count + 1] = buffer[1];
            origin[count + 2] = buffer[2];
            origin[count + 3] = buffer[3];
            origin[count + 4] = buffer[4];
            origin[count + 5] = buffer[5];
            origin[count + 6] = buffer[6];
            origin[count + 7] = buffer[7];
            origin[count + 8] = buffer[8];
            origin[count + 9] = buffer[9];
            origin[count + 10] = buffer[10];
            origin[count + 11] = buffer[11];
            origin[count + 12] = buffer[12];
            origin[count + 13] = buffer[13];
            origin[count + 14] = buffer[14];
            origin[count + 15] = buffer[15];
        }
    }
}
