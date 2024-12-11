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
const uint32_t HPS_CONTROL_ADDRESS = 0xFF200000;
const uint32_t LED_ADDRESS = 0xFF200008;
const uint32_t BASE_PERIOD_ADDRESS = 0xFF200004;


// Flags and variables for options
bool verbose = false;
bool show_help = false;
bool pattern_mode = false;
bool file_mode = false;
char *filename = NULL;
uint32_t patterns[100];
unsigned int times[100];
int pattern_count = 0;

// Function Declarations
void usage();
void handle_signal(int signal);
void display_pattern(uint32_t *target_virtual_addr, uint32_t pattern, unsigned int time_ms, bool verbose);
void process_patterns_from_file();
void process_patterns_from_args(int argc, char **argv);

void usage() 
{
    printf("Usage: led_patterns [options]\n");
    printf("Options:\n");
    printf("  -h           Display this help message.\n");
    printf("  -v           Enable verbose mode (prints LED pattern and display time).\n");
    printf("  -p           Specify patterns and times (e.g., -p 0x55 500 0x0f 1500).\n");
    printf("  -f <file>    Read patterns and times from file.\n");
    printf("Example:\n");
    printf("  led_patterns -p 0x55 500 0x0f 1500\n");
    printf("  led_patterns -f mypatterns.txt\n");
}

void handle_signal(int signal) 
{
    keep_running = false;
}

void display_pattern(uint32_t *target_virtual_addr, uint32_t pattern, unsigned int time_ms, bool verbose) 
{
    *target_virtual_addr = pattern;
    if (verbose) 
    {
        printf("LED pattern = %08x Display time = %d msec\n", pattern, time_ms);
    }
    usleep(time_ms * 1000); // Convert ms to microseconds
}

void process_patterns_from_file() 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        perror("Error opening file");
        exit(1);
    }
    while (fscanf(file, "%x %u", &patterns[pattern_count], &times[pattern_count]) == 2) 
    {
        pattern_count++;
    }
    fclose(file);
}

void process_patterns_from_args(int argc, char **argv) 
{
    for (int i = optind - 1; i < argc && argv[i][0] != '-'; i += 2) 
    {
        if (i + 1 >= argc) 
        {
            fprintf(stderr, "Error: pattern/time pair incomplete.\n");
            exit(1);
        }
        patterns[pattern_count] = strtoul(argv[i], NULL, 0);
        times[pattern_count] = atoi(argv[i + 1]);
        pattern_count++;
    }
}

int main(int argc, char **argv) 
{
    int opt;

    // Handle Ctrl-C
    signal(SIGINT, handle_signal);

    // Parse options
    while ((opt = getopt(argc, argv, "hvp:f:")) != -1) 
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
                break;
            case 'f':
                file_mode = true;
                filename = optarg;
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

    if (pattern_mode) 
    {
        process_patterns_from_args(argc, argv);
    } 
    else if (file_mode) 
    {
        process_patterns_from_file();
    } 
    else 
    {
        usage();
        return 1;
    }

    // Open /dev/mem for memory access
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) 
    {
        perror("Error opening /dev/mem");
        return 1;
    }

    // Mapping the memory addresses
    uint32_t *control_addr = (uint32_t *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HPS_CONTROL_ADDRESS & ~(PAGE_SIZE - 1));
    uint32_t *led_addr = (uint32_t *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LED_ADDRESS & ~(PAGE_SIZE - 1));
    uint32_t *base_period_addr = (uint32_t *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_PERIOD_ADDRESS & ~(PAGE_SIZE - 1));

    if (control_addr == MAP_FAILED || led_addr == MAP_FAILED || base_period_addr == MAP_FAILED) 
    {
        perror("Error mapping memory");
        close(fd);
        return 1;
    }

    volatile uint32_t *control_virtual_addr = control_addr + (HPS_CONTROL_ADDRESS & (PAGE_SIZE - 1)) / sizeof(uint32_t);
    volatile uint32_t *led_virtual_addr = led_addr + (LED_ADDRESS & (PAGE_SIZE - 1)) / sizeof(uint32_t);
    volatile uint32_t *base_period_virtual_addr = base_period_addr + (BASE_PERIOD_ADDRESS & (PAGE_SIZE - 1)) / sizeof(uint32_t);

    // Display patterns based on mode
    if (pattern_mode) 
    {
        while (keep_running) 
        {
            for (int i = 0; i < pattern_count && keep_running; i++) 
            {
                display_pattern((uint32_t *)led_virtual_addr, patterns[i], times[i], verbose);
            }
        }
    } 
    else if (file_mode) 
    {
        for (int i = 0; i < pattern_count && keep_running; i++) 
        {
            display_pattern((uint32_t *)led_virtual_addr, patterns[i], times[i], verbose);
        }
    }

    // Clean up
    munmap((void *)control_addr, PAGE_SIZE);
    munmap((void *)led_addr, PAGE_SIZE);
    munmap((void *)base_period_addr, PAGE_SIZE);
    close(fd);
    return 0;
}
