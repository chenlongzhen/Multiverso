#include "block_queue.h"

namespace wordembedding {

	void BlockQueue::Push(DataBlock *data_block) {
		std::unique_lock<std::mutex> lock(mtx_);
		queues_.push(data_block);
		repo_not_empty_.notify_all();
		lock.unlock();
		multiverso::Log::Info("push block size %d\n", data_block->Size());
	}

	DataBlock* BlockQueue::Pop() {
		std::unique_lock<std::mutex> lock(mtx_);
		// block queue is empty, just wait here.
		while (queues_.size() == 0) {
			(repo_not_empty_).wait(lock);
		}

		DataBlock* temp = queues_.front();
		queues_.pop();
		lock.unlock();
		return temp;
	}

	int const BlockQueue::GetQueueSize() {
		return queues_.size();
	}
}