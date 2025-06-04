#include "titlepage.h"  // Title page header 
#include "enigma.h"       // Enigma core implementation 
#include <QApplication> 
#include <QComboBox> 
#include <QGridLayout> 
#include <QLabel> 
#include <QLineEdit> 
#include <QPushButton> 
#include <QSpinBox> 
#include <QTextEdit> 
#include <QVBoxLayout> 
#include <QWidget> 
#include <QMessageBox> 
#include <QTimer> 
#include <QDebug> 
#include <unordered_map> 
 
class EnigmaMachineGUI : public QWidget { 
    Q_OBJECT 
 
public: 
    EnigmaMachineGUI(QWidget* parent = nullptr) : QWidget(parent) { 
// Set black background         setStyleSheet("background-color: black; color: white;");         setStyleSheet( 
            "QComboBox {" 
            "    border: 2px solid white;" 
            "    border-radius: 5px;" 
            "    padding: 2px;" 
            "}" 
            "QComboBox:hover {" 
            "    border: 2px solid grey;" 
            "}" 
            "QComboBox:focus {" 
            "    border: 2px solid white;" 
            "}" 
            "QComboBox::drop-down {" 
            "    border: white;" 
            "}" 
            ); 
 
        // Setup Enigma components         availableRotors = { 
            {"Rotor I", "EKMFLGDQVZNTOWYHXUSPAIBRCJ"}, 
            {"Rotor II", "AJDKSIRUXBLHWTMCQGZNPYFVOE"},             {"Rotor III", "BDFHJLCPRTXVZNYEIWGAKMUSQO"} 
        }; 
 
        reflectors = { 
            {"Reflector B", "YRUHQSLDPXNGOKMIEBFZCWVJAT"}, 
            {"Reflector C", "FVPJIAOYEDRZXWGCTKUQSBNMHL"} }; 
 
        plugboard = new Plugboard(); 
 
        // GUI Setup 
        QVBoxLayout* mainLayout = new QVBoxLayout(this); 
 
        // Rotor Selection 
        QHBoxLayout* rotorSelectionLayout = new QHBoxLayout(); 
        for (int i = 0; i < 3; ++i) { 
            QComboBox* rotorSelect = new QComboBox();             for (const auto& rotor : availableRotors) { 
                rotorSelect->addItem(QString::fromStdString(rotor.first)); 
            } 
            rotorSelectors.push_back(rotorSelect); 
            rotorSelectionLayout->addWidget(new QLabel(QString("Rotor %1:").arg(i + 1)));             rotorSelectionLayout->addWidget(rotorSelect); 
        } 
        mainLayout->addLayout(rotorSelectionLayout); 
 
        // Rotor Settings 
        QHBoxLayout* rotorLayout = new QHBoxLayout(); 
        for (int i = 0; i < 3; ++i) { 
            QSpinBox* rotorPos = new QSpinBox();             rotorPos->setRange(0, 25);             rotorPositions.push_back(rotorPos); 
            rotorLayout->addWidget(new QLabel(QString("Rotor %1 Position:").arg(i + 1)));             rotorLayout->addWidget(rotorPos); 
} 
mainLayout->addLayout(rotorLayout); 
 
        // Reflector Selection 
        QHBoxLayout* reflectorLayout = new QHBoxLayout();         reflectorSelector = new QComboBox();         for (const auto& reflector : reflectors) { 
            reflectorSelector->addItem(QString::fromStdString(reflector.first)); 
        } 
        reflectorLayout->addWidget(new QLabel("Reflector:"));         reflectorLayout->addWidget(reflectorSelector);         mainLayout->addLayout(reflectorLayout); 
 
        // Plugboard Configuration 
        QHBoxLayout* plugboardLayout = new QHBoxLayout();         plugboardEdit = new QLineEdit(); 
        plugboardEdit->setPlaceholderText("Enter plugboard pairs (e.g., AB CD EF)");         plugboardLayout->addWidget(new QLabel("Plugboard Config:"));         plugboardLayout->addWidget(plugboardEdit);         mainLayout->addLayout(plugboardLayout); 
 
        // Text Input/Output 
        QHBoxLayout* textLayout = new QHBoxLayout();         plaintextEdit = new QTextEdit(); 
        plaintextEdit->setPlaceholderText("Enter plaintext..."); 
        plaintextEdit->setStyleSheet("background-color: #000044; color: white; border: 1px solid blue;");         textLayout->addWidget(plaintextEdit); 
 
        ciphertextEdit = new QTextEdit(); 
        ciphertextEdit->setPlaceholderText("Ciphertext will appear here..."); ciphertextEdit->setReadOnly(true); 
        ciphertextEdit->setStyleSheet("background-color: #000044; color: white; border: 1px solid blue;");         textLayout->addWidget(ciphertextEdit);         mainLayout->addLayout(textLayout); 
 
        // Encrypt and Decrypt Buttons 
        QHBoxLayout* buttonLayout = new QHBoxLayout(); 
        QPushButton* encryptButton = new QPushButton("Encrypt");         QPushButton* decryptButton = new QPushButton("Decrypt");         buttonLayout->addWidget(encryptButton);         buttonLayout->addWidget(decryptButton);         mainLayout->addLayout(buttonLayout); 
 
        // Lampboard 
        QGridLayout* lampboardLayout = new QGridLayout(); 
        for (char c = 'A'; c <= 'Z'; ++c) { 
            QPushButton* lamp = new QPushButton(QString(c));             lamp->setFixedSize(40, 40);             lamp->setStyleSheet( 
                "QPushButton { background-color: #444; color: white; border-radius: 20px; }" 
                "QPushButton:pressed { background-color: yellow; color: black; }");             lampButtons[c] = lamp; 
            lampboardLayout->addWidget(lamp, (c - 'A') / 9, (c - 'A') % 9); 
        } 
        mainLayout->addLayout(lampboardLayout); 
 
        // Connect Signals to Slots 
        connect(encryptButton, &QPushButton::clicked, this, &EnigmaMachineGUI::EncryptMessage); connect(decryptButton, &QPushButton::clicked, this, &EnigmaMachineGUI::DecryptMessage); connect(plaintextEdit, &QTextEdit::textChanged, this, &EnigmaMachineGUI::HandleLiveInput); 
 
        setLayout(mainLayout);         setWindowTitle("Enigma Machine"); 
    } 
 
