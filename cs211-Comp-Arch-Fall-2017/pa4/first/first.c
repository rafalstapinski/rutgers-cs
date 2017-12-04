#include <stdio.h>
// #include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "first.h"

int memory_reads = 0;
int memory_writes = 0;
int cache_hits = 0;
int cache_misses = 0;

int memory_reads_p = 0;
int memory_writes_p = 0;
int cache_hits_p = 0;
int cache_misses_p = 0;

unsigned int log_2( unsigned int x )
{
  unsigned int ans = 0 ;
  while( x>>=1 ) ans++;
  return ans;
}

uint64_t getBits(uint64_t value, uint64_t start, uint64_t end)
{

  uint64_t mask = 0;
  uint64_t i = 0;

  for (i = start; i <= end; i++)
  {
    mask |= 1 << i;
  }

  return mask & value;

}

char *hex_to_bin(char c)
{
  switch(c) {
    case '0':
      return "0000";
    case '1':
      return "0001";
    case '2':
      return "0010";
    case '3':
      return "0011";
    case '4':
      return "0100";
    case '5':
      return "0101";
    case '6':
      return "0110";
    case '7':
      return "0111";
    case '8':
      return "1000";
    case '9':
      return "1001";
    case 'a':
      return "1010";
    case 'b':
      return "1011";
    case 'c':
      return "1100";
    case 'd':
      return "1101";
    case 'e':
      return "1110";
    case 'f':
      return "1111";
  }
  return "";
}

char *get_binary_address(char address[])
{

  // printf("%s\n", address);

  char *res = malloc(49);
  res[48] = '\0';

  int i = 47;
  while (i >= 0)
  {
    res[i] = '0';
    i--;
  }
  int j = 44;

  i = 0;

  for (i = strlen(address) - 1; i >= 0; i--)
  {
    res[j] = hex_to_bin(address[i])[0];
    res[j+1] = hex_to_bin(address[i])[1];
    res[j+2] = hex_to_bin(address[i])[2];
    res[j+3] = hex_to_bin(address[i])[3];
    j-=4;

  }

  return res;

}

void direct_cache(char *cache[], char *cache_p[], char cmd, int block_size, int cache_size, char *binary_address)
{

  int offset_start = 48 - log_2(block_size);
  int slot_start = offset_start - log_2(cache_size / block_size);

  // printf("%d %d\n", offset_start, slot_start);

  char offset_str[48];
  char slot_str[48];
  char tag_str[48];

  strncpy(offset_str, binary_address + offset_start, 48 - offset_start);
  strncpy(slot_str, binary_address + slot_start, offset_start - slot_start);
  strncpy(tag_str, binary_address, 49 - strlen(offset_str) - strlen(slot_str));

  int prefetch = 0;

  long slot = strtol(slot_str, NULL, 2);

  if (cmd == 'W')
  {

    if (strcmp(cache[slot], tag_str) == 0)
    {
      cache_hits++;
      memory_writes++;
    }
    else {
      memory_writes++;
      cache_misses++;
      memory_reads++;
      strcpy(cache[slot], tag_str);

    }

    // prefetch cache

    if (strcmp(cache_p[slot], tag_str) == 0)
    {
      cache_hits_p++;
      memory_writes_p++;
    }
    else {
      memory_writes_p++;
      cache_misses_p++;
      memory_reads_p++;
      strcpy(cache_p[slot], tag_str);

      prefetch = 1;

    }

  }
  else if (cmd == 'R')
  {

    if (strcmp(cache[slot], tag_str) == 0)
    {
      cache_hits++;
    }
    else {
      cache_misses++;
      memory_reads++;
      strcpy(cache[slot], tag_str);

      prefetch = 1;

    }

    // prefetch cache

    if (strcmp(cache_p[slot], tag_str) == 0)
    {
      cache_hits_p++;
    }
    else {
      cache_misses_p++;
      memory_reads_p++;
      strcpy(cache_p[slot], tag_str);

      prefetch = 1;

    }

  }

  if (prefetch)
  {

    if (slot < (cache_size / block_size) - 1)
    {
      strcpy(cache_p[slot + 1], tag_str);
    }

  }

}

int main(int argc, char *argv[])
{

  // set cache size, block size, and related offsets for addresses

  int cache_size = atoi(argv[1]);
  int block_size = atoi(argv[4]);

  int cache_length = cache_size / block_size;

  // struct Block **cache = (struct Block **) malloc(sizeof(struct Block) * cache_length);

  char *d_cache[cache_length];
  char *d_cache_p[cache_length];

  int assoc;

  if (strcmp("direct", argv[2]) == 0)
  {

    assoc = 1;

    int i = 0;
    for (i = 0; i < cache_length; i++)
    {
      d_cache[i] = malloc(49);
      d_cache_p[i] = malloc(49);
    }

  }
  else if (strcmp("assoc", argv[2]) == 0)
  {

    assoc = cache_size / block_size;

  }
  else {

    char *a = strtok(argv[2], ":");
    a = strtok(NULL, a);
    assoc = atoi(a);

  }

  if (assoc > 0)
  {
    cache_length = cache_size / block_size / assoc;
  }

  // begin reading file

  FILE *file;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  char eip[20];
  char cmd;
  char address_str[20];
  char *binary_address;
  // uint64_t address;

  file = fopen(argv[5], "r");

  if (!file)
  {
    printf("File not found.\n");
    return 1;
  }

  while ((read = getline(&line, &len, file)) != -1)
  {

    sscanf(line, "%s %c %s\n", eip, &cmd, address_str);

    binary_address = get_binary_address(address_str + 2);


    // direct
    if (assoc == 1)
    {

      direct_cache(d_cache, d_cache_p, cmd, block_size, cache_size, binary_address);

    }

  }

  printf("no-prefetch\n");
  printf("Memory reads: %d\n", memory_reads);
  printf("Memory writes: %d\n", memory_writes);
  printf("Cache hits: %d\n", cache_hits);
  printf("Cache misses: %d\n", cache_misses);
  printf("with-prefetch\n");
  printf("Memory reads: %d\n", memory_reads_p);
  printf("Memory writes: %d\n", memory_writes_p);
  printf("Cache hits: %d\n", cache_hits_p);
  printf("Cache misses: %d\n", cache_misses_p);

  fclose(file);

  if (line)
  {
    free(line);
  }

  return 0;

}
