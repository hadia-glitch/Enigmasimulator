#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>
#include <string>
#include <cctype>
#include <memory>
#include <unordered_map>

class Rotor {
public:
    Rotor(const std::string& wiring, int position = 0)
        : wiring(wiring), position(position) {}

    char Encrypt(char input) {
        int index = (toupper(input) - 'A' + position) % 26;
        return wiring[index];
    }

    char ReverseEncrypt(char input) {
        int index = wiring.find(toupper(input));
        index = (index - position + 26) % 26;
        return 'A' + index;
    }

    void Rotate() {
        position = (position + 1) % 26;
    }

    int getPosition() const {
        return position;
    }

private:
    std::string wiring;
    int position;
};

class Reflector {
public:
    Reflector(const std::string& wiring) : wiring(wiring) {}

    char Reflect(char input) {
        return wiring[toupper(input) - 'A'];
    }

private:
    std::string wiring;
};

class Plugboard {
public:
    Plugboard() {
        for (char c = 'A'; c <= 'Z'; ++c) {
            swapMap[c] = c;
        }
    }

    void Configure(const std::string& pairs) {
        for (size_t i = 0; i < pairs.size(); i += 2) {
            swapMap[pairs[i]] = pairs[i + 1];
            swapMap[pairs[i + 1]] = pairs[i];
        }
    }

    char Swap(char input) {
        return swapMap[input];
    }

    void Clear() {
        for (char c = 'A'; c <= 'Z'; ++c) {
            swapMap[c] = c;
        }
    }

private:
    std::unordered_map<char, char> swapMap;
};

class EnigmaMachine {
public:
    EnigmaMachine(const std::vector<Rotor>& rotors, const Reflector& reflector, const Plugboard& plugboard)
        : rotors(rotors), reflector(reflector), plugboard(plugboard) {}

    std::string EncryptDecrypt(const std::string& input) {
        std::string result;
        for (char c : input) {
            if (isalpha(c)) {  // Ensure it's a valid character
                c = toupper(c);  // Convert to uppercase (if needed)
                result += EncryptDecryptChar(c);  // Encrypt or decrypt each character
            } else {
                result += c; // Preserve non-alphabet characters
            }
        }
        return result;
    }

    void ConfigurePlugboard(const std::string& pairs) {
        plugboard.Clear();
        plugboard.Configure(pairs);
    }

private:
    char EncryptDecryptChar(char input) {
        // Step 1: Pass through the plugboard
        input = plugboard.Swap(input);

        // Step 2: Pass through the rotors
        for (auto& rotor : rotors) {
            input = rotor.Encrypt(input);
        }

        // Step 3: Reflect
        input = reflector.Reflect(input);

        // Step 4: Pass through the rotors in reverse order
        for (auto it = rotors.rbegin(); it != rotors.rend(); ++it) {
            input = it->ReverseEncrypt(input);
        }

        // Step 5: Pass through the plugboard again
        input = plugboard.Swap(input);

        // Step 6: Rotate the rotors
        rotateRotors();

        return input;
    }

    void rotateRotors() {
        bool rotateNext = true;
        for (auto& rotor : rotors) {
            if (rotateNext) {
                rotor.Rotate();
                rotateNext = (rotor.getPosition() == 0);  // Only rotate the next rotor if the current rotor completed a full turn
            } else {
                break;
            }
        }
    }

    std::vector<Rotor> rotors;
    Reflector reflector;
    Plugboard plugboard;
};

#endif // ENIGMA_H








