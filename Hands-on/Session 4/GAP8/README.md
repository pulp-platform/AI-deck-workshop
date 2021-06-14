This example is taken from Bitcraze: https://github.com/bitcraze/AIdeck_examples/tree/master/GAP8/test_functionalities/wifi_jpeg_streamer.

## Requirements
- Wi-Fi card on your laptop
- GAP-SDK installed: [Install GAP sdk](https://greenwaves-technologies.com/setting-up-sdk/)

## Description

These two examples show how to stream JPEG images from the AI-Deck to a socket connected via Wi-FI.
- `wifi_jpeg_streamer/` example streams normal grayscale images
- `wifi_jpeg_streamer_inv_col/` example streams grayscale images with inverted colors, using the `inverting()` from Hands-on session 2.
- `viewer.py`: the Python visualizer receives the images streamed from NINA to the Laptop. By default, it connects to the IP of the AI-deck when use in AccessPoint mode.
