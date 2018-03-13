#include <fstream>
#include <array>
#include "gtest/gtest.h"
#include "../mp3_parse.c++"

typedef std::array<int16_t,9> cvt_arr;


cvt_arr get_bitrate_row(int n)
{
  return cvt_arr {
      mp3::v_1_l_i.bit_rates[n],
      mp3::v_1_l_ii.bit_rates[n],
      mp3::v_1_l_iii.bit_rates[n],
      mp3::v_2_l_i.bit_rates[n],
      mp3::v_2_l_ii.bit_rates[n],
      mp3::v_2_l_iii.bit_rates[n],
      mp3::v_2_5_l_i.bit_rates[n],
      mp3::v_2_5_l_ii.bit_rates[n],
      mp3::v_2_5_l_iii.bit_rates[n],
    };
}

cvt_arr bitrates(short int a, short int b, short int c, short int d, short int e, short int f)
{
  return cvt_arr {a, b, c, d, e, f, d, e, f};
}

namespace {

  TEST(mp3_parse, sanity)
  {
    EXPECT_EQ(mp3::Header{}.version, mp3::Header{}.version);
  }

  TEST(mp3_parse, parse)
  {
    bool threw = false;
    // creating an invalid header throws for no valid variant
    try {
      auto h = mp3::parse(0);
    } catch (const std::runtime_error &e) {
      threw = true;
    }
    EXPECT_EQ(threw, true);
    // EXPECT_EQ(h.version, mp3::v_2_5);
  }

  TEST(mp3_parse, row_colum)
    // double checking transcription of data describing bitrates by comparing
    // by column https://www.mp3-tech.org/programmer/frame_header.html
  {
    EXPECT_EQ(bitrates(  0,   0,   0,    0,   0,   0), get_bitrate_row(0));
    EXPECT_EQ(bitrates( 32,  32,  32,   32,   8,   8), get_bitrate_row(1));
    EXPECT_EQ(bitrates( 64,  48,  40,   48,  16,  16), get_bitrate_row(2));
    EXPECT_EQ(bitrates( 96,  56,  48,   56,  24,  24), get_bitrate_row(3));
    EXPECT_EQ(bitrates(128,  64,  56,   64,  32,  32), get_bitrate_row(4));
    EXPECT_EQ(bitrates(160,  80,  64,   80,  40,  40), get_bitrate_row(5));
    EXPECT_EQ(bitrates(192,  96,  80,   96,  48,  48), get_bitrate_row(6));
    EXPECT_EQ(bitrates(224, 112,  96,  112,  56,  56), get_bitrate_row(7));
    EXPECT_EQ(bitrates(256, 128, 112,  128,  64,  64), get_bitrate_row(8));
    EXPECT_EQ(bitrates(288, 160, 128,  144,  80,  80), get_bitrate_row(9));
    EXPECT_EQ(bitrates(320, 192, 160,  160,  96,  96), get_bitrate_row(10));
    EXPECT_EQ(bitrates(352, 224, 192,  176, 112, 112), get_bitrate_row(11));
    EXPECT_EQ(bitrates(384, 256, 224,  192, 128, 128), get_bitrate_row(12));
    EXPECT_EQ(bitrates(416, 320, 256,  224, 144, 144), get_bitrate_row(13));
    EXPECT_EQ(bitrates(448, 384, 320,  256, 160, 160), get_bitrate_row(14));
    EXPECT_EQ(bitrates( -1,  -1,  -1,   -1,  -1,  -1), get_bitrate_row(15));
  }

  TEST(mp3_parse, pass_through)
  {
    std::ifstream is("test.mp3", std::ifstream::binary);
    std::ofstream os("dup.mp3", std::ofstream::binary);
    EXPECT_EQ(mp3::pass_through(is, os), 0);
  }
}
