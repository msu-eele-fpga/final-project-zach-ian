#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <time.h>

// Globals
volatile bool keep_running = true;
const size_t PAGE_SIZE = 4096; // Typically 4096 bytes
const uint32_t STEP_SIZE_ADDRESS = 0xFF200000;
const uint32_t POTENTIOMETER_VALUE_ADDRESS = 0xFF200004;

// Flags and variables for options
bool verbose = false;
bool show_help = false;
bool pattern_mode = false;
uint32_t step_size_value = 0;

// Function Declarations
void usage();
void handle_signal(int signal);

void usage()
{
    printf("Usage: potent_array [options]\n");
    printf("Options:\n");
    printf("  -h           Display this help message.\n");
    printf("  -v           Enable verbose mode (prints potentiometer value and other details).\n");
    printf("  -p <value>   Write 1 or 0 to the step size address.\n");
    printf("Example:\n");
    printf("  potent_array -p 1\n");
    printf("  potent_array -v\n");
}

void handle_signal(int signal)
{
    keep_running = false;
}

int main(int argc, char **argv)
{
    int opt;

    // Handle Ctrl-C
    signal(SIGINT, handle_signal);

    // Parse options
    while ((opt = getopt(argc, argv, "hvp:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            show_help = true;
            break;
        case 'v':
            verbose = true;
            break;
        case 'p':
            pattern_mode = true;
            step_size_value = strtoul(optarg, NULL, 0);
            if (step_size_value != 0 && step_size_value != 1)
            {
                fprintf(stderr, "Error: Invalid value for -p. Must be 0 or 1.\n");
                return 1;
            }
            break;
        default:
            usage();
            return 1;
        }
    }

    if (show_help)
    {
        usage();
        return 0;
    }

    // Open /dev/mem for memory access
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1)
    {
        perror("Error opening /dev/mem");
        return 1;
    }

    // Mapping the memory addresses
    uint32_t *control_addr = (uint32_t *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, STEP_SIZE_ADDRESS & ~(PAGE_SIZE - 1));
    uint32_t *potentiometer_value_addr = (uint32_t *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, POTENTIOMETER_VALUE_ADDRESS & ~(PAGE_SIZE - 1));

    if (control_addr == MAP_FAILED || potentiometer_value_addr == MAP_FAILED)
    {
        perror("Error mapping memory");
        close(fd);
        return 1;
    }

    volatile uint32_t *control_virtual_addr = control_addr + (STEP_SIZE_ADDRESS & (PAGE_SIZE - 1)) / sizeof(uint32_t);
    volatile uint32_t *potentiometer_virtual_addr = potentiometer_value_addr + (POTENTIOMETER_VALUE_ADDRESS & (PAGE_SIZE - 1)) / sizeof(uint32_t);

    // Verbose mode: display potentiometer value
    if (verbose)
    {
        printf("Potentiometer value = %08x\n", *potentiometer_virtual_addr);
    }

    // Pattern mode: write to step size
    if (pattern_mode)
    {
        *control_virtual_addr = step_size_value;
        if (verbose)
        {
            printf("Step size set to %u\n", step_size_value);
        }
    }

    // Clean up
    munmap((void *)control_addr, PAGE_SIZE);
    munmap((void *)potentiometer_value_addr, PAGE_SIZE);
    close(fd);
    return 0;
}
