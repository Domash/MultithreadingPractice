#include <condition_variable>
#include <queue>
#include <mutex>

template <class T>
class FixedSizeQueue {
public:

    FixedSizeQueue(std::size_t max_size) : max_size_(max_size) {}

    T pop() {
        std::unique_lock<std::mutex> guard(lock_);
        empty.wait(guard, [this] {
            return !queue.empty();
        });
        full.notify_one();

        T top = queue.top();
        queue.pop();

        return top;
    }

    void push(T value) {
        std::unique_lock<std::mutex> guard(lock_);
        full.wait(guard, [this] {
            return queue.size() < max_size_;
        });
        empty.notify_one();

        queue.push(value);
    }

private:

    std::queue<T> queue;
    std::size_t max_size_;

    std::mutex lock_;
    std::condition_variable empty;
    std::condition_variable full;

};


int main() {

    return 0;
}