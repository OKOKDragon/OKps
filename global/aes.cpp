#include <random>

#include ".\aes.hpp"
#include ".\string.hpp"

namespace OKps::AES
{
    bool byte_device::operator==(byte_device const & right)const noexcept
    {
        return this->MEMBER_key == right.MEMBER_key;
    }
    bool byte_device::operator!=(byte_device const & right)const noexcept
    {
        return this->MEMBER_key != right.MEMBER_key;
    }
    const std::byte byte_device::Sbox[256] =
    {
        /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
        std::byte(0x63), std::byte(0x7c), std::byte(0x77), std::byte(0x7b), std::byte(0xf2), std::byte(0x6b), std::byte(0x6f), std::byte(0xc5), std::byte(0x30), std::byte(0x01), std::byte(0x67), std::byte(0x2b), std::byte(0xfe), std::byte(0xd7), std::byte(0xab), std::byte(0x76), /*0*/
        std::byte(0xca), std::byte(0x82), std::byte(0xc9), std::byte(0x7d), std::byte(0xfa), std::byte(0x59), std::byte(0x47), std::byte(0xf0), std::byte(0xad), std::byte(0xd4), std::byte(0xa2), std::byte(0xaf), std::byte(0x9c), std::byte(0xa4), std::byte(0x72), std::byte(0xc0), /*1*/
        std::byte(0xb7), std::byte(0xfd), std::byte(0x93), std::byte(0x26), std::byte(0x36), std::byte(0x3f), std::byte(0xf7), std::byte(0xcc), std::byte(0x34), std::byte(0xa5), std::byte(0xe5), std::byte(0xf1), std::byte(0x71), std::byte(0xd8), std::byte(0x31), std::byte(0x15), /*2*/
        std::byte(0x04), std::byte(0xc7), std::byte(0x23), std::byte(0xc3), std::byte(0x18), std::byte(0x96), std::byte(0x05), std::byte(0x9a), std::byte(0x07), std::byte(0x12), std::byte(0x80), std::byte(0xe2), std::byte(0xeb), std::byte(0x27), std::byte(0xb2), std::byte(0x75), /*3*/
        std::byte(0x09), std::byte(0x83), std::byte(0x2c), std::byte(0x1a), std::byte(0x1b), std::byte(0x6e), std::byte(0x5a), std::byte(0xa0), std::byte(0x52), std::byte(0x3b), std::byte(0xd6), std::byte(0xb3), std::byte(0x29), std::byte(0xe3), std::byte(0x2f), std::byte(0x84), /*4*/
        std::byte(0x53), std::byte(0xd1), std::byte(0x00), std::byte(0xed), std::byte(0x20), std::byte(0xfc), std::byte(0xb1), std::byte(0x5b), std::byte(0x6a), std::byte(0xcb), std::byte(0xbe), std::byte(0x39), std::byte(0x4a), std::byte(0x4c), std::byte(0x58), std::byte(0xcf), /*5*/
        std::byte(0xd0), std::byte(0xef), std::byte(0xaa), std::byte(0xfb), std::byte(0x43), std::byte(0x4d), std::byte(0x33), std::byte(0x85), std::byte(0x45), std::byte(0xf9), std::byte(0x02), std::byte(0x7f), std::byte(0x50), std::byte(0x3c), std::byte(0x9f), std::byte(0xa8), /*6*/
        std::byte(0x51), std::byte(0xa3), std::byte(0x40), std::byte(0x8f), std::byte(0x92), std::byte(0x9d), std::byte(0x38), std::byte(0xf5), std::byte(0xbc), std::byte(0xb6), std::byte(0xda), std::byte(0x21), std::byte(0x10), std::byte(0xff), std::byte(0xf3), std::byte(0xd2), /*7*/
        std::byte(0xcd), std::byte(0x0c), std::byte(0x13), std::byte(0xec), std::byte(0x5f), std::byte(0x97), std::byte(0x44), std::byte(0x17), std::byte(0xc4), std::byte(0xa7), std::byte(0x7e), std::byte(0x3d), std::byte(0x64), std::byte(0x5d), std::byte(0x19), std::byte(0x73), /*8*/
        std::byte(0x60), std::byte(0x81), std::byte(0x4f), std::byte(0xdc), std::byte(0x22), std::byte(0x2a), std::byte(0x90), std::byte(0x88), std::byte(0x46), std::byte(0xee), std::byte(0xb8), std::byte(0x14), std::byte(0xde), std::byte(0x5e), std::byte(0x0b), std::byte(0xdb), /*9*/
        std::byte(0xe0), std::byte(0x32), std::byte(0x3a), std::byte(0x0a), std::byte(0x49), std::byte(0x06), std::byte(0x24), std::byte(0x5c), std::byte(0xc2), std::byte(0xd3), std::byte(0xac), std::byte(0x62), std::byte(0x91), std::byte(0x95), std::byte(0xe4), std::byte(0x79), /*a*/
        std::byte(0xe7), std::byte(0xc8), std::byte(0x37), std::byte(0x6d), std::byte(0x8d), std::byte(0xd5), std::byte(0x4e), std::byte(0xa9), std::byte(0x6c), std::byte(0x56), std::byte(0xf4), std::byte(0xea), std::byte(0x65), std::byte(0x7a), std::byte(0xae), std::byte(0x08), /*b*/
        std::byte(0xba), std::byte(0x78), std::byte(0x25), std::byte(0x2e), std::byte(0x1c), std::byte(0xa6), std::byte(0xb4), std::byte(0xc6), std::byte(0xe8), std::byte(0xdd), std::byte(0x74), std::byte(0x1f), std::byte(0x4b), std::byte(0xbd), std::byte(0x8b), std::byte(0x8a), /*c*/
        std::byte(0x70), std::byte(0x3e), std::byte(0xb5), std::byte(0x66), std::byte(0x48), std::byte(0x03), std::byte(0xf6), std::byte(0x0e), std::byte(0x61), std::byte(0x35), std::byte(0x57), std::byte(0xb9), std::byte(0x86), std::byte(0xc1), std::byte(0x1d), std::byte(0x9e), /*d*/
        std::byte(0xe1), std::byte(0xf8), std::byte(0x98), std::byte(0x11), std::byte(0x69), std::byte(0xd9), std::byte(0x8e), std::byte(0x94), std::byte(0x9b), std::byte(0x1e), std::byte(0x87), std::byte(0xe9), std::byte(0xce), std::byte(0x55), std::byte(0x28), std::byte(0xdf), /*e*/
        std::byte(0x8c), std::byte(0xa1), std::byte(0x89), std::byte(0x0d), std::byte(0xbf), std::byte(0xe6), std::byte(0x42), std::byte(0x68), std::byte(0x41), std::byte(0x99), std::byte(0x2d), std::byte(0x0f), std::byte(0xb0), std::byte(0x54), std::byte(0xbb), std::byte(0x16) /*f*/
    };

