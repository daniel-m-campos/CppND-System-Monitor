# CppND-System-Monitor

## Requirements

This submission uses `<filesystem>` which requires gcc version 9 or greater. To prepare the Ubuntu 16.04.6 LTS OS, run

```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-snapshot
sudo apt-get install gcc-9 g++-9

export CC=/usr/bin/gcc-9
export CXX=/usr/bin/g++-9
```

before `make build`. Alternatively, just run `. setup.sh` from inside the project directory.

![System Monitor](images/monitor.png)

## Udacity Linux Workspace

[Udacity](https://www.udacity.com/) provides a browser-based
Linux [Workspace](https://engineering.udacity.com/creating-a-gpu-enhanced-virtual-desktop-for-udacity-497bdd91a505) for
students.

You are welcome to develop this project on your local machine, and you are not required to use the Udacity Workspace.
However, the Workspace provides a convenient and consistent Linux development environment we encourage you to try.

## ncurses

[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the
terminal. This project relies on ncurses for display output.

Within the Udacity Workspace, `.student_bashrc` automatically installs ncurses every time you launch the Workspace.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor-Project-Updated.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Follow along with the lesson.

5. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.

6. Submit!