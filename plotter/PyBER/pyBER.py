#!/usr/bin/python3

# The MIT License (MIT)
#
# Copyright (c) 2016 PyBER
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph.console
import numpy as np
import fileExplorer
from pyqtgraph.dockarea import *

app  = QtGui.QApplication([])
win  = QtGui.QMainWindow()
area = DockArea()
win.setCentralWidget(area)
win.resize(1280,800)
win.setWindowTitle('PyBER (deluxe) plotter')

# Create docks, place them into the window one at a time.
# Note that size arguments are only a suggestion; docks will still have to
# fill the entire dock area and obey the limits of their internal widgets.
dFile = Dock("File explorer",                  size=(275,400))
dLege = Dock("Legend",                         size=(275,400))
dBER  = Dock("Bit Error Rate (BER)",           size=(400,200))
dFER  = Dock("Frame Error Rate (FER)",         size=(400,200))
dBEFE = Dock("BE/FE",                          size=(400,125))
dThr  = Dock("Decoder information throughput", size=(400,125))

area.addDock(dFile, 'left'         )
area.addDock(dBER,  'right',  dFile)
area.addDock(dFER,  'right',  dBER )
area.addDock(dBEFE, 'bottom', dBER )
area.addDock(dThr,  'bottom', dFER )
area.addDock(dLege, 'bottom', dFile)

# Add widgets into each dock
pg.setConfigOptions(antialias=True)

wLege = QtGui.QTabWidget()
dLege.addWidget(wLege)

wBER = pg.PlotWidget(labels={'left': "Bit Error Rate", 'bottom': "Eb/N0 (dB)"})
wBER.plot(np.random.normal(size=100))
wBER.showGrid(True,  True)
wBER.setLogMode(False, True)
wBER.showLabel('left', True)
wBER.showLabel('bottom', True)
dBER.addWidget(wBER)

wFER = pg.PlotWidget(labels={'left': "Frame Error Rate", 'bottom': "Eb/N0 (dB)"})
wFER.plot(np.random.normal(size=100))
wFER.showGrid(True,  True)
wFER.setLogMode(False, True)
wFER.showLabel('left', True)
wFER.showLabel('bottom', True)
dFER.addWidget(wFER)

wBEFE = pg.PlotWidget(labels={'left': "BE/FE", 'bottom': "Eb/N0 (dB)"})
wBEFE.plot(np.random.normal(size=100))
wBEFE.showGrid(True,  True)
wBEFE.showLabel('left', True)
wBEFE.showLabel('bottom', True)
dBEFE.addWidget(wBEFE)

wThr = pg.PlotWidget(labels={'left': "Throughput (Mbps)", 'bottom': "Eb/N0 (dB)"})
wThr.plot(np.random.normal(size=100))
wThr.showGrid(True,  True)
wThr.showLabel('left', True)
wThr.showLabel('bottom', True)
dThr.addWidget(wThr)

wFile = fileExplorer.generatePannel(wBER, wFER, wBEFE, wThr, wLege)
dFile.addWidget(wFile)

win.show()

# Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()