    const std::byte byte_device::InvSbox[256] =
    {
        /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
        std::byte(0x52), std::byte(0x09), std::byte(0x6a), std::byte(0xd5), std::byte(0x30), std::byte(0x36), std::byte(0xa5), std::byte(0x38), std::byte(0xbf), std::byte(0x40), std::byte(0xa3), std::byte(0x9e), std::byte(0x81), std::byte(0xf3), std::byte(0xd7), std::byte(0xfb), /*0*/
        std::byte(0x7c), std::byte(0xe3), std::byte(0x39), std::byte(0x82), std::byte(0x9b), std::byte(0x2f), std::byte(0xff), std::byte(0x87), std::byte(0x34), std::byte(0x8e), std::byte(0x43), std::byte(0x44), std::byte(0xc4), std::byte(0xde), std::byte(0xe9), std::byte(0xcb), /*1*/
        std::byte(0x54), std::byte(0x7b), std::byte(0x94), std::byte(0x32), std::byte(0xa6), std::byte(0xc2), std::byte(0x23), std::byte(0x3d), std::byte(0xee), std::byte(0x4c), std::byte(0x95), std::byte(0x0b), std::byte(0x42), std::byte(0xfa), std::byte(0xc3), std::byte(0x4e), /*2*/
        std::byte(0x08), std::byte(0x2e), std::byte(0xa1), std::byte(0x66), std::byte(0x28), std::byte(0xd9), std::byte(0x24), std::byte(0xb2), std::byte(0x76), std::byte(0x5b), std::byte(0xa2), std::byte(0x49), std::byte(0x6d), std::byte(0x8b), std::byte(0xd1), std::byte(0x25), /*3*/
        std::byte(0x72), std::byte(0xf8), std::byte(0xf6), std::byte(0x64), std::byte(0x86), std::byte(0x68), std::byte(0x98), std::byte(0x16), std::byte(0xd4), std::byte(0xa4), std::byte(0x5c), std::byte(0xcc), std::byte(0x5d), std::byte(0x65), std::byte(0xb6), std::byte(0x92), /*4*/
        std::byte(0x6c), std::byte(0x70), std::byte(0x48), std::byte(0x50), std::byte(0xfd), std::byte(0xed), std::byte(0xb9), std::byte(0xda), std::byte(0x5e), std::byte(0x15), std::byte(0x46), std::byte(0x57), std::byte(0xa7), std::byte(0x8d), std::byte(0x9d), std::byte(0x84), /*5*/
        std::byte(0x90), std::byte(0xd8), std::byte(0xab), std::byte(0x00), std::byte(0x8c), std::byte(0xbc), std::byte(0xd3), std::byte(0x0a), std::byte(0xf7), std::byte(0xe4), std::byte(0x58), std::byte(0x05), std::byte(0xb8), std::byte(0xb3), std::byte(0x45), std::byte(0x06), /*6*/
        std::byte(0xd0), std::byte(0x2c), std::byte(0x1e), std::byte(0x8f), std::byte(0xca), std::byte(0x3f), std::byte(0x0f), std::byte(0x02), std::byte(0xc1), std::byte(0xaf), std::byte(0xbd), std::byte(0x03), std::byte(0x01), std::byte(0x13), std::byte(0x8a), std::byte(0x6b), /*7*/
        std::byte(0x3a), std::byte(0x91), std::byte(0x11), std::byte(0x41), std::byte(0x4f), std::byte(0x67), std::byte(0xdc), std::byte(0xea), std::byte(0x97), std::byte(0xf2), std::byte(0xcf), std::byte(0xce), std::byte(0xf0), std::byte(0xb4), std::byte(0xe6), std::byte(0x73), /*8*/
        std::byte(0x96), std::byte(0xac), std::byte(0x74), std::byte(0x22), std::byte(0xe7), std::byte(0xad), std::byte(0x35), std::byte(0x85), std::byte(0xe2), std::byte(0xf9), std::byte(0x37), std::byte(0xe8), std::byte(0x1c), std::byte(0x75), std::byte(0xdf), std::byte(0x6e), /*9*/
        std::byte(0x47), std::byte(0xf1), std::byte(0x1a), std::byte(0x71), std::byte(0x1d), std::byte(0x29), std::byte(0xc5), std::byte(0x89), std::byte(0x6f), std::byte(0xb7), std::byte(0x62), std::byte(0x0e), std::byte(0xaa), std::byte(0x18), std::byte(0xbe), std::byte(0x1b), /*a*/
        std::byte(0xfc), std::byte(0x56), std::byte(0x3e), std::byte(0x4b), std::byte(0xc6), std::byte(0xd2), std::byte(0x79), std::byte(0x20), std::byte(0x9a), std::byte(0xdb), std::byte(0xc0), std::byte(0xfe), std::byte(0x78), std::byte(0xcd), std::byte(0x5a), std::byte(0xf4), /*b*/
        std::byte(0x1f), std::byte(0xdd), std::byte(0xa8), std::byte(0x33), std::byte(0x88), std::byte(0x07), std::byte(0xc7), std::byte(0x31), std::byte(0xb1), std::byte(0x12), std::byte(0x10), std::byte(0x59), std::byte(0x27), std::byte(0x80), std::byte(0xec), std::byte(0x5f), /*c*/
        std::byte(0x60), std::byte(0x51), std::byte(0x7f), std::byte(0xa9), std::byte(0x19), std::byte(0xb5), std::byte(0x4a), std::byte(0x0d), std::byte(0x2d), std::byte(0xe5), std::byte(0x7a), std::byte(0x9f), std::byte(0x93), std::byte(0xc9), std::byte(0x9c), std::byte(0xef), /*d*/
        std::byte(0xa0), std::byte(0xe0), std::byte(0x3b), std::byte(0x4d), std::byte(0xae), std::byte(0x2a), std::byte(0xf5), std::byte(0xb0), std::byte(0xc8), std::byte(0xeb), std::byte(0xbb), std::byte(0x3c), std::byte(0x83), std::byte(0x53), std::byte(0x99), std::byte(0x61), /*e*/
        std::byte(0x17), std::byte(0x2b), std::byte(0x04), std::byte(0x7e), std::byte(0xba), std::byte(0x77), std::byte(0xd6), std::byte(0x26), std::byte(0xe1), std::byte(0x69), std::byte(0x14), std::byte(0x63), std::byte(0x55), std::byte(0x21), std::byte(0x0c), std::byte(0x7d)  /*f*/
    };


