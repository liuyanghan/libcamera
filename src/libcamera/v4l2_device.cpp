/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * v4l2_device.cpp - V4L2 Device
 */

#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "log.h"
#include "media_object.h"
#include "v4l2_device.h"

/**
 * \file v4l2_device.h
 * \brief V4L2 Device API
 */
namespace libcamera {

LOG_DEFINE_CATEGORY(V4L2)

/**
 * \struct V4L2Capability
 * \brief struct v4l2_capability object wrapper and helpers
 *
 * The V4L2Capability structure manages the information returned by the
 * VIDIOC_QUERYCAP ioctl.
 */

/**
 * \fn const char *V4L2Capability::driver()
 * \brief Retrieve the driver module name
 * \return The string containing the name of the driver module
 */

/**
 * \fn const char *V4L2Capability::card()
 * \brief Retrieve the device card name
 * \return The string containing the device name
 */

/**
 * \fn const char *V4L2Capability::bus_info()
 * \brief Retrieve the location of the device in the system
 * \return The string containing the device location
 */

/**
 * \fn unsigned int V4L2Capability::device_caps()
 * \brief Retrieve the capabilities of the device
 * \return The device specific capabilities if V4L2_CAP_DEVICE_CAPS is set or
 * 	   driver capabilities otherwise
 */

/**
 * \fn bool V4L2Capability::isMultiplanar()
 * \brief Identify if the device implements the V4L2 multiplanar APIs
 * \return True if the device supports multiplanar APIs
 */

/**
 * \fn bool V4L2Capability::isCapture()
 * \brief Identify if the device is capable of capturing video
 * \return True if the device can capture video frames
 */

/**
 * \fn bool V4L2Capability::isOutput()
 * \brief Identify if the device is capable of outputting video
 * \return True if the device can output video frames
 */

/**
 * \fn bool V4L2Capability::hasStreaming()
 * \brief Determine if the device can perform Streaming I/O
 * \return True if the device provides Streaming I/O IOCTLs
 */

/**
 * \class V4L2DeviceFormat
 * \brief The V4L2 device image format and sizes
 *
 * This class describes the image format and resolution to be programmed on a
 * V4L2 video device. The image format is defined by a fourcc code (as specified
 * by the V4L2 API with the V4L2_PIX_FMT_* macros), a resolution (width and
 * height) and one to three planes with configurable line stride and a total
 * per-plane size in bytes.
 *
 * Image formats, as defined by the V4L2 APIs, are categorized as packed,
 * semi-planar and planar, and describe the layout of the image pixel components
 * stored in memory.
 *
 * Packed image formats store pixel components one after the other, in a
 * contiguous memory area. Examples of packed image formats are YUYV
 * permutations, RGB with different pixel sub-sampling ratios such as RGB565 or
 * RGB666 or Raw-Bayer formats such as SRGGB8 or SGRBG12.
 *
 * Semi-planar and planar image formats store the pixel components in separate
 * and possibly non-contiguous memory areas, named planes, whose sizes depend on
 * the pixel components sub-sampling ratios, which are defined by the format.
 * Semi-planar formats use two planes to store pixel components and notable
 * examples of such formats are the NV12 and NV16 formats, while planar formats
 * use three planes to store pixel components and notable examples are YUV422
 * and YUV420.
 *
 * Image formats supported by the V4L2 API are defined and described in Section
 * number 2 of the "Part I - Video for Linux API" chapter of the "Linux Media
 * Infrastructure userspace API", part of the Linux kernel documentation.
 *
 * In the context of this document, packed image formats are referred to as
 * "packed formats" and semi-planar and planar image formats are referred to as
 * "planar formats".
 *
 * V4L2 also defines two different sets of APIs to work with devices that store
 * planes in contiguous or separate memory areas. They are named "Single-plane
 * APIs" and "Multi-plane APIs" respectively and are documented in Section 2.1
 * and Section 2.2 of the above mentioned "Part I - Video for Linux API"
 * documentation.
 *
 * The single-plane API allows, among other parameters, the configuration of the
 * image resolution, the pixel format and the stride length. In that case the
 * stride applies to all planes (possibly sub-sampled). The multi-plane API
 * allows configuring the resolution, the pixel format and a per-plane stride
 * length and total size.
 *
 * Packed image formats, which occupy a single memory area, are easily described
 * through the single-plane API. When used on a device that implements the
 * multi-plane API, only the size and stride information contained in the first
 * plane are taken into account.
 *
 * Planar image formats, which occupy distinct memory areas, are easily
 * described through the multi-plane APIs. When used on a device that implements
 * the single-plane API, all planes are stored one after the other in a
 * contiguous memory area, and it is not possible to configure per-plane stride
 * length and size, but only a global stride length which is applied to all
 * planes.
 *
 * The V4L2DeviceFormat class describes both packed and planar image formats,
 * regardless of the API type (single or multi plane) implemented by the device
 * the format has to be applied to. The total size and bytes per line of images
 * represented with packed formats are configured using the first entry of the
 * V4L2DeviceFormat::planes array, while the per-plane size and per-plane stride
 * length of images represented with planar image formats are configured using
 * the opportune number of entries of the V4L2DeviceFormat::planes array, as
 * prescribed by the image format definition (semi-planar formats use 2 entries,
 * while planar formats use the whole 3 entries). The number of valid entries of
 * the V4L2DeviceFormat::planes array is defined by the
 * V4L2DeviceFormat::planesCount value.
 */

/**
 * \var V4L2DeviceFormat::width
 * \brief The image width in pixels
 */

/**
 * \var V4L2DeviceFormat::height
 * \brief The image height in pixels
 */

/**
 * \var V4L2DeviceFormat::fourcc
 * \brief The fourcc code describing the pixel encoding scheme
 *
 * The fourcc code, as defined by the V4L2 API with the V4L2_PIX_FMT_* macros,
 * that identifies the image format pixel encoding scheme.
 */

/**
 * \var V4L2DeviceFormat::planes
 * \brief The per-plane memory size information
 *
 * Images are stored in memory in one or more data planes. Each data plane has a
 * specific line stride and memory size, which could differ from the image
 * visible sizes to accommodate padding at the end of lines and end of planes.
 * Only the first \ref planesCount entries are considered valid.
 */

/**
 * \var V4L2DeviceFormat::planesCount
 * \brief The number of valid data planes
 */

/**
 * \class V4L2Device
 * \brief V4L2Device object and API
 *
 * The V4L2 Device API class models an instance of a V4L2 device node.
 * It is constructed with the path to a V4L2 video device node. The device node
 * is only opened upon a call to open() which must be checked for success.
 *
 * The device capabilities are validated when the device is opened and the
 * device is rejected if it is not a suitable V4L2 capture or output device, or
 * if the device does not support streaming I/O.
 *
 * No API call other than open(), isOpen() and close() shall be called on an
 * unopened device instance.
 *
 * Upon destruction any device left open will be closed, and any resources
 * released.
 */

/**
 * \brief Construct a V4L2Device
 * \param deviceNode The file-system path to the video device node
 */
V4L2Device::V4L2Device(const std::string &deviceNode)
	: deviceNode_(deviceNode), fd_(-1)
{
}

/**
 * \brief Construct a V4L2Device from a MediaEntity
 * \param entity The MediaEntity to build the device from
 *
 * Construct a V4L2Device from a MediaEntity's device node path.
 */
V4L2Device::V4L2Device(const MediaEntity *entity)
	: V4L2Device(entity->deviceNode())
{
}

V4L2Device::~V4L2Device()
{
	close();
}

/**
 * \brief Open a V4L2 device and query its capabilities
 * \return 0 on success, or a negative error code otherwise
 */
int V4L2Device::open()
{
	int ret;

	if (isOpen()) {
		LOG(V4L2, Error) << "Device already open";
		return -EBUSY;
	}

	ret = ::open(deviceNode_.c_str(), O_RDWR);
	if (ret < 0) {
		ret = -errno;
		LOG(V4L2, Error)
			<< "Failed to open V4L2 device '" << deviceNode_
			<< "': " << strerror(-ret);
		return ret;
	}
	fd_ = ret;

	ret = ioctl(fd_, VIDIOC_QUERYCAP, &caps_);
	if (ret < 0) {
		ret = -errno;
		LOG(V4L2, Error)
			<< "Failed to query device capabilities: "
			<< strerror(-ret);
		return ret;
	}

	LOG(V4L2, Debug)
		<< "Opened '" << deviceNode_ << "' "
		<< caps_.bus_info() << ": " << caps_.driver()
		<< ": " << caps_.card();

	if (!caps_.isCapture() && !caps_.isOutput()) {
		LOG(V4L2, Debug) << "Device is not a supported type";
		return -EINVAL;
	}

	if (!caps_.hasStreaming()) {
		LOG(V4L2, Error) << "Device does not support streaming I/O";
		return -EINVAL;
	}

	if (caps_.isCapture())
		bufferType_ = caps_.isMultiplanar()
			    ? V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE
			    : V4L2_BUF_TYPE_VIDEO_CAPTURE;
	else
		bufferType_ = caps_.isMultiplanar()
			    ? V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE
			    : V4L2_BUF_TYPE_VIDEO_OUTPUT;

	return 0;
}

/**
 * \brief Check if device is successfully opened
 * \return True if the device is open, false otherwise
 */
bool V4L2Device::isOpen() const
{
	return fd_ != -1;
}

/**
 * \brief Close the device, releasing any resources acquired by open()
 */
void V4L2Device::close()
{
	if (fd_ < 0)
		return;

	::close(fd_);
	fd_ = -1;
}

/**
 * \fn const char *V4L2Device::driverName()
 * \brief Retrieve the name of the V4L2 device driver
 * \return The string containing the driver name
 */

/**
 * \fn const char *V4L2Device::deviceName()
 * \brief Retrieve the name of the V4L2 device
 * \return The string containing the device name
 */

/**
 * \fn const char *V4L2Device::busName()
 * \brief Retrieve the location of the device in the system
 * \return The string containing the device location
 */

/**
 * \brief Retrieve the image format set on the V4L2 device
 * \return 0 for success, a negative error code otherwise
 */
int V4L2Device::getFormat(V4L2DeviceFormat *format)
{
	return caps_.isMultiplanar() ? getFormatMultiplane(format) :
				       getFormatSingleplane(format);
}

/**
 * \brief Configure an image format on the V4L2 device
 *
 * Apply the supplied \a format to the device, and return the actually
 * applied format parameters, as \ref V4L2Device::getFormat would do.
 *
 * \return 0 for success, a negative error code otherwise
 */
int V4L2Device::setFormat(V4L2DeviceFormat *format)
{
	return caps_.isMultiplanar() ? setFormatMultiplane(format) :
				       setFormatSingleplane(format);
}

int V4L2Device::getFormatSingleplane(V4L2DeviceFormat *format)
{
	struct v4l2_format v4l2Format = {};
	struct v4l2_pix_format *pix = &v4l2Format.fmt.pix;
	int ret;

	v4l2Format.type = bufferType_;
	ret = ioctl(fd_, VIDIOC_G_FMT, &v4l2Format);
	if (ret) {
		ret = -errno;
		LOG(Error) << "Unable to get format: " << strerror(-ret);
		return ret;
	}

	format->width = pix->width;
	format->height = pix->height;
	format->fourcc = pix->pixelformat;
	format->planesCount = 1;
	format->planes[0].bpl = pix->bytesperline;
	format->planes[0].size = pix->sizeimage;

	return 0;
}

int V4L2Device::setFormatSingleplane(V4L2DeviceFormat *format)
{
	struct v4l2_format v4l2Format = {};
	struct v4l2_pix_format *pix = &v4l2Format.fmt.pix;
	int ret;

	v4l2Format.type = bufferType_;
	pix->width = format->width;
	pix->height = format->height;
	pix->pixelformat = format->fourcc;
	pix->bytesperline = format->planes[0].bpl;

	ret = ioctl(fd_, VIDIOC_S_FMT, &v4l2Format);
	if (ret) {
		ret = -errno;
		LOG(Error) << "Unable to set format: " << strerror(-ret);
		return ret;
	}

	/*
	 * Return to caller the format actually applied on the device,
	 * which might differ from the requested one.
	 */
	format->width = pix->width;
	format->height = pix->height;
	format->fourcc = pix->pixelformat;
	format->planesCount = 1;
	format->planes[0].bpl = pix->bytesperline;
	format->planes[0].size = pix->sizeimage;

	return 0;
}

int V4L2Device::getFormatMultiplane(V4L2DeviceFormat *format)
{
	struct v4l2_format v4l2Format = {};
	struct v4l2_pix_format_mplane *pix = &v4l2Format.fmt.pix_mp;
	int ret;

	v4l2Format.type = bufferType_;
	ret = ioctl(fd_, VIDIOC_G_FMT, &v4l2Format);
	if (ret) {
		ret = -errno;
		LOG(Error) << "Unable to get format: " << strerror(-ret);
		return ret;
	}

	format->width = pix->width;
	format->height = pix->height;
	format->fourcc = pix->pixelformat;
	format->planesCount = pix->num_planes;

	for (unsigned int i = 0; i < format->planesCount; ++i) {
		format->planes[i].bpl = pix->plane_fmt[i].bytesperline;
		format->planes[i].size = pix->plane_fmt[i].sizeimage;
	}

	return 0;
}

int V4L2Device::setFormatMultiplane(V4L2DeviceFormat *format)
{
	struct v4l2_format v4l2Format = {};
	struct v4l2_pix_format_mplane *pix = &v4l2Format.fmt.pix_mp;
	int ret;

	v4l2Format.type = bufferType_;
	pix->width = format->width;
	pix->height = format->height;
	pix->pixelformat = format->fourcc;
	pix->num_planes = format->planesCount;

	for (unsigned int i = 0; i < pix->num_planes; ++i) {
		pix->plane_fmt[i].bytesperline = format->planes[i].bpl;
		pix->plane_fmt[i].sizeimage = format->planes[i].size;
	}

	ret = ioctl(fd_, VIDIOC_S_FMT, &v4l2Format);
	if (ret) {
		ret = -errno;
		LOG(Error) << "Unable to set format: " << strerror(-ret);
		return ret;
	}

	/*
	 * Return to caller the format actually applied on the device,
	 * which might differ from the requested one.
	 */
	format->width = pix->width;
	format->height = pix->height;
	format->fourcc = pix->pixelformat;
	format->planesCount = pix->num_planes;
	for (unsigned int i = 0; i < format->planesCount; ++i) {
		format->planes[i].bpl = pix->plane_fmt[i].bytesperline;
		format->planes[i].size = pix->plane_fmt[i].sizeimage;
	}

	return 0;
}

} /* namespace libcamera */
