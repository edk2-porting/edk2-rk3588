# DISPLAYLINK DRIVERS
This package contains a GOP driver for Universal USB-connected docks containing the
DisplayLink DL-6xxx chip or newer.

[DisplayLink Website](http://www.displaylink.com)

[Products](https://www.displaylink.com/products/universal-docking-stations)

# INDEX

* [Resolutions Supported](#resolutions-supported)
* [Frame rates](#frame-rates)
* [Multiple monitor outputs](#multiple-monitor-outputs)
* [Multiple DisplayLink devices](#multiple-displaylink-devices)
* [Behaviour with no monitor connected](#behaviour-with-no-monitor-connected)

# Resolutions supported

The driver supports the following resolutions:

640 x 480 @ 60Hz

800 x 600 @ 60Hz

1024x768 @ 60Hz

1360x768 @ 60Hz

1280x960 @ 60Hz

1280x1024 @ 60Hz

1600x900 @ 60Hz

1400x1050 @ 60Hz

1600x1200 @ 60Hz

1920x1080 @ 60Hz


Note that the list of resolutions advertised by the driver may be smaller than
this if a connected monitor does not support a particular resolution. The driver
interrogates connected monitors to see which modes can be supported.It is the
responsibility of the BIOS to select the video mode from this list which most
closely matches its requirements. In some cases this may lead to the BIOS
scaling its display.

# Frame rates

The driver is limited to a maximum of ten frames per second. Some slower systems
at higher screen resolutions may perform at a lower rate than this.

# Multiple monitor outputs

If multiple monitors are connected to the DisplayLinkdevice, the display will be
duplicated (cloned) across all outputs at the same resolution. The resolution
used will be limited by the capability of the monitor with the
lowest specification.

# Multiple DisplayLink devices

The driver will support the connection of multiple DisplayLink devices. The
exact behaviourof the system with multiple devices connected is defined by the
rest of the BIOS; usually, the BIOS causes the displays to be duplicated
(cloned) across all devices. Note that the system performance and frame rate
will be affected by the number of DisplayLink devices connected.

# Behaviour with no monitor connected

The driver uses the EDID (Extended Display Identification Data) protocol to
detect the list of resolutions that a monitor will support.In some monitors this
may take some time, and occasionally no EDID information will be returned at
all. In this case the driver will not be able to detect that there is a monitor
connected. To improve the user experience in these cases, the driver will behave
as if there is a monitor connected, and will fall back to presenting the full
range of supported resolutions to the BIOS.
