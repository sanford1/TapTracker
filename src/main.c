#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "tracker.h"

int main(int argc, char *argv[])
{
    (void) argc, (void)argv;

    const char* sharedMemKey = "tgm2p_data";
    int fd = shm_open(sharedMemKey, O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU);
    if (fd < 0)
    {
        perror("Could not create shared memory object");
        return 1;
    }

    const size_t vSize = sizeof(int32_t) * 13;

    int32_t* addr = mmap(NULL, vSize, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Parent: Could not map memory");
    }

    runTracker(addr, 240, 540);

    if (munmap(addr, vSize) != 0)
        perror("Error unmapping memory pointer");

    if (close(fd) != 0)
        perror("Error closing file");

    return 0;
}
