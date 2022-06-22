# DIFI Out of Tree Module

The Digital Intermediate Frequency Interoperability (DIFI) Out of Tree (OOT) Module allows users to communicate with [DIFI](https://dificonsortium.org/about/) devices by streaming and receiving DIFI samples within a GNU Radio flowgraph.


## Table of Contents
- [DIFI Blocks using the IEEE-ISTO Std 4900-2021: Digital IF Interoperability Standard](#difi-blocks-using-the-ieee-isto-std-4900-2021-digital-if-interoperability-standard)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installing DIFI OOT](#installing-difi-oot)
  - [Running the Unit Tests](#running-the-unit-tests)
- [Examples](#examples)
- [Frequently Asked Questions](#frequently-asked-questions)
- [Support](#support)
- [License](#license)

## DIFI Blocks Using the IEEE-ISTO Std 4900-2021: Digital IF Interoperability Standard
This is a set of GNU Radio blocks based on IEEE-ISTO Std 4900-2021: Digital IF Interoperability Standard version 1.0.

There are two DIFI blocks (source and sink) as part of this OOT module. The Bit Depths currently supported are 8 and 16 with upcoming support for the full range of bit depths specified in the DIFI standard.

 * __DIFI Source Block__\
	The DIFI source block receives UDP DIFI packets from a given IP address and port. It then forwards them to GNU Radio as a complex64 (gr_complex) or signed complex 8 (std::complex<char>).
	This block emits the following tags in the following situations:
	  - pck_n tag: Emitted when a missed packet occurs, will update the upstream blocks with the current packet number to expect and the current time stamps
	  - context tag: Emitted when a new DIFI context packet is received with the context packet dynamic information
	  - static_change: Emitted when the static parts of the DIFI context packet changes

    The DIFI Advanced tab contains more advanced settings for the DIFI block and should be used by users who know the devices and network in use.

   Context Packet Mismatch Behavior:
      - Default: Throws exceptions if context packet is incorrect or non-compliant
      - Ignore Mismatches - Forward data, no warnings: Entirely ignore the context packet, only forwards data
      - Throw Warnings - Forward: Displays Warnings about context packet mismatch or non-compliant context packets, but still forward DIFI data
      - Throw Warnings - No Forward: Displays Warnings about context packet mismatch or non-compliant context packets, but won't forward data until a correct context packet is received or one that matches the given settings

 * __DIFI Sink Block__\
	The DIFI sink block forwards packets to a given IP address and port number and packets the data with the given bit depth. This block operates in two modes, standalone and paired:

	- Pair Mode: The block expects to be paired with a DIFI source block that sends context packets, timing information, and packet count information. The sink block forwards context packets received via tags. For data packets, it adds the correct timestamps and packet number. The data format is packeted as complex64 (gr_complex) or complex signed 8 (std::complex<char>) samples.

	- Standalone Mode: In standalone mode, it is expected the user will supply the context packet information via GRC or the constructor of the class. For now, the context packet payload data are static once specified by the user. Similar to paired mode, the data format to pack is complex64 (gr_complex) or complex signed 8 (std::complex<char>) samples.

	Scaling Mode: To help mitigate quantization error, the DIFI Sink has an optional helper feature to apply a gain & offset to the input signal. The first mode "Manual" allows a user to manually set gain & offset. In Min-Max mode the user supplies the max and min expected I & Q values and the block solves for a gain & offset based on these and the specified bit depth.

	Note: this block converts from float 32 I and Q down to the specified bit depth for I and Q, which can cause significant quantization error for small signals.

For a brief tutorial on using these blocks, see the [DIFI Examples](./examples/README.md#difi-examples).


## Getting Started

The following installation instructions will get you up and running with the DIFI OOT Module on your local machine.

### Prerequisites
This project depends on the GNU Radio 3.9.x or 3.10.x runtime and development dependencies. See the [GNU Radio installation instructions](https://wiki.gnuradio.org/index.php/InstallingGR#From_Binaries) for steps on
installing GNU Radio from binaries (note that GNU Radio packaged with Ubuntu 20 is only 3.8). Some package managers do not automatically install all of the development dependencies,
so you may need to separately install and configure some of them. The DIFI OOT module requires the following:

- GNU Radio 3.9.x or 3.10.x
- Python 3.8 or greater
- cmake
- liborc-dev
- doxygen
- pytest
- pybind11

See the installation steps below for how to install these dependencies.

### Installing DIFI OOT

The following steps show how to install this OOT module on a Debian-based OS with GNU Radio already installed.  They have been tested to work under Ubuntu 20.  If you see error messages after running any of the following steps, stop and check our [FAQ](./docs/FAQ.md) for how to
resolve the problem.

```bash
sudo apt-get install python3-pip cmake liborc-dev doxygen
sudo pip install pytest pybind11

git clone https://github.com/DIFI-Consortium/gr-difi
cd gr-difi

mkdir build
cd build
cmake ..
make -j4
sudo make install
sudo ldconfig
```

(If you run into a non-existent path error after `cmake ..`, try recreating your build directory and use `cmake -DCMAKE_FIND_ROOT_PATH=/usr ..` instead)

At this point the OOT module should have been installed, and you should see additional blocks within GNU Radio Companion.

### Running the Unit Tests
If you would like to run the QA tests, there are two methods:
 1. From within the build directory, run:
    ```
    make test
    ```

    You can review detailed test output (including any failures) in Testing/Temporary/LastTest.log.

 2. From within the python directory, run:
    ```
    python -m pytest qa_*
    ```

    Pytest will show detailed test results directly in the output of this command.

### Resolutions to Common Problems During Installation and Tests
For a list common problems and resolutions, please check our [FAQ](./docs/FAQ.md) to see if your issue has been addressed.

## Examples
The [examples](./examples) folder has a collection of flowgraphs and supporting files that illustrate common ways of
using the blocks provided in this module. See the [README in the examples folder](./examples/README.md) to get started.



## Frequently Asked Questions
For a list of common questions, including problems and resolutions, please check our [FAQ](./docs/FAQ.md)

## Support

This project uses [GitHub Issues](https://github.com/DIFI-Consortium/gr-difi/issues) to track bugs and feature requests.

Before filing a new issue, please check our [FAQ](./docs/FAQ.md) for potential solutions to common problems.

## License

This project is licensed under the GNU General Public License v3.0 or later - see the [LICENSE](LICENSE) file for details