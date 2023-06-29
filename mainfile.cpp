#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// ��������� ��� ������ � ����������� ���� �����
class Tape {
public:
    virtual bool read(int& data) = 0;
    virtual bool write(int data) = 0;
    virtual bool rewind() = 0;
    virtual bool shift() = 0;
};

// �����, ����������� ������ � ������ ����� ����
class FileTape : public Tape {
private:
    std::fstream file;

public:
    FileTape(const std::string& filename) {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }

    ~FileTape() {
        file.close();
    }

    bool read(int& data) {
        return file.read(reinterpret_cast<char*>(&data), sizeof(int));
    }

    bool write(int data) {
        return file.write(reinterpret_cast<const char*>(&data), sizeof(int));
    }

    bool rewind() {
        file.seekg(0, std::ios::beg);
        return true;
    }

    bool shift() {
        file.seekp(0, std::ios::beg);
        return true;
    }
};

// �����, ����������� �������� ���������� ������ � ������� ����� �� ��������
class TapeSorter {
private:
    int availableMemory;

    void sortTape(Tape& inputTape, Tape& outputTape, int bufferSize) {
        std::vector<int> buffer(bufferSize);

        int count = 0;
        while (inputTape.read(buffer[count])) {
            count++;

            // ���� ����� ��������� ��������, ��������� ��� � ���������� �� �������� �����
            if (count == bufferSize) {
                std::sort(buffer.begin(), buffer.end());
                for (int data : buffer) {
                    outputTape.write(data);
                }
                count = 0;
            }
        }

        // ���� � ������ �������� ������, ��������� �� � ���������� �� �������� �����
        if (count > 0) {
            buffer.resize(count);
            std::sort(buffer.begin(), buffer.end());
            for (int data : buffer) {
                outputTape.write(data);
            }
        }
    }

public:
    TapeSorter(int memorySize) : availableMemory(memorySize) {}

    void sort(const std::string& inputFile, const std::string& outputFile) {
        FileTape inputTape(inputFile);
        FileTape outputTape(outputFile);

        sortTape(inputTape, outputTape, availableMemory / sizeof(int));
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " input_file output_file" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int availableMemory = 1024; // ����� ����� ������ ��������� ������ � ������

    TapeSorter sorter(availableMemory);
    sorter.sort(inputFile, outputFile);

    std::cout << "Sorting complete." << std::endl;

    return 0;
}
