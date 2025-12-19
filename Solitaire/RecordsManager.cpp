#include "RecordsManager.h"
#include <iomanip>
#include <sstream>

RecordsManager::RecordsManager() {
    load();
}

void RecordsManager::load() {
    records.clear();

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    while (file && records.size() < MAX_RECORDS) {
        int64_t microseconds;
        std::string formatted;

        if (file.read(reinterpret_cast<char*>(&microseconds), sizeof(microseconds))) {
            char buffer[256];
            file.getline(buffer, 256, '\0');
            formatted = buffer;

            records.emplace_back(sf::microseconds(microseconds), formatted);
        }
    }

    file.close();

    std::sort(records.begin(), records.end());
}

void RecordsManager::save() const {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для сохранения рекордов" << std::endl;
        return;
    }

    for (const auto& record : records) {
        int64_t microseconds = record.time.asMicroseconds();
        file.write(reinterpret_cast<const char*>(&microseconds), sizeof(microseconds));
        file.write(record.formattedTime.c_str(), record.formattedTime.size() + 1);
    }

    file.close();
}

bool RecordsManager::isNewRecord(sf::Time time) const {
    if (records.size() < MAX_RECORDS) return true;
    return time < records.back().time;
}

void RecordsManager::addRecord(sf::Time time, const std::string& formattedTime) {
    records.emplace_back(time, formattedTime);
    std::sort(records.begin(), records.end());

    if (records.size() > MAX_RECORDS) {
        records.resize(MAX_RECORDS);
    }

    save();
}

void RecordsManager::clear() {
    records.clear();
    std::remove(filename.c_str());
}

std::string RecordsManager::formatTimeWithMs(sf::Time time) {
    int totalSeconds = static_cast<int>(time.asSeconds());
    int milliseconds = time.asMilliseconds() % 1000;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    if (hours > 0) {
        oss << std::setfill('0') << std::setw(2) << hours << ":";
    }
    oss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds;

    return oss.str();
}

std::string RecordsManager::formatTimeForDisplay(sf::Time time) {
    int totalSeconds = static_cast<int>(time.asSeconds());
    int milliseconds = time.asMilliseconds() % 1000;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    if (hours > 0) {
        oss << hours << " ч ";
    }
    if (minutes > 0 || hours > 0) {
        oss << minutes << " мин ";
    }
    oss << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds << " сек";

    return oss.str();
}