private slots: 
    void EncryptMessage() { 
        ProcessMessage(true); 
    } 
 
    void DecryptMessage() { 
        ProcessMessage(false); 
    } 
 
    void ProcessMessage(bool encrypt) { 
        EnigmaMachine enigma = ConfigureEnigma(); 
 
        QString input = plaintextEdit->toPlainText();         if (input.isEmpty()) { 
            QMessageBox::warning(this, "Error", "Input cannot be empty!");             return; 
        } 
 
        std::string result = enigma.EncryptDecrypt(input.toStdString());         ciphertextEdit->setText(QString::fromStdString(result)); 
        IlluminateMultiple(result); // Simultaneous illumination of all encoded letters 
    } 
 
    void HandleLiveInput() { 
        EnigmaMachine enigma = ConfigureEnigma(); 
 
        QString input = plaintextEdit->toPlainText();         if (input.isEmpty()) {             IlluminateNone();             return; 
        } 
 
        std::string encodedString;         for (QChar c : input) {             if (c.isLetter()) { 
                char lastChar = c.toUpper().toLatin1(); 
                encodedString += enigma.EncryptDecrypt(std::string(1, lastChar)); 
            } 
        } 
        IlluminateMultiple(encodedString); 
    } 
 
private: 
    EnigmaMachine ConfigureEnigma() {         std::vector<Rotor> selectedRotors;         for (size_t i = 0; i < rotorSelectors.size(); ++i) {             int index = rotorSelectors[i]->currentIndex();             selectedRotors.emplace_back(availableRotors[index].second); 
        } 
 
        int reflectorIndex = reflectorSelector->currentIndex(); 
Reflector reflector(reflectors[reflectorIndex].second); 
 
        EnigmaMachine enigma(selectedRotors, reflector, *plugboard); 
 
        std::vector<int> positions;         for (auto* spinBox : rotorPositions) {             positions.push_back(spinBox->value()); 
        } 
        enigma.SetRotorPositions(positions); 
 
        QString plugboardInput = plugboardEdit->text();         if (!plugboardInput.isEmpty()) { 
            plugboard->Configure(plugboardInput.toStdString()); 
        } 
 
        return enigma; 
    } 
 
    void IlluminateSingle(char letter) {         IlluminateNone();         letter = toupper(letter); 
        if (lampButtons.find(letter) != lampButtons.end()) {             lampButtons[letter]->setStyleSheet( 
                "QPushButton { background-color: yellow; color: black; }"); 
        } 
    } 
 
    void IlluminateMultiple(const std::string& letters) {         IlluminateNone();         for (char letter : letters) { 
            letter = toupper(letter); 
            if (lampButtons.find(letter) != lampButtons.end()) {                 lampButtons[letter]->setStyleSheet( 
                    "QPushButton { background-color: yellow; color: black; }"); 
            } 
        } 
    } 
 
    void IlluminateNone() { 
        for (auto& [letter, button] : lampButtons) {             button->setStyleSheet( 
                "QPushButton { background-color: #444; color: white; }"); 
        } 
    } 
 
    QTextEdit* plaintextEdit; 
    QTextEdit* ciphertextEdit; 
    QLineEdit* plugboardEdit;     QComboBox* reflectorSelector;     std::vector<QSpinBox*> rotorPositions;     std::vector<QComboBox*> rotorSelectors;     std::unordered_map<char, QPushButton*> lampButtons;     Plugboard* plugboard; 
    std::vector<std::pair<std::string, std::string>> availableRotors;     std::vector<std::pair<std::string, std::string>> reflectors; 
}; 
 
int main(int argc, char* argv[]) {     QApplication app(argc, argv);     TitlePage titlePage;     titlePage.show(); 
 
    EnigmaMachineGUI mainWindow; 
    QTimer::singleShot(3000, &titlePage, &QWidget::close); 
    QTimer::singleShot(3000, &mainWindow, &QWidget::show); 
 
    return app.exec(); 
} 
 
#include "main.moc" 
 
