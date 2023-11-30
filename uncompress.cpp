#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <compressed_file> <output_file>" << std::endl;
        return 1;
    }

    // Open the input file for reading
    FancyInputStream inFile(argv[1]);
    if (!inFile.good()) {
        std::cerr << "Error: Unable to open input file " << argv[1] << std::endl;
        return 1;
    }

    // Read bytes from the file. Read the header and count the number of occurrences of each byte value
    std::vector<int> freqs(256, 0);

    // Attempt to read the first int from the file
    int firstInt = inFile.read_int();

    // Check if the file is empty
    if (firstInt == -1) {
        // Output an empty file or handle as needed
        FancyOutputStream outFile(argv[2]);
        outFile.flush();
        return 0;
    }

    // Increment the frequency count for the first int
    freqs[0] = firstInt;
    int charCount = 0;
    for (size_t i = 1; i < freqs.size(); ++i) {
        freqs[i] = inFile.read_int();
        charCount += freqs[i];
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

    // If the input file is empty, handle it appropriately
    //find other way to see if file is empty
    //if in.read[btye/bit/int] == -1 -> empty
    // if (freqs[0] == 0) {
    //     // Output an empty file or handle as needed
    //     outFile.flush();
    //     return 0;
    // }

    // Using the Huffman coding tree, decode each byte and write to the output file
    int headerSize = freqs.size() * sizeof(int);
    int bytesRead = headerSize -1;
    int fileSize = inFile.filesize();
    cout << headerSize;
    // cout << bytesRead;
    // cout << fileSize;

    // while (inFile.good() && (bytesRead < inFile.filesize()))

    for (int i = 1; i < charCount; i++) { //change to iterate for the content of the file (fileSize - headerSize)
        unsigned char symbol = huffmanTree.decode(inFile);
        outFile.write_byte(symbol);
    }

    outFile.flush();

    return 0;
}