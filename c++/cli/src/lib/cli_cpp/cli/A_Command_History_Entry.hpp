/**
 * @file    A_Command_History_Entry.hpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#ifndef __CLI_A_COMMAND_HISTORY_ENTRY_HPP__
#define __CLI_A_COMMAND_HISTORY_ENTRY_HPP__

// C++ Standard Libraries
#include <deque>
#include <string>

// CLI Libraries
#include "cmd/A_CLI_Command_Result.hpp"


namespace CLI{

/**
 * @class A_Command_History_Entry
 */
class A_Command_History_Entry{

    public:
        
    
        /**
         * @brief Constructor
         */
        A_Command_History_Entry( const int&                        command_count,
                                 const std::string&                command_string,
                                 const CMD::A_CLI_Command_Result&  command_result );
   
        
        /**
         * @brief Get the Command ID
         */
        inline int Get_Command_ID()const{
            return m_command_id;
        }

        
        /**
         * @brief Get the Command String
         */
        inline std::string Get_Command_String()const{
            return m_command_string;
        }


        /**
         * @brief Get the command result
         */
        inline CMD::A_CLI_Command_Result Get_Command_Result()const{
            return m_command_result;
        }


    private:
        
        /// Command Counter
        int m_command_id;


        /// Command String
        std::string m_command_string;


        /// Command Result
        CMD::A_CLI_Command_Result m_command_result;

}; // End of A_Command_History_Entry Class


/**
 * @brief A_Command_History
 */
class A_Command_History{

    public:

        /// Pointer TYpe
        typedef std::shared_ptr<A_Command_History> ptr_t;


        /**
         * @brief Add Entry
         */
        void Add_Entry( const A_Command_History_Entry& entry );

        
        /**
         * @brief Get entry
         */
        inline A_Command_History_Entry Get_Entry( const int& entry )const{
            return m_command_history[entry];
        }

        
        /**
         * @brief Get back entry.
         */
         inline A_Command_History_Entry Get_Back()const{
             return m_command_history.back();
         }

        
        /**
         * @brief Get the size.
         */
        inline int Size()const{
            return m_command_history.size();
        }

        

    private:
        
        /// History
        std::deque<A_Command_History_Entry> m_command_history;


}; // End A_Command_History Class


} // End of CLI Namespace

#endif
