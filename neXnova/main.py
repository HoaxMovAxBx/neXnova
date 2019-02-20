#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys
import urllib
import json
from OpenGL import GL
from PyQt5.QtQml import QQmlApplicationEngine
from PyQt5.QtCore import QUrl,Qt
from PyQt5.QtGui import QGuiApplication

app = QGuiApplication(sys.argv)
engine = QQmlApplicationEngine()

qml_file = os.path.join(os.path.dirname(__file__),"gamescreen.qml")
engine.load(QUrl.fromLocalFile(os.path.abspath(qml_file)))

app.exec_()
del engine
