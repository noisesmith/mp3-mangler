#include <cstdint>
#include <iostream>
#include "mp3_parse.hpp"

namespace mp3
{
  struct Header parse(uint32_t frame)
  {
    auto header = Header
    {
      .version =        static_cast    <Version>  ((frame >> 18) & 3),
        .layer =          static_cast      <Layer>  ((frame >> 16) & 3),
        .protect =        static_cast  <Protected>  ((frame >> 15) & 1),
        ._br_bits =       static_cast    <uint8_t>  ((frame >> 12) & 7),
        ._sr_bits =       static_cast    <uint8_t>  ((frame >> 10) & 3),
        .pad =            static_cast        <bool> ((frame >> 9) & 1),
        .priv =           static_cast        <bool> ((frame >> 8) & 1),
        .mode =           static_cast <ChannelMode> ((frame >> 6) & 3),
        ._me_bits =       static_cast     <uint8_t> ((frame >> 4) & 3),
        .copyright =      static_cast        <bool> ((frame >> 3) & 1),
        .original =       static_cast        <bool> ((frame >> 2) & 1),
        .emphasis =       static_cast    <Emphasis> (frame & 3)
    };
    auto target = mp3::variants.find(T(header.version, header.layer));
    if (target == mp3::variants.end())
      throw std::runtime_error("mp3::parse (mp3_parse): no valid variant here");
    else {
      auto layer_version = target->second;
      header.bit_rate = layer_version.bit_rates[header._br_bits];
      header.sample_rate = layer_version.sample_rates[header._sr_bits];
      header.mode_ext = layer_version.stereo_modes[header._me_bits];
    }
    return header;
  }

  bool get_header (uint32_t bytes, std::vector<struct Header> header_container)
  {
    if((bytes >> 21) != 0b11111111111)
      return false;
    else {
      // header_container.push_back(parse(bytes));
      return true;
    }
  }

  int process_frames(std::ifstream& is, std::ofstream& os, std::function<char(char)> f)
  {
    std::vector<struct Header> headers;
    char c;
    auto header = new std::vector<char>();
    auto data = new std::vector<char>();
    while (is.good() && os.good() && !is.eof())
    {
      is.get(c);
      if (is.gcount() == 0)
        return 0;
      if (header->size() != 0 || static_cast <uint8_t>(c) == 0xff) {
          header->push_back(c);
      }
      if (header->size() != 0) {
        os.put(c);
      } else {
          data->push_back(c);
          os.put(f(c));
      }
      if (header->size() == 4) {
        uint32_t data = (header->at(0) << 24);
        data += (header->at(1) << 16);
        data += (header->at(2) << 8);
        data += header->at(3);
         // bool header_p = get_header(data, headers);
        header->clear();
      }
    }
    return 0;
  }
}
