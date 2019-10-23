//
// Created by huz123 on 10/18/19.
//

#include <iostream>
#include "main.hh"
#include "NALUAnalyzer.hh"

int main(int argc, char* argv[]) {
    std::cout << argv[1] << std::endl;

    std::unique_ptr<BitAnalyzer> bitAnalyzer(new NALUAnalyzer(argv[1]));

    int64_t nextBitSize = 0, numNALU = 0;

    while ((nextBitSize = bitAnalyzer->getNextNALUSize()) >= 0 &&
           numNALU < 100) {
        std::cout << numNALU << " " << nextBitSize << std::endl;
        numNALU += 1;
    }

    return 0;
}
