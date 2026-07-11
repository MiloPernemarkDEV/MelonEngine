#ifndef MELON_JOB_SYSTEM_H
#define MELON_JOB_SYSTEM_H

#include "types.h"

class JobSystem {
public:
	JobSystem(); 
	~JobSystem();

	u32 get_num_threads() const;

private:
	struct Impl; 
	Impl* impl; 
};

#endif // MELON_JOB_SYSTEM_H