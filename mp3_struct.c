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
  result |= size[0] & 0x7f << 21;
  result |= size[1] & 0x7f << 14;
  result |= size[2] & 0x7f << 7;
  result |= size[3] & 0x7f;
  return result;
}

int get_bit_rate(struct mp3_frame_header s) {
  int* pIndex = (int*) s.bit_rate_index;
  switch (*pIndex) {
  case 0x1:
    return 32;
  case 0x10:
    return 40;
  case 0x0011:
    return 48;
  case 0x0100:
    return 56;
  case 0x0101:
    return 64;
  case 0x0110:
    return 80;
  case 0x0111:
    return 96;
  case 0x1000:
    return 112;
  case 0x1001:
    return 128;
  case 0x1010:
    return 160;
  case 0x1011:
    return 192;
  case 0x1100:
    return 224;
  case 0x1101:
    return 256;
  case 0x1110:
    return 320;
  case 0:
  case 0x1111:
  default:
    printf("trouble get_bit_rate with code: %d\n", *pIndex);
    return 0;
  }
}
int get_sampling_rate_frequency(struct mp3_frame_header s) {
  bool* pIndex = s.sampling_rate_frequency_index;
  if (!pIndex[0] && !pIndex[1]) {
    return 44100; // 0x00
  } else if (!pIndex[0] && pIndex[1]) {
    return 48000; // 0x01
  } else if (pIndex[0] && !pIndex[1]) {
    return 32000; // 0x10
  } else {
    printf("trouble get_sampling_rate_frequency with index: %x%x", pIndex[0], pIndex[1]);
    return 0;
  }
}
int get_frame_len(struct mp3_frame_header s) {
  int bit_rate = get_bit_rate(s);
  int sampling_rate = get_sampling_rate_frequency(s);
  bool is_padding = s.padding;
  return (int)((144 * bit_rate / sampling_rate) + is_padding);
}
