# Frequently asked questions

## How can I file an issue?
See our guidelines on [How to file issues and get help](../../SUPPORT.md) or [Reporting Security Issues](../../SECURITY.md#reporting-security-issues).

## How can I contribute to this project?
See our [Contributing](../../README.md#contributing) guide for more details.

## I'm getting a CMAKE error that says it cannot find Gnuradio. What do I do?
If you see an error like the following:

```
CMake Error at CMakeLists.txt:77 (find_package):
    Could not find a configuration file for package "Gnuradio" that is compatible with requested version "3.9".
```

It's likely that either GNU Radio isn't installed, or your environment isn't set up properly for CMAKE to be able to find where it was installed. Please check the [GNU Radio Installation Instructions](https://wiki.gnuradio.org/index.php/InstallingGR) for suggestions on ways to properly install and configure GNU Radio appropriately for your environment.


## Why is my build complaining about liborc and failing?
If you get a failure related to liborc when trying to compile the OOT module that looks like this:
```
make[2]: *** No rule to make target '/usr/lib/x86_64-linux-gnu/liborc-0.4.so', needed by 'lib/libgnuradio-difi.so.v1.0-compat-xxx-xunknown'.  Stop.
make[1]: *** [CMakeFiles/Makefile2:251: lib/CMakeFiles/gnuradio-difi.dir/all] Error 2
make: *** [Makefile:141: all] Error 2
```

You'll need to install the liborc package. On Ubuntu 20.04, you can install the missing package by running:
```
sudo apt install liborc-0.4-dev
```

You should now be able to compile gr-difi.

## I just compiled and installed the OOT Module, but all of the unit tests are failing. What now?
Check the test output for the specific errors that were thrown. If you ran the tests with `make test`, the error logs should be in Testing/Temporary/LastTest.log, starting from the build directory.

If you ran the tests using python and pytest, the errors should be in the output of the test command.


## I'm getting an ImportError for libgnuradio-difi when running the unit tests
If you see an error that looks like the following:

```
ImportError: libgnuradio-difi.so.1.0.0git: cannot open shared object file: No such file or directory
```

If you were able to run `make` and `make install` with no errors, try running the following:

```
sudo ldconfig
```

This should enable the Linux dynamic library loader to find your newly installed shared object file.

## I'm getting a ModuleNotFoundError
If you see the following error,

```
ModuleNotFoundError: No module named 'difi'
```
then Python can't find the gr-difi module. Try running

```
export PYTHONPATH=/usr/local/lib/python3/dist-packages/
```

If that resolves the issue, you may want to update your environment to include `/usr/local/lib/python3/dist-packages/` in your PYTHONPATH, possibly by adding the following line in your ~/.bashrc file:

```
export PYTHONPATH=/usr/local/lib/python3/dist-packages/:$PYTHONPATH
```

## I'm seeing gr::log :ERROR: statements in Unit and Integration test output
If you see the following lines in your test output:

```
gr::log :ERROR: vmcircbuf_prefs::get - /home/youruser/.gnuradio/prefs/vmcircbuf_default_factory: No such file or directory
gr::log :ERROR: vmcircbuf_createfilemapping - vmcircbuf_createfilemapping: createfilemapping is not available
```

This means that GNU Radio is unable to find a certain file in your GNU Radio configuration directory. This file is generally created the
first time you run a flowgraph in GNU Radio Companion, but you can also create one yourself by running:

```
mkdir -p "${HOME}/.gnuradio/prefs/"
echo "gr::vmcircbuf_sysv_shm_factory" > "${HOME}/.gnuradio/prefs/vmcircbuf_default_factory"
```
