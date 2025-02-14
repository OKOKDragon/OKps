
#include <random>

#include ".\value_cast.hpp"

#include ".\aes.hpp"


namespace OKps::AES
{
    bool byte_device::operator ==(byte_device const & right)const noexcept
    {
        return this->MEMBER_key == right.MEMBER_key;
    }
    bool byte_device::operator !=(byte_device const & right)const noexcept
    {
        return this->MEMBER_key != right.MEMBER_key;
    }
    std::byte const byte_device::MEMBER_Sbox[256] =
    {
        /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
        value_cast<std::byte>(0x63), value_cast<std::byte>(0x7c), value_cast<std::byte>(0x77), value_cast<std::byte>(0x7b), value_cast<std::byte>(0xf2), value_cast<std::byte>(0x6b), value_cast<std::byte>(0x6f), value_cast<std::byte>(0xc5), value_cast<std::byte>(0x30), value_cast<std::byte>(0x01), value_cast<std::byte>(0x67), value_cast<std::byte>(0x2b), value_cast<std::byte>(0xfe), value_cast<std::byte>(0xd7), value_cast<std::byte>(0xab), value_cast<std::byte>(0x76), /*0*/
        value_cast<std::byte>(0xca), value_cast<std::byte>(0x82), value_cast<std::byte>(0xc9), value_cast<std::byte>(0x7d), value_cast<std::byte>(0xfa), value_cast<std::byte>(0x59), value_cast<std::byte>(0x47), value_cast<std::byte>(0xf0), value_cast<std::byte>(0xad), value_cast<std::byte>(0xd4), value_cast<std::byte>(0xa2), value_cast<std::byte>(0xaf), value_cast<std::byte>(0x9c), value_cast<std::byte>(0xa4), value_cast<std::byte>(0x72), value_cast<std::byte>(0xc0), /*1*/
        value_cast<std::byte>(0xb7), value_cast<std::byte>(0xfd), value_cast<std::byte>(0x93), value_cast<std::byte>(0x26), value_cast<std::byte>(0x36), value_cast<std::byte>(0x3f), value_cast<std::byte>(0xf7), value_cast<std::byte>(0xcc), value_cast<std::byte>(0x34), value_cast<std::byte>(0xa5), value_cast<std::byte>(0xe5), value_cast<std::byte>(0xf1), value_cast<std::byte>(0x71), value_cast<std::byte>(0xd8), value_cast<std::byte>(0x31), value_cast<std::byte>(0x15), /*2*/
        value_cast<std::byte>(0x04), value_cast<std::byte>(0xc7), value_cast<std::byte>(0x23), value_cast<std::byte>(0xc3), value_cast<std::byte>(0x18), value_cast<std::byte>(0x96), value_cast<std::byte>(0x05), value_cast<std::byte>(0x9a), value_cast<std::byte>(0x07), value_cast<std::byte>(0x12), value_cast<std::byte>(0x80), value_cast<std::byte>(0xe2), value_cast<std::byte>(0xeb), value_cast<std::byte>(0x27), value_cast<std::byte>(0xb2), value_cast<std::byte>(0x75), /*3*/
        value_cast<std::byte>(0x09), value_cast<std::byte>(0x83), value_cast<std::byte>(0x2c), value_cast<std::byte>(0x1a), value_cast<std::byte>(0x1b), value_cast<std::byte>(0x6e), value_cast<std::byte>(0x5a), value_cast<std::byte>(0xa0), value_cast<std::byte>(0x52), value_cast<std::byte>(0x3b), value_cast<std::byte>(0xd6), value_cast<std::byte>(0xb3), value_cast<std::byte>(0x29), value_cast<std::byte>(0xe3), value_cast<std::byte>(0x2f), value_cast<std::byte>(0x84), /*4*/
        value_cast<std::byte>(0x53), value_cast<std::byte>(0xd1), value_cast<std::byte>(0x00), value_cast<std::byte>(0xed), value_cast<std::byte>(0x20), value_cast<std::byte>(0xfc), value_cast<std::byte>(0xb1), value_cast<std::byte>(0x5b), value_cast<std::byte>(0x6a), value_cast<std::byte>(0xcb), value_cast<std::byte>(0xbe), value_cast<std::byte>(0x39), value_cast<std::byte>(0x4a), value_cast<std::byte>(0x4c), value_cast<std::byte>(0x58), value_cast<std::byte>(0xcf), /*5*/
        value_cast<std::byte>(0xd0), value_cast<std::byte>(0xef), value_cast<std::byte>(0xaa), value_cast<std::byte>(0xfb), value_cast<std::byte>(0x43), value_cast<std::byte>(0x4d), value_cast<std::byte>(0x33), value_cast<std::byte>(0x85), value_cast<std::byte>(0x45), value_cast<std::byte>(0xf9), value_cast<std::byte>(0x02), value_cast<std::byte>(0x7f), value_cast<std::byte>(0x50), value_cast<std::byte>(0x3c), value_cast<std::byte>(0x9f), value_cast<std::byte>(0xa8), /*6*/
        value_cast<std::byte>(0x51), value_cast<std::byte>(0xa3), value_cast<std::byte>(0x40), value_cast<std::byte>(0x8f), value_cast<std::byte>(0x92), value_cast<std::byte>(0x9d), value_cast<std::byte>(0x38), value_cast<std::byte>(0xf5), value_cast<std::byte>(0xbc), value_cast<std::byte>(0xb6), value_cast<std::byte>(0xda), value_cast<std::byte>(0x21), value_cast<std::byte>(0x10), value_cast<std::byte>(0xff), value_cast<std::byte>(0xf3), value_cast<std::byte>(0xd2), /*7*/
        value_cast<std::byte>(0xcd), value_cast<std::byte>(0x0c), value_cast<std::byte>(0x13), value_cast<std::byte>(0xec), value_cast<std::byte>(0x5f), value_cast<std::byte>(0x97), value_cast<std::byte>(0x44), value_cast<std::byte>(0x17), value_cast<std::byte>(0xc4), value_cast<std::byte>(0xa7), value_cast<std::byte>(0x7e), value_cast<std::byte>(0x3d), value_cast<std::byte>(0x64), value_cast<std::byte>(0x5d), value_cast<std::byte>(0x19), value_cast<std::byte>(0x73), /*8*/
        value_cast<std::byte>(0x60), value_cast<std::byte>(0x81), value_cast<std::byte>(0x4f), value_cast<std::byte>(0xdc), value_cast<std::byte>(0x22), value_cast<std::byte>(0x2a), value_cast<std::byte>(0x90), value_cast<std::byte>(0x88), value_cast<std::byte>(0x46), value_cast<std::byte>(0xee), value_cast<std::byte>(0xb8), value_cast<std::byte>(0x14), value_cast<std::byte>(0xde), value_cast<std::byte>(0x5e), value_cast<std::byte>(0x0b), value_cast<std::byte>(0xdb), /*9*/
        value_cast<std::byte>(0xe0), value_cast<std::byte>(0x32), value_cast<std::byte>(0x3a), value_cast<std::byte>(0x0a), value_cast<std::byte>(0x49), value_cast<std::byte>(0x06), value_cast<std::byte>(0x24), value_cast<std::byte>(0x5c), value_cast<std::byte>(0xc2), value_cast<std::byte>(0xd3), value_cast<std::byte>(0xac), value_cast<std::byte>(0x62), value_cast<std::byte>(0x91), value_cast<std::byte>(0x95), value_cast<std::byte>(0xe4), value_cast<std::byte>(0x79), /*a*/
        value_cast<std::byte>(0xe7), value_cast<std::byte>(0xc8), value_cast<std::byte>(0x37), value_cast<std::byte>(0x6d), value_cast<std::byte>(0x8d), value_cast<std::byte>(0xd5), value_cast<std::byte>(0x4e), value_cast<std::byte>(0xa9), value_cast<std::byte>(0x6c), value_cast<std::byte>(0x56), value_cast<std::byte>(0xf4), value_cast<std::byte>(0xea), value_cast<std::byte>(0x65), value_cast<std::byte>(0x7a), value_cast<std::byte>(0xae), value_cast<std::byte>(0x08), /*b*/
        value_cast<std::byte>(0xba), value_cast<std::byte>(0x78), value_cast<std::byte>(0x25), value_cast<std::byte>(0x2e), value_cast<std::byte>(0x1c), value_cast<std::byte>(0xa6), value_cast<std::byte>(0xb4), value_cast<std::byte>(0xc6), value_cast<std::byte>(0xe8), value_cast<std::byte>(0xdd), value_cast<std::byte>(0x74), value_cast<std::byte>(0x1f), value_cast<std::byte>(0x4b), value_cast<std::byte>(0xbd), value_cast<std::byte>(0x8b), value_cast<std::byte>(0x8a), /*c*/
        value_cast<std::byte>(0x70), value_cast<std::byte>(0x3e), value_cast<std::byte>(0xb5), value_cast<std::byte>(0x66), value_cast<std::byte>(0x48), value_cast<std::byte>(0x03), value_cast<std::byte>(0xf6), value_cast<std::byte>(0x0e), value_cast<std::byte>(0x61), value_cast<std::byte>(0x35), value_cast<std::byte>(0x57), value_cast<std::byte>(0xb9), value_cast<std::byte>(0x86), value_cast<std::byte>(0xc1), value_cast<std::byte>(0x1d), value_cast<std::byte>(0x9e), /*d*/
        value_cast<std::byte>(0xe1), value_cast<std::byte>(0xf8), value_cast<std::byte>(0x98), value_cast<std::byte>(0x11), value_cast<std::byte>(0x69), value_cast<std::byte>(0xd9), value_cast<std::byte>(0x8e), value_cast<std::byte>(0x94), value_cast<std::byte>(0x9b), value_cast<std::byte>(0x1e), value_cast<std::byte>(0x87), value_cast<std::byte>(0xe9), value_cast<std::byte>(0xce), value_cast<std::byte>(0x55), value_cast<std::byte>(0x28), value_cast<std::byte>(0xdf), /*e*/
        value_cast<std::byte>(0x8c), value_cast<std::byte>(0xa1), value_cast<std::byte>(0x89), value_cast<std::byte>(0x0d), value_cast<std::byte>(0xbf), value_cast<std::byte>(0xe6), value_cast<std::byte>(0x42), value_cast<std::byte>(0x68), value_cast<std::byte>(0x41), value_cast<std::byte>(0x99), value_cast<std::byte>(0x2d), value_cast<std::byte>(0x0f), value_cast<std::byte>(0xb0), value_cast<std::byte>(0x54), value_cast<std::byte>(0xbb), value_cast<std::byte>(0x16) /*f*/
    };

