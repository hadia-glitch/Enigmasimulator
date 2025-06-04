#ifndef ENIGMA_H 
#define ENIGMA_H 
 
#include <string> 
#include <vector> 
#include <unordered_map> 
#include <stdexcept> #include <algorithm> 
#include <sstream> // Required for std::istringstream 
 
// Rotor class representing a single rotor in the Enigma machine class Rotor { public: 
    explicit Rotor(const std::string& mapping, const std::string& notchPosition = "") 
        : mapping(mapping), reverseMapping(26), position(0), notchPosition(notchPosition) {         if (mapping.length() != 26) { 
            throw std::invalid_argument("Rotor mapping must have exactly 26 characters."); 
        } 
        for (size_t i = 0; i < mapping.size(); ++i) {             reverseMapping[mapping[i] - 'A'] = 'A' + i; 
        } 
    } 
 
    void SetPosition(int pos) {         position = pos % 26; 
    } 
 
    char EncryptForward(char input) const {         int index = (input - 'A' + position) % 26; 
        return mapping[index] - position < 'A' ? mapping[index] - position + 26 : mapping[index] - position; 
    } 
 
    char EncryptBackward(char input) const {         int index = (input - 'A' + position) % 26;         return reverseMapping[index] - position < 'A' ? reverseMapping[index] - position + 26 : reverseMapping[index] - position; 
    } 
 
    void Step() { 
        position = (position + 1) % 26; 
    } 
 
    bool AtNotch() const { 
        return notchPosition.find('A' + position) != std::string::npos; 
    } 
 
private: 
    std::string mapping;     std::vector<char> reverseMapping;     int position;     std::string notchPosition; 
}; 
 
// Reflector class representing the reflector in the Enigma machine class Reflector { public: 
    explicit Reflector(const std::string& mapping) {         if (mapping.length() != 26) { 
            throw std::invalid_argument("Reflector mapping must have exactly 26 characters."); 
        } 
        this->mapping = mapping; 
    } 
 
    char Reflect(char input) const {         return mapping[input - 'A']; 
    } 
 
private: 
    std::string mapping; 
}; 
 
// Plugboard class representing the plugboard configuration class Plugboard { 
public:
    Plugboard() : wiring(26) {         for (char c = 'A'; c <= 'Z'; ++c) {             wiring[c - 'A'] = c; 
        } 
    } 
 
    void Configure(const std::string& pairs) {         Reset(); 
        std::istringstream ss(pairs);         std::string pair;         while (ss >> pair) { 
            if (pair.length() != 2 || !isalpha(pair[0]) || !isalpha(pair[1])) {                 throw std::invalid_argument("Invalid plugboard configuration."); 
            } 
            char a = toupper(pair[0]);             char b = toupper(pair[1]);             wiring[a - 'A'] = b;             wiring[b - 'A'] = a; 
        } 
    } 
 
    char Substitute(char input) const {         return wiring[input - 'A']; 
    } 
 
private: 
    void Reset() { 
        for (char c = 'A'; c <= 'Z'; ++c) { 
            wiring[c - 'A'] = c; 
        } 
    } 
 
    std::vector<char> wiring; 
}; 
 
// EnigmaMachine class representing the entire Enigma machine class EnigmaMachine { public: 
    EnigmaMachine(const std::vector<Rotor>& rotors, const Reflector& reflector, const Plugboard& plugboard) 
        : rotors(rotors), reflector(reflector), plugboard(plugboard) {} 
 
    void SetRotorPositions(const std::vector<int>& positions) {         if (positions.size() != rotors.size()) { 
            throw std::invalid_argument("Number of positions must match the number of rotors."); 
        } 
        for (size_t i = 0; i < rotors.size(); ++i) {             rotors[i].SetPosition(positions[i]); 
        } 
    } 
 
    std::string EncryptDecrypt(const std::string& message) {         std::string result;         for (char c : message) {             if (!isalpha(c)) { 
                result += c; // Non-alphabetic characters remain unchanged                 continue; 
            }
            char letter = toupper(c); 
 
            // Plugboard substitution             letter = plugboard.Substitute(letter); 
 
            // Forward through rotors             for (auto& rotor : rotors) {                 letter = rotor.EncryptForward(letter); 
            } 
 
            // Reflector 
            letter = reflector.Reflect(letter); 
 
            // Backward through rotors 
            for (auto it = rotors.rbegin(); it != rotors.rend(); ++it) {                 letter = it->EncryptBackward(letter); 
            } 
 
            // Plugboard substitution             letter = plugboard.Substitute(letter); 
 
            result += letter; 
 
            // Step rotors 
            StepRotors(); 
        } 
        return result; 
    } 
 
private: 
    void StepRotors() { 
        for (size_t i = 0; i < rotors.size(); ++i) {             rotors[i].Step(); 
            if (i == rotors.size() - 1 || !rotors[i].AtNotch()) {                 break; 
            } 
        } 
    } 
 
    std::vector<Rotor> rotors;     Reflector reflector; 
    Plugboard plugboard; 
}; 
 
#endif // ENIGMA_H 







