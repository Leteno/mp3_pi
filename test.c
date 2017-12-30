#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include "mp3_struct.h"

int mp3_check(char* filename);

int main(int argc, char** argv) {
  printf("helloworld\n");
  mp3_check("test.mp3");
  return 0;
}

int mp3_check(char* filename)
{
  int fd;
  #define BUFFER_SIZE 256
  char buff1[BUFFER_SIZE + 1];
  char buff2[BUFFER_SIZE + 1];

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    printf("read fail");
    return -1;
  }

  bool duty = false;
  int read_count = 1;
  char* buff;
  char* last_buff;
  struct progress progress0;
  struct mp3_header _mp3_header;
  struct mp3_frame_header _mp3_frame_header;
  progress0.is_begun = false;

  while (read_count) {
    buff = duty ? buff1 : buff2;
    last_buff = !duty ? buff1 : buff2;
    duty = !duty;
    read_count = read(fd, buff, BUFFER_SIZE);
    if (!progress0.is_begun) {
      progress0.is_begun = true;
      if (is_start_with_tag(buff)) {
      } else {
        memcpy((void*)&_mp3_header, buff, sizeof(struct mp3_header)); // buggy thank god sizeof(mp3_header) << sizeof(buff)
        printf("mp3_header tag_identifier: %3s, tag_size: %d\n", _mp3_header.tag_identifier, get_tag_size(_mp3_header));
      }
    }
  }

  close(fd);
  return 0;
}
