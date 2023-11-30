#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <original_file> <compressed_file>" << std::endl;
        return 1;
    }

    // Open the input file for reading
    FancyInputStream inFile(argv[1]);
    if (!inFile.good()) {
        std::cerr << "Error: Unable to open input file " << argv[1] << std::endl;
        return 1;
    }

    // Read bytes from the file. Count the number of occurrences of each byte value
    std::vector<int> freqs(256, 0);
    int byte;

    // Attempt to read the first byte from the file
    int firstByte = inFile.read_byte();
    std::cout << "First Byte: " << firstByte << std::endl;

    // Check if the file is empty
    if (firstByte == -1) {
        // Output an empty file or handle as needed
        FancyOutputStream outFile(argv[2]);
        outFile.flush();
        return 0;
    }

    // Increment the frequency count for the first byte
    freqs[firstByte]++;

    // Reset the input file to read from the beginning
    inFile.reset();


    while ((byte = inFile.read_byte()) != -1) {
        freqs[byte]++;
    }

    // Construct a Huffman coding tree
    HCTree huffmanTree;
    huffmanTree.build(freqs);

    // Open the output file for writing
    FancyOutputStream outFile(argv[2]);
    if (!outFile.good()) {
        std::cerr << "Error: Unable to open output file " << argv[2] << std::endl;
        return 1;
    }

    // Write the header to the output file
    for (int freq : freqs) {
        outFile.write_int(freq);
    }

    // Reset the input file to read from the beginning
    inFile.reset();

    // Using the Huffman coding tree, translate each byte and write to the output file
    while ((byte = inFile.read_byte()) != -1) {
        huffmanTree.encode(byte, outFile);
    }

    return 0;
}