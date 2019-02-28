# Innometrics client for X11

## Building
### Requirements
- Qt5
- xcb and xcb_util-wm
- sqlite

### Compiling
1. Create a build directory. =cd= into it:
```shell
mkdir build && cd build
```

2. Call =qmake= referencing the =.pro= file:
```shell
qmake ../x-innometrics.pro
```

3. Build the application
```shell
make
```

## Running
