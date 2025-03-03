#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QWidget>
#include <QIcon>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QMessageBox>

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window() {
        setGeometry(0, 0, 1032, 700);
        setWindowTitle("Source Code Viewer");
        setStyleSheet("background-color: rgb(66, 89, 84); border-radius: 10px;");
        setWindowIcon(QIcon("icon.png"));

        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        urlInput = new QLineEdit(this);
        urlInput->setFixedHeight(32);
        urlInput->setStyleSheet("background-color: black; font: 81 16pt 'Rockwell Extra Bold'; color: white; border: none; border-radius: 10px;");
        urlInput->setPlaceholderText("Enter your URL");
        layout->addWidget(urlInput);

        updateButton = new QPushButton("OK", this);
        updateButton->setFixedSize(41, 31);
        updateButton->setStyleSheet("background-color: rgb(65, 81, 66); font: 81 16pt 'Rockwell Extra Bold'; border: none; border-radius: 25px;");
        updateButton->setCursor(Qt::PointingHandCursor);
        connect(updateButton, &QPushButton::clicked, this, &Window::setSrcText);
        layout->addWidget(updateButton);

        label = new QLabel("Source Code Viewer", this);
        label->setStyleSheet("background-color: rgb(96, 109, 99); font: 81 16pt 'Rockwell Extra Bold'; color: white; border: none; border-radius: 10px;");
        label->setFixedHeight(31);
        layout->addWidget(label);

        srcTextEdit = new QPlainTextEdit(this);
        srcTextEdit->setStyleSheet("background-color: black; font: 81 16pt 'Rockwell Extra Bold'; color: white; border: none; border-radius: 10px;");
        srcTextEdit->setReadOnly(true);
        layout->addWidget(srcTextEdit);

        centralWidget->setLayout(layout);
    }

private slots:
    void setSrcText() {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &Window::handleResponse);
        QUrl url(urlInput->text());
        manager->get(QNetworkRequest(url));
    }

    void handleResponse(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            label->setText(urlInput->text() + " - Source Code Viewer");
            srcTextEdit->setPlainText(reply->readAll());
        } else {
            switch (reply->error()) {
                case QNetworkReply::ContentNotFoundError:
                    srcTextEdit->setPlainText("404 Error");
                    label->setText("404 URL Not found");
                    break;
                case QNetworkReply::AccessDenied:
                    srcTextEdit->setPlainText("");
                    label->setText("403 Error. Permission denied");
                    break;
                default:
                    srcTextEdit->setPlainText(reply->readAll());
                    label->setText("Status code: " + QString::number(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()));
                    break;
            }
        }
        reply->deleteLater();
    }

private:
    QWidget *centralWidget;
    QLineEdit *urlInput;
    QPushButton *updateButton;
    QLabel *label;
    QPlainTextEdit *srcTextEdit;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}

#include "main.moc"