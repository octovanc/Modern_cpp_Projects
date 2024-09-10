#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <stdexcept>
#include <string>

class BrainfuckInterpreter {
public:
    // Constructor to initialize the memory tape
    BrainfuckInterpreter(size_t tapeSize = 30000) : tape(tapeSize, 0), dataPointer(0) {}

    // Main function to run the Brainfuck program
    void runProgram(const std::string &program, const std::string &inputFilePath) {
        openInputFile(inputFilePath);  // Open the input file for ','
        executeCommands(program);      // Execute the Brainfuck commands
        closeInputFile();              // Close the input file when done
    }

private:
    std::vector<uint8_t> tape;  // Memory tape initialized to 30000 cells
    size_t dataPointer;         // Pointer to the current position on the tape
    std::ifstream inputFile;    // Input file stream for the ',' command
    std::stack<size_t> loopStack;  // Stack for managing loops

    // Function to open the input file for the ',' command
    void openInputFile(const std::string &inputFilePath) {
        inputFile.open(inputFilePath);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Failed to open input file: " + inputFilePath);
        }
    }

    // Function to close the input file
    void closeInputFile() {
        if (inputFile.is_open()) {
            inputFile.close();
        }
    }

    // Function to execute Brainfuck commands from the program
    void executeCommands(const std::string &program) {
        for (size_t i = 0; i < program.size(); ++i) {
            switch (program[i]) {
                case '>':
                    moveRight();
                    break;
                case '<':
                    moveLeft();
                    break;
                case '+':
                    incrementByte();
                    break;
                case '-':
                    decrementByte();
                    break;
                case '.':
                    outputByte();
                    break;
                case ',':
                    inputByte();
                    break;
                case '[':
                    beginLoop(i, program);
                    break;
                case ']':
                    endLoop(i);
                    break;
                default:
                    // Ignore non-command characters
                    break;
            }
        }
    }

    // Command Functions
    void moveRight() {
        if (dataPointer + 1 >= tape.size()) {
            throw std::out_of_range("Data pointer out of bounds (right).");
        }
        ++dataPointer;
    }

    void moveLeft() {
        if (dataPointer == 0) {
            throw std::out_of_range("Data pointer out of bounds (left).");
        }
        --dataPointer;
    }

    void incrementByte() {
        ++tape[dataPointer];
    }

    void decrementByte() {
        --tape[dataPointer];
    }

    void outputByte() const {
        std::cout << static_cast<char>(tape[dataPointer]);
    }

    void inputByte() {
        if (inputFile.is_open() && !inputFile.eof()) {
            char inputChar;
            inputFile.get(inputChar);
            if (!inputFile.eof()) {
                tape[dataPointer] = static_cast<uint8_t>(inputChar);
            } else {
                tape[dataPointer] = 0;  // EOF behavior: return 0
            }
        } else {
            tape[dataPointer] = 0;  // Default input if no file or EOF
        }
    }

    // Loop handling
    void beginLoop(size_t &instructionPointer, const std::string &program) {
        if (tape[dataPointer] == 0) {
            // Jump forward to matching ']'
            size_t matchingBracket = findMatchingBracket(instructionPointer, program, true);
            if (matchingBracket == std::string::npos) {
                throw std::runtime_error("Unmatched '['.");
            }
            instructionPointer = matchingBracket;
        } else {
            loopStack.push(instructionPointer);  // Store position of '['
        }
    }

    void endLoop(size_t &instructionPointer) {
        if (tape[dataPointer] != 0) {
            if (loopStack.empty()) {
                throw std::runtime_error("Unmatched ']'.");
            }
            instructionPointer = loopStack.top() - 1;  // Jump back to '['
        } else {
            loopStack.pop();  // Exit loop
        }
    }

    // Helper function to find the matching bracket for loops
    size_t findMatchingBracket(size_t start, const std::string &program, bool forward) const {
        int depth = 1;
        if (forward) {
            for (size_t i = start + 1; i < program.size(); ++i) {
                if (program[i] == '[') {
                    ++depth;
                } else if (program[i] == ']') {
                    --depth;
                    if (depth == 0) {
                        return i;
                    }
                }
            }
        } else {
            for (size_t i = start; i != std::string::npos; --i) {
                if (program[i] == ']') {
                    ++depth;
                } else if (program[i] == '[') {
                    --depth;
                    if (depth == 0) {
                        return i;
                    }
                }
            }
        }
        return std::string::npos;  // No matching bracket found
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./brainfuck_interpreter <input_file>\n";
        return 1;
    }

    BrainfuckInterpreter interpreter;
    std::string program;

    // Read Brainfuck source code from stdin
    std::getline(std::cin, program);

    try {
        interpreter.runProgram(program, argv[1]);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}