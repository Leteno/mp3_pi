#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include "mp3_struct.h"
#include "progress.h"

int mp3_check(char* filename);

int on_frame_header_read_finish(struct parsing_mp3_frame_header_progress progress);
int on_frame_data_read_finish(struct parsing_mp3_frame_data_progress progress);

int main(int argc, char** argv) {
  printf("helloworld\n");
  mp3_check("test.mp3");
  return 0;
}

int mp3_check(char* filename)
{
  int fd;
  #define BUFFER_SIZE 256

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    printf("read fail");
    return -1;
  }

  struct parsing_mp3_header_progress header_progress;
  struct parsing_mp3_header_data_progress header_data_progress;
  struct parsing_mp3_frame_header_progress frame_header_progress;
  struct parsing_mp3_frame_data_progress frame_data_progress;
  struct mp3_header _mp3_header;
  struct mp3_frame_header _mp3_frame_header;
  init_mp3_header_progress(&header_progress, &_mp3_header);
  init_mp3_frame_header_progress(&frame_header_progress, &_mp3_frame_header);

  int read_count = 1, buffer_left_count;
  char buff[BUFFER_SIZE];


  while (read_count) {
    read_count = read(fd, buff, BUFFER_SIZE);
    buffer_left_count = read_count;
  read_loop:
    if (buffer_left_count == 0) {
      continue;
    }
    if (header_progress.expected_len > 0) {
      int consume = parse_mp3_header(&header_progress, buff + read_count - buffer_left_count, buffer_left_count);
      buffer_left_count -= consume;
      if (header_progress.expected_len == 0) {
        init_mp3_header_data_progress(&header_data_progress, *(header_progress.header));
      }
      goto read_loop;
    } else if (header_data_progress.expected_len > 0) {
      int consume = parse_mp3_header_data(&header_data_progress, buff + read_count - buffer_left_count, buffer_left_count);
      buffer_left_count -= consume;
      goto read_loop;
    } else if (frame_header_progress.expected_len > 0) {
      int consume = parse_mp3_frame_header(&frame_header_progress, buff + read_count - buffer_left_count, buffer_left_count);
      buffer_left_count -= consume;
      if (frame_header_progress.expected_len == 0) {
        free_mp3_frame_data_progress(&frame_data_progress);
        init_mp3_frame_data_progress(&frame_data_progress, *(frame_header_progress.header));
        on_frame_header_read_finish(frame_header_progress);
      }
      goto read_loop;
    } else if (frame_data_progress.expected_len > 0) {
      int consume = parse_mp3_frame_data(&frame_data_progress, buff, buffer_left_count);
      buffer_left_count -= consume;
      if (frame_data_progress.expected_len == 0) {
        on_frame_data_read_finish(frame_data_progress);
        init_mp3_frame_header_progress(&frame_header_progress, &_mp3_frame_header);
      }
      goto read_loop;
    }
  }
  free_mp3_header_data_progress(&header_data_progress);

  close(fd);
  return 0;
}

int on_frame_header_read_finish(struct parsing_mp3_frame_header_progress progress) {
  struct mp3_frame_header header = *(progress.header);
  if (!exam_frame_header(header)) {
    printf("exam fail on %s", (char* )progress.header);
    exit(1);
  }
  int bit_rate = get_bit_rate(header);
  int sampling_rate = get_sampling_rate_frequency(header);
  int frame_len = get_frame_len(header);
  printf("got frame header: bit_rate:%d, sampling_rate:%d, frame_len:%d\n", bit_rate, sampling_rate, frame_len);
  return 0;
}

int on_frame_data_read_finish(struct parsing_mp3_frame_data_progress progress) {
  struct mp3_frame_data data = *(progress.frame_data);
  printf("frame_data_size: %d\n", data.data_size);
  return 0;
}
