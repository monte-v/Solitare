#pragma once
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

struct Record {
    sf::Time time;
    std::string formattedTime;

    Record() = default;
    Record(sf::Time t, const std::string& ft) : time(t), formattedTime(ft) {}

    bool operator<(const Record& other) const {
        return time < other.time;  
    }
};

class RecordsManager {
private:
    std::vector<Record> records;
    static constexpr int MAX_RECORDS = 10;
    std::string filename = "records.dat";

public:
    RecordsManager();

    void load();
    void save() const;

    bool isNewRecord(sf::Time time) const;
    void addRecord(sf::Time time, const std::string& formattedTime);
    const std::vector<Record>& getRecords() const { return records; }

    void clear();

    static std::string formatTimeWithMs(sf::Time time);
    static std::string formatTimeForDisplay(sf::Time time);
};