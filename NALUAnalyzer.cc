//
// Created by huz123 on 10/19/19.
//

#include "NALUAnalyzer.hh"

#include <cstdio>
#include <cstring>

NALUAnalyzer::NALUAnalyzer(std::string h264FileName) {
    filePtr = fopen(h264FileName.c_str(), "r");
    if (filePtr == nullptr) {
        assert(0);
        return;
    }

    fseek(filePtr, 0L, SEEK_END);

    posEnd = ftell(filePtr) - 4;
}

int64_t NALUAnalyzer::getNextNALUSize() {
    auto buf = new char[kNALUBoundarySize1];
    int64_t size = -1;

    while (posCurr++ <= posEnd) {
        fseek(filePtr, posCurr, SEEK_SET);
        size_t res = fread(buf, sizeof(char), kNALUBoundarySize1, filePtr);

        if (res != kNALUBoundarySize1) {
            return size;
        }

        //the position of "00 00 00 01"
        if (memcmp(kNALUBoundary1, buf, kNALUBoundarySize1) == 0) {
            auto lastPos = NALUBoundary.back();
            NALUBoundary.push_back(posCurr);
            size = posCurr - lastPos;
            break;
        } else if ((memcmp(kNALUBoundary1, buf, kNALUBoundarySize1) != 0) &&
                   (memcmp(kNALUBoundary2, (buf + sizeof(char)),
                           kNALUBoundarySize2) == 0)) {
            auto lastPos = NALUBoundary.back();
            NALUBoundary.push_back(posCurr);
            size = posCurr - lastPos;
            break;
        }
    }
    delete[] buf;
    return size;
}