
#ifndef MP3_STRUCT_H
#define MP3_STRUCT_H

#include <stdbool.h>

struct mp3_header {
  char tag_identifier[3];
  char tag_version[2];
  char flag;
  char tag_size_with_some_trick[4];
};
bool is_start_with_frame_header(char* buff, int buff_size);
int get_header_data_size(struct mp3_header header);

// AAAAAAAA   AAABBCCD   EEEEFFGH   IIJJKLMM 
/**
   see http://www.multiweb.cz/twoinches/mp3inside.htm for detail
   A:frame_syncronizer
   B:mpeg_version
   C:layer
   D:crc_protection
   E:bit_rate_index
   F:sampling_rate_frequency_index
   G:padding
   H:private_bit
   I:channel
   J:mode_extension
   K:copyright
   L:original
   M:emphasis
 */
struct mp3_frame_header {
  char first_byte;
  char second_byte;
  char third_byte;
  char forth_byte;
};

int get_bit_rate(struct mp3_frame_header s);
int get_sampling_rate_frequency(struct mp3_frame_header s);
int get_frame_len(struct mp3_frame_header s);

struct mp3_frame_data {
  char *data; // do not forget to free it
  int data_size;
};

#endif
