#include <map>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
void Serialize(T pod, std::ostream& out);

void Serialize(const std::string& str, std::ostream& out);

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);

template <typename T>
void Deserialize(std::istream& in, T& pod);

void Deserialize(std::istream& in, std::string& str);

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);

// Для простых примитивных типов (int, bool, char и т. д.) и plain old data
template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&pod), sizeof(T));
}

void Serialize(const std::string& str, std::ostream& out) {
    size_t size = str.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    out.write(reinterpret_cast<const char*>(&str[0]), size);
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    size_t size = data.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    for (size_t i = 0; i < size; ++i) {
        Serialize(data[i], out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    size_t size = data.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    std::vector<std::pair<T1, T2>> data_in_vec;
    for (const auto& pair : data) {
        Serialize(pair.first, out);
        Serialize(pair.second, out);
    }
}



template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod), sizeof(T));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    str.resize(size);
    in.read(reinterpret_cast<char*>(&str[0]), size);
}
template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    data.resize(size);
    for (size_t i = 0; i < size; ++i) {
        T elem;
        Deserialize(in, elem);
        data[i] = elem;
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    for (size_t i = 0; i < size; ++i) {
        T1 key;
        Deserialize(in, key);
        T2 value;
        Deserialize(in, value);
        data[key] = value;
    }
}
