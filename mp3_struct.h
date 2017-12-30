
#ifndef MP3_STRUCT_H
#define MP3_STRUCT_H

#include <stdbool.h>

struct mp3_header {
  char tag_identifier[3];
  char tag_version[2];
  char flag;
  char tag_size_with_some_trick[4];
};
bool is_start_with_tag(char* buff);
int get_tag_size(struct mp3_header header);

struct mp3_frame_header {
  bool frame_syncronizer[11];
  bool mpeg_ver[2];
  bool layer[2];
  bool crc_protection;
  bool bit_rate_index[4];
  bool sampling_rate_frequency_index[2];
  bool padding;
  bool private_bit;
  bool channel[2];
  bool mode_extension[2];
  bool copyright;
  bool original;
  bool emphasis[2];
};

int get_bit_rate(struct mp3_frame_header s);
int get_sampling_rate_frequency(struct mp3_frame_header s);
int get_frame_len(struct mp3_frame_header s);

struct progress {
  bool is_begun;
  bool parsing_tag;
  int left_len;
};

#endif
