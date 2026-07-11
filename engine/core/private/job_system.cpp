#include "job_system.h"
#include <mutex>
#include <thread>
#include "arena_memory.h"
#include "defines.h"

constexpr s32 fallback_num_threads = 2;

struct JobSystem::Impl {
	u32 numThreads = 0;
	std::mutex mtx;

	static u32 calculate_num_threads();
};

u32 JobSystem::Impl::calculate_num_threads()
{
	const u32 availableThreads = std::thread::hardware_concurrency();
	if (availableThreads == 0) {
		return fallback_num_threads;
	}

	const u32 physical_cores = availableThreads > 1 ? availableThreads / 2 : 1;
	u32 worker_threads = physical_cores - 1;

	if (worker_threads == 0) {
		worker_threads = 1;
	}
	return worker_threads;
}

JobSystem::JobSystem()
{
	impl = g_GlobalArena.add<Impl>();
	impl->numThreads = Impl::calculate_num_threads();
}

JobSystem::~JobSystem()
{
}


u32 JobSystem::get_num_threads() const {
	return impl->numThreads;
}