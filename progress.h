#ifndef PROGRESS_H
#define PROGRESS_H
#include "mp3_struct.h"

struct parsing_mp3_header_progress {
  struct mp3_header* header;
  int expected_len;
};

struct parsing_mp3_header_data_progress {
  char *data;
  int expected_len;
  int data_size;
};

struct parsing_mp3_frame_header_progress {
  struct mp3_frame_header* header;
  int expected_len;
};

struct parsing_mp3_frame_data_progress {
  struct mp3_frame_data* frame_data;
  int expected_len;
};

int init_mp3_header_progress(struct parsing_mp3_header_progress* progress, struct mp3_header *header);
int init_mp3_header_data_progress(struct parsing_mp3_header_data_progress* progress, struct mp3_header header);
int free_mp3_header_data_progress(struct parsing_mp3_header_data_progress* progress);
int init_mp3_frame_header_progress(struct parsing_mp3_frame_header_progress* progress, struct mp3_frame_header *header);
int init_mp3_frame_data_progress(struct parsing_mp3_frame_data_progress* progress, struct mp3_frame_header frame_header);
int free_mp3_frame_data_progress(struct parsing_mp3_frame_data_progress* progress);

int parse_mp3_header(struct parsing_mp3_header_progress* progress, char* buff, int buff_left_size);
int parse_mp3_header_data(struct parsing_mp3_header_data_progress* progress, char* buff, int buffer_left_size);
int parse_mp3_frame_header(struct parsing_mp3_frame_header_progress* progress, char*buff, int buff_left_size);
int parse_mp3_frame_data(struct parsing_mp3_frame_data_progress* progress, char*buff, int buff_left_size);

#endif
