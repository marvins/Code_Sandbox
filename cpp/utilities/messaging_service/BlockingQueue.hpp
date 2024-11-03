/**
 * @file    BlockingQueue.hpp
 * @author  Marvin Smith
 * @date    4/17/2017
*/
#ifndef BLOCKING_QUEUE_HPP
#define BLOCKING_QUEUE_HPP

#include <mutex>
#include <queue>
#include <condition_variable>


/**
 * @class Blocking_Queue
 *
 * @brief Simple thread-safe class for blocking push/pop operations.
 */
template<class T>
class Blocking_Queue
{

    public:

        /**
         * @brief Default Constructor
         */
        Blocking_Queue()
          : m_break(false),
            m_complete(false)
        {
        }


        /**
         * @brief Clear the Queue
         */
        void clear()
        {
            // Release all waiting queue pops
            {
                std::unique_lock<std::mutex> lock{m_mutex};
                m_break = true;
                m_complete = false;
                m_cv.notify_all();
            }

            // Clean up
            {
                std::unique_lock<std::mutex> lock{m_mutex};
                std::queue<T> empty;
                std::swap( m_queue, empty );
                m_break = false;
                m_complete = false;
            }
        }


        /**
         * @brief Push Value onto Queue
         *
         * @return True if successful, false if not initialized or full.
         */
        bool push(T&& value)
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            if(m_break || m_complete)
            {
                return false;
            }
            else
            {
                m_queue.push(value);
                m_cv.notify_one();
                return true;
            }
        }


        /**
         * @brief Push value onto queue.
         */
        bool push(const T& value)
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            if(m_break || m_complete)
            {
                return false;
            }
            else
            {
                m_queue.push(value);
                m_cv.notify_one();
                return true;
            }
        }

        bool try_pop(T& value)
        {
            std::unique_lock<std::mutex> lock{m_mutex};

            if(m_break)
            {
                return false;
            }
            else if(m_queue.empty())
            {
                return false;
            }
            else
            {
                value = m_queue.front();
                m_queue.pop();
                return true;
            }
        }

        bool pop(T& value)
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_cv.wait( lock, [this]
                               {
                                  if( m_complete ) return true;
                                  else if( m_break ) return true;
                                  else return (!m_queue.empty() );
                               });

            if(m_break)
            {
                return false;
            }
            else if(m_complete && m_queue.empty() )
            {
                return false;
            }
            else
            {
                value = m_queue.front();
                m_queue.pop();
                return true;
            }
        }

        void start()
        {
            validate();
        }

        void complete()
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_complete = true;
            m_cv.notify_all();

        }

        void invalidate()
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_break = true;
            m_complete = false;
            m_cv.notify_all();

        }

        void validate()
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_break = false;
            m_complete = false;
        }

        bool is_valid()
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            if(m_break) return false;
            if(m_complete) return false;
            else return true;
        }

    private:
        std::mutex m_mutex;
        std::condition_variable m_cv;
        bool m_break;
        bool m_complete;
        std::queue<T> m_queue;

};


#endif
