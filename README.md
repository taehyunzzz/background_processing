# Background Subtraction with OpenCV MOG2
Remove background from your image using OpenCV MOG2. You can also replace your background with a whole new image. Randomizing background of an object will enhance learning for one-shot object detection models such as YOLO. This code allows you to process multiple images at once. Original source code acquired from [opencv tutorial](https://docs.opencv.org/master/d1/dc5/tutorial_background_subtraction.html).

## 1. Requirements
```
opencv 4.0.1
```

## 2. Usage
(1) Fix paths to your background images.<br/> 
(2) Create a .txt list of images to process and set path to it.<br/>
(3) Set path to your base background image.<br/>
(4) Set names and paths for your results.<br/>
(5) Set process mode (Subtraction/Randomization)<br/>
(6) Run the code!<br/>

## 3. Demo
### Input Image and Base Background Image
<p align="center">
  <img width="350" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/input.jpg" alt="Input"> \
  <img width="350" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/base_background.jpg" alt="Background">
</p>

### Output Images(Background Subtraction & Randomization)
<p align="center">
  <img width="350" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/output_subtracted.jpg" alt="Background Subtraction"> \
  <img width="350" src="https://github.com/taehyunzzz/background_processing/blob/master/samples/output_random.jpg" alt="Background Randomization">
</p>

