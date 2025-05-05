# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for
# full license information.

import cv2 as cv
import requests

import time
import os

def resize(img, scale_percent):

    width = int(img.shape[1] * scale_percent)
    height = int(img.shape[0] * scale_percent)

    dim = (width, height)

    return cv.resize(img, dim, interpolation = cv.INTER_AREA)


def processFrame(frameBytes):
    global INFERENCE_URL

    multipart_form_data = {'frame': ("frame.jpg", frameBytes)}

    response = requests.post(INFERENCE_URL,files=multipart_form_data, timeout=2)

    return response.json()

def main():
    global vf
    global INFERENCE_URL
    global ALERT_URL

    print("Python3 cv2 version: %s" % cv.__version__)

    CAMERA_INDEX = int(os.getenv('CAMERA_INDEX', "1"))
    CAMERA_INTERVAL = float(os.getenv('CAMERA_INTERVAL', "1.0"))
    INFERENCE_URL = os.getenv('INFERENCE_URL', "http://raspberrypi1:8080/analyze")
    ALERT_URL = os.getenv('ALERT_URL', "http://raspberrypi1:8081/alert")

    vf = cv.VideoCapture(CAMERA_INDEX)

    if not vf.isOpened():
        print("Error opening camera!!!")
        exit()


    try:

        while True:

        
            (ret, frame) = vf.read()

            if not ret:
                print("No frame data!!!")
                continue

            #frame = resize(frame, 0.5)

            frameBytes = cv.imencode( '.jpg', frame)[1].tobytes()
            r = processFrame(frameBytes)

            print (r)

            if r != None and "detections" in r:

                try:
                    print("Posting detections as event..")
                    requests.post(ALERT_URL,json=r, timeout=2)
                    print("Event done!")

                except:
                    print("Error on posting detections!")

            time.sleep(CAMERA_INTERVAL)
            

    except Exception as e:
        print("Unexpected error %s " % e)
        raise
    finally:
        print("Shutting down Camera Modulre...")
        vf.release()

if __name__ == "__main__":
    main()

