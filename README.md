# Background Subtraction with OpenCV MOG2
Remove background from your image using OpenCV MOG2. You can also replace your background with a whole new image. Randomizing background of an object will enhance learning for one-shot object detection models such as YOLO. Original source code acquired from [opencv tutorial](https://docs.opencv.org/master/d1/dc5/tutorial_background_subtraction.html).

## 1. Requirements
```
opencv 4.0.1
```

## 2. Usage
(1) Fix paths to your images. 
(2) Create a .txt list of images to process and set path to it.
(3) Set path to your base background image.
(4) Set names and paths for your results. 
(5) Set process mode (Subtraction/Randomization)
(6) Run the code!

## 3.Demo
###Input Image
<p align="center">
  <img width="700" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/input.jpg">
</p>

###Base Background Image
<p align="center">
  <img width="700" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/base_background.jpg">
</p>

###Output(Background Subtraction)
<p align="center">
  <img width="700" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/output_subtracted.jpg">
</p>

###Output(Background Randomization)
<p align="center">
  <img width="700" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/output_random.jpg">
</p>

