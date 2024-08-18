#ifndef BUFFER_MANAGER__H
#define BUFFER_MANAGER__H
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

template <typename T, typename MTX=std::mutex>
class BufferManager
{
    public:
        BufferManager(MTX &mtx_) : mtx{mtx_} {};

        void await_push(T item)
        {
            std::unique_lock<MTX> ul (mtx);
            buffer.push(item);
            ul.unlock();
            cv.notify_one();
        }

        T await_pop()
        {
            std::unique_lock<MTX> ul (mtx);
            auto check_buffer = [this]() {
                return !this->buffer.empty();
            };
            
            cv.wait(ul, check_buffer);
            T item = buffer.front();
            buffer.pop();
            return item;
        }

    private:
        std::queue<T> buffer;
        MTX &mtx;
        std::condition_variable cv;
};


#endif