    std::byte const byte_device::MEMBER_InvSbox[256] =
    {
        /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
        value_cast<std::byte>(0x52), value_cast<std::byte>(0x09), value_cast<std::byte>(0x6a), value_cast<std::byte>(0xd5), value_cast<std::byte>(0x30), value_cast<std::byte>(0x36), value_cast<std::byte>(0xa5), value_cast<std::byte>(0x38), value_cast<std::byte>(0xbf), value_cast<std::byte>(0x40), value_cast<std::byte>(0xa3), value_cast<std::byte>(0x9e), value_cast<std::byte>(0x81), value_cast<std::byte>(0xf3), value_cast<std::byte>(0xd7), value_cast<std::byte>(0xfb), /*0*/
        value_cast<std::byte>(0x7c), value_cast<std::byte>(0xe3), value_cast<std::byte>(0x39), value_cast<std::byte>(0x82), value_cast<std::byte>(0x9b), value_cast<std::byte>(0x2f), value_cast<std::byte>(0xff), value_cast<std::byte>(0x87), value_cast<std::byte>(0x34), value_cast<std::byte>(0x8e), value_cast<std::byte>(0x43), value_cast<std::byte>(0x44), value_cast<std::byte>(0xc4), value_cast<std::byte>(0xde), value_cast<std::byte>(0xe9), value_cast<std::byte>(0xcb), /*1*/
        value_cast<std::byte>(0x54), value_cast<std::byte>(0x7b), value_cast<std::byte>(0x94), value_cast<std::byte>(0x32), value_cast<std::byte>(0xa6), value_cast<std::byte>(0xc2), value_cast<std::byte>(0x23), value_cast<std::byte>(0x3d), value_cast<std::byte>(0xee), value_cast<std::byte>(0x4c), value_cast<std::byte>(0x95), value_cast<std::byte>(0x0b), value_cast<std::byte>(0x42), value_cast<std::byte>(0xfa), value_cast<std::byte>(0xc3), value_cast<std::byte>(0x4e), /*2*/
        value_cast<std::byte>(0x08), value_cast<std::byte>(0x2e), value_cast<std::byte>(0xa1), value_cast<std::byte>(0x66), value_cast<std::byte>(0x28), value_cast<std::byte>(0xd9), value_cast<std::byte>(0x24), value_cast<std::byte>(0xb2), value_cast<std::byte>(0x76), value_cast<std::byte>(0x5b), value_cast<std::byte>(0xa2), value_cast<std::byte>(0x49), value_cast<std::byte>(0x6d), value_cast<std::byte>(0x8b), value_cast<std::byte>(0xd1), value_cast<std::byte>(0x25), /*3*/
        value_cast<std::byte>(0x72), value_cast<std::byte>(0xf8), value_cast<std::byte>(0xf6), value_cast<std::byte>(0x64), value_cast<std::byte>(0x86), value_cast<std::byte>(0x68), value_cast<std::byte>(0x98), value_cast<std::byte>(0x16), value_cast<std::byte>(0xd4), value_cast<std::byte>(0xa4), value_cast<std::byte>(0x5c), value_cast<std::byte>(0xcc), value_cast<std::byte>(0x5d), value_cast<std::byte>(0x65), value_cast<std::byte>(0xb6), value_cast<std::byte>(0x92), /*4*/
        value_cast<std::byte>(0x6c), value_cast<std::byte>(0x70), value_cast<std::byte>(0x48), value_cast<std::byte>(0x50), value_cast<std::byte>(0xfd), value_cast<std::byte>(0xed), value_cast<std::byte>(0xb9), value_cast<std::byte>(0xda), value_cast<std::byte>(0x5e), value_cast<std::byte>(0x15), value_cast<std::byte>(0x46), value_cast<std::byte>(0x57), value_cast<std::byte>(0xa7), value_cast<std::byte>(0x8d), value_cast<std::byte>(0x9d), value_cast<std::byte>(0x84), /*5*/
        value_cast<std::byte>(0x90), value_cast<std::byte>(0xd8), value_cast<std::byte>(0xab), value_cast<std::byte>(0x00), value_cast<std::byte>(0x8c), value_cast<std::byte>(0xbc), value_cast<std::byte>(0xd3), value_cast<std::byte>(0x0a), value_cast<std::byte>(0xf7), value_cast<std::byte>(0xe4), value_cast<std::byte>(0x58), value_cast<std::byte>(0x05), value_cast<std::byte>(0xb8), value_cast<std::byte>(0xb3), value_cast<std::byte>(0x45), value_cast<std::byte>(0x06), /*6*/
        value_cast<std::byte>(0xd0), value_cast<std::byte>(0x2c), value_cast<std::byte>(0x1e), value_cast<std::byte>(0x8f), value_cast<std::byte>(0xca), value_cast<std::byte>(0x3f), value_cast<std::byte>(0x0f), value_cast<std::byte>(0x02), value_cast<std::byte>(0xc1), value_cast<std::byte>(0xaf), value_cast<std::byte>(0xbd), value_cast<std::byte>(0x03), value_cast<std::byte>(0x01), value_cast<std::byte>(0x13), value_cast<std::byte>(0x8a), value_cast<std::byte>(0x6b), /*7*/
        value_cast<std::byte>(0x3a), value_cast<std::byte>(0x91), value_cast<std::byte>(0x11), value_cast<std::byte>(0x41), value_cast<std::byte>(0x4f), value_cast<std::byte>(0x67), value_cast<std::byte>(0xdc), value_cast<std::byte>(0xea), value_cast<std::byte>(0x97), value_cast<std::byte>(0xf2), value_cast<std::byte>(0xcf), value_cast<std::byte>(0xce), value_cast<std::byte>(0xf0), value_cast<std::byte>(0xb4), value_cast<std::byte>(0xe6), value_cast<std::byte>(0x73), /*8*/
        value_cast<std::byte>(0x96), value_cast<std::byte>(0xac), value_cast<std::byte>(0x74), value_cast<std::byte>(0x22), value_cast<std::byte>(0xe7), value_cast<std::byte>(0xad), value_cast<std::byte>(0x35), value_cast<std::byte>(0x85), value_cast<std::byte>(0xe2), value_cast<std::byte>(0xf9), value_cast<std::byte>(0x37), value_cast<std::byte>(0xe8), value_cast<std::byte>(0x1c), value_cast<std::byte>(0x75), value_cast<std::byte>(0xdf), value_cast<std::byte>(0x6e), /*9*/
        value_cast<std::byte>(0x47), value_cast<std::byte>(0xf1), value_cast<std::byte>(0x1a), value_cast<std::byte>(0x71), value_cast<std::byte>(0x1d), value_cast<std::byte>(0x29), value_cast<std::byte>(0xc5), value_cast<std::byte>(0x89), value_cast<std::byte>(0x6f), value_cast<std::byte>(0xb7), value_cast<std::byte>(0x62), value_cast<std::byte>(0x0e), value_cast<std::byte>(0xaa), value_cast<std::byte>(0x18), value_cast<std::byte>(0xbe), value_cast<std::byte>(0x1b), /*a*/
        value_cast<std::byte>(0xfc), value_cast<std::byte>(0x56), value_cast<std::byte>(0x3e), value_cast<std::byte>(0x4b), value_cast<std::byte>(0xc6), value_cast<std::byte>(0xd2), value_cast<std::byte>(0x79), value_cast<std::byte>(0x20), value_cast<std::byte>(0x9a), value_cast<std::byte>(0xdb), value_cast<std::byte>(0xc0), value_cast<std::byte>(0xfe), value_cast<std::byte>(0x78), value_cast<std::byte>(0xcd), value_cast<std::byte>(0x5a), value_cast<std::byte>(0xf4), /*b*/
        value_cast<std::byte>(0x1f), value_cast<std::byte>(0xdd), value_cast<std::byte>(0xa8), value_cast<std::byte>(0x33), value_cast<std::byte>(0x88), value_cast<std::byte>(0x07), value_cast<std::byte>(0xc7), value_cast<std::byte>(0x31), value_cast<std::byte>(0xb1), value_cast<std::byte>(0x12), value_cast<std::byte>(0x10), value_cast<std::byte>(0x59), value_cast<std::byte>(0x27), value_cast<std::byte>(0x80), value_cast<std::byte>(0xec), value_cast<std::byte>(0x5f), /*c*/
        value_cast<std::byte>(0x60), value_cast<std::byte>(0x51), value_cast<std::byte>(0x7f), value_cast<std::byte>(0xa9), value_cast<std::byte>(0x19), value_cast<std::byte>(0xb5), value_cast<std::byte>(0x4a), value_cast<std::byte>(0x0d), value_cast<std::byte>(0x2d), value_cast<std::byte>(0xe5), value_cast<std::byte>(0x7a), value_cast<std::byte>(0x9f), value_cast<std::byte>(0x93), value_cast<std::byte>(0xc9), value_cast<std::byte>(0x9c), value_cast<std::byte>(0xef), /*d*/
        value_cast<std::byte>(0xa0), value_cast<std::byte>(0xe0), value_cast<std::byte>(0x3b), value_cast<std::byte>(0x4d), value_cast<std::byte>(0xae), value_cast<std::byte>(0x2a), value_cast<std::byte>(0xf5), value_cast<std::byte>(0xb0), value_cast<std::byte>(0xc8), value_cast<std::byte>(0xeb), value_cast<std::byte>(0xbb), value_cast<std::byte>(0x3c), value_cast<std::byte>(0x83), value_cast<std::byte>(0x53), value_cast<std::byte>(0x99), value_cast<std::byte>(0x61), /*e*/
        value_cast<std::byte>(0x17), value_cast<std::byte>(0x2b), value_cast<std::byte>(0x04), value_cast<std::byte>(0x7e), value_cast<std::byte>(0xba), value_cast<std::byte>(0x77), value_cast<std::byte>(0xd6), value_cast<std::byte>(0x26), value_cast<std::byte>(0xe1), value_cast<std::byte>(0x69), value_cast<std::byte>(0x14), value_cast<std::byte>(0x63), value_cast<std::byte>(0x55), value_cast<std::byte>(0x21), value_cast<std::byte>(0x0c), value_cast<std::byte>(0x7d)  /*f*/
    };


