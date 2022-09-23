from pycimg import CImg
import numpy as np

# Load image from file
img = CImg('test/test.png')
img.display()

# Access pixel data as numpy array
arr = img.asarray()
# Set pixels in upper left 100 x 100 px rectangle
arr[:,:,0:99,0:99] = 0
# Pixel data is shared with the image instance
img.display()

# Create image from numpy array
img = CImg(np.random.randn(100,100))