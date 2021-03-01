import sys
import time as tm
import threading as threading

def getTime():
    return tm.time() * 1000000 - startTime
    
def userInputThread():
    while(True):
        inputStr = input()
        if inputStr == "stop":
            print("STOPPING")
            break
    
def main():

    pThread = threading.Thread(target = userInputThread)

    pThread.start()

    print("Python Ready")
    
    pThread.join()
    

if __name__ == "__main__":
   global startTime, file
   startTime = int(sys.argv[1:][0])
   file = sys.argv[1:][1]
   main()
