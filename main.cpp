#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// ��������� ��� ������ � ����������� ���� �����
class TapeInterface {
public:
    virtual void read(int& data) = 0;
    virtual void write(const int& data) = 0;
};

// �����, ����������� ������ � ������ ����� ����
class FileTape : public TapeInterface {
private:
    std::fstream file;

public:
    FileTape(const std::string& filename) {
        file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    }

    ~FileTape() {
        file.close();
    }

    void read(int& data) override {
        file.read(reinterpret_cast<char*>(&data), sizeof(int));
    }

    void write(const int& data) override {
        file.write(reinterpret_cast<const char*>(&data), sizeof(int));
    }

    void rewind() {
        file.seekg(0, std::ios::beg);
    }

    void shift() {
        file.seekp(sizeof(int), std::ios::cur);
    }
};

// �����, ����������� �������� ���������� ������ � ������� ����� �� ��������
class TapeSorter {
public:
    void sort(TapeInterface& inputTape, TapeInterface& outputTape) {
        std::vector<int> data;

        // ������ ������ � ������� ����� � ������
        int value;
        while (true) {
            inputTape.read(value);
            if (inputTape) {
                data.push_back(value);
            }
            else {
                break;
            }
        }

        // ���������� ������
        std::sort(data.begin(), data.end());

        // ������ ��������������� ������ �� �������� �����
        for (const auto& item : data) {
            outputTape.write(item);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./tape_sort <input_file> <output_file>\n";
        return 1;
    }

    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];

    FileTape inputTape(inputFilename);
    FileTape outputTape(outputFilename);

    TapeSorter sorter;
    sorter.sort(inputTape, outputTape);

    std::cout << "Sorting completed.\n";

    return 0;
}