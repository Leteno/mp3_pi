#include "mp3_struct.h"
#include "stdio.h"
#include <unistd.h>

bool is_start_with_frame_header(char* buff, int buff_size) { // buggy, buff_size
  // first 11 bits should be 1
  bool* bitReader = (bool*) buff;
  int frame_syncronizer_size = 11;
  for (int i = 0; i < frame_syncronizer_size; i++) {
    if (!*(bitReader + i)) return false; // may cause overflow
  }
  return true;
}

int get_header_data_size(struct mp3_header header) {
  int result = 0;
  char* size = header.tag_size_with_some_trick;
  result |= (size[0] & 0x7f) << 21;
  result |= (size[1] & 0x7f) << 14;
  result |= (size[2] & 0x7f) << 7;
  result |= size[3] & 0x7f;
  return result;
}

bool exam_frame_header(struct mp3_frame_header header) {
  int frame_sync = 0;
  frame_sync |= (header.first_byte & 0xFF) << 3;
  frame_sync |= (header.second_byte & 0xE0) >> 5;
  return frame_sync == 0x07FF;
}

int get_bit_rate(struct mp3_frame_header s) {
  int bit_rate_code = (s.third_byte & 0xFF) >> 4;
  switch (bit_rate_code) {
  case 0x1:
    return 32;
  case 0x2:
    return 40;
  case 0x3:
    return 48;
  case 0x4:
    return 56;
  case 0x5:
    return 64;
  case 0x6:
    return 80;
  case 0x7:
    return 96;
  case 0x8:
    return 112;
  case 0x9:
    return 128;
  case 0xA:
    return 160;
  case 0xB:
    return 192;
  case 0xC:
    return 224;
  case 0xD:
    return 256;
  case 0xE:
    return 320;
  case 0:
  case 0xF:
  default:
    printf("trouble get_bit_rate with code: %d\n", bit_rate_code);
    return 0;
  }
}

int get_sampling_rate_frequency(struct mp3_frame_header s) {
  int sampling_rate_frequency_code = (s.third_byte & 0xC) >> 2;
  switch(sampling_rate_frequency_code) {
  case 0x00:
    return 44100;
  case 0x01:
    return 48000;
  case 0x02:
    return 32000;
  case 0x03:
  default:
    printf("trouble get_sampling_rate_frequency_code: %d\n", sampling_rate_frequency_code);
    return 0;
  }
}

int get_frame_len(struct mp3_frame_header s) {
  int bit_rate = get_bit_rate(s);
  int sampling_rate = get_sampling_rate_frequency(s);
  int padding = (s.third_byte & 0x02) >> 1;
  // bit_rate in k
  return (int)((144 * bit_rate * 1000 / sampling_rate) + padding);
}

int get_frame_data_len(struct mp3_frame_header s) {
  int frame_len = get_frame_len(s);
  return frame_len - sizeof(struct mp3_frame_header);
}
