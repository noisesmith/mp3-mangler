#include <tuple>
#include <map>
#include <cstdint>
#include <vector>
#include <fstream>

#define T(A, B) std::make_tuple(A, B)

namespace mp3
{
  enum Version{v_2_5, v_reserved, v_2, v_1};

  enum Layer{l_reserved, l_iii, l_ii, l_i};

  enum Protected{p_crc, p_none};

  enum ChannelMode{stereo, joint_stereo, dual_channel, single_channel};

  enum Emphasis {none, e_50_15_ms, e_reserved, e_ccit_j_17};

  struct Header
  {
    Version version;
    Layer layer;
    Protected protect;
    uint8_t _br_bits;
    uint8_t _sr_bits;
    bool pad;
    bool priv;
    ChannelMode mode;
    uint8_t _me_bits;
    bool copyright;
    bool original;
    Emphasis emphasis;
    int bit_rate;
    int sample_rate;
    std::tuple<int, int> mode_ext;
  };

  struct Header parse(uint32_t frame);

  struct LayerVersion
  {
    const int16_t bit_rates[16];
    const int sample_rates[4];
    const std::tuple<int, int> stereo_modes[4];
  };

  const struct LayerVersion v_1_l_i = LayerVersion {
    .bit_rates = {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1},
    .sample_rates = {44100, 48000, 32000, -1},
    .stereo_modes = {T(4, 31), T(8, 31), T(12, 31), T(16, 31)}
  };

  const struct LayerVersion v_1_l_ii = LayerVersion {
    .bit_rates = {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1},
    .sample_rates = {44100, 48000, 32000, -1},
    .stereo_modes = {T(4, 31), T(8, 31), T(12, 31), T(16, 31)}
  };

  const struct LayerVersion v_1_l_iii = LayerVersion {
    .bit_rates = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1},
    .sample_rates = {44100, 48000, 32000, -1},
    .stereo_modes = {T(0, 0), T(1, 0), T(0, 1), T(1, 1)}
  };

  const struct LayerVersion v_2_l_i = LayerVersion {
    .bit_rates = {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1},
    .sample_rates = {22050, 24000, 16000, -1},
    .stereo_modes = {T(4, 31), T(8, 31), T(12, 31), T(16, 31)}
  };

  const struct LayerVersion v_2_l_ii = LayerVersion {
    .bit_rates = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
    .sample_rates = {22050, 24000, 16000, -1},
    .stereo_modes = {T(4, 31), T(8, 31), T(12, 31), T(16, 31)}
  };

  const struct LayerVersion v_2_l_iii = LayerVersion {
    .bit_rates = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
    .sample_rates = {22050, 24000, 16000, -1},
    .stereo_modes = {T(0, 0), T(1, 0), T(0, 1), T(1, 1)}
  };

  const struct LayerVersion v_2_5_l_i = LayerVersion {
    .bit_rates = {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1},
    .sample_rates = {11025, 12000, 8000, -1},
    .stereo_modes = {T(4, 31), T(8, 31), T(12, 31), T(16, 31)}
  };

  const struct LayerVersion v_2_5_l_ii = LayerVersion {
    .bit_rates = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
    .sample_rates = {11025, 12000, 8000, -1},
    .stereo_modes = {T(4, 31), T(8, 31), T(12, 31), T(16, 31)}
  };

  const struct LayerVersion v_2_5_l_iii = LayerVersion {
    .bit_rates = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
    .sample_rates = {11025, 12000, 8000, -1},
    .stereo_modes = {T(0, 0), T(1, 0), T(0, 1), T(1, 1)}
  };

  const std::map<std::tuple<Version, Layer>, LayerVersion> variants = {
    {T(v_1,   l_i),   v_1_l_i},
    {T(v_1,   l_ii),  v_1_l_ii},
    {T(v_1,   l_iii), v_1_l_iii},
    {T(v_2,   l_i),   v_2_l_i},
    {T(v_2,   l_ii),  v_2_l_ii},
    {T(v_2,   l_iii), v_2_l_iii},
    {T(v_2_5, l_i),   v_2_5_l_i},
    {T(v_2_5, l_ii),  v_2_5_l_ii},
    {T(v_2_5, l_iii), v_2_5_l_iii}
  };

  bool get_header(uint32_t bytes, std::vector<struct Header> header_container);

  int pass_through(std::ifstream& is, std::ofstream& os);
}
