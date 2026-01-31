// int icalcCrc(char *ptr, int count)
// {
//   int crc;
//   char i;
//   crc = 0;
//   while (--count >= 0) {
//     crc = crc ^ (int) *ptr++ << 8;
//     i = 8;
//     do  {
//       if (crc & 0x8000) {
//         crc = crc << 1 ^ 0x1021;
//       } else {
//         crc = crc << 1;
//       }
//     } while(--i);
//   }
//   return (crc);
// }

uint16_t uicalcCrc(uint8_t c, uint16_t crc)
{
  int count;
  crc = crc ^ (uint16_t) c << 8;
  count = 8;
  do {
    if (crc & 0x8000) {
      crc = crc << 1 ^ 0x1021;
    } else {
      crc = crc << 1;
    }
  } while (--count);
  return crc;
}