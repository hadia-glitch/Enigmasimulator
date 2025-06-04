
#ifndef TITLEPAGE_H 
#define TITLEPAGE_H 
#include <QWidget> 
#include <QVBoxLayout> 
#include <QLabel> 
#include <QPixmap> 
 
class TitlePage : public QWidget { 
    Q_OBJECT 
 
public:
    TitlePage(QWidget* parent = nullptr) : QWidget(parent) {         setStyleSheet("background-color: black; color: white;");         setFixedSize(500, 400); // Adjust the window size as needed 
 
        QVBoxLayout* layout = new QVBoxLayout(this); 
 
        // Title label 
        QLabel* titleLabel = new QLabel("Enigma Simulator", this); 
        titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; text-align: center;");         titleLabel->setAlignment(Qt::AlignCenter);         layout->addWidget(titleLabel); 
 
        // Image loading from file system 
        QLabel* imageLabel = new QLabel(this); 
        QPixmap image("C:/Users/user/Desktop/enigma.jfif"); // Path to your image 
        if (image.isNull()) { 
            // Display error message if the image fails to load             imageLabel->setText("Image failed to load!");             imageLabel->setAlignment(Qt::AlignCenter); 
            imageLabel->setStyleSheet("color: red; font-weight: bold;"); 
        } else { 
            // Scale and display the image 
            image = image.scaled(300, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);             imageLabel->setPixmap(image);             imageLabel->setAlignment(Qt::AlignCenter); 
        } 
        layout->addWidget(imageLabel); 
 
setLayout(layout); 
    } 
}; 
 
#endif // TITLEPAGE_H 
