std::vector<uint8_t> encoder(String word, std::map<char, std::vector<uint8_t>> dict);
String decoder(std::vector<uint8_t> bitArray, std::map<std::vector<uint8_t>, char> dict);