    byte_device::byte_device(byte_device const & origin)noexcept
        :MEMBER_key(origin.MEMBER_key)
    {
        std::byte buffer[16];

        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
    }
    void byte_device::operator=(byte_device const & origin)noexcept
    {
        if (this == &origin) [[unlikely]]
        {
            return;
        }
        this->MEMBER_key = origin.MEMBER_key;

        std::byte buffer[16];

        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        this->KeyExpansion(buffer);

    }

    byte_device::byte_device(byte_device && origin) noexcept
        :MEMBER_key(std::move(origin.MEMBER_key))
    {
        origin.MEMBER_key = random_aes_key();

        std::byte buffer[16];

        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = this->MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        origin.KeyExpansion(buffer);
    }
    byte_device::byte_device()
        :MEMBER_key(random_aes_key())
    {
        std::byte buffer[16];

        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = this->MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
    }
    void byte_device::operator=(byte_device && origin) noexcept
    {
        if (this == &origin) [[unlikely]]
        {
            return;
        }
        this->MEMBER_key = std::move(origin.MEMBER_key);
        origin.MEMBER_key = random_aes_key();
        std::byte buffer[16];

        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = this->MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        origin.KeyExpansion(buffer);
    }

    byte_device::byte_device(std::byte const * key) noexcept
    {
        for (unsigned int count = 0;count < 16;count++)
        {
            this->MEMBER_key[count] = key[count];
        }
        this->KeyExpansion(key);
    }

    byte_device::byte_device(byte_device::key_type const & key)noexcept
        :MEMBER_key(key)
    {
        std::byte buffer[16];

        for (unsigned int count = 0; count < 16; count++)
        {
            buffer[count] = key[count];
        }
        this->KeyExpansion(buffer);
    }

    byte_device::~byte_device()noexcept
    {
    }

    void byte_device::encrypt(std::byte * const input)const
    {
        std::byte state[4][4];

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

        byte_device::AddRoundKey(state, this->w[0]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[1]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[2]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[3]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[4]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[5]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[6]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[7]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[8]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->w[9]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::AddRoundKey(state, this->w[10]);

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

    byte_device::key_type const & byte_device::key() const noexcept
    {
        return this->MEMBER_key;
    }

    void byte_device::decrypt(std::byte * const input)const
    {
        std::byte  state[4][4];

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

        byte_device::AddRoundKey(state, this->w[10]);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[9]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[8]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[7]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[6]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[5]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[4]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[3]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[2]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[1]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->w[0]);

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
    void* OKps::byte_device::Cipher(void* input, int length)
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
    }this->make_align

    void* OKps::byte_device::InvCipher(void* input, int length)
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

    void byte_device::INNER_KeyExpansion(int const i, std::byte const rc[]) noexcept
    {
        std::byte  t[4];

        t[0] = 0 ? this->w[i][0][0 - 1] : this->w[i - 1][0][3];
        t[1] = 0 ? this->w[i][1][0 - 1] : this->w[i - 1][1][3];
        t[2] = 0 ? this->w[i][2][0 - 1] : this->w[i - 1][2][3];
        t[3] = 0 ? this->w[i][3][0 - 1] : this->w[i - 1][3][3];

        std::byte temp = t[0];

        t[0] = byte_device::Sbox[(unsigned int)(t[1])];
        t[1] = byte_device::Sbox[(unsigned int)(t[2])];
        t[2] = byte_device::Sbox[(unsigned int)(t[3])];
        t[3] = byte_device::Sbox[(unsigned int)(t[0])];

        t[3] = byte_device::Sbox[(unsigned int)(temp)];
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

    void byte_device::KeyExpansion(std::byte const * key) noexcept
    {
        std::byte rc[] = {std::byte(0x01), std::byte(0x02), std::byte(0x04), std::byte(0x08), std::byte(0x10), std::byte(0x20), std::byte(0x40), std::byte(0x80), std::byte(0x1b), std::byte(0x36)};

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

    std::byte byte_device::FFmul(std::byte const & a, std::byte const & b)noexcept
    {
        std::byte bw[4];
        std::byte res = std::byte(0);
        bw[0] = b;

        bw[1] = bw[0] << 1;
        if ((bw[0] & std::byte(0x80)) != std::byte(0))
        {
            bw[1] ^= std::byte(0x1b);
        }

        bw[2] = bw[1] << 1;
        if ((bw[1] & std::byte(0x80)) != std::byte(0))
        {
            bw[2] ^= std::byte(0x1b);
        }

        bw[3] = bw[2] << 1;
        if ((bw[2] & std::byte(0x80)) != std::byte(0))
        {
            bw[3] ^= std::byte(0x1b);
        }

        if (((a >> 0) & std::byte(0x01)) != std::byte(0))
        {
            res ^= bw[0];
        }

        if (((a >> 1) & std::byte(0x01)) != std::byte(0))
        {
            res ^= bw[1];
        }

        if (((a >> 2) & std::byte(0x01)) != std::byte(0))
        {
            res ^= bw[2];
        }

        if (((a >> 3) & std::byte(0x01)) != std::byte(0))
        {
            res ^= bw[3];
        }

        return res;
    }

    void byte_device::SubBytes(std::byte state[][4])noexcept
    {

        state[0][0] = byte_device::Sbox[unsigned int(state[0][0])];
        state[0][1] = byte_device::Sbox[unsigned int(state[0][1])];
        state[0][2] = byte_device::Sbox[unsigned int(state[0][2])];
        state[0][3] = byte_device::Sbox[unsigned int(state[0][3])];

        state[1][0] = byte_device::Sbox[unsigned int(state[1][0])];
        state[1][1] = byte_device::Sbox[unsigned int(state[1][1])];
        state[1][2] = byte_device::Sbox[unsigned int(state[1][2])];
        state[1][3] = byte_device::Sbox[unsigned int(state[1][3])];

        state[2][0] = byte_device::Sbox[unsigned int(state[2][0])];
        state[2][1] = byte_device::Sbox[unsigned int(state[2][1])];
        state[2][2] = byte_device::Sbox[unsigned int(state[2][2])];
        state[2][3] = byte_device::Sbox[unsigned int(state[2][3])];

        state[3][0] = byte_device::Sbox[unsigned int(state[3][0])];
        state[3][1] = byte_device::Sbox[unsigned int(state[3][1])];
        state[3][2] = byte_device::Sbox[unsigned int(state[3][2])];
        state[3][3] = byte_device::Sbox[unsigned int(state[3][3])];
    }

    void byte_device::ShiftRows(std::byte state[][4])noexcept
    {
        std::byte t[4];

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

    void byte_device::MixColumns(std::byte state[][4])noexcept
    {
        std::byte t[4];

        t[0] = state[0][0];
        t[1] = state[1][0];
        t[2] = state[2][0];
        t[3] = state[3][0];

        state[0][0] = byte_device::FFmul(std::byte(0x02), t[0]) ^ byte_device::FFmul(std::byte(0x03), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]);

        state[1][0] = byte_device::FFmul(std::byte(0x02), t[1]) ^ byte_device::FFmul(std::byte(0x03), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]);

        state[2][0] = byte_device::FFmul(std::byte(0x02), t[2]) ^ byte_device::FFmul(std::byte(0x03), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]);

        state[3][0] = byte_device::FFmul(std::byte(0x02), t[3]) ^ byte_device::FFmul(std::byte(0x03), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[6 % 4]);

        t[0] = state[0][1];
        t[1] = state[1][1];
        t[2] = state[2][1];
        t[3] = state[3][1];

        state[0][1] = byte_device::FFmul(std::byte(0x02), t[0]) ^ byte_device::FFmul(std::byte(0x03), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]);

        state[1][1] = byte_device::FFmul(std::byte(0x02), t[1]) ^ byte_device::FFmul(std::byte(0x03), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]);

