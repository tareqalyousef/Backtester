#include <csvReader.h>
#include <fmt/core.h>

using std::getline;

CSVReader::CSVReader(string path)
    : path(path)
{
    file = ifstream(path);

    string line = "";
    getline(file, line);
    string word = "";

    for (int i = 0; i <= line.size(); i++)
    {
        if (line[i] == ',' || i == line.size())
        {
            headers.push_back(word);
            word = "";
            continue;
        }

        word += line[i];
    }
}

CSVReader::~CSVReader()
{
    file.close();
}

bool CSVReader::hasNext()
{
    return file.peek() != EOF;
}

unordered_map<string, CSVField> CSVReader::getRow()
{
    unordered_map<string, CSVField> map;
    string line = "";
    getline(file, line);
    string word = "";
    int headerIdx = 0;

    for (int i = 0; i <= line.size(); i++)
    {
        if (i == line.size() || line[i] == ',')
        {
            map[headers[headerIdx]] = CSVField(word);
            word = "";
            headerIdx++;
            continue;
        }

        word += line[i];
    }

    return map;
}