//
// Created by huz123 on 10/19/19.
//

#ifndef H264ANALYZER_BITANALZYER_HH
#define H264ANALYZER_BITANALZYER_HH

#include <cstdint>
#include <memory>

class BitAnalyzer {
public:
    virtual int64_t getNextNALUSize() = 0;

    std::shared_ptr<char> getCurrPos() const {return currPos;}
private:
    std::shared_ptr<char> currPos{nullptr};
};

#endif //H264ANALYZER_BITANALZYER_HH