        state[2][1] = byte_device::FFmul(std::byte(0x02), t[2]) ^ byte_device::FFmul(std::byte(0x03), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]);

        state[3][1] = byte_device::FFmul(std::byte(0x02), t[3]) ^ byte_device::FFmul(std::byte(0x03), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[6 % 4]);

        t[0] = state[0][2];
        t[1] = state[1][2];
        t[2] = state[2][2];
        t[3] = state[3][2];

        state[0][2] = byte_device::FFmul(std::byte(0x02), t[0]) ^ byte_device::FFmul(std::byte(0x03), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]);
        ;

        state[1][2] = byte_device::FFmul(std::byte(0x02), t[1]) ^ byte_device::FFmul(std::byte(0x03), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]);

        state[2][2] = byte_device::FFmul(std::byte(0x02), t[2]) ^ byte_device::FFmul(std::byte(0x03), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]);

        state[3][2] = byte_device::FFmul(std::byte(0x02), t[3]) ^ byte_device::FFmul(std::byte(0x03), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[6 % 4]);

        t[0] = state[0][3];
        t[1] = state[1][3];
        t[2] = state[2][3];
        t[3] = state[3][3];

        state[0][3] = byte_device::FFmul(std::byte(0x02), t[0]) ^ byte_device::FFmul(std::byte(0x03), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]);

        state[1][3] = byte_device::FFmul(std::byte(0x02), t[1]) ^ byte_device::FFmul(std::byte(0x03), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]);

        state[2][3] = byte_device::FFmul(std::byte(0x02), t[2]) ^ byte_device::FFmul(std::byte(0x03), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]);

        state[3][3] = byte_device::FFmul(std::byte(0x02), t[3]) ^ byte_device::FFmul(std::byte(0x03), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x01), t[6 % 4]);
    }

    void byte_device::AddRoundKey(std::byte state[][4], std::byte const k[][4]) noexcept
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

    void byte_device::InvSubBytes(std::byte state[][4])noexcept
    {

        state[0][0] = byte_device::InvSbox[unsigned int(state[0][0])];
        state[0][1] = byte_device::InvSbox[unsigned int(state[0][1])];
        state[0][2] = byte_device::InvSbox[unsigned int(state[0][2])];
        state[0][3] = byte_device::InvSbox[unsigned int(state[0][3])];

        state[1][0] = byte_device::InvSbox[unsigned int(state[1][0])];
        state[1][1] = byte_device::InvSbox[unsigned int(state[1][1])];
        state[1][2] = byte_device::InvSbox[unsigned int(state[1][2])];
        state[1][3] = byte_device::InvSbox[unsigned int(state[1][3])];

        state[2][0] = byte_device::InvSbox[unsigned int(state[2][0])];
        state[2][1] = byte_device::InvSbox[unsigned int(state[2][1])];
        state[2][2] = byte_device::InvSbox[unsigned int(state[2][2])];
        state[2][3] = byte_device::InvSbox[unsigned int(state[2][3])];

        state[3][0] = byte_device::InvSbox[unsigned int(state[3][0])];
        state[3][1] = byte_device::InvSbox[unsigned int(state[3][1])];
        state[3][2] = byte_device::InvSbox[unsigned int(state[3][2])];
        state[3][3] = byte_device::InvSbox[unsigned int(state[3][3])];
    }

    void byte_device::InvShiftRows(std::byte state[][4])noexcept
    {
        std::byte t[4];

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

    void byte_device::InvMixColumns(std::byte state[][4])noexcept
    {
        std::byte t[4];

        t[0] = state[0][0];
        t[1] = state[1][0];
        t[2] = state[2][0];
        t[3] = state[3][0];

        state[0][0] = byte_device::FFmul(std::byte(0x0e), t[0]) ^ byte_device::FFmul(std::byte(0x0b), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[3 % 4]);

        state[1][0] = byte_device::FFmul(std::byte(0x0e), t[1]) ^ byte_device::FFmul(std::byte(0x0b), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[4 % 4]);

        state[2][0] = byte_device::FFmul(std::byte(0x0e), t[2]) ^ byte_device::FFmul(std::byte(0x0b), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[5 % 4]);

        state[3][0] = byte_device::FFmul(std::byte(0x0e), t[3]) ^ byte_device::FFmul(std::byte(0x0b), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[6 % 4]);

        t[0] = state[0][1];
        t[1] = state[1][1];
        t[2] = state[2][1];
        t[3] = state[3][1];

        state[0][1] = byte_device::FFmul(std::byte(0x0e), t[0]) ^ byte_device::FFmul(std::byte(0x0b), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[3 % 4]);

        state[1][1] = byte_device::FFmul(std::byte(0x0e), t[1]) ^ byte_device::FFmul(std::byte(0x0b), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[4 % 4]);

        state[2][1] = byte_device::FFmul(std::byte(0x0e), t[2]) ^ byte_device::FFmul(std::byte(0x0b), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[5 % 4]);

        state[3][1] = byte_device::FFmul(std::byte(0x0e), t[3]) ^ byte_device::FFmul(std::byte(0x0b), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[6 % 4]);

        t[0] = state[0][2];
        t[1] = state[1][2];
        t[2] = state[2][2];
        t[3] = state[3][2];

        state[0][2] = byte_device::FFmul(std::byte(0x0e), t[0]) ^ byte_device::FFmul(std::byte(0x0b), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[3 % 4]);

        state[1][2] = byte_device::FFmul(std::byte(0x0e), t[1]) ^ byte_device::FFmul(std::byte(0x0b), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[4 % 4]);

        state[2][2] = byte_device::FFmul(std::byte(0x0e), t[2]) ^ byte_device::FFmul(std::byte(0x0b), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[5 % 4]);

        state[3][2] = byte_device::FFmul(std::byte(0x0e), t[3]) ^ byte_device::FFmul(std::byte(0x0b), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[6 % 4]);

        t[0] = state[0][3];
        t[1] = state[1][3];
        t[2] = state[2][3];
        t[3] = state[3][3];

        state[0][3] = byte_device::FFmul(std::byte(0x0e), t[0]) ^ byte_device::FFmul(std::byte(0x0b), t[1 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[3 % 4]);

        state[1][3] = byte_device::FFmul(std::byte(0x0e), t[1]) ^ byte_device::FFmul(std::byte(0x0b), t[2 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[4 % 4]);

        state[2][3] = byte_device::FFmul(std::byte(0x0e), t[2]) ^ byte_device::FFmul(std::byte(0x0b), t[3 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[5 % 4]);

        state[3][3] = byte_device::FFmul(std::byte(0x0e), t[3]) ^ byte_device::FFmul(std::byte(0x0b), t[4 % 4]) ^ byte_device::FFmul(std::byte(0x0d), t[5 % 4]) ^ byte_device::FFmul(std::byte(0x09), t[6 % 4]);
    }

    void byte_device::make_align(std::byte * buffer, const unsigned int count)
    {
        if (count > 16)
        {
            throw std::invalid_argument("末尾对齐处理的位数不合法");
        }
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<unsigned int> distribution(0, unsigned int(std::numeric_limits<unsigned char>::max())); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

        for (unsigned int counter = count; counter < 16; counter++)
        {
            buffer[counter] = std::byte(unsigned char(distribution(random_engine)));
        }
    }

    void byte_device::make_align(char * buffer, const unsigned int count)
    {
        if (count > 16)
        {
            throw std::invalid_argument("末尾对齐处理的位数不合法");
        }
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<unsigned int> distribution(0, unsigned int(std::numeric_limits<unsigned char>::max())); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

        for (unsigned int counter = count; counter < 16; counter++)
        {
            buffer[counter] = char(unsigned char(distribution(random_engine)));
        }
    }

    void byte_device::align_encrypt(std::string & origin)const
    {
        std::size_t count = 0;
        while (count + 16 <= origin.size())
        {
            count += 16;
        }
        count = origin.size() - count; // 末尾对齐的原文位数
        this->encrypt(origin);
        std::byte buffer[16];

        for (std::size_t count1 = 0; count1 < count; count1++)
        {
            buffer[count1] = value_cast(origin[origin.size() - count + count1]);
        }
        byte_device::make_align(buffer, count);
        this->encrypt(buffer);
        for (std::size_t count1 = 0; count1 < count; count1++)
        {
            origin[origin.size() - count + count1] = value_cast(buffer[count1]);
        }
        for (int count1 = count; count1 < 16; count1++)
        {
            origin.push_back(value_cast(buffer[count1]));
        }
        origin.push_back(value_cast((std::byte)((std::underlying_type_t<std::byte>)count)));

    }

    void byte_device::align_encrypt(byte_type & origin)const
    {
        std::size_t count = 0;
        while (count + 16 <= origin.size())
        {
            count += 16;
        }
        count = origin.size() - count; // 末尾对齐的原文位数
        this->encrypt(origin);
        std::byte buffer[16];

        for (std::size_t count1 = 0; count1 < count; count1++)
        {
            buffer[count1] = origin[origin.size() - count + count1];
        }
        byte_device::make_align(buffer, count);
        this->encrypt(buffer);
        for (std::size_t count1 = 0; count1 < count; count1++)
        {
            origin[origin.size() - count + count1] = buffer[count1];
        }
        for (int count1 = count; count1 < 16; count1++)
        {
            origin.push_back(buffer[count1]);
        }
        origin.push_back((std::byte)((std::underlying_type_t<std::byte>)count));
    }

    void byte_device::align_decrypt(std::string & origin)const
    {
        if (origin.size() % 16 != 1)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        unsigned int const count = unsigned int(origin[origin.size() - 1]);
        if (count > 16)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        origin.pop_back();
        this->decrypt(origin);
        for (unsigned int count1 = 0; count1 < (unsigned int)16 - count; count1++)
        {
            origin.pop_back();
        }

    }

    void byte_device::align_decrypt(byte_type & origin)const
    {
        if (origin.size() % 16 != 1)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        unsigned int const count = unsigned int(origin[origin.size() - 1]);
        if (count > 16)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        origin.pop_back();
        this->decrypt(origin);
        for (unsigned int count1 = 0; count1 < (unsigned int)16 - count; count1++)
        {
            origin.pop_back();
        }
    }
    void file_device::decrypt(const std::string & origin_route, const std::string & result_route, const key_type & key, const std::size_t thread_count)
    {
        this->decrypt((TYPE_path)origin_route, (TYPE_path)result_route, key, thread_count);
    }
    void file_device::decrypt_work()
    {
        std::vector<TYPE_reader> origin;
        std::vector<TYPE_writter> result;
        origin.resize(this->MEMBER_thread_number);
        result.resize(this->MEMBER_thread_number);
        for (std::uintmax_t counter_1 = 0; counter_1 < this->MEMBER_thread_number; counter_1++)
        {
            origin[counter_1] = (std::make_shared<std::ifstream>());
            origin[counter_1]->open(this->MEMBER_origin_route, std::ios::in | std::ios::binary);
            if (not origin[counter_1]->is_open())
            {
                std::string hinter = std::string("为读取文件 ")
                    + this->MEMBER_origin_route.string()
                    + " 创建第 "
                    + std::to_string(counter_1 + 1)
                    + " 个std::ifstream对象时失败";
                try
                {
                    throw std::runtime_error(hinter);
                }
                catch (std::runtime_error const & error)
                {
                    this->MEMBER_error = std::current_exception();
                }
                for (std::uintmax_t counter_2 = 0; counter_2 < counter_1; counter_2++)
                {
                    origin[counter_2]->close();
                }

                return;
            }
        }

        for (std::uintmax_t counter_1 = 0; counter_1 < this->MEMBER_thread_number; counter_1++)
        {
            result[counter_1] = (std::make_shared<std::ofstream>());
            result[counter_1]->open(this->MEMBER_result_route, std::ios::out | std::ios::binary);
            if (not result[counter_1]->is_open())
            {
                std::string hinter = std::string("为写入文件 ")
                    + this->MEMBER_result_route.string()
                    + " 创建第 "
                    + std::to_string(counter_1 + 1)
                    + " 个std::ofstream对象时失败";
                try
                {
                    throw std::runtime_error(hinter);
                }
                catch (std::runtime_error const & error)
                {
                    this->MEMBER_error = std::current_exception();
                }
                for (std::uintmax_t counter_2 = 0; counter_2 < counter_1; counter_2++)
                {
                    result[counter_2]->close();
                }
                for (std::uintmax_t counter_2 = 0; counter_2 < this->MEMBER_thread_number; counter_2++)
                {
                    origin[counter_2]->close();
                }

                return;
            }
        }

        /*
        分配任务
        原文为 16n+16+1 字节，其中1是end_info信息，即末尾对齐的位数，16是末尾对齐处理的末尾块，16n是 n 个一般的密文块
        把16n分给 m 个加密器线程，它们全部运行结束以后，再由本函数处理 16+1
        注意byte_device算法的密文必须以16字节为一组，得到16字节明文，所以要分配的不是 16n 个字节，而是 n 个 16字节块
        则应该分给每个加密器线程 n/m 块，而留下 n-(n/m)*m 块，再加上末尾的 16+1 字节，由本函数处理
        */
        // 一般16字节密文块的块数 n
        auto const block_count = this->MEMBER_file_size / 16 - 1;
        // 加密器线程的任务起始点
        auto begin = (decltype(block_count))(0);
        // 单个加密器线程的任务块数
        auto const single_work_block_count = block_count / this->MEMBER_thread_number;

        std::vector<file_device::TYPE_cipher_ptr> cipher;
        cipher.resize(this->MEMBER_thread_number);
        for (auto count = (decltype(this->MEMBER_thread_number))(0); count < this->MEMBER_thread_number; count++)
        {
            cipher[count] = (std::make_shared<file_device::cipher>(origin[count], result[count], begin, single_work_block_count, this->MEMBER_key));
            cipher[count]->decrypt();
            // 下一个加密器线程的起始点
            begin += single_work_block_count * 16;
        }
        for (auto count = static_cast<decltype(this->MEMBER_thread_number)>(0); count < this->MEMBER_thread_number; count++)
        {
            cipher[count]->join();
        }

        // 所有加密器线程结束以后，现在begin是本函数需要处理的起始点

        byte_device f_aes(this->MEMBER_key);
        {
            char buffer[16];
            // 处理剩余的 n-(n/m)*m 块
            for (auto count = static_cast<decltype(block_count)>(0); count < block_count - single_work_block_count * this->MEMBER_thread_number; count++)
            {

                origin[this->MEMBER_thread_number - 1]->read(buffer, 16);

                std::string TEMP_buffer = file_device::INNER_buffer_string(buffer, 16);

                f_aes.decrypt(TEMP_buffer);

                result[this->MEMBER_thread_number - 1]->write(TEMP_buffer.c_str(), 16);
            }
        }
        // 处理末尾对齐
        {
            char buffer[17];
            // 读取最后16+1字节，则buf[16]是 end_info ，末尾对齐处理的位数

            origin[this->MEMBER_thread_number - 1]->read(buffer, 17);

            std::string TEMP_buffer = file_device::INNER_buffer_string(buffer, 17);

            f_aes.decrypt(TEMP_buffer);

            result[this->MEMBER_thread_number - 1]->write(TEMP_buffer.c_str(), (unsigned int)(TEMP_buffer[16]));

        }

        for (std::uintmax_t counter_2 = 0; counter_2 < this->MEMBER_thread_number; counter_2++)
        {
            origin[counter_2]->close();
            result[counter_2]->close();
        }
    }

    byte_device::key_type byte_device::random_aes_key()
    {
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<unsigned int> distribution(0, unsigned int(std::numeric_limits<unsigned char>::max())); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间
        byte_device::key_type result;

        for (std::size_t counter = 0; counter < (std::size_t)16; counter++)
        {

            result[counter] = std::byte(unsigned char(distribution(random_engine))); // 生成一个0到255的随机整数，转换为unsigned char，存储到key中
        }
        return result;
    }

    byte_device::key_type byte_device::random_encrypt(byte_device::byte_type & origin)
    {
        auto key = byte_device::random_aes_key();
        auto worker = byte_device(key);
        worker.align_encrypt(origin);
        return key;
    }
    byte_device::key_type byte_device::random_encrypt(std::string & origin)
    {
        auto key = byte_device::random_aes_key();
        auto worker = byte_device(key);
        worker.align_encrypt(origin);
        return key;
    }

    std::string file_device::INNER_buffer_string(const char * buffer, std::size_t size)
    {
        std::string result;
        result.resize(size);
        for (std::size_t count = 0; count < size; count++)
        {
            result[count] = (buffer[count]);
        }
        return result;
    }
    file_device::file_device()noexcept
        : MEMBER_error(std::current_exception())
    {
    }

    file_device::~file_device()noexcept
    {
    }

    void file_device::encrypt(const std::string & origin_route,
        const std::string & result_route,
        const file_device::key_type & key,
        const std::size_t thread_count)
    {
        this->encrypt((TYPE_path)origin_route, (TYPE_path)result_route, key, thread_count);
    }

    void file_device::encrypt_work()
    {
        std::vector<TYPE_reader> origin;
        std::vector<TYPE_writter> result;
        origin.resize(this->MEMBER_thread_number);
        result.resize(this->MEMBER_thread_number);
        for (std::uintmax_t counter_1 = 0; counter_1 < this->MEMBER_thread_number; counter_1++)
        {
            origin[counter_1] = (std::make_shared<std::ifstream>());
            origin[counter_1]->open(this->MEMBER_origin_route, std::ios::in | std::ios::binary);
            if (not origin[counter_1]->is_open())
            {
                std::string hinter = std::string("为读取文件 ")
                    + this->MEMBER_origin_route.string()
                    + " 创建第 "
                    + std::to_string(counter_1 + 1)
                    + " 个std::ifstream对象时失败";
                try
                {
                    throw std::runtime_error(hinter);
                }
                catch (std::runtime_error const & error)
                {
                    this->MEMBER_error = std::current_exception();
                }
                for (std::uintmax_t counter_2 = 0; counter_2 < counter_1; counter_2++)
                {
                    origin[counter_2]->close();
                }

                return;
            }
        }

        for (std::uintmax_t counter_1 = 0; counter_1 < this->MEMBER_thread_number; counter_1++)
        {
            result[counter_1] = (std::make_shared<std::ofstream>());
            result[counter_1]->open(this->MEMBER_result_route, std::ios::out | std::ios::binary);
            if (not result[counter_1]->is_open())
            {
                std::string hinter = std::string("为写入文件 ")
                    + this->MEMBER_result_route.string()
                    + " 创建第 "
                    + std::to_string(counter_1 + 1)
                    + " 个std::ofstream对象时失败";
                try
                {
                    throw std::runtime_error(hinter);
                }
                catch (std::runtime_error const & error)
                {
                    this->MEMBER_error = std::current_exception();
                }
                for (std::uintmax_t counter_2 = 0; counter_2 < counter_1; counter_2++)
                {
                    result[counter_2]->close();
                }
                for (std::uintmax_t counter_2 = 0; counter_2 < this->MEMBER_thread_number; counter_2++)
                {
                    origin[counter_2]->close();
                }

                return;
            }
        }

        /*
        分配任务
         原文为 16n+end_info 字节，其中end_info字节是需要末尾对齐的部分
         16n 分给 m 个加密器线程处理，它们全部结束以后，end_info 字节由本函数处理
         注意basic_aes_device算法的原文必须以16字节为一组，得到16字节密文，所以要分配的不是 16n 个字节，而是 n 个 16字节块
         则应该分给每个加密器线程 n/m 块，而留下 n-(n/m)*m 块，再加上末尾对齐的 end_info 字节，由本函数处理，处理时应该使用下标[this->cipher_number-1]的文件对象和this->write_size数组
        */
        // 原文的总块数
        auto const block_count = this->MEMBER_file_size / byte_device::key_length;
        // 末尾对齐处理的位数
        auto end_info = (unsigned int)(this->MEMBER_file_size - block_count * byte_device::key_length);
        // 加密器线程的任务起始点
        auto begin = (decltype(block_count))(0);
        // 单个加密器线程的任务块数
        const auto single_work_block_count = block_count / this->MEMBER_thread_number;

        std::vector<file_device::TYPE_cipher_ptr> cipher;
        cipher.resize(this->MEMBER_thread_number);
        for (auto count = (decltype(this->MEMBER_thread_number))(0); count < this->MEMBER_thread_number; count++)
        {
            cipher[count] = (std::make_shared<file_device::cipher>(origin[count], result[count], begin, single_work_block_count, this->MEMBER_key));
            cipher[count]->encrypt();
            // 下一个加密器线程的起始点
            begin += single_work_block_count * 16;
        }
        for (auto count = (decltype(this->MEMBER_thread_number))(0); count < this->MEMBER_thread_number; count++)
        {
            cipher[count]->join();
        }

        // 所有加密器线程结束以后，现在begin是本函数需要处理的起始点

        char buffer[16]; // 16字节缓存，一组明文
        byte_device f_aes(this->MEMBER_key);
        // 处理原文中剩余的 完整16字节块
        for (auto count = (decltype(block_count))(0); count < block_count - single_work_block_count * this->MEMBER_thread_number; count++)
        {
            origin[this->MEMBER_thread_number - 1]->read(buffer, byte_device::key_length);
            std::string TEMP_buffer = file_device::INNER_buffer_string(buffer, byte_device::key_length);
            f_aes.encrypt(TEMP_buffer);
            result[this->MEMBER_thread_number - 1]->write(TEMP_buffer.c_str(), byte_device::key_length);
        }
        // 处理末尾对齐

        origin[this->MEMBER_thread_number - 1]->read(buffer, end_info);

        f_aes.make_align(buffer, end_info);
        std::string TEMP_buffer = file_device::INNER_buffer_string(buffer, byte_device::key_length);
        f_aes.encrypt(TEMP_buffer);

        result[this->MEMBER_thread_number - 1]->write(TEMP_buffer.c_str(), byte_device::key_length);

        {
            auto info = (char)(end_info);

            result[this->MEMBER_thread_number - 1]->write(&info, 1);

        }

        for (std::uintmax_t counter_2 = 0; counter_2 < this->MEMBER_thread_number; counter_2++)
        {
            origin[counter_2]->close();
            result[counter_2]->close();
        }

    }

    file_device::cipher::cipher(TYPE_reader const & origin,
        TYPE_writter const & result,
        const std::size_t begin,
        const std::size_t block_number,
        const key_type & key)
        : begin(begin)
        , block_number(block_number)
        , key(key)
        , origin(origin)
        , result(result)
    {

    }

    file_device::cipher::~cipher()noexcept
    {
    }

    void file_device::cipher::encrypt()
    {
        this->MEMBER_worker = std::thread(&file_device::cipher::encrypt_work, this);
    }
    void file_device::cipher::decrypt()
    {
        this->MEMBER_worker = std::thread(&file_device::cipher::decrypt_work, this);
    }
    void file_device::cipher::encrypt_work()
    {
        byte_device aes_calculator(this->key);
        char buf[byte_device::key_length];
        this->origin->seekg(this->begin);
        this->result->seekp(this->begin);
        for (auto count = (decltype(this->block_number))(0); count < this->block_number; count++)
        {
            this->origin->read(buf, byte_device::key_length);
            std::string TEMP_buffer = file_device::INNER_buffer_string(buf, byte_device::key_length);
            aes_calculator.encrypt(TEMP_buffer);
            this->result->write(TEMP_buffer.c_str(), byte_device::key_length);
        }
    }
    void file_device::cipher::decrypt_work()
    {
        byte_device aes_calculator(this->key);
        char buf[byte_device::key_length];
        this->origin->seekg(this->begin);
        this->result->seekp(this->begin);
        for (auto count = static_cast<decltype(this->block_number)>(0); count < this->block_number; count++)
        {
            this->origin->read(buf, byte_device::key_length);
            std::string TEMP_buffer = file_device::INNER_buffer_string(buf, byte_device::key_length);
            aes_calculator.decrypt(TEMP_buffer);
            this->result->write(TEMP_buffer.c_str(), byte_device::key_length);
        }
    }
    void file_device::cipher::join()
    {
        this->MEMBER_worker.join();
    }
    void byte_device::encrypt(std::string & origin)const
    {
        std::byte buffer[byte_device::key_length];
        for (size_t count = 0; count + byte_device::key_length <= origin.size(); count += byte_device::key_length)
        {

            /*
            初始化缓冲区
            手动循环展开，因为msvc不会优化
            */
            buffer[0] = value_cast(origin[count]);
            buffer[1] = value_cast(origin[count + 1]);
            buffer[2] = value_cast(origin[count + 2]);
            buffer[3] = value_cast(origin[count + 3]);
            buffer[4] = value_cast(origin[count + 4]);
            buffer[5] = value_cast(origin[count + 5]);
            buffer[6] = value_cast(origin[count + 6]);
            buffer[7] = value_cast(origin[count + 7]);
            buffer[8] = value_cast(origin[count + 8]);
            buffer[9] = value_cast(origin[count + 9]);
            buffer[10] = value_cast(origin[count + 10]);
            buffer[11] = value_cast(origin[count + 11]);
            buffer[12] = value_cast(origin[count + 12]);
            buffer[13] = value_cast(origin[count + 13]);
            buffer[14] = value_cast(origin[count + 14]);
            buffer[15] = value_cast(origin[count + 15]);

            this->encrypt(buffer);

            /*
            密文写入原串
            */
            origin[count] = value_cast(buffer[0]);
            origin[count + 1] = value_cast(buffer[1]);
            origin[count + 2] = value_cast(buffer[2]);
            origin[count + 3] = value_cast(buffer[3]);
            origin[count + 4] = value_cast(buffer[4]);
            origin[count + 5] = value_cast(buffer[5]);
            origin[count + 6] = value_cast(buffer[6]);
            origin[count + 7] = value_cast(buffer[7]);
            origin[count + 8] = value_cast(buffer[8]);
            origin[count + 9] = value_cast(buffer[9]);
            origin[count + 10] = value_cast(buffer[10]);
            origin[count + 11] = value_cast(buffer[11]);
            origin[count + 12] = value_cast(buffer[12]);
            origin[count + 13] = value_cast(buffer[13]);
            origin[count + 14] = value_cast(buffer[14]);
            origin[count + 15] = value_cast(buffer[15]);
        }
    }
    void byte_device::encrypt(byte_type & origin)const
    {

        std::byte buffer[byte_device::key_length];
        for (size_t count = 0; count + byte_device::key_length <= origin.size(); count += byte_device::key_length)
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

            this->encrypt(buffer);

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
    void file_device::encrypt(TYPE_path const & origin_route, TYPE_path const & result_route, const key_type & key, const std::size_t thread_count)
    {
        this->MEMBER_error = std::current_exception();
        this->MEMBER_file_size = std::filesystem::file_size(origin_route);
        this->MEMBER_key = key;
        this->MEMBER_origin_route = origin_route;
        this->MEMBER_result_route = result_route;
        if (thread_count == 0)
        {
            auto const hardware_threads = std::thread::hardware_concurrency();
            if (hardware_threads == 0)
            {
                this->MEMBER_thread_number = 1;
            }
            else
            {
                this->MEMBER_thread_number = hardware_threads;
            }
        }
        else
        {
            this->MEMBER_thread_number = thread_count;
        }
        std::size_t constexpr const TEMP_least_work = (std::size_t)(1024) * 64;//每个线程最少要处理64kb数据，否则太浪费
        if (this->MEMBER_file_size / this->MEMBER_thread_number < TEMP_least_work)
        {
            this->MEMBER_thread_number = this->MEMBER_file_size / TEMP_least_work;
            if (this->MEMBER_thread_number == 0)
            {
                this->MEMBER_thread_number = 1;
            }
        }

        this->MEMBER_worker = std::thread(&file_device::encrypt_work, this);
        this->MEMBER_worker.join();
        if (this->MEMBER_error)
        {
            std::rethrow_exception(this->MEMBER_error);
        }
    }
    void file_device::decrypt(TYPE_path const & origin_route, TYPE_path const & result_route, const key_type & key, const std::size_t thread_count)
    {
        this->MEMBER_error = std::current_exception();
        this->MEMBER_file_size = std::filesystem::file_size(origin_route);
        if (not (this->MEMBER_file_size >= 17 and this->MEMBER_file_size % 16 == 1))
        {
            std::string hinter = std::string("要解密的文件 ")
                + origin_route.string()
                + " 格式错误";
            throw std::invalid_argument(hinter);
        }
        this->MEMBER_key = key;
        this->MEMBER_origin_route = origin_route;
        this->MEMBER_result_route = result_route;
        if (thread_count == 0)
        {
            auto const hardware_threads = std::thread::hardware_concurrency();
            if (hardware_threads == 0)
            {
                this->MEMBER_thread_number = 1;
            }
            else
            {
                this->MEMBER_thread_number = hardware_threads;
            }
        }
        else
        {
            this->MEMBER_thread_number = thread_count;
        }
        std::size_t constexpr const TEMP_least_work = (std::size_t)(1024) * 64;//每个线程最少要处理64kb数据，否则太浪费
        if (this->MEMBER_file_size / this->MEMBER_thread_number < TEMP_least_work)
        {
            this->MEMBER_thread_number = this->MEMBER_file_size / TEMP_least_work;
            if (this->MEMBER_thread_number == 0)
            {
                this->MEMBER_thread_number = 1;
            }
        }

        this->MEMBER_worker = std::thread(&file_device::decrypt_work, this);
        this->MEMBER_worker.join();
        if (this->MEMBER_error)
        {
            std::rethrow_exception(this->MEMBER_error);
        }

    }
    void byte_device::decrypt(std::string & origin)const
    {
        std::byte buffer[byte_device::key_length];
        for (size_t count = 0; count + byte_device::key_length <= origin.size(); count += byte_device::key_length)
        {

            /*
            初始化缓冲区
            手动循环展开，因为msvc不会优化
            */
            buffer[0] = value_cast(origin[count]);
            buffer[1] = value_cast(origin[count + 1]);
            buffer[2] = value_cast(origin[count + 2]);
            buffer[3] = value_cast(origin[count + 3]);
            buffer[4] = value_cast(origin[count + 4]);
            buffer[5] = value_cast(origin[count + 5]);
            buffer[6] = value_cast(origin[count + 6]);
            buffer[7] = value_cast(origin[count + 7]);
            buffer[8] = value_cast(origin[count + 8]);
            buffer[9] = value_cast(origin[count + 9]);
            buffer[10] = value_cast(origin[count + 10]);
            buffer[11] = value_cast(origin[count + 11]);
            buffer[12] = value_cast(origin[count + 12]);
            buffer[13] = value_cast(origin[count + 13]);
            buffer[14] = value_cast(origin[count + 14]);
            buffer[15] = value_cast(origin[count + 15]);

            this->decrypt(buffer);

            /*
            密文写入原串
            */
            origin[count] = value_cast(buffer[0]);
            origin[count + 1] = value_cast(buffer[1]);
            origin[count + 2] = value_cast(buffer[2]);
            origin[count + 3] = value_cast(buffer[3]);
            origin[count + 4] = value_cast(buffer[4]);
            origin[count + 5] = value_cast(buffer[5]);
            origin[count + 6] = value_cast(buffer[6]);
            origin[count + 7] = value_cast(buffer[7]);
            origin[count + 8] = value_cast(buffer[8]);
            origin[count + 9] = value_cast(buffer[9]);
            origin[count + 10] = value_cast(buffer[10]);
            origin[count + 11] = value_cast(buffer[11]);
            origin[count + 12] = value_cast(buffer[12]);
            origin[count + 13] = value_cast(buffer[13]);
            origin[count + 14] = value_cast(buffer[14]);
            origin[count + 15] = value_cast(buffer[15]);
        }

    }
    void byte_device::decrypt(byte_type & origin)const
    {

        std::byte buffer[byte_device::key_length];
        for (size_t count = 0; count + byte_device::key_length <= origin.size(); count += byte_device::key_length)
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

            this->decrypt(buffer);

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
