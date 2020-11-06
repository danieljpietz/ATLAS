# Copyright (c) 2020 Daniel Pietz

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import owl
import csv
import sys
import os
import datetime
import threading

if(len(sys.argv) < 3):
    print("ERROR: Use python record.py MarkerCount BodyCount Optional: Session Name")
    exit(1)
    pass

if (len(sys.argv) == 4):
    SESSIONNAME = str(sys.argv[3]) + " - "+ str(datetime.datetime.now())
else:
    SESSIONNAME = str(datetime.datetime.now())
    pass
SESSIONNAME = SESSIONNAME.replace(":", " ")
SERVER = "172.19.34.203"

MarkerCount = int(sys.argv[1])
BodyCount = int(sys.argv[2])

shouldStop = False
frameIsReady = False
if not os.path.isdir("Sessions"):
    os.mkdir("Sessions")
    pass

SESSIONPATH = os.path.join("Sessions", SESSIONNAME)

os.mkdir(SESSIONPATH)

#SAVEFILEMARKERS = os.path.join(SESSIONPATH, SESSIONNAME + " Markers.csv")
#SAVEFILEBODIES = os.path.join(SESSIONPATH, SESSIONNAME + " Bodies.csv")

#markerFile =  open(SAVEFILEMARKERS, 'a+', newline='')
#markerWriter = csv.writer(markerFile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)

#bodiesFile =  open(SAVEFILEBODIES, 'a+', newline='')
#bodiesWriter = csv.writer(bodiesFile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)

def recordThread():
    global shouldStop
    global frameIsReady
    global markerWriter
    global bodiesWriter
    global BodyCount
    global MarkerCount
    i = 0
    # instantiate context
    o = owl.Context()
    # connect to server with timeout of 10000000 microseconds
    o.open(SERVER, "timeout=10000000")
    # initialize session
    o.initialize("streaming=1")

    MarkerFiles = MarkerCount * [None]
    MarkerWriters = MarkerCount * [None]
    
    for i in range(0, MarkerCount):
        MarkerFiles[i] =  open(os.path.join(SESSIONPATH,"Marker " + str(i)) + ".csv", 'a+', newline='')
        MarkerWriters[i] = csv.writer(MarkerFiles[i], delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
        pass

    BodyFiles = BodyCount * [None]
    BodyWriters = BodyCount * [None]
    
    for i in range(0, BodyCount):
        BodyFiles[i] =  open(os.path.join(SESSIONPATH,"Body " + str(i)) + ".csv", 'a+', newline='')
        BodyWriters[i] = csv.writer(BodyFiles[i], delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
        pass
    

    # main loop
    evt = None
    while (evt or (o.isOpen() and o.property("initialized"))) and (shouldStop == False):
        #while (frameIsReady == True):
            #pass
        # poll for events with a timeout (microseconds)
        evt = o.nextEvent(1000000)
        # nothing received, keep waiting
        if not evt: continue
        # process event
        if evt.type_id == owl.Type.FRAME:
            # print markers
            if "markers" in evt:
                for m in evt.markers:
                    MarkerEventArr = [m.time, m.x, m.y, m.z]
                    MarkerWriters[m.id].writerow(MarkerEventArr)
                    pass
                
            # print rigids
            if "rigids" in evt:
                for r in evt.rigids:
                    BodyEventArr = [r.time]
                    for P in r.pose:
                        BodyEventArr.append(P)
                        pass
                    
                    BodyWriters[r.id].writerow(BodyEventArr)
                    pass
        elif evt.type_id == owl.Type.ERROR:
            # handle errors
            print(evt.name, evt.data)
            if evt.name == "fatal":
                break
        elif evt.name == "done":
            # done event is sent when master connection stops session
            print("done")
            break
        #frameIsReady = True
    # end main loop

    # end session
    o.done()
    # close socket
    o.close()
    pass

def inputThread():
    global shouldStop
    while(shouldStop == False):
        cmd = input().lower()
        if(cmd == "kill"):
            shouldStop = True;
        pass
    
    pass


RecordThread = threading.Thread(target = recordThread)
InputThread = threading.Thread(target = inputThread)

print("BEGINNING SESSION: " + SESSIONNAME + " ON: " + SERVER)

RecordThread.start()
InputThread.start();
