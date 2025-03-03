from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
import sys
import requests

class Window(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setGeometry(0, 0, 1032, 700)
        self.setWindowTitle("Source code Viewer")
        self.setStyleSheet("background-color: rgb(66, 89, 84); border-radius: 10px;")
        self.setWindowIcon(QIcon("icon.png"))

        self.central_widget = QWidget(self)
        self.setCentralWidget(self.central_widget)

        layout = QVBoxLayout(self.central_widget)

        self.url = QLineEdit(self)
        self.url.setFixedHeight(32)
        self.url.setStyleSheet("background-color: black; font: 81 16pt 'Rockwell Extra Bold'; color: white; border: none; border-radius: 10px;")
        self.url.setPlaceholderText("Enter your URL")
        layout.addWidget(self.url)

        self.UpdatURL = QPushButton("OK", self)
        self.UpdatURL.setFixedSize(41, 31)
        self.UpdatURL.setStyleSheet("background-color: rgb(65, 81, 66); font: 81 16pt 'Rockwell Extra Bold'; border: none; border-radius: 25px;")
        self.UpdatURL.setCursor(Qt.PointingHandCursor)
        self.UpdatURL.clicked.connect(self.setSrcText)
        layout.addWidget(self.UpdatURL)

        self.label = QLabel("Source Code Viewer", self)
        self.label.setStyleSheet("background-color: rgb(96, 109, 99); font: 81 16pt 'Rockwell Extra Bold'; color: white; border: none; border-radius: 10px;")
        self.label.setFixedHeight(31)
        layout.addWidget(self.label)

        self.src = QPlainTextEdit(self)
        self.src.setStyleSheet("background-color: black; font: 81 16pt 'Rockwell Extra Bold'; color: white; border: none; border-radius: 10px;")
        self.src.setReadOnly(True)
        layout.addWidget(self.src)

        self.central_widget.setLayout(layout)

    def setSrcText(self) -> None:
        try:
            response = requests.get(str(self.url.text()))
            if response.status_code == 200:
                self.label.setText(f"{self.url.text()} - Source Code Viewer")
                self.src.setPlainText(str(response.text))
                return
            elif response.status_code == 404:
                self.src.setPlainText("404 Error")
                self.label.setText("404 URL Not founded")
                return
            elif response.status_code == 403:
                self.src.setPlainText("")
                self.label.setText("403 Error. Permission denied")
                return
            else:
                self.src.setPlainText(str(response.text))
                self.label.setText(f"Status code: {response.status_code}")
                return
        except BaseException as _be:
            self.label.setText(f"Error as {_be}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())