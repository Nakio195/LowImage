# LowImage
Little piece of soft aiming to reduce image size with a given main color

## Goal
The perspective of this soft is to reduce the weigth of an image and so reduce its energy cost when transferring the image over Internet.

## Use
|Arguments     |Description                                                 |
|---           |---                                                         |
|-color / -c   |  Main color output R,G,B                                  |
|-image / -i   |  Path to image to process                                 |
|-type  / -t   |   Reducing method                                         |
|              |           0 - Floyd-Steinberg dithering                   |
|              |           1 - Canny contours detection and coloration      |
