# hawkeye
# this project is used to detect IoT devices in the background, and manipulate the detected desired objects of our choice.
# this project is at a prototype level, and is limited to only three mock devices at the moment.
# the actual implementation would take much longer, and would require major resources to be used, such as an extensive set of cloud resources
# to train the model for hundreds of different IoT devices, and perform highly optimized detection algorithms.
# currently we are using yolov5s model, its pre-trained version, to train the model, and from that we obtain a pytorch model file, which 
# we use for detection of IoT Devices

# this project requires you to clone yolov5 from ultralytics, and replacing the detect.py in the ultralytics/yolov5 directory with the file
# provided here. Modify the constants mentioned as required. The model will not run without the requirements being satisfied. The reason
# we did not add ultralytics to this branch, is because of few complexities i faced. honestly this was so much easier.

# hawkeye - 1 and hawkeye - 2 are embedded c codes, written in arduino ide, which is used to run the esp32s we have used for the project. we 
# will be uploading the architecture and sequence diagrams later, for easy future references.

# the idea of the project is loosely based on MQTT communication protocol, with an attempt to mock how IoT devices would function in an 
# environment
