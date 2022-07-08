title: gr-difi
brief: Connect to DIFI devices from within a GNU Radio flowgraph
tags:
  - difi
  - cloud
  - satellite
  - ground station
author:
  - DIFI Consortium
copyright_owner:
  - DIFI Consortium
dependencies:
  - gnuradio (>= 3.9.0)
repo: https://github.com/DIFI-Consortium/gr-difi.git
gr_supported_version: v3.9, v3.10
stable_release: HEAD
---
The Digital Intermediate Frequency Interoperability (DIFI) Out of Tree (OOT) Module allows users to communicate with DIFI devices by streaming and receiving DIFI samples within a GNU Radio flowgraph.  This OOT contains a DIFI Sink to create DIFI streams from GR, and a DIFI Source to take in DIFI streams for processing.  Both TCP and UDP DIFI streams are supported.
