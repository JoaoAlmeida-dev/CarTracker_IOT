CarTracker_IOT
==============

A node-red backend for a car tracking iot project.

* Scientific [article](https://github.com/JoaoAlmeida-dev/CarTracker_IOT/blob/main/Security%20tracker%20for%20cars-article.pdf) written detailing the project, it's motivation and its implementation;
* Scientific [poster](https://github.com/JoaoAlmeida-dev/CarTracker_IOT/blob/main/Security%20tracker%20for%20cars-poster.pdf) created to display the project;
* Pdf [presentation](https://github.com/JoaoAlmeida-dev/CarTracker_IOT/blob/main/Security%20tracker%20for%20cars.pdf).

## Skills
Internet of Things (IoT) · Arduino · C · Node-RED · Global Positioning System (GPS) · Lora-Wan

## About
This project was meant to serve as a protection and data-gathering device for a personal car.
It achieves this by using two different operational modes that the user switches manually with the press of a button

### Tracking mode: 
Periodically registers the current GPS location and communicates this data through LORA to our backend server in Node-RED where this data is used for visualizations.

### Alert mode:
The device activates its gyroscopic sensor to detect any sudden movement, upon which, it will communicate the alert to the backend Server, which will send email and WhatsApp notifications to the user.

### Future work
*  Improve the hardware implementation of the project, properly solder components and design a circuit board to reduce the size of the device; 
*  Improve the backend server in robustness and features, statistical analysis of the tracking functionalities;
*  Develop a mobile app to interact and connect to the device, easing the user experience and the communication between the device and the backend.
*  This application could serve as the front end to the analysis and visualizations previously mentioned.

## The device
![CarTracking-IOT](https://github.com/JoaoAlmeida-dev/CarTracker_IOT/assets/24848457/6bc2c43f-a4fc-49a4-802a-b7e804f16241)
