# Innometrics client for X11

## Building
### Requirements
- Qt5
- `xcb` and `xcb_util-wm`
- `sqlite`

### Compiling
1. Create a build directory. `cd` into it:
```shell
mkdir build && cd build
```

2. Call `qmake` referencing the `.pro` file:
```shell
qmake ../x-innometrics.pro
```

3. Build the application
```shell
make
```

## Running
Run `x-innometrics` binary. Login or Register from `User` menu. From now on, if the server is up, your activity will be sent to the remote innometrics server.

## Storage
Application should create a directory `~/.local/share/x-innometrics` and store setting and database for activities.
