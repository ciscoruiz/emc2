#ifndef __emc2_cache_Status_h
#define __emc2_cache_Status_h

namespace emc2 {

namespace cache {

struct Status { enum _v { Empty, Available, Uploading, Error, WaitingDelete }; };

}
}

#endif

