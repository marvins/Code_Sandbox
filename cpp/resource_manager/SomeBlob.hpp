#pragma once

#include <cstring>
#include <string>

class SomeBlob
{
    public:

        /**
         * @brief Default Constructor
         */
        SomeBlob() = default;

        /**
         * @brief Parameterized Constructor
         * @param buffer_size
         * @param init_val
         * @param message
         */
        explicit SomeBlob( size_t      buffer_size,
                           char        init_val,
                           std::string message )
          : m_buffer(new char[buffer_size]),
            m_buffer_size_bytes(buffer_size),
            m_message(message)
        {
            memset( m_buffer, init_val, m_buffer_size_bytes );
        }

        /**
         * @brief Get the Internal Buffer
         * @return
         */
        char* Get_Buffer() const
        {
            return m_buffer;
        }

        /**
         * @brief Get the buffer size in bytes
         * @return
         */
        uint64_t Get_Buffer_Size_Bytes() const
        {
            return m_buffer_size_bytes;
        }

        /**
         * @brief Get the Message String
         * @return
         */
        std::string Get_Message() const
        {
            return m_message;
        }

        /**
         * @brief Set the message text
         * @param new_message
         */
        void Set_Message( std::string new_message )
        {
            m_message = new_message;
        }

    private:

        char* m_buffer { nullptr };
        uint64_t m_buffer_size_bytes { 0 };
        std::string m_message { "hello world" };
};

std::ostream& operator << ( std::ostream& sout, const SomeBlob& rhs )
{
    sout << " Buffer Size: " << rhs.Get_Buffer_Size_Bytes() << ", Message: " << rhs.Get_Message() << ", Buffer: " << std::string(rhs.Get_Buffer(), rhs.Get_Buffer_Size_Bytes());
    return sout;
}