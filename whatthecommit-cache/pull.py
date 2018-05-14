#!/bin/python3

import http.client
import signal
import threading
import urllib.request as req

m = []
threads = []

stop = False


class CommitThread (threading.Thread):
    def __init__(self, threadID):
        threading.Thread.__init__(self)
        self.threadID = threadID

    def run(self):
        duplicates = 0
        while duplicates < 10 and stop is False:
            c = self.get_commit()
            if c in m:
                duplicates += 1
            else:
                m.append(c)
                duplicates = 0
                # print('{}: {}'.format(self.threadID, c))
                print(c)

    def get_commit(self):
        try:
            request = req.urlopen('http://whatthecommit.com/index.txt')
            if request.code != 200:
                print('Return code was {}'.format(request.code))
            return request.read().decode('utf-8').strip()

        except http.client.HTTPException as e:
            print(e)
            return None


def signal_handler(signal, frame):
    global stop
    stop = True

signal.signal(signal.SIGINT, signal_handler)

if __name__ == '__main__':
    threads = [CommitThread(i) for i in range(0, 32)]

    for t in threads:
        t.start()

    for t in threads:
        t.join()
