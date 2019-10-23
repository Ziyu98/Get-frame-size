//
// Created by huz123 on 10/19/19.
//

#ifndef H264ANALYZER_FRAMEANALYZER_HH
#define H264ANALYZER_FRAMEANALYZER_HH

#include <cstdint>
#include <vector>
#include <cassert>
#include "bitAnalzyer.hh"

class NALUAnalyzer : public BitAnalyzer {
public:
    NALUAnalyzer(std::string h264FileName);

    ~NALUAnalyzer() { fclose(filePtr); }

    // return number of bits
    virtual int64_t getNextNALUSize() override;

private:
    static const uint32_t kNALUBoundarySize1{4};
    static const uint32_t kNALUBoundarySize2{3};

    const char kNALUBoundary1[kNALUBoundarySize1] = {0x00, 0x00, 0x00, 0x01};
    const char kNALUBoundary2[kNALUBoundarySize2] = {0x00, 0x00, 0x01};

    int64_t posCurr{0}, posEnd{0};
    int64_t posLen{0};

    std::vector<int64_t> NALUBoundary = {0};
    FILE* filePtr{nullptr};
};

#endif //H264ANALYZER_FRAMEANALYZER_HH
