/*
 * Lab8.c - A cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  The replacement policy is LRU.
 *
 *  Used for Lab Exercise 8, CSCI 247, Fall 2017
 *  Author: Alexandra Puchko
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most one cache miss. (I examined the trace,
 *  the largest request I saw was for 8 bytes).
 *  2. Instruction loads (I) are ignored, since we are interested in evaluating
 *  trans.c in terms of its data cache performance.
 *  3. data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus an possible eviction.
 *
 */


#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <time.h>



#define ADDRESS_LENGTH 64

/* Type: Memory address */
typedef unsigned long long int mem_addr_t;

/* Type: Cache line  */
typedef struct cache_line {
    char valid;
    mem_addr_t tag;
	unsigned long int lru_count;
} cache_line_t;

/* Type: Cache set, an array of Cache lines */
typedef cache_line_t* cache_set_t;

/* Type: Cache, an array of cache sets */
typedef cache_set_t* cache_t;

/* Globals set by command line args */
int verbosity = 0; /* print trace if set */

/* The cache we are simulating */
cache_t cache;

/* Globals set by command line args */
int s = 0; /* set index bits */
int b = 0; /* block offset bits */
int E = 0; /* associativity , number of lines per set */
char* trace_file = NULL;

/* Derived from command line args */
int S; /* number of sets */
int B; /* block size (bytes) */

/* Counters used to record cache statistics */
int miss_count = 0;
int hit_count = 0;
int eviction_count = 0;

/* Global LRU counter */
unsigned long int lruCounter = 0;

/*
 * initCache - Allocate memory, write 0's for valid and tag and LRU
 * also computes the set_index_mask
 */
void initCache()
{
    int i,j;
    cache = (cache_set_t*) malloc(sizeof(cache_set_t) * S);
    for (i=0; i<S; i++){
        cache[i]=(cache_line_t*) malloc(sizeof(cache_line_t) * E);
        for (j=0; j<E; j++){
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
        }
    }
}


/*
 * freeCache - free allocated memory
 */
void freeCache()
{
    int i;
    for (i=0; i<S; i++){
        free(cache[i]);
    }
    free(cache);
}


/*
 * accessData - Access data at memory address addr.
 *   If it is already in cache, increast hit_count
 *   If it is not in cache, bring it in cache, increase miss count.
 *   Also increase eviction_count if a line is evicted.
 */
void accessData(mem_addr_t addr)
{
    // CSCI 247 students neeed to complete this function
    // initially, no cachng is being done

    // the jth cache line in the ith set of the cache is cache[i][j]
    // the items in the line that are needed for the simulation are:
    // cache[i][j].valid and cache[i][j].tag

    // if eviction is needed, find the line with the lowest lru_count

    // 1) From the address passed as its parameter, determine the set index and tag values.
    int tag_value = addr >> (s + b);
    int set_index = (addr >> b) & (S - 1);


    // 2) Search through the lines for that set index to see whether
    // there is a line with its valid bit set to 1 and its tag value matching the tag from the address.
    // If so, increment the hit count, update this cache line’s lru_count to the incremented
    // global lruCount value and return.
     cache_set_t cache_set = cache[set_index];
     for(int j = 0; j < E; j++){
      if(cache_set[j].valid && (cache_set[j].tag == tag_value)){
        hit_count++;
        lruCounter++;
        cache_set[j].lru_count = lruCounter;
        return;
      }
     }

  // 3) In the case of a cache miss, increment the miss count and look for a line in the set whose valid bit is 0.
  // If the set contains such a line, set its valid bit to 1,
  // set its tag to the tag value of the address and set its lru_count to the incremented global lruCount value.

     miss_count++;
     for(int j = 0; j < E; j++){
      if(cache_set[j].valid == 0){
        cache_set[j].valid = 1;
        cache_set[j].tag = tag_value;
        lruCounter++;
        cache_set[j].lru_count = lruCounter;
        return;
      }
    }

// 4) If all the lines in the cache set are valid,
// find the cache line in this cache set with the lowest lru_count as the victim
// for replacement by the current memory reference.
// The line with the lowest lru_count is the least recently used line in that cache set.

     cache_line_t min_line = cache_set[0];
     for(int i = 1; i < E; i++){
        if(cache_set[i].lru_count < min_line.lru_count){
          min_line = cache_set[i];
        }
     }
     min_line.tag = tag_value;
     eviction_count++;
}


/*
 * replayTrace - replays the given trace file against the cache
 */
void replayTrace(char* trace_fn)
{
    char buf[1000];
    mem_addr_t addr=0;
    unsigned int len=0;
    FILE* trace_fp = fopen(trace_fn, "r");

    if(!trace_fp){
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);
    }

    while( fgets(buf, 1000, trace_fp) != NULL) {
        if(buf[1]=='S' || buf[1]=='L' || buf[1]=='M') {
            sscanf(buf+3, "%llx,%u", &addr, &len);

            if(verbosity)
                printf("%c %llx,%u ", buf[1], addr, len);

            accessData(addr);

            /* If the instruction is R/W then access again */
            if(buf[1]=='M')
                accessData(addr);

            if (verbosity)
                printf("\n");
        }
    }

    fclose(trace_fp);
}

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 * main - Main routine
 */
int main(int argc, char* argv[])
{

    /* get the command-line arguments */
    char c;
    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
        switch(c){
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        case 'h':
            printUsage(argv);
            exit(0);
        default:
            printUsage(argv);
            exit(1);
        }
    }

    /* Make sure that all required command line args were specified */
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    /* Compute S, E and B from command line args */
    S = (unsigned int) pow(2, s);
    B = (unsigned int) pow(2, b);
    /* Initialize cache */
    initCache();

    /* initialize the random number generator used for line eviction */
    srand(time(NULL));

    /* run the trace file to simulate cache performance */
    replayTrace(trace_file);

    /* Free allocated memory */
    freeCache();

    /* Output the hit and miss statistics for the autograder */
    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count);
    return 0;
}
