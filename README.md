![image](https://github.com/foxidokun/raytracing/blob/master/output.png)

### Ray tracing
This is small educational raytracing renderer from C++ course at MIPT. It is based on __"Ray Tracing in One Weekend"__
article, but with better architecture and code quality.

### Usage
To change render parameters you should see `include/config.h`. It is possible to render and save as file or display the image
on the screen (see PREVIEW_MODE), but keep in mind that a quality render takes ~15-20 minutes on Ryzen 5700X,
so the preview parameters should be much lower.

The scene itself is defined in `src/main.cpp`.

### How to build
It it powered by CMake -- so I hope you will figure it out how to compile it 
by yourself
