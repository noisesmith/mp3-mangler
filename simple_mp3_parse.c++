#include <fstream>
#include <iostream>
#include "mp3_parse.hpp"

int main(int argc, char **argv) {
  // input
  std::streambuf* ibuf;
  if(argc > 1) {
    std::cerr << "reading " << argv[1] << std::endl;
    std::ifstream finput(argv[1], std::ifstream::binary);
    ibuf = finput.rdbuf();
  } else {
    ibuf = std::cin.rdbuf();
  }
  std::istream input(ibuf);
  // output
  std::streambuf* obuf;
  if(argc > 2) {
    // TODO - currently stdout writing is working, but not file?
    std::cerr << "writing " << argv[2] << std::endl;
    std::ofstream foutput(argv[2], std::ofstream::binary);
    obuf = foutput.rdbuf();
  } else {
    obuf = std::cout.rdbuf();
  }
  std::ostream output(obuf);
  auto f = [](char c){ return c^1; };
  mp3::process_frames(input, output, f);
  return 0;
}