    byte_device::byte_device(byte_device const & origin)noexcept
        :MEMBER_key(origin.MEMBER_key)
    {
        std::byte buffer[key_length];

        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
    }
    void byte_device::operator =(byte_device const & origin)noexcept
    {
        if (this == std::addressof(origin))
        {
            return;
        }
        this->MEMBER_key = origin.MEMBER_key;

        std::byte buffer[key_length];

        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        this->KeyExpansion(buffer);

    }

    byte_device::byte_device(byte_device && origin) noexcept
        :MEMBER_key(std::move(origin.MEMBER_key))
    {
        origin.MEMBER_key = random_aes_key();

        std::byte buffer[key_length];

        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = this->MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        origin.KeyExpansion(buffer);
    }
    byte_device::byte_device()
        :MEMBER_key(random_aes_key())
    {
        std::byte buffer[key_length];

        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = this->MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
    }
    void byte_device::operator =(byte_device && origin) noexcept
    {
        if (this == std::addressof(origin))
        {
            return;
        }
        this->MEMBER_key = std::move(origin.MEMBER_key);
        origin.MEMBER_key = random_aes_key();
        std::byte buffer[key_length];

        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = this->MEMBER_key[count];
        }
        this->KeyExpansion(buffer);
        for (std::size_t count = 0; count < key_length; count++)
        {
            buffer[count] = origin.MEMBER_key[count];
        }
        origin.KeyExpansion(buffer);
    }

    byte_device::byte_device(std::byte const * key) noexcept
    {
        for (std::size_t count = 0;count < key_length;count++)
        {
            this->MEMBER_key[count] = key[count];
        }
        this->KeyExpansion(key);
    }

    byte_device::byte_device(byte_device::key_type const & key)noexcept
        :MEMBER_key(key)
    {
        std::byte buffer[key_length];

        for (std::size_t count = 0; count < key_length; count++)
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

        /*
        msvc似乎不会默认展开此处的循环，故手动展开
        */

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

        byte_device::AddRoundKey(state, this->MEMBER_widen_key[0]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[1]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[2]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[3]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[4]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[5]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[6]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[7]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[8]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::MixColumns(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[9]);

        byte_device::SubBytes(state);
        byte_device::ShiftRows(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[10]);

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

        byte_device::AddRoundKey(state, this->MEMBER_widen_key[10]);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[9]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[8]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[7]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[6]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[5]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[4]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[3]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[2]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[1]);
        byte_device::InvMixColumns(state);

        byte_device::InvShiftRows(state);
        byte_device::InvSubBytes(state);
        byte_device::AddRoundKey(state, this->MEMBER_widen_key[0]);

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

        t[0] = 0 ? this->MEMBER_widen_key[i][0][0 - 1] : this->MEMBER_widen_key[i - 1][0][3];
        t[1] = 0 ? this->MEMBER_widen_key[i][1][0 - 1] : this->MEMBER_widen_key[i - 1][1][3];
        t[2] = 0 ? this->MEMBER_widen_key[i][2][0 - 1] : this->MEMBER_widen_key[i - 1][2][3];
        t[3] = 0 ? this->MEMBER_widen_key[i][3][0 - 1] : this->MEMBER_widen_key[i - 1][3][3];

        std::byte temp = t[0];

        t[0] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(t[1])];
        t[1] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(t[2])];
        t[2] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(t[3])];
        t[3] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(t[0])];

        t[3] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(temp)];
        t[0] ^= rc[i - 1];

        this->MEMBER_widen_key[i][0][0] = this->MEMBER_widen_key[i - 1][0][0] ^ t[0];
        this->MEMBER_widen_key[i][1][0] = this->MEMBER_widen_key[i - 1][1][0] ^ t[1];
        this->MEMBER_widen_key[i][2][0] = this->MEMBER_widen_key[i - 1][2][0] ^ t[2];
        this->MEMBER_widen_key[i][3][0] = this->MEMBER_widen_key[i - 1][3][0] ^ t[3];

        t[0] = 1 ? this->MEMBER_widen_key[i][0][0] : this->MEMBER_widen_key[i - 1][0][3];
        t[1] = 1 ? this->MEMBER_widen_key[i][1][0] : this->MEMBER_widen_key[i - 1][1][3];
        t[2] = 1 ? this->MEMBER_widen_key[i][2][0] : this->MEMBER_widen_key[i - 1][2][3];
        t[3] = 1 ? this->MEMBER_widen_key[i][3][0] : this->MEMBER_widen_key[i - 1][3][3];

        this->MEMBER_widen_key[i][0][1] = this->MEMBER_widen_key[i - 1][0][1] ^ t[0];
        this->MEMBER_widen_key[i][1][1] = this->MEMBER_widen_key[i - 1][1][1] ^ t[1];
        this->MEMBER_widen_key[i][2][1] = this->MEMBER_widen_key[i - 1][2][1] ^ t[2];
        this->MEMBER_widen_key[i][3][1] = this->MEMBER_widen_key[i - 1][3][1] ^ t[3];

        t[0] = 2 ? MEMBER_widen_key[i][0][1] : MEMBER_widen_key[i - 1][0][3];
        t[1] = 2 ? MEMBER_widen_key[i][1][1] : MEMBER_widen_key[i - 1][1][3];
        t[2] = 2 ? MEMBER_widen_key[i][2][1] : MEMBER_widen_key[i - 1][2][3];
        t[3] = 2 ? MEMBER_widen_key[i][3][1] : MEMBER_widen_key[i - 1][3][3];

        this->MEMBER_widen_key[i][0][2] = this->MEMBER_widen_key[i - 1][0][2] ^ t[0];
        this->MEMBER_widen_key[i][1][2] = this->MEMBER_widen_key[i - 1][1][2] ^ t[1];
        this->MEMBER_widen_key[i][2][2] = this->MEMBER_widen_key[i - 1][2][2] ^ t[2];
        this->MEMBER_widen_key[i][3][2] = this->MEMBER_widen_key[i - 1][3][2] ^ t[3];

        t[0] = 3 ? this->MEMBER_widen_key[i][0][2] : this->MEMBER_widen_key[i - 1][0][3];
        t[1] = 3 ? this->MEMBER_widen_key[i][1][2] : this->MEMBER_widen_key[i - 1][1][3];
        t[2] = 3 ? this->MEMBER_widen_key[i][2][2] : this->MEMBER_widen_key[i - 1][2][3];
        t[3] = 3 ? this->MEMBER_widen_key[i][3][2] : this->MEMBER_widen_key[i - 1][3][3];

        this->MEMBER_widen_key[i][0][3] = this->MEMBER_widen_key[i - 1][0][3] ^ t[0];
        this->MEMBER_widen_key[i][1][3] = this->MEMBER_widen_key[i - 1][1][3] ^ t[1];
        this->MEMBER_widen_key[i][2][3] = this->MEMBER_widen_key[i - 1][2][3] ^ t[2];
        this->MEMBER_widen_key[i][3][3] = this->MEMBER_widen_key[i - 1][3][3] ^ t[3];
    }

    void byte_device::KeyExpansion(std::byte const * key) noexcept
    {
        constexpr std::byte const rc[] = {std::byte(0x01), std::byte(0x02), std::byte(0x04), std::byte(0x08), std::byte(0x10), std::byte(0x20), std::byte(0x40), std::byte(0x80), std::byte(0x1b), std::byte(0x36)};

        this->MEMBER_widen_key[0][0][0] = key[0];
        this->MEMBER_widen_key[0][0][1] = key[4];
        this->MEMBER_widen_key[0][0][2] = key[8];
        this->MEMBER_widen_key[0][0][3] = key[12];

        this->MEMBER_widen_key[0][1][0] = key[1];
        this->MEMBER_widen_key[0][1][1] = key[5];
        this->MEMBER_widen_key[0][1][2] = key[9];
        this->MEMBER_widen_key[0][1][3] = key[13];

        this->MEMBER_widen_key[0][2][0] = key[2];
        this->MEMBER_widen_key[0][2][1] = key[6];
        this->MEMBER_widen_key[0][2][2] = key[10];
        this->MEMBER_widen_key[0][2][3] = key[14];

        this->MEMBER_widen_key[0][3][0] = key[3];
        this->MEMBER_widen_key[0][3][1] = key[7];
        this->MEMBER_widen_key[0][3][2] = key[11];
        this->MEMBER_widen_key[0][3][3] = key[15];

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

        state[0][0] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[0][0])];
        state[0][1] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[0][1])];
        state[0][2] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[0][2])];
        state[0][3] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[0][3])];

        state[1][0] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[1][0])];
        state[1][1] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[1][1])];
        state[1][2] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[1][2])];
        state[1][3] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[1][3])];

        state[2][0] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[2][0])];
        state[2][1] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[2][1])];
        state[2][2] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[2][2])];
        state[2][3] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[2][3])];

        state[3][0] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[3][0])];
        state[3][1] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[3][1])];
        state[3][2] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[3][2])];
        state[3][3] = byte_device::MEMBER_Sbox[static_cast<unsigned int>(state[3][3])];
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

        state[0][0] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[0][0])];
        state[0][1] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[0][1])];
        state[0][2] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[0][2])];
        state[0][3] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[0][3])];

        state[1][0] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[1][0])];
        state[1][1] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[1][1])];
        state[1][2] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[1][2])];
        state[1][3] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[1][3])];

        state[2][0] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[2][0])];
        state[2][1] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[2][1])];
        state[2][2] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[2][2])];
        state[2][3] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[2][3])];

        state[3][0] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[3][0])];
        state[3][1] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[3][1])];
        state[3][2] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[3][2])];
        state[3][3] = byte_device::MEMBER_InvSbox[static_cast<unsigned int>(state[3][3])];
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
        if (count > key_length)
        {
            throw std::invalid_argument("末尾对齐处理的位数不合法");
        }
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<unsigned int> distribution(static_cast<unsigned int>(0), static_cast<unsigned int>(std::numeric_limits<std::underlying_type_t<std::byte>>::max())); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

        for (unsigned int counter = count; counter < key_length; counter++)
        {
            buffer[counter] = static_cast<std::byte>(static_cast<std::underlying_type_t<std::byte>>(distribution(random_engine)));
        }
    }

    void byte_device::make_align(char * buffer, const unsigned int count)
    {
        if (count > key_length)
        {
            throw std::invalid_argument("末尾对齐处理的位数不合法");
        }
        std::random_device seed;                                 // 用于生成随机数种子
        std::mt19937 random_engine(seed());                      // 随机数生成器
        std::uniform_int_distribution<unsigned int> distribution(static_cast<unsigned int>(0), static_cast<unsigned int>(std::numeric_limits<std::underlying_type_t<std::byte>>::max())); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

        for (unsigned int counter = count; counter < key_length; counter++)
        {
            buffer[counter] = static_cast<char>(static_cast<std::underlying_type_t<std::byte>>(distribution(random_engine)));
        }
    }

    void byte_device::align_encrypt(std::string & origin)const
    {
        std::size_t count = 0;
        while (count + key_length <= origin.size())
        {
            count += key_length;
        }
        count = origin.size() - count; // 末尾对齐的原文位数
        this->encrypt(origin);
        std::byte buffer[key_length];

        for (std::size_t count1 = 0; count1 < count; count1++)
        {
            buffer[count1] = value_cast<std::byte>(origin[origin.size() - count + count1]);
        }
        byte_device::make_align(buffer, count);
        this->encrypt(buffer);
        for (std::size_t count1 = 0; count1 < count; count1++)
        {
            origin[origin.size() - count + count1] = value_cast<char>(buffer[count1]);
        }
        for (int count1 = count; count1 < key_length; count1++)
        {
            origin.push_back(value_cast<char>(buffer[count1]));
        }
        origin.push_back(value_cast<char>(value_cast<std::byte>(count)));

    }

    void byte_device::align_encrypt(byte_type & origin)const
    {
        std::size_t count = 0;
        while (count + byte_device::key_length <= origin.size())
        {
            count += byte_device::key_length;
        }
        count = origin.size() - count; // 末尾对齐的原文位数
        this->encrypt(origin);
        std::byte buffer[byte_device::key_length];

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
        for (std::size_t count1 = count; count1 < byte_device::key_length; count1++)
        {
            origin.push_back(buffer[count1]);
        }
        origin.push_back(value_cast<std::byte>(count));
    }

    void byte_device::align_decrypt(std::string & origin)const
    {
        if (origin.size() % byte_device::key_length != 1)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        unsigned int const count = origin[origin.size() - 1];
        if (count > byte_device::key_length)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        origin.pop_back();
        this->decrypt(origin);
        for (std::size_t count1 = 0; count1 < byte_device::key_length - count; count1++)
        {
            origin.pop_back();
        }

    }

    void byte_device::align_decrypt(byte_type & origin)const
    {
        if (origin.size() % byte_device::key_length != 1)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        unsigned int const count = value_cast<unsigned int>(origin[origin.size() - 1]);
        if (count > byte_device::key_length)
        {
            throw std::invalid_argument("输入串的格式错误");
        }
        origin.pop_back();
        this->decrypt(origin);
        for (std::size_t count1 = 0; count1 < byte_device::key_length - count; count1++)
        {
            origin.pop_back();
        }
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
        auto const block_count = this->MEMBER_file_size / byte_device::key_length - 1;
        // 加密器线程的任务起始点
        auto begin = static_cast<decltype(block_count)>(0);
        // 单个加密器线程的任务块数
        auto const single_work_block_count = block_count / this->MEMBER_thread_number;

        {
            using cipher_ptr = std::unique_ptr<cipher>;
            auto cipher_holder = std::make_unique<cipher_ptr[]>(this->MEMBER_thread_number);

            for (auto count = static_cast<decltype(this->MEMBER_thread_number)>(0); count < this->MEMBER_thread_number; count++)
            {
                cipher_holder[count] = std::make_unique<cipher>(origin[count], result[count], begin, single_work_block_count, this->MEMBER_key);
                cipher_holder[count]->decrypt();
                // 下一个加密器线程的起始点
                begin += single_work_block_count * byte_device::key_length;
            }
            for (auto count = static_cast<decltype(this->MEMBER_thread_number)>(0); count < this->MEMBER_thread_number; count++)
            {
                cipher_holder[count]->join();
            }
        }

       // 所有加密器线程结束以后，现在begin是本函数需要处理的起始点

        byte_device f_aes(this->MEMBER_key);
        {
            char buffer[byte_device::key_length];
            // 处理剩余的 n-(n/m)*m 块
            for (auto count = static_cast<decltype(block_count)>(0); count < block_count - single_work_block_count * this->MEMBER_thread_number; count++)
            {

                origin[this->MEMBER_thread_number - 1]->read(buffer, byte_device::key_length);

                std::string TEMP_buffer = file_device::INNER_buffer_string(buffer, byte_device::key_length);

                f_aes.decrypt(TEMP_buffer);

                result[this->MEMBER_thread_number - 1]->write(TEMP_buffer.c_str(), byte_device::key_length);
            }
        }
        // 处理末尾对齐
        {
            char buffer[byte_device::key_length + 1];
            // 读取最后16+1字节，则buf[16]是 end_info ，末尾对齐处理的位数

            origin[this->MEMBER_thread_number - 1]->read(buffer, byte_device::key_length + 1);

            std::string TEMP_buffer = file_device::INNER_buffer_string(buffer, byte_device::key_length + 1);

            f_aes.decrypt(TEMP_buffer);

            result[this->MEMBER_thread_number - 1]->write(TEMP_buffer.c_str(), static_cast<unsigned int>(TEMP_buffer[byte_device::key_length]));

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
        std::uniform_int_distribution<unsigned int> distribution(0, static_cast<unsigned int>(std::numeric_limits<unsigned char>::max())); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间
        byte_device::key_type result;

        for (std::size_t counter = 0; counter < byte_device::key_length; counter++)
        {

            result[counter] = value_cast<std::byte>(distribution(random_engine)); // 生成一个0到255的随机整数，转换为unsigned char，存储到key中
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

    std::string file_device::INNER_buffer_string(const char * const buffer, std::size_t const size)
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
        auto end_info = static_cast<unsigned int>(this->MEMBER_file_size - block_count * byte_device::key_length);
        // 加密器线程的任务起始点
        auto begin = static_cast<decltype(block_count)>(0);
        // 单个加密器线程的任务块数
        const auto single_work_block_count = block_count / this->MEMBER_thread_number;

        {
            using cipher_ptr = std::unique_ptr<cipher>;
            auto cipher_holder = std::make_unique<cipher_ptr[]>(this->MEMBER_thread_number);

            for (auto count = static_cast<decltype(this->MEMBER_thread_number)>(0); count < this->MEMBER_thread_number; count++)
            {
                cipher_holder[count] = (std::make_unique<file_device::cipher>(origin[count], result[count], begin, single_work_block_count, this->MEMBER_key));
                cipher_holder[count]->encrypt();
                // 下一个加密器线程的起始点
                begin += single_work_block_count * byte_device::key_length;
            }
            for (auto count = static_cast<decltype(this->MEMBER_thread_number)>(0); count < this->MEMBER_thread_number; count++)
            {
                cipher_holder[count]->join();
            }
        }

       // 所有加密器线程结束以后，现在begin是本函数需要处理的起始点

        char buffer[byte_device::key_length]; // 16字节缓存，一组明文
        byte_device f_aes(this->MEMBER_key);
        // 处理原文中剩余的 完整16字节块
        for (auto count = static_cast<decltype(block_count)>(0); count < block_count - single_work_block_count * this->MEMBER_thread_number; count++)
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
            auto info = static_cast<char>(end_info);

            result[this->MEMBER_thread_number - 1]->write(std::addressof(info), 1);

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
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                buffer[i] = value_cast<std::byte>(origin[count + i]);
            }

            this->encrypt(buffer);

            /*
            密文写入原串
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                origin[count + i] = value_cast<char>(buffer[i]);
            }
        }
    }
    void byte_device::encrypt(byte_type & origin)const
    {

        std::byte buffer[byte_device::key_length];
        for (size_t count = 0; count + byte_device::key_length <= origin.size(); count += byte_device::key_length)
        {

            /*
            初始化缓冲区
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                buffer[i] = origin[count + i];
            }

            this->encrypt(buffer);

            /*
            密文写入原串
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                origin[count + i] = buffer[i];
            }
        }
    }
    void file_device::encrypt(path_type const & origin_route, path_type const & result_route, const key_type & key, const std::size_t thread_count)
    {
        namespace fs = std::filesystem;
        if (not fs::exists(origin_route))
        {
            std::string const hint = std::string("路径 ")
                + origin_route.string()
                + " 不存在";
            throw std::invalid_argument(hint);
        }
        else if (fs::is_directory(origin_route))
        {
            std::string const hint = std::string("路径 ")
                + origin_route.string()
                + " 不是文件";
            throw std::invalid_argument(hint);
        }
        if (fs::exists(result_route) and fs::is_directory(result_route))
        {
            std::string const hint = std::string("路径 ")
                + result_route.string()
                + " 已经存在，但不是文件";
            throw std::invalid_argument(hint);
        }
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
    void file_device::decrypt(path_type const & origin_route, path_type const & result_route, const key_type & key, const std::size_t thread_count)
    {
        namespace fs = std::filesystem;
        if (not fs::exists(origin_route))
        {
            std::string const hint = std::string("路径 ")
                + origin_route.string()
                + " 不存在";
            throw std::invalid_argument(hint);
        }
        else if (fs::is_directory(origin_route))
        {
            std::string const hint = std::string("路径 ")
                + origin_route.string()
                + " 不是文件";
            throw std::invalid_argument(hint);
        }
        if (fs::exists(result_route) and fs::is_directory(result_route))
        {
            std::string const hint = std::string("路径 ")
                + result_route.string()
                + " 已经存在，但不是文件";
            throw std::invalid_argument(hint);
        }
        this->MEMBER_error = std::current_exception();
        this->MEMBER_file_size = std::filesystem::file_size(origin_route);
        if (not (this->MEMBER_file_size >= (byte_device::key_length + 1) and this->MEMBER_file_size % byte_device::key_length == 1))
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
        std::size_t constexpr const TEMP_least_work = static_cast<std::size_t>(1024) * 64;//每个线程最少要处理64kb数据，否则太浪费
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
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                buffer[i] = value_cast<std::byte>(origin[count + i]);
            }

            this->decrypt(buffer);

            /*
            密文写入原串
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                origin[count + i] = value_cast<char>(buffer[i]);
            }

        }

    }
    void byte_device::decrypt(byte_type & origin)const
    {

        std::byte buffer[byte_device::key_length];
        for (size_t count = 0; count + byte_device::key_length <= origin.size(); count += byte_device::key_length)
        {

            /*
            初始化缓冲区
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                buffer[i] = origin[count + i];
            }

            this->decrypt(buffer);

            /*
            密文写入原串
            */
            for (std::size_t i = 0;i < key_length;++i)
            {
                origin[count + i] = buffer[i];
            }

        }
    }
}
