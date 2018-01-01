#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "progress.h"
#include "mp3_struct.h"
#include "util.h"

#define HEADER_SIZE 10
#define FRAME_HEADER_SIZE 4

int init_mp3_header_progress(struct parsing_mp3_header_progress* progress, struct mp3_header* header) {
  progress->expected_len = HEADER_SIZE;
  progress->header = header;
  return 0;
}

int init_mp3_header_data_progress(struct parsing_mp3_header_data_progress* progress, struct mp3_header header) {
  int header_data_size = get_header_data_size(header);
  progress->expected_len = header_data_size;
  progress->data_size = header_data_size;
  progress->data = (char*) malloc((header_data_size + 1) * sizeof(char));
  return 0;
}

int init_mp3_frame_header_progress(struct parsing_mp3_frame_header_progress* progress, struct mp3_frame_header* header) {
  progress->expected_len = FRAME_HEADER_SIZE;
  progress->header = header;
  return 0;
}
int init_mp3_frame_data_progress(struct parsing_mp3_frame_data_progress* progress, struct mp3_frame_header frame_header) {
  int frame_len = get_frame_len(frame_header);
  progress->expected_len = frame_len;
  progress->frame_data->data_size = frame_len;
  progress->frame_data->data = (char*) malloc((frame_len + 1) * sizeof(char));
  return 0;
}
int free_mp3_frame_data_progress(struct parsing_mp3_frame_data_progress* progress) {
  if (progress && progress->frame_data && progress->frame_data->data) {
    free(progress->frame_data->data);
  }
  return 0;
}

int parse_mp3_header(struct parsing_mp3_header_progress* progress, char* buff, int buff_left_size) {
  if (!progress->expected_len) {
    return 0;
  }
  if (is_start_with_frame_header(buff, buff_left_size)) { // this file does not have mp3_header
    progress->expected_len = 0;
    return 0;
  }
  int mem_copy_size = MIN(progress->expected_len, buff_left_size);
  memcpy(progress->header + (HEADER_SIZE - progress->expected_len),
         buff,
         mem_copy_size);
  progress->expected_len -= mem_copy_size;
  return mem_copy_size;
}

int parse_mp3_header_data(struct parsing_mp3_header_data_progress* progress, char* buff, int buffer_left_size) {
  if (!progress->expected_len) {
    return 0;
  }
  int mem_copy_size = MIN(progress->expected_len, buffer_left_size);
  memcpy(progress->data,
         buff,
         mem_copy_size);
  progress->expected_len -= mem_copy_size;
  return mem_copy_size;
}

int parse_mp3_frame_header(struct parsing_mp3_frame_header_progress* progress, char* buff, int buff_left_size) {
  if (progress->expected_len == FRAME_HEADER_SIZE) {
    // assert(is_start_with_frame_header(buff, buff_left_size));
    // Frankly speaking, it is needed. But { if(buff_left_size < 11) print("gg"); }
  }
  if (!progress->expected_len) {
    return 0;
  }
  int mem_copy_size = MIN(progress->expected_len, buff_left_size);
  memcpy(progress->header + (FRAME_HEADER_SIZE - progress->expected_len),
         buff,
         mem_copy_size);
  progress->expected_len -= mem_copy_size;
  return mem_copy_size;
}

int parse_mp3_frame_data(struct parsing_mp3_frame_data_progress* progress, char* buff, int buff_left_size) {
  if (!progress->expected_len) {
    return 0;
  }
  int mem_copy_size = MIN(progress->expected_len, buff_left_size);
  memcpy(progress->frame_data->data,
         buff,
         mem_copy_size);
  progress->expected_len -= mem_copy_size;
  return mem_copy_size;
